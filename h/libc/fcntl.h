#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( none )
#endif
#pragma info( restore )
#endif

#ifndef __fcntl_h
   #define __fcntl_h

   #ifdef __cplusplus
      extern "C" {
   #endif

   /********************************************************************/
   /*  <fcntl.h> header file                                           */
   /*                                                                  */
   /*  (C) Copyright IBM Corp. 1991, 1995.                             */
   /*  - Licensed Material - Program-Property of IBM                   */
   /*  - All rights reserved                                           */
   /*                                                                  */
   /********************************************************************/

   #if (defined( __SPC__ ) ||  defined(__EXTENDED__))
      #define O_RDONLY        0x00000004  /* open for read only */
      #define O_WRONLY        0x00000001  /* open for write only */
      #define O_RDWR          0x00000002  /* open for read and write */
      #define O_APPEND        0x00000008  /* writes done at eof */
      #define O_CREAT         0x00000100  /* create and open file */
      #define O_TRUNC         0x00000200  /* open and truncate    */
      #define O_EXCL          0x00000400  /* open only if file doesn't already exist */
      #define O_TEXT          0x00004000  /* file mode is text (translated) */
      #define O_BINARY        0x00008000  /* file mode is binary (untranslated) */
      #define O_NOINHERIT     0x00000080  /* child process doesn't inherit file */
      #define O_RAW           O_BINARY
   #endif

   #ifdef __cplusplus
      }
   #endif

#endif

#if __IBMC__ || __IBMCPP__
#pragma info( none )
#ifndef __CHKHDR__
   #pragma info( restore )
#endif
#pragma info( restore )
#endif

