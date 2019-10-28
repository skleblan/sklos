#include "uart.h"
#include "vectors.h"
#include "bootldr.h"

void load_new_program()
{
  unsigned char buffer[0xF]; //bigger buffer?
  //int rcvd_len = 0;
  bootldr_pkt_t* packet;
  //char* cur_start_addr;
  //int i = 0;
  //int tempaddr;

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

