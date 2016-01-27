/***!Start!***                                                                     
 *                                                                                 
 *   ORIGINS: 27                                                                   
 *                                                                                 
 *   IBM CONFIDENTIAL -- (IBM Confidential Restricted when                         
 *   combined with the aggregated modules for this product)                        
 *   OBJECT CODE ONLY SOURCE MATERIALS                                             
 *                                                                                 
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996 
 *   All Rights Reserved                                                           
 *   Licensed Materials - Property of IBM                                          
 *   The source code for this program is not published or otherwise divested       
 *   of its trade secrets, irrespective of what has been deposited with the        
 *   U.S. Copyright Office.                                                        
 *                                                                                 
 ***!End!***/                                                                      
                                                                                   
/*
 * @(#) 1.4.1.1 src/somk/somtdbcs.c, somk.sys, som2.1 1/3/96 13:16:46 [7/30/96 15:23:26]
 */

#ifndef somtdbcs_c
#define somtdbcs_c

/*
 * -- somtinitDbcs --
 * This routine initialises the somtisDbcs routine for OS/2, AIX, DOS, etc.
 */

#if defined(_WDOS) || defined(_WIN16)
   #define CP_JAPAN     932
   #define CP_KOREA     934
   #define CP_SCHINESE  936
   #define CP_TCHINESE  938

   #ifdef _WDOS
       #include <dos.h>
       #define DOS_GET_DBCP   0x6601
       #define DOS_GET_DBCSEV 0x6300

	static int GetKBCodePage(void)
	{
	    union REGS regs;
	    regs.x.ax = DOS_GET_DBCP;
	    intdos(&regs, &regs);

	    return (regs.x.bx);
	}
    #endif /* _WDOS  */

#elif defined(__OS2__)

#else /* AIX */

   #include <nl_types.h>
   #include <locale.h>

#endif


static void somtinitDbcs(char *dbcsBuf, size_t bufsize)
{
#if defined(_WDOS) || defined(_WIN16)
    int cp = GetKBCodePage();

    switch (cp) {
	case CP_JAPAN:
	    dbcsBuf[0] = 0x81;
	    dbcsBuf[1] = 0x9f;
	    dbcsBuf[2] = 0xe0;
	    dbcsBuf[3] = 0xfc;
	    dbcsBuf[4] = 0;
	    dbcsBuf[5] = 0;
	    break;

	case CP_KOREA:
	    dbcsBuf[0] = 0x81;
	    dbcsBuf[1] = 0xbf;
	    dbcsBuf[2] = 0;
	    dbcsBuf[3] = 0;
	    break;

	case CP_SCHINESE:
	    dbcsBuf[0] = 0x81;
	    dbcsBuf[1] = 0xfc;
	    dbcsBuf[2] = 0;
	    dbcsBuf[3] = 0;
	    break;

	case CP_TCHINESE:
	    dbcsBuf[0] = 0x81;
	    dbcsBuf[1] = 0xfc;
	    dbcsBuf[2] = 0;
	    dbcsBuf[3] = 0;
	    break;

	default:
	    dbcsBuf[0] = 0;
	    dbcsBuf[1] = 0;
	    break;
    }
#elif __OS2__
    COUNTRYCODE    cc;

    cc.country  = 0; /* Use default country */
    cc.codepage = 0; /* Use current codepage */
    if (DosQueryDBCSEnv(bufsize, &cc, dbcsBuf)) {
	dbcsBuf[0] = '\0';
	dbcsBuf[1] = '\0';
    }
#else
    setlocale(LC_CTYPE, "");
#endif /* AIX */
}

#endif /* somtdbcs_c */
