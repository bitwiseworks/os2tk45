
#ifdef E32TO16
     extern ACSLAN (char *,char *);
     #pragma linkage(ACSLAN, far16 pascal)
#else
     extern unsigned pascal far ACSLAN (char far *,char far *);
#endif
