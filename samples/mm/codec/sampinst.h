/*****************************************************************************/
/*                                                                           */
/* SAMPINST.H: Sample Codec instance data definition.  This instance         */
/*             structure holds both the encode and decode specific items.    */
/*                                                                           */
/* Copyright (c) IBM Corporation 1991,1994             All Rights Reserved   */
/*                                                                           */
/*****************************************************************************/

typedef struct _main_inst
   {
   BOOL   fInstanceInUse;    /* Flag holding state of instance.              */
   ULONG  ulWidth;           /* The frame width for the instance.            */
   ULONG  ulHeight;          /* The frame height for the instance.           */
   ULONG  ulColorEncoding;   /* The color format of the source data.         */
   } MAIN_INST;

typedef MAIN_INST *PMAIN_INST;
