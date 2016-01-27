/****************************** Module Header ******************************\
*
* Module Name: LAYOUT.H
*
* OS/2 Layout Utility include file.
*
* Copyright (c) International Business Machines Corporation 1993
*
\***************************************************************************/

#ifndef INCL_LAYOUT   /* Eliminate multiple definitions... */
   #define INCL_LAYOUT
   
   
   /**************************************************************************/
   /***                      Layout Utility constants                      ***/
   /**************************************************************************/
   
   
   /*** Locales ***************************************************************/
   
   #define Locale_Arabic   "Ar_AA"
   #define Locale_Hebrew   "Iw_IL"
   
   
   /*
   * Layout Values Names used for LayoutSet/QueryValues()
   * Types are defined as ULONG (32 bits) split into 2 parts
   * Lower 16 bits  - LayoutTextDescriptor
   * Higher 16 bits - any other type
   */
   
   /*** Layout text descriptors. These Names can be ORed together *************/
   
   #define AllTextDescptors        0x0000ffff
   #define Orientation             0x00000001
   #define TypeOfText              0x00000002
   #define Swapping                0x00000004
   #define Numerals                0x00000008
   #define TextShaping             0x00000010
   #define ArabicSpecialShaping    0x00000020
   #define ArabicOneCellShaping    0x00000040
   #define Word_Break              0x00000080
   #define BidiType                0x00000100
   
   /*** General Layout Value names. These names should NOT be ORed together ***/
   
   #define ActiveShapeEditing      (0x0001<<16)
   #define ActiveBidirection       (0x0002<<16)
   #define ShapeCharset            (0x0003<<16)
   #define ShapeCharsetSize        (0x0004<<16)
   #define ShapeContextSize        (0x0005<<16)
   #define CellSize                (0x0006<<16)
   #define InputMode               (0x0007<<16)
   #define InOnlyTextDescr         (0x0008<<16)
   #define OutOnlyTextDescr        (0x0009<<16)
   #define InOutTextDescrMask      (0x000a<<16)
   #define CallerAllocMem          (0x000b<<16)
   
   /*
   * QueryValueSize can be ORed with any of
   * the above Names at query time to determine
   * the amount of memory needed for that value
   */
   #define QueryValueSize          (0x8000<<16)
   
   
   /*
   * Bidirectional attributes values (in Bidi attributes word)
   * To be used as values for the following Layout text descriptors:
   *   - InOnlyTextDescr
   *   - OutOnlyTextDescr
   *
   * Note: These bidi attributes value must match the BDA_XXX
   *       values in pmbidi.h.
   */
   
   /* Text Type */
   #define TEXT_VISUAL             0x00000000
   #define TEXT_IMPLICIT           0x01000000
   #define TEXT_EXPLICIT           0x02000000
   
   /* Bidi Type */
   #define BIDI_DEFAULT            0x00000000
   #define BIDI_UCS                0x04000000
   
   /* Text Orientation */
   #define ORIENTATION_LTR         0x00000000
   #define ORIENTATION_RTL         0x00010000
   
   /* ORIENTATION_CONTEXT_LTR is equivalent  */
   /* to BDA_TEXT_ORIENT_CONTEXT in pmbidi.h */
   #define ORIENTATION_CONTEXT_LTR 0x00020000
   /* ORIENTATION_CONTEXT_RTL has */
   /* no equivalent in pmbidi.h   */
   #define ORIENTATION_CONTEXT_RTL 0x00030000
   
   /* Numerals */
   #define NUMERALS_NOMINAL        0x00000000
   #define NUMERALS_NATIONAL       0x00002000
   #define NUMERALS_CONTEXTUAL     0x00003000
   
   /* Word Break */
   #define BREAK                   0x00000200
   #define NO_BREAK                0x00000000
   
   /* Symmetric Swapping */
   #define SWAPPING                0x00000100
   #define NO_SWAPPING             0x00000000
   /* Text shape */
   #define TEXT_SHAPED             0x00000000
   #define TEXT_NOMINAL            0x00000010
   #define TEXT_INITIAL            0x00000011
   #define TEXT_MIDDLE             0x00000012
   #define TEXT_FINAL              0x00000013
   #define TEXT_ISOLATED           0x00000014
   
   
   /*** Edit type values used In LayoutEditShape  *************************/
   #define EDITINPUT               0
   #define EDITREPLACE             1
   
   
   /**************************************************************************/
   /***                          ERROR CODES                               ***/
   /**************************************************************************/
   #define E2BIG            999
   
   /**************************************************************************/
   /***                          STRUCTURES                                ***/
   /**************************************************************************/
   
   #pragma pack (2)                     /* force structure alignment packing */
   
   #define LAYOUT_VALUES LayoutValueRec
   typedef struct _LAYOUT_VALUES
   {
      ULONG   name;                              /* Name of Layout Value Item */
      PVOID   value;                             /* Data of Layout Value Item */
   } LAYOUT_VALUES ;
   
   #define LAYOUT_EDIT_SIZE LayoutEditSizeRec
   typedef struct _LAYOUT_EDIT_SIZE  
   {
      ULONG front;                                          /* previous chars */
      ULONG back;                                         /* succeeding chars */
   } LAYOUT_EDIT_SIZE;
   
   #define LAYOUT_TEXT_DESCRIPTOR LayoutTextDescriptorRec
   typedef struct _LAYOUT_TEXT_DESCRIPTOR
   {
      ULONG in;                                    /* input buffer description */
      ULONG out;                                         /* output description */
   } LAYOUT_TEXT_DESCRIPTOR;
   
   #pragma pack()                              /* restore packing to default */
   
   #define LAYOUT_OBJECT LayoutObject
   typedef PVOID  LAYOUT_OBJECT;
   
   typedef PVOID *PLAYOUT_OBJECT;
   
   #define PLAYOUT_VALUES LayoutValues
   typedef LAYOUT_VALUES *PLAYOUT_VALUES;
   
   #define PLAYOUT_EDIT_SIZE LayoutEditSize
   typedef LAYOUT_EDIT_SIZE *PLAYOUT_EDIT_SIZE;
   
   #define PLAYOUT_TEXT_DESCRIPTOR LayoutTextDescriptor
   typedef LAYOUT_TEXT_DESCRIPTOR *PLAYOUT_TEXT_DESCRIPTOR;
   
   /************************************************************************/
   /***                Layout Text Conversion Functions                  ***/
   /************************************************************************/
   
   #define LayoutCreateObject  layout_object_create
   #define LayoutDestroyObject layout_object_destroy
   #define LayoutTransformText layout_object_transform
   #define LayoutEditShape     layout_object_editshape
   #define LayoutSetValues     layout_object_setvalues
   #define LayoutQueryValues   layout_object_getvalues
   
   APIRET APIENTRY LayoutCreateObject(const PUCHAR locale_name,
                                      PLAYOUT_OBJECT plh);
   
   APIRET APIENTRY LayoutDestroyObject(LAYOUT_OBJECT  plh);
   
   APIRET APIENTRY LayoutSetValues(    LAYOUT_OBJECT  plh,
                                   PLAYOUT_VALUES values,
                                   PULONG         index_returned);
   
   
   APIRET APIENTRY LayoutQueryValues(  LAYOUT_OBJECT  plh,
                                     PLAYOUT_VALUES values,
                                     PULONG         index_returned);
   
   APIRET APIENTRY LayoutTransformText(LAYOUT_OBJECT  plh,
                                       PUCHAR         InpBuf,
                                       PULONG         InpSize,
                                       PVOID          OutBuf,
                                       PULONG         OutSize,
                                       PULONG         InpToOut,
                                       PULONG         OutToInp,
                                       PUCHAR         BidiLevel);
   
   APIRET APIENTRY LayoutEditShape(    LAYOUT_OBJECT  plh,
                                   BOOL           EditType,
                                   PULONG         index,
                                   PUCHAR         InpBuf,
                                   PULONG         InpSize,
                                   PVOID          OutBuf,
                                   PULONG         OutSize);
   
   
#endif  /* INCL_LAYOUT */
