/***************************************************************************\
*                                                                           *
*    Multimedia Mixer Software Development Kit Include File                 *
*                                                                           *
*                                                                           *
*                                                                           *
\***************************************************************************/
#ifdef __cplusplus
   extern "C" {
#endif

#ifndef _OS2MIXER_H_
#define _OS2MIXER_H_
typedef ULONG  HMIXER;

typedef HMIXER FAR *PHMIXER;


typedef struct _MIXERLINEINFO {
    ULONG     ulLength;              /*length of the struct */
    ULONG     ulNumChannels;     /* number of channels on input */
    ULONG     ulSupport;             /* supported functionality (MIXLINEINFO) */
    ULONG     ulConnectionsPossible; /* lines connectable to */
     ULONG    ulLine;                /* which line to operate on */
 } MIXERLINEINFO;

typedef MIXERLINEINFO FAR *PMIXERLINEINFO;

typedef struct {
        ULONG   ulLength;      /* length of the struct */
        ULONG   ulConnection;
        ULONG   ulLine;
        ULONG   ulFlags;
} LINECONNECTIONS;
typedef LINECONNECTIONS FAR *PLINECONNECTIONS;


/* flags for MIXERCONTROL ulFlags field */

#define MIX_MASTER        0x00000001L  /* indicates that this is master effect for the control */

typedef struct _MIXERCONTROL{
        ULONG   ulLength;      /* length of the struct */
        ULONG   ulLine;
        ULONG   ulControl;
        ULONG   ulSetting;
        ULONG   ulFlags;       /* Flags which modify MIXERCONTROL */
        } MIXERCONTROL;
typedef MIXERCONTROL *PMIXERCONTROL;

/* -- need masteraudio command for the following. */


/* flags for MIXERLINEINFO ulSupport field */

#define MIX_BALANCE       0x00100000L     /* separate balance volume control */
#define MIX_ALC           0x00000004L         /* supports Auto Level Control */
#define MIX_MONITOR       0x00000002L     /* supports Monitor Control */
#define MIX_CROSSOVER     0x00000010L   /* supports crossover change */
#define MIX_LOUDNESS      0x00000020L    /* supports loudness equalization */
#define MIX_MUTE          0x00000040L        /* supports channel mute */
#define MIX_REVERB        0x00000080L      /* supports reverb */
#define MIX_STEREOENHANCE 0x00000100L /* supports stereo enhance */
#define MIX_CUSTOM1       0x00000200L     /* supports custom effect #1 */
#define MIX_CUSTOM2       0x00000400L     /* supports custom effect #2 */
#define MIX_CUSTOM3       0x00000800L     /* supports custom effect #3 */
#define MIX_LRVOLUME      0x00001000L    /* separate left-right volume control */
#define MIX_BASS          0x00800000L        /* supports Bass */
#define MIX_MID           0x00020000L         /* supports Mid */
#define MIX_TREBLE        0x00400000L      /* supports Treble */
#define MIX_PITCH         0x00200000L       /* supports pitch modifications */
#define MIX_GAIN          0x01000000L        /* supports gain modifications */
#define MIX_CHORUS        0x00004000L      /* supports Bass */
#define MIX_VOLUME        0x00080000L      /* supports volume controls */


#define MIX_STEREO_OFF            0x00000000
#define MIX_STEREO_STUDIO         0x40004000
#define MIX_STEREO_HALL           0x80008000
#define MIX_STEREO_STADIUM        0xc000c000

#define MCI_STEREO_OFF            0x00000000
#define MCI_STEREO_STUDIO         0x00000001
#define MCI_STEREO_HALL           0x00000002
#define MCI_STEREO_STADIUM        0x00000004


#define MIX_LOUDNESS_OFF          0x00000000
#define MIX_LOUDNESS_ON           0xFFFFFFFF

#define MCI_LOUDNESS_OFF          0x00000000
#define MCI_LOUDNESS_ON           0x00000001



/* ERROR MESSAGES */

#define MIXERR_BASE            512

#define    MIXERR_NOERR              0
#define    MIXERR_INVALIDHANDLE      MIXERR_BASE+0
#define    MIXERR_INVALIDINPUT       MIXERR_BASE+1
#define    MIXERR_INVALIDOUTPUT      MIXERR_BASE+2
#define    MIXERR_NOTSUPPORTED       MIXERR_BASE+7




/* INPUT LINES */
/* SOURCES */
#define   IN_SYNTHESIZER   0
#define   IN_MIXER         1
#define   IN_EXTERNAL      2
#define   IN_INTERNAL      3
#define   IN_MICROPHONE    4
#define   IN_PCM           5
#define   IN_PC_SPEAKER    6

/* SINKS */

#define   OUT_AMPLIFIER    0
#define   OUT_PCM          1


/* SOURCES */
#define   SOURCE_SYNTHESIZER          0x000000001
#define   SOURCE_LINE                 0x000000002
#define   SOURCE_INTERNAL_AUDIO       0x000000004
#define   SOURCE_MICROPHONE           0x000000008
#define   SOURCE_WAVE                 0x000000010
#define   SOURCE_PC_SPEAKER           0x000000020
#define   SOURCE_NULL                 0x000000040
#define   SOURCE_MIDI                 0x000000080

/* SINKS */
/* lad--these defines are invalid. */

#define   SINK_LINE_OUT               0x000100000
#define   SINK_SPEAKER                0x000200000
#define   SINK_HEADPHONES             0x000400000
#define   SINK_NULL                   0x000800000
#define   SINK_ALL                    0x001000000
/* lad--wpos addition */
#define   SINK_WAVE                   0x002000000
#define   SINK_MIDI                   0x004000000 /* field name???? */

/* lad--wpos addition */


#endif

#ifdef __cplusplus
}
#endif

