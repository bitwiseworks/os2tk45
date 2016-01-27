/*static char *SCCSID = "@(#)convconv.c	13.2 92/01/23";*/
#pragma title ("CONVCONV.C - Case Converter Example Routines")

/********************** START OF SPECIFICATIONS *****************************/
/*                                                                          */
/* SOURCE FILE NAME: CONVCONV.C                                             */
/*                                                                          */
/* DESCRIPTIVE NAME: CONV converter Procedure Example Routines.             */
/*                                                                          */
/* COPYRIGHT:                                                               */
/*                  Copyright (c) IBM Corporation  1992, 1993               */
/*                         All Rights Reserved                              */
/*                                                                          */
/* STATUS: OS/2 MM Release 1.10                                             */
/*                                                                          */
/* FUNCTION: This module contains case conversion routines for the          */
/*           File Format I/O Procedure.                                     */
/*                                                                          */
/* NOTES:                                                                   */
/*    DEPENDENCIES: NONE                                                    */
/*    RESTRICTIONS: Runs in protect mode only                               */
/*                                                                          */
/* ENTRY POINTS:                                                            */
/*                                                                          */
/*   ROUTINES: convhlpToUpper                                               */
/*             convhlpToLower                                               */
/*                                                                          */
/* EXTERNAL REFERENCES:                                                     */
/*                                                                          */
/* MODIFICATION HISTORY:                                                    */
/* DATE      DEVELOPER         CHANGE DESCRIPTION                           */
/* 09/30/91  JWO               Creation.                                    */
/*                                                                          */
/*********************** END OF SPECIFICATIONS ******************************/

#define INCL_NOPMAPI                   // no PM include files required
#define INCL_DOSRESOURCES
#define INCL_CONVPROC                  // use only include CONV proc info

#include <os2.h>
#include <mmioos2.h>                   // mmio standard defines/prototypes
#include "convproc.h"                  // header for CONV procedure

#pragma subtitle ("convhlpToUpper - Convert text to upper case.")
#pragma page ()

/****************************************************************************
 *  This routine will accept a NULL terminated buffer of characters and     *
 *  convert them to upper case characters.  The characters are mapped to    *
 *  the characters in the szMap  array by their ASCII value.  Therefore,    *
 *  the uppercase ASCII chars are where the lowercase ASCII chars are.      *
 *  All the other characters are left to be their normal ASCII value.       *
 ****************************************************************************/

VOID APIENTRY convhlpToUpper( PUCHAR psz )
{
   static CHAR szMap[]  =
      {
       0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
      18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, ' ', '!',
      '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.',
      '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
      ':', ';', '<', '=', '>', '?', '@',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
      'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      '[', '\\', ']', '^', '_', '`',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
      'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      '{', '|', '}', '~', 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
      137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
      151, 152, 153, 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 162,
      163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
      177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
      191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
      205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
      219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
      233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
      247, 248, 249, 250, 251, 252, 253, 254, 255
      };

   while (*psz)
      {
      *psz = *(szMap  + (*psz));
      psz++;
      }
}

#pragma subtitle ("convhlpToLower - Convert text to lower case.")
#pragma page ()

/****************************************************************************
 *  This routine will accept a NULL terminated buffer of characters and     *
 *  convert them to lower case characters.  The characters are mapped to    *
 *  the characters in the szMap  array by their ASCII value.  Therefore,    *
 *  the lowercase ASCII chars are where the uppercase ASCII chars are.      *
 *  All the other characters are left to be their normal ASCII value.       *
 ****************************************************************************/

VOID APIENTRY convhlpToLower( PUCHAR psz )
{
   static CHAR szMap[]  =
      {
       0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17,
      18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, ' ', '!',
      '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.',
      '/', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
      ':', ';', '<', '=', '>', '?', '@',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
      'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '[', '\\', ']', '^', '_', '`',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
      'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '{', '|', '}', '~', 127, 128, 129, 130, 131, 132, 133, 134, 135, 136,
      137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150,
      151, 152, 153, 154, 155, 156, 157, 158, 158, 159, 160, 161, 162, 162,
      163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
      177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
      191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
      205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
      219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
      233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246,
      247, 248, 249, 250, 251, 252, 253, 254, 255
      };

   while (*psz)
      {
      *psz = *(szMap  + (*psz));
      psz++;
      }
}
