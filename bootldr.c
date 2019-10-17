#include "uart.h"

extern void BRANCHTO( unsigned int );

enum bootldr_pkt_type
{
//  addr_start,
  data,
  branch_cmd
};

struct bootldr_pkt_struct
{
  char begin_sync;
  char datalen;
  char invdatalen;
  enum bootldr_pkt_type type;
  unsigned char sequence_dnu;
  char passfail;
  unsigned int addr;
  unsigned int data;
  char end_sync;
  char cksum;
};

#if 0
union bootldr_pkt
{
  unsigned char bytes[12];
  struct bootldr_pkt_struct struct;
};
#endif

typedef struct bootldr_pkt_struct bootldr_pkt_t;

void load_new_program()
{
  unsigned char buffer[0xF]; //bigger buffer?
  int rcvd_len = 0;
  bootldr_pkt_t* packet;
  char* cur_start_addr;
  int i = 0;
  int tempaddr;

  while(1)
  {
    read(buffer, 0xF);
    packet = (bootldr_pkt_t*) buffer;
    
    //check packet sync. begin and end
    
    //check checksum
    
    //check inverted datalength
    
    switch(packet->type)
    {
      case data:
	PUT32(packet->addr, packet->data);
	write((unsigned char*)packet, 0xF);
	break;
      case branch_cmd:
	BRANCHTO(packet->addr);
	break;
    }

  }
}

