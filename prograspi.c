#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fp;

#include "ser.h"
#include "bootldr.h"

//-----------------------------------------------------------------------------
int check_packet ( bootldr_pkt_t* response_pkt  )
{
    unsigned int ra;
    unsigned int rb;
    unsigned int rc;

    bootldr_pkt_t data = *response_pkt;

    if(data.begin_sync!=0x7C) return(2);
    if(data.datalen!=(~ data.invdatalen & 0xFF)) return(2);
    if(rdata[3]!=sdata[3]) return(2);
    if(rdata[4]!=sdata[4]) return(2);
    if(rdata[3+rdata[1]]!=0x7D) return(2);
    ra=3;
    ra=rdata[1]+5;
    for(rb=0,rc=0;rb<ra;rb++) rc+=rdata[rb];
    rc&=0xFF;
    if(rc!=0xFF) return(2);
    return(rdata[5]);
}
//-----------------------------------------------------------------------------
int raspload ( unsigned int addr, unsigned int data )
{
    unsigned int ra;
    unsigned int rc;
    bootldr_pkt_t datapkt;
    char* sdata = (char*)&datapkt;

    if(firstaddr==0xFFFFFFFF) firstaddr=addr;

    datapkt.begin_sync = 0x7C;
    datapkt.datalen = 4;
    datapkt.invdatalen = ~4;
    datapkt.type = data;
    datapkt.passfail = 0;
    datapkt.addr = addr;
    datapkt.data = data;
    datapkt.end_sync = 0x7D;

    rc = 0;
    //sum up all the bytes
    for(ra=0; ra < 0xF-0x1; ra++) rc+=sdata[ra];

    //calculate the checksum using the previous sum
    datapkt.cksum = (~rc)&0xFF;

    //send data
    ser_senddata(sdata, 0xF);
    ra=0;
    while(1)
    {
        //read data from serial port. returns num chars written
        ra = ser_copystring(rdata);

	//wait until we've received 10 chars
        if(ra==10)
        {
            rc=check_packet();
            if(rc) return(1);
	    //free up a certain number of chars in serial buffer
            ser_dump(ra);
            break;
        }
    }
    return(0);
}
//-----------------------------------------------------------------------------
//Reads file line by line
//Sends each line to the serial port
//
int readhex ( FILE *fp )
{
    char gstring[80];
    char newline[1024];
    unsigned char hexline[1024];

    //intimately tied to the .balign value in the bootloader startup
    #define RAMMASK 0x1FFFFF

    unsigned int addhigh;
    unsigned int add;
    unsigned int data;

    unsigned int ra;

    unsigned int line;

    unsigned char checksum;

    unsigned int len;
    unsigned int maxadd;

    unsigned char t;

    maxadd=0;

    addhigh=0;

    line=0;
    while(fgets(newline,sizeof(newline)-1,fp))
    {
        line++;
        //printf("%s",newline);
        if(newline[0]!=':')
        {
            printf("Syntax error <%u> no colon\n",line);
            continue;
        }
        gstring[0]=newline[1];
        gstring[1]=newline[2];
        gstring[2]=0;
        len=strtoul(gstring,NULL,16);
        for(ra=0;ra<(len+5);ra++)
        {
            gstring[0]=newline[(ra<<1)+1];
            gstring[1]=newline[(ra<<1)+2];
            gstring[2]=0;
            hexline[ra]=(unsigned char)strtoul(gstring,NULL,16);
        }
        checksum=0;
        for(ra=0;ra<(len+5);ra++) checksum+=hexline[ra];
        //checksum&=0xFF;
        if(checksum)
        {
            printf("checksum error <%u>\n",line);
        }
        add=hexline[1]; add<<=8;
        add|=hexline[2];
        add|=addhigh;
        if(add>RAMMASK)
        {
            printf("address too big 0x%08X\n",add);
            //return(1);
            continue;
        }
        if(add&3)
        {
            printf("bad address 0x%08X\n",add);
            return(1);
        }
        t=hexline[3];
        if(t!=0x02)
        {
            if(len&3)
            {
                printf("bad length\n");
                return(1);
            }
        }

        //:0011223344
        //;llaaaattdddddd
        //01234567890
        switch(t)
        {
            default:
                printf("UNKNOWN type %02X <%u>\n",t,line);
                break;
            case 0x00:
                for(ra=0;ra<len;ra+=4)
                {
                    if(add>RAMMASK)
                    {
                        printf("address too big 0x%08X\n",add);
                        break;
                    }
                    data=           hexline[ra+4+1];
                    data<<=8; data|=hexline[ra+4+0];
                    if(raspload(add,data)) return(1);
                    add+=2;

                    data=           hexline[ra+4+3];
                    data<<=8; data|=hexline[ra+4+2];
                    if(raspload(add,data)) return(1);
                    add+=2;
                    if(add>maxadd) maxadd=add;
                }
                break;
            case 0x01:
//                printf("End of data\n");
                break;
            case 0x02:
                addhigh=hexline[5];
                addhigh<<=8;
                addhigh|=hexline[4];
                addhigh<<=16;
                break;
            case 0x03:
                    data=           hexline[4+0];
                    data<<=8; data|=hexline[4+1];
                    data<<=8; data|=hexline[4+2];
                    data<<=8; data|=hexline[4+3];
                    printf("TYPE 3 0x%08X\n",data);
                break;
        }
    }

    //printf("%u lines processed\n",line);
    //printf("%08X\n",maxadd);
    //for(ra=0;ra<maxadd;ra+=4)
    //{
        //printf("0x%08X: 0x%08X\n",ra,ram[ra>>2]);
    //}
    return(0);


}

//-----------------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int cksum;
    bootldr_pkt_t jump_pkt;
    char* sdata = (char*)&jump_pkt;

    if(argc<3)
    {
        printf("prograspi filename.hex /dev/ttyXYZ\n");
        return(1);
    }
    fp=fopen(argv[1],"rt");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    printf("file opened\n");


    if(ser_open(argv[2]))
    {
        printf("ser_open() failed\n");
        return(1);
    }
    printf("port opened\n");

    seq=17;
    firstaddr=0xFFFFFFFF;

    //Open File. Read Data. Send to Serial Port
    if( readhex(fp) )
    {
        return(1);
    }

    printf("Entry point: 0x%08X\n",firstaddr);

    jump_pkt.begin_sync = 0x7C;
    jump_pkt.datalen = 0;
    jump_pkt.invdatalen = ~0;
    jump_pkt.type = branch_cmd;
    jump_pkt.passfail = 0;
    jump_pkt.addr = firstaddr;
    jump_pkt.end_sync = 0x7D;

    for(rd=0,cksum=0;rd<ra;rd++) cksum+=sdata[rd];
    jump_pkt.cksum=(~cksum)&0xFF;

    //for(rd=0,cksum=0x00;rd<ra;rd++) cksum+=sdata[rd];
    //cksum&=0xFF;

    ser_senddata(sdata,ra);

    ser_close();
    printf("\n\n");
    return(0);
}

