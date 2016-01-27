/*------------------------------------------------------------------*/
/*             TYPE DEFINITIONS FOR DATA DECLARATIONS               */
/*------------------------------------------------------------------*/

  typedef unsigned char byte;
  #ifdef E32TO16
  typedef unsigned short word;
  typedef unsigned long dword;
  #pragma seg16(address)
  typedef unsigned char * _Seg16 address;
  #else
  typedef unsigned short int word;
  typedef unsigned long int dword;
  typedef unsigned char far * address;
  #endif

