#ifndef __BOOTLDR_H__
#define __BOOTLDR_H__

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

#endif
