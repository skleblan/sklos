#include "uart.h"

enum bootldr_pkt_type
{
  addr_start,
  data,
  branch_cmd
};

struct bootldr_pkt_struct
{
  char begin_sync;
  char datalen;
  char invdatalen;
  enum bootldr_pkt_type type;
  unsigned char sequence;
  char passfail;
  unsigned char data[8];
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
  char buffer[0xF]; //bigger buffer?
  int rcvd_len = 0;
  bootldr_pkt_t* packet;
  char* cur_start_addr;
  int i = 0;

  while(1)
  {
    read(buffer, 0xF);
    packet = buffer;
    
    //check packet sync. begin and end
    
    //check checksum
    
    //check inverted datalength
    
    switch(packet->type)
    {
      case addr_start:
	cur_start_addr = 0;
	for(i = 0; i < 4; i++)
	{
	  cur_start_addr |= packet->data[i]<<(8*(i-1));
	}
	break;
      case data:
	break;
      case branch_cmd:
	break;
    }

  }
}

