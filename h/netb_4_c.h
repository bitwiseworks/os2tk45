#ifdef E32TO16
extern unsigned NETBIOS (char *);
#pragma linkage(NETBIOS, far16 pascal)      /* added for 32 bit compiler */
#else
     extern unsigned pascal far NETBIOS (char far *);
#endif
