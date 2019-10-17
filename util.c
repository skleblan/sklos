#include "util.h"

unsigned int strlen(const char* txt, unsigned int maxlen)
{
  unsigned int idx = 0;
  while(txt[idx] != 0 && idx < maxlen) idx++;
  return idx;
}

signed int strncmp(const char* first, const char* second, unsigned int len)
{
  int i;
  for(i = 0; i < len; i++)
  {
    if(first[i] > second[i]) return 1;
    if(first[i] < second[i]) return -1;
  }
  return 0;
}

int max(int first, int second)
{
  if(first > second) return first;
  return second;
}

int min(int first, int second)
{
  if(first < second) return first;
  return second;
}
