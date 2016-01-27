/**********************************************************************\
 *                                                                    *
 *               Copyright (c) IBM Corporation  1991, 1992            *
 *                         All Rights Reserved                        *
 *                                                                    *
\**********************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif


#pragma pack(1)

#define MIDINAMESIZE       40
#define MIDIPATCHNUM      128
#define MIDITYPEAPPNAME   "MidiTypes"     /* Appname in MIDITYPE.INI */

/********************************************************************/
/* These structures describe the format of the MIDI maps that are   */
/* located in the MIDITYPE.INI file.                                */
/********************************************************************/

typedef struct {
   USHORT uGenMidiPatchNumber;   /* Device To General Midi Conversion */
   USHORT uDevMidiPatchNumber;   /* General Midi to Device Conversion */
   USHORT uVolumePercent;        /* Channel Patch Volume Scaler */
   USHORT uGenMidiKeyNumber;     /* Device To General Midi Perc. Key Conversion */
   USHORT uDevMidiKeyNumber;     /* General Midi to Device Perc. Key Conversion */
} MIDITYPEENTRY;
typedef MIDITYPEENTRY FAR * PMIDITYPEENTRY;

typedef struct {
   USHORT     uStyle;           /* Midi Style */
   USHORT     uDrums10;         /* Patch 10 Drums if Perckey not supported */
   USHORT     uDrums16;         /* Patch 16 Drums if Perckey not supported */
   ULONG      ulReserved;       /* Reserved */
   MIDITYPEENTRY MidiTypeEntry[MIDIPATCHNUM]; /* Array of MIDITYPEENTRYs */
   CHAR       szPatchAndPercKeyName[2*MIDIPATCHNUM*MIDINAMESIZE];
                                /* List of 128 Patch Names */
                                /* that are null terminated, then a */
                                /* list of 128 Percussion key names that */
                                /* are double null terminated */
                                /* Each item is null terminated */
} MIDITYPE;
typedef MIDITYPE FAR * PMIDITYPE;

/* Style Flags */

#define MT_PERCUSSIONKEYS     0x1  /* Percussion Keys are supported */
#define MT_MAPPERCKEYPATCHES  0x2   /* Map Percussion Key Patches */

#pragma pack()

#ifdef __cplusplus
}
#endif

