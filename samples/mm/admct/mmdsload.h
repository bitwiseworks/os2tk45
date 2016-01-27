/************************* START OF SPECIFICATIONS **************************/
/* SOURCE FILE NAME: mmdsload.h                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: Load a file via DosLoadModule but with full path       */
/*                                                                          */
/* COPYRIGHT:               IBM Confidential                                */
/*              Copyright (c) IBM Corporation  1990, 1991, 1992             */
/*                        All Rights Reserved                               */
/*                                                                          */
/* STATUS: MME  Release Warp (4meg)                                         */
/*                                                                          */
/* FUNCTION: Load a file via DosLoadModules but with full path              */
/*                                                                          */
/*                                                                          */
/* NOTES:                                                                   */
/*    DEPENDENCIES: NONE                                                    */
/*    RESTRICTIONS: NONE                                                    */
/*                                                                          */
/* ENTRY POINTS: MM_DosLoadModule & QueryMMBase                             */
/*                                                                          */
/* ROUTINES: MM_DosLoadModule                                               */
/*           QueryMMBase                                                    */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*                                                                          */
/*                                                                          */
/* MODIFICATION HISTORY:                                                    */
/* DATE      DEVELOPER                CHANGE DESCRIPTION                    */
/* 04/21/94  LL Post                  original creation                     */
/*********************** END OF SPECIFICATIONS ******************************/
ULONG APIENTRY MM_DosLoadModule(PSZ      pszNameBuf,
                                ULONG    ulNameBuf,
                                PSZ      pszName,
                                PHMODULE ppHMod);

USHORT QueryMMBASE (PSZ pszBasePath);

extern CHAR           pszDLL_Path[CCHMAXPATH+1];
