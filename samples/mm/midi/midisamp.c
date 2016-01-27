/***************************************************************************
*
*  File Name   : midisamp.c
*
*  Description : This module contains a sample illustrating
*                use of the new real-time MIDI APIs
*
*  Command
*  Line Syntax : midisamp [filename.mid] [-debug]
*
*
*  Notes       : The basic structure of this program can be
*                broken down as follows:
*
*                [ init -----> query   (version, classes, instances)
*                [           > setup   (create/enable instances, add links,
*                [                      setup MIDI timer)
*                [           > parse   (demonstrates how to parse MIDI data)
*
*                [ play                (send message)
*
*                [ cleanup             (stop timer, remove links, disable/
*                [                      delete instances)
*
*
*  DEPENDENCIES: MMPM/2 must be installed
*
*  MIDI ROUTINES ILLUSTRATED (in order used):
*
*                MIDIQueryVersion
*                MIDIQueryNumClasses
*                MIDIQueryClassList
*                MIDICreateInstance
*                MIDIQueryInstanceName
*                MIDIQueryNumInstances
*                MIDIQueryInstanceList
*                MIDIAddLink
*                MIDIEnableInstance
*                MIDISetup
*                MIDITimer
*                MIDISendMessages
*                TimerSleep
*                MIDIRemoveLink
*                MIDIDisableInstance
*                MIDIDeleteInstance
*
*
*
*       (C) COPYRIGHT International Business Machines Corp. 1995
*       All Rights Reserved
*       Licensed Materials - Property of IBM
*
*       US Government Users Restricted Rights - Use, duplication or
*       disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*
*       DISCLAIMER OF WARRANTIES.  The following [enclosed] code is
*       sample code created by IBM Corporation. This sample code is not
*       part of any standard or IBM product and is provided to you solely
*       for the purpose of assisting you in the development of your
*       applications.  The code is provided "AS IS", without
*       warranty of any kind.  IBM shall not be liable for any damages
*       arising out of your use of the sample code, even if they have been
*       advised of the possibility of such damages.
*
***************************************************************************/


#define INCL_BASE
#define INCL_ERRORS
#define INCL_NOPMAPI
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSFILEMGR
#define INCL_DOSQUEUES
#define INCL_DOSPROCESS
#define INCL_DOSMEMMGR

#define  MIDIVER_MAJOR   0xC000    // used for MIDIQueryVersion
#define  MIDIVER_MINOR   0x3C00
#define  MIDIVER_BUGFIX  0x03C0
#define  MIDIVER_PHASE   0x0030
#define  MIDIVER_BUILD   0x000F

#define  HARDWARE_NOT_FOUND            9000              // used for debug
#define  HARDWARE_UNABLE_TO_RECEIVE    9001
#define  MMIOOPEN_ERROR                9002

#define  MIDI_HARDWARE_CLASS_NAME      "Hardware\0"      // used to search for hardware class number
#define  MIDI_APPLICATION_CLASS_NAME   "Application\0"   // used to search for application class number
#define  INIT_INSTANCE  1        // initial instance of MIDI



#include <os2.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <os2me.h>
#include <meerror.h>
#include <mididll.h>
#include <mmioos2.h>
#include <midios2.h>





// Global Variables

ULONG  ulTempo    = 1200;    // initialize current tempo to 120 beats per minute

ULONG  ulPerClock = 0;       // this will be set by CalcDelay()
USHORT usCPQN     = 0;       // this is set via an IBM SysEx Device Driver Control Message

BOOL  __MIDIDebug = 0;       // debug mode flag

ULONG ulInstanceNum = INIT_INSTANCE;   // initial instance of MIDI




typedef struct {
   ULONG    ulMsgIndex;
   MESSAGE  paMessage[20];
   PVOID    pNext;
} MESSAGELIST;

typedef MESSAGELIST *PMESSAGELIST;





// Function Prototypes

VOID   CalcDelay (void);
USHORT QueryMMBASE (PCHAR pszMapFile);

VOID   check( ULONG rc, CHAR *call_id );     // used to check the return codes from MIDI functions
VOID   spinbar( void );                      // simple spinbar to show activity in debug mode




VOID main (INT argc, CHAR *argv[])
{
   ULONG             ulNumClasses = 0;             // used to query class list
   ULONG             ulNumInstances = 0;           // used to query number of instances
   ULONG             ulSlotNumber = 0;             // used by add link
   ULONG             ulVersion = 0;                // used by query version
   ULONG             ulEndTime = 0;                // used for TimerSleep
   MINSTANCE         mInstance = 0;                // used to query instance number
   MINSTANCE         ulApplInst = 0;               // holds inst # from create instance
   ULONG             ulAppClassNumber = 0;         // class number of app nodes
   ULONG             ulHdwClassNumber = 0;         // class number of hardware nodes
   ULONG             ulHdwInst = 0;                // used to enable instance (hardware)
   PMIDICLASSINFO    paClassInfo = NULL;           // holds class list
   PMIDIINSTANCEINFO paInstanceInfo = NULL;        // holds instance list
   CHAR              szMidi1[MIDI_NAME_LENGTH];    // used to create app instance
   CHAR              szMidi2[MIDI_NAME_LENGTH];    // used to create app instance
   CHAR              szInstName[MIDI_NAME_LENGTH]; // used to query instance name
   PMESSAGE          paMessage;                    // ptr to array of MESSAGE structs
   PMIDISETUP        pMidiSetup;                   // for obtaining MIDI Timer
   ULONG             ulMidiTime;                   // used to obtain current MIDI subsystem time
   PULONG            pulMidiTime;                  // ptr to current MIDI subsystem time
   BOOL              fCanRecv = FALSE;             // used to test attributes
   BOOL              fCanSend = FALSE;             // used to test attributes

   ULONG             ulOpenFlags = 0;              // used for mmioOpen of MIDI file
   HMMIO             hmmio;                        // file handle for MIDI file
   MMIOINFO          mmioinfo;                     // contains parameters used by mmioOpen
   ULONG             ulBytesToRead = 0;            // used by mmioRead
   ULONG             ulBytesRead = 0;              // used by mmioRead
   PBYTE             pMidiBuffer;                  // used by mmioRead
   PBYTE             pBufferPtr;                   // current file position

   ULONG             ulTimeStamp = 0;              // absolute time stamp for messages
   BYTE              bRunningStatus = 0;           // current running status
   BYTE              b = 0;                        // current file byte
   BYTE              bNextByte = 0;                // next file byte
   BYTE              bTestByte = 0;                // keep track of status byte
   USHORT            usTempoLSB = 0;               // first tempo data byte
   USHORT            usTempoMSB = 0;               // second tempo data byte
   USHORT            usLSBCompValue = 0;           // used to calculate wait time
   USHORT            usMSBCompValue = 0;           // used to calculate wait time
   USHORT            usClocks = 0;                 // used to calculate wait time
   PMESSAGELIST      pMessageList = NULL;          // ptr to linked list of message lists
   PMESSAGELIST      pCurrMessageList = NULL;      // ptr to current message list in linked list
   PMESSAGELIST      pNewMessageList = NULL;       // ptr to new message list in linked list
   ULONG             ulMsgNum = 0;                 // counter for messages in message list
   ULONG             ulNumMessages = 0;            // MIDISendMessages number of messages
   ULONG             ulStartTime = 0;              // current MIDI time
   ULONG             ulCurrentTime = 0;            // current MIDI time
   ULONG             ulMaxRTSysexLength;           // maximum allowed length of a real-time SysEx message

   CHAR              szFileName[CCHMAXPATH];       // holds MIDI file name
   CHAR              szFilePath[CCHMAXPATH];       // holds MIDI file name path
   CHAR              szInstance[3] = "1";          // holds instance number

   ULONG             i = 0;                        // index
   ULONG             rc = 0;                       // return code

   // initialize strings

   memset(szMidi1, '\0', MIDI_NAME_LENGTH);        // used to create app instance
   strcpy(szMidi2, "MIDI Sample ");               // used to create app instance
   strcpy(szMidi1, szMidi2);                      // used to create app instance
   _ultoa(ulInstanceNum, szInstance, 10);
   strcat(szMidi1, szInstance);
   memset(szInstName, '\0', MIDI_NAME_LENGTH);     // used to query instance name




// OUTLINE: INIT

   if ( argc > 1 )
      if ( strcmp( argv[argc - 1], "-debug\0" ) == FALSE )
      {
         __MIDIDebug = TRUE;
         setbuf( stdout, 0 );                      // so that spinbar is real-time
      }

   printf("\n\n");





// OUTLINE: QUERY

   // query the RTMIDI.DLL and device driver versions before we begin

   rc = MIDIQueryVersion( &ulVersion );
   check( rc, "MIDIQueryVersion\0" );

   printf("Real-Time MIDI (version %lu.%lu) installed.\n", (ulVersion & MIDIVER_MAJOR) >> 14,
                                                           (ulVersion & MIDIVER_MINOR) >> 10 );

   if (__MIDIDebug)
      printf("     ::Bug fix #%lu, Phase %lu, Build %lu.\n", (ulVersion & MIDIVER_BUGFIX) >> 6,
                                                             (ulVersion & MIDIVER_PHASE) >> 4,
                                                             (ulVersion & MIDIVER_BUILD)  );





   // query the Multimedia base drive and path and change to include
   // "sounds" directory where sample MIDI files are located

   QueryMMBASE ((PCHAR)szFilePath);
   strcat((PCHAR)szFilePath, "\\SOUNDS\\");

   if ( (argc > 2) || ((argc == 2) && !__MIDIDebug) )  // parse command line arguments
   {                                   // command line syntax:  midisamp [midifile.mid] [-debug]
      strcpy(szFileName, argv[1]);
   }
   else                                // else prompt for midi filename
   {
      printf("\nPlease provide a MIDI file from MMOS2\\SOUNDS: (ex: bach.mid) \n");
      scanf("%s", szFileName);
   }

   strcat((PCHAR)szFilePath, szFileName);

   printf("\n\n");





   // query the number of valid MIDI classes; for this beta release,
   // two classes are supported:  Hardware and Application.
   // this information will be used for MIDIQueryClassList

   printf("querying class information...\n");

   rc = MIDIQueryNumClasses(&ulNumClasses, 0);
   check( rc, "MIDIQueryNumClasses\0" );



                   // allocate a buffer to store information on each class

   paClassInfo = (PMIDICLASSINFO) malloc((sizeof(MIDICLASSINFO) * ulNumClasses));

                   // query information on each class:  class number, class name,
                   // number of slots supported, and class attributes;
                   // ulNumClasses equals two

   rc = MIDIQueryClassList(ulNumClasses, paClassInfo, 0);
   check( rc, "MIDIQueryClassList\0" );





   // now that we have the list of valid classes, find the class number which
   // identifies hardware nodes and application nodes (we'll use this later
   // when we enable and subsequently disable the hardware node instance)

   for (i = 0; i < ulNumClasses; i++)
   {
      if (!stricmp(paClassInfo[i].szmClassName, MIDI_HARDWARE_CLASS_NAME))
      {
         ulHdwClassNumber = paClassInfo[i].ulClassNumber;

         printf("   found hardware class.\n");

         if ( __MIDIDebug )
            printf("     ::Hardware class number is %lu\n",ulHdwClassNumber);
      }
      if (!stricmp(paClassInfo[i].szmClassName, MIDI_APPLICATION_CLASS_NAME))
      {
         ulAppClassNumber = paClassInfo[i].ulClassNumber;

         printf("   found application class.\n");

         if ( __MIDIDebug )
            printf("     ::Application class number is %lu\n",ulAppClassNumber);
      }
   }





   // create an application instance; the calling application provides a name
   // for the application instance

   // NOTE:  hardware nodes are names defined by the device driver;
   //        the calling application cannot provide names for hardware
   //        nodes or create hardware instances

   printf("creating application node...\n");

   rc = MIDICreateInstance(ulAppClassNumber, &ulApplInst, szMidi1, 0);

   while ( rc == MIDIERR_DUPLICATE_INSTANCE_NAME )
   {
      strcpy(szMidi1, szMidi2);                      // used to create app instance
      ulInstanceNum ++;
      _ultoa(ulInstanceNum, szInstance, 10);
      strcat(szMidi1, szInstance);
      rc = MIDICreateInstance(ulAppClassNumber, &ulApplInst, szMidi1, 0);
   }

   check( rc, "MIDICreateInstance\0" );

   if ( __MIDIDebug )
      printf("     ::Application node is %lx\n",ulApplInst);





   // query the number of instances in preparation for obtaining specific
   // information for each instance:  instance number, class number,
   // instance name, number of links, and instance attributes.
   // this information will be used for MIDIQueryInstanceList

   printf("querying instance information...\n");

   rc = MIDIQueryNumInstances(&ulNumInstances, 0);
   check( rc, "MIDIQueryNumInstances\0" );

   if ( __MIDIDebug )
      printf("     ::Number of instances is %lu\n", ulNumInstances );

                   // allocate a buffer to store information on each instance

   paInstanceInfo = (PMIDIINSTANCEINFO) malloc ((sizeof(MIDIINSTANCEINFO) * ulNumInstances));

                   // ulNumInstances equals three (we created two instances of type
                   // Application class, and there is one hardware node instance)

   rc = MIDIQueryInstanceList(ulNumInstances, paInstanceInfo, 0);
   check( rc, "MIDIQueryInstanceList\0" );





   // now that we have the list of instances, find the hardware node instance
   // number by searching the list of instances for an instance where the
   // class number indicates it is a hardware node

   for (i = 0; i < ulNumInstances; i++)
   {
      if (paInstanceInfo[i].ulClassNumber == ulHdwClassNumber)
      {
         ulHdwInst = paInstanceInfo->minstance;

         printf("   found hardware node.\n");

         if ( __MIDIDebug )
            printf("     ::Hardware node is %lx\n", ulHdwInst );

         break;
      }
   }

   if ( ulHdwInst == 0 )
      check( HARDWARE_NOT_FOUND, "find hardware node number\0" );



                   // query the name of the hardware node; it is also in the
                   // list of instances obtained with MIDIQueryInstanceList

   rc = MIDIQueryInstanceName(ulHdwInst, szInstName, 0);
   check( rc, "MIDIQueryInstanceName\0" );

   if ( __MIDIDebug )
      printf("     ::Hardware node name is %s\n", szInstName );





   // create a link from application instance 1 to the hardware node instance
   // in preparation for sending a message later (MIDISendMessages)

   printf("linking application node to hardware node...\n");

   rc = MIDIAddLink(ulApplInst, ulHdwInst, 0, 0);
   check( rc, "MIDIAddLink\0" );

   if ( __MIDIDebug )
      printf("     ::Linked application node %lx to hardware node %lx\n", ulApplInst, ulHdwInst );





   // find instance info for the hardware node to determine if
   // this hardware node can send and accept (receive) messages;
   // MIDI_INST_ATTR_CAN_RECV = 1, MIDI_INST_ATTR_CAN_SEND = 2

   for (i = 0; i < ulNumInstances; i++)
   {
      if (paInstanceInfo[i].minstance == ulHdwInst)
      {
         if (paInstanceInfo[i].ulAttributes & MIDI_INST_ATTR_CAN_RECV)
         {
            fCanRecv = TRUE;           // instance can receive (accept) messages

            if ( __MIDIDebug )
               printf("     ::Hardware can receive MIDI data.\n");
         }
         if (paInstanceInfo->ulAttributes & MIDI_INST_ATTR_CAN_SEND)
         {
            fCanSend = TRUE;           // instance can send messages

            if ( __MIDIDebug )
               printf("     ::Hardware can send MIDI data.\n");
         }
      }
   }





   // if the hardware node is able to send (establish links from itself to other
   // nodes) and receive (accept links from other nodes to itself), then it is
   // OK to enable this hardware node to send and receive messages

   // since this is for playback only, enable the hardware node to receive only

   printf("enabling hardware node for receive...\n");

   if (fCanRecv)
   {
      rc = MIDIEnableInstance(ulHdwInst, MIDI_ENABLE_RECEIVE);
      check( rc, "MIDIEnableInstance\0" );

      if ( __MIDIDebug )
         printf("     ::Hardware node %lx enabled for receive.\n", ulHdwInst );
   }
   else
      check( HARDWARE_UNABLE_TO_RECEIVE, "hardware can't receive\0" );





// OUTLINE: SETUP

   // MIDISendMessages preparation begins here
   // the MIDISetup API gets a pointer to the MIDI timer

   printf("setting up MIDI...\n");

   pMidiSetup = (PVOID) malloc(sizeof(MIDISETUP));
   memset(pMidiSetup, '\0', sizeof(MIDISETUP));
   pulMidiTime = &ulMidiTime;
   pMidiSetup->pulMaxRTSysexLength = &ulMaxRTSysexLength;
   pMidiSetup->ppulMIDICurrentTime = &pulMidiTime;
   pMidiSetup->ulStructLength = sizeof(MIDISETUP);

   rc = MIDISetup(pMidiSetup, 0);
   check( rc, "MIDISetup\0" );





   // initialize the mmioinfo structure for the MIDI I/O Proc, and set
   // mmioinfo.ulTranslate to translate the MIDI file data and header

   printf("reading MIDI file...\n");

   memset (&mmioinfo, '\0', sizeof(MMIOINFO));
   ulOpenFlags |= MMIO_READ | MMIO_DENYNONE;
   mmioinfo.fccIOProc = mmioFOURCC('M','I','D','I');
   mmioinfo.ulTranslate |= MMIO_TRANSLATEDATA | MMIO_TRANSLATEHEADER;

   hmmio = mmioOpen(szFilePath, &mmioinfo, ulOpenFlags);
   if ( __MIDIDebug )
   {
      printf("     ::mmioOpen (hmmio = %lx)\n", (ULONG)hmmio );
      printf("     ::file = %s\n", szFilePath );
   }
   if ( !hmmio )
      check( MMIOOPEN_ERROR, "mmioOpen\0" );


   ulBytesToRead = 64000;

                   // allocate a buffer to store the MIDI song data

   pMidiBuffer = ( PVOID ) malloc(ulBytesToRead);

                   // read in the MIDI song data

   ulBytesRead = mmioRead (hmmio, pMidiBuffer, ulBytesToRead);
   if ( __MIDIDebug )
      printf("     ::mmioRead (bytes read = %lu)\n", ulBytesRead );

                   // keep a pointer to the beginning of our buffer

   pBufferPtr = pMidiBuffer;





   CalcDelay();    // Call CalcDelay to initialize global variable ulPerClock





// OUTLINE: PARSE

   // parse the MIDI data

   // IBM Sysex Messages begin with F0 00 00 3A ... F7
   // Non-IBM Sysex Messages begin with F0, but bytes that
   // follow will differ; Non-IBM Sysex Messages should be discarded

   printf("\nparsing the MIDI data...\n");

   while (pMidiBuffer < (pBufferPtr + ulBytesRead))
   {
      b = *pMidiBuffer;



      // 0xF0 marks the beginning of a Sysex Message
      // 0xF7 marks the end of a Sysex Message
      // note:  this is endian specific

      if (b == 0xF0)
      {
         if (*(PULONG)pMidiBuffer == 0x3A0000F0L)
         {
            pMidiBuffer += 4;          // advance to the byte following F0 00 00 3A nn
            b = *pMidiBuffer;
            switch (b)
            {
               case 0x00:
               case 0x02:
               case 0x04:
               case 0x05:
               case 0x06:
                           break;



               // 0x01 indicates Timing Compression Message (long version)
               //      F0 00 00 3A 01 ll mm F7, where:
               //
               //      ll = number of system real-time timing clocks LSB
               //      mm = number of system real-time timing clocks MSB
               //
               // Number of timing clocks is calculated by combining
               // the low 7 bits of mm with the low 7-bits of ll to
               // produce a single 16-bit value

               case 0x01:  pMidiBuffer++;
                           usLSBCompValue = *pMidiBuffer++;
                           usLSBCompValue &= 0x7F;
                           usMSBCompValue = *pMidiBuffer++;     // current byte 0xF7
                           usMSBCompValue &= 0x7F;
                           usClocks = (usMSBCompValue << 7) + usLSBCompValue;
                           ulTimeStamp += (ULONG)usClocks * ulPerClock;
                           break;



               // 0x03 indicates Device Driver Control Message
               //      the byte after 0x03 is the command
               //      F0 00 00 3A 03 01 tt pp 00 F7, where:
               //
               //      01 = clocks per quarter note (Timing Generation Control)
               //      tt = System real-time control flags (skip this byte)
               //      pp = System real-time 24 CPQN rate prescaler
               //      00 = not used (skip this byte)
               //
               //
               //      F0 00 00 3A 03 02 t1 tm dd F7, where:
               //
               //      02 = tempo (Tempo Control)
               //      t1 = Tempo LSB
               //      tm = Tempo MSB
               //      dd = duration (skip this byte)

               case 0x03:  pMidiBuffer++;
                           bNextByte = *pMidiBuffer;

                           if (bNextByte == 0x01)
                           {
                                       // bypass System real-time Control Flags and advance
                                       // to System real-time 24 CPQN rate prescaler

                              pMidiBuffer += 2;
                              usCPQN = *pMidiBuffer++;          // current byte 0x00
                              CalcDelay();
                           }

                           if (bNextByte == 0x02)
                           {
                              pMidiBuffer++;
                              usTempoLSB = *pMidiBuffer++;
                              usTempoMSB = *pMidiBuffer++;
                              ulTempo = (ULONG)((usTempoMSB << 7) + usTempoLSB);
                              CalcDelay();
                           }
                           pMidiBuffer++;                       // current byte 0xF7
                           break;



               // 0x07 - 0x7F indicates Timing Compression Message (short version)
               //      F0 00 00 3A nn F7, where:
               //
               //      nn is the number of timing clocks (nn = 7 through 127)

               default:    // 0x07 - 0x7F
                           ulTimeStamp += (ULONG)b * ulPerClock;
                           pMidiBuffer++;                       // current byte 0xF7

            } /* end switch */
         }
         else
         {
            while (*pMidiBuffer != 0xF7)         // bypass non-IBM Sysex Message
            {
               pMidiBuffer++;                    // current byte 0xF7
            }
         }
      } /* end if */





      // 0xF8 indicates a Delay Byte message
      // 0xF8s are delay byte messages which may be sent at any time--
      // EVEN BETWEEN BYTES OF A MESSAGE WHICH HAS A DIFFERENT STATUS.
      // delay byte messages are acted upon, after which the receiving
      // process resumes under the previous state (stored in bRunningStatus)

      // NOTE:  this is endian specific

      else if (b == 0xF8)
      {
         ulTimeStamp += ulPerClock;              // process current 0xF8

                   // while the next byte after the 0xF8 is an 0xF8 increment
                   // pMidiBuffer; we want to leave this section pointing to
                   // the last 0xF8

         while (*(PUSHORT)pMidiBuffer == 0xF8F8)
         {
            pMidiBuffer++;
            b = *pMidiBuffer;
            ulTimeStamp += ulPerClock;
         }
      }





      // anything else is a MIDI message
      // prepare to process the MIDI message

      else
      {
         // if this is the first message list in our linked list of
         // message lists, allocate a buffer for the first message list

         if (!pMessageList)
         {
            pMessageList = (PMESSAGELIST) malloc( sizeof( MESSAGELIST ) );
            memset(pMessageList, 0, sizeof(MESSAGELIST));
            pMessageList->pNext = NULL;
            pCurrMessageList = pMessageList;
         }



         // for this example, the maximum number of messages for each
         // message list is 20; any more than that and we create a new list.

         if (pCurrMessageList->ulMsgIndex < 20)
         {
            ulMsgNum = pCurrMessageList->ulMsgIndex;
         }
         else
         {
            pNewMessageList = (PMESSAGELIST) malloc( sizeof( MESSAGELIST ) );
            memset(pNewMessageList, 0, sizeof(MESSAGELIST));
            pNewMessageList->pNext = NULL;
            pCurrMessageList->pNext = pNewMessageList;
            pCurrMessageList = pNewMessageList;
            ulMsgNum = pCurrMessageList->ulMsgIndex;
         }



         // timestamp the message and identify the source instance

         pCurrMessageList->paMessage[ulMsgNum].ulTime = (ulTimeStamp+500)/1000;
         pCurrMessageList->paMessage[ulMsgNum].ulSourceInstance = ulApplInst;



         // if byte is a status byte; store the status byte in the message
         // and in bTestByte for processing the message later;
         // advance the pointer to the first data byte

         if (b >= 0x80)
         {
            pCurrMessageList->paMessage[ulMsgNum].msg.bytes.bStatus = b;
            pMidiBuffer++;
            bTestByte = b;

                   // bRunningStatus only applies to certain status bytes

            if (b >= 0x80 && b < 0xF0)
               bRunningStatus = b;
            else
               bRunningStatus = 0;
         }
                   // if byte is less than 0x80
                   // current byte is a data byte, so we need bRunningStatus
         else
         {
            pCurrMessageList->paMessage[ulMsgNum].msg.bytes.bStatus = bRunningStatus;
            bTestByte = bRunningStatus;
         }



         // this is a MIDI message and the byte is a status byte, where:
         //
         //                    Status                       # of Data Bytes
         // =============================================   ===============
         // 0x8n = Note Off event                                  2
         // 0x9n = Note On event                                   2
         // 0xAn = Polyphonic key pressure/after touch             2
         // 0xBn = Control Change or Selects Channel Mode          2
         // 0xCn = Program Change                                  1
         // 0xDn = Channel pressure/after touch                    1
         // 0xEn = Pitch wheel change                              2

         switch (bTestByte & 0xF0)
         {
            case 0x80:
            case 0x90:
            case 0xA0:
            case 0xB0:
            case 0xE0:
                        pCurrMessageList->paMessage[ulMsgNum].msg.bytes.abData[0] = *pMidiBuffer++;
                        pCurrMessageList->paMessage[ulMsgNum].msg.bytes.abData[1] = *pMidiBuffer;
                        break;
            case 0xC0:
            case 0xD0:
                        pCurrMessageList->paMessage[ulMsgNum].msg.bytes.abData[0] = *pMidiBuffer;
                        break;
            default:
                        break;
         }
         pCurrMessageList->ulMsgIndex++;

      } /* end if */

      pMidiBuffer++;                   // advance to next byte in file

      if ( __MIDIDebug )
         spinbar();

   } /* end while */





// OUTLINE: PLAY

   // Now we are done parsing the MIDI file into messages.  Our next step is
   // to send the messages to the application node we created earlier.

   // MIDITimer allows the application to start and stop the MIDI Timer
   // start the MIDI timer

   printf("\n\nsending MIDI messages...\n");

   rc = DosSetPriority(PRTYS_THREAD, PRTYC_TIMECRITICAL, 0, 0);
   check( rc, "MIDITimer [DOS_SET_PRIORITY]\0" );

   rc = MIDITimer( MIDI_START_TIMER, 0 );
   check( rc, "MIDITimer [MIDI_START_TIMER]\0" );


   ulStartTime = *pulMidiTime + 1;     // pointer to current MIDI time obtained from
                                       // the previous call to MIDISetup
                                       // NOTE: we add 1 to compensate for processor time

   for (; pMessageList; pMessageList = pMessageList->pNext)
   {
      ulNumMessages = pMessageList->ulMsgIndex;

                   // calculate the time stamp for each message; the time
                   // stamp is the absolute time, so add the time stamp
                   // for each message to the current MIDI time in ulStartTime

      for (i = 0; i < ulNumMessages; i++)
      {
         pMessageList->paMessage[i].ulTime += ulStartTime;
      }

      rc = MIDISendMessages(pMessageList->paMessage, ulNumMessages, 0);

      if ( rc )
         check( rc, "MIDISendMessages\0" );

      if ( __MIDIDebug )
         spinbar();


                   // ulEndTime is the time stamp of the last message in the
                   // message list

      ulEndTime = pMessageList->paMessage[ulNumMessages-1].ulTime;

                   // adjust ulEndTime to sleep for a little less time (4) to allow for
                   // processing ulTime for each message

      ulCurrentTime = *pulMidiTime;

      if (ulCurrentTime + 2 < ulEndTime)
      {
         rc = TimerSleep(ulEndTime - 2 - ulCurrentTime, 0);

         if ( rc )
            check( rc, "TimerSleep\0" );
      }

   } /* end for */

   printf("\n\nfinished sending MIDI messages.\n\n\n");





// OUTLINE: CLEANUP

   // cleanup
   // stop the MIDI timer after we are done sending our MIDI messages

   printf("cleaning up...\n");

   // wait 1/10th of a second for RTMIDI to play the last notes
   rc = TimerSleep(100, 0);
   check( rc, "TimerSleep\0" );

   rc = MIDITimer( MIDI_STOP_TIMER, 0 );
   check( rc, "MIDITimer [MIDI_STOP_TIMER]\0" );





   // remove the link from the application node to the
   // hardware node, disable our hardware node, delete our application
   // instances, and free up all memory allocated

   // remove the link from instance # 1 to the hardware node

   rc = MIDIRemoveLink(ulApplInst, ulHdwInst, ulSlotNumber, 0);
   check( rc, "MIDIRemoveLink\0" );

   rc = MIDIDisableInstance(ulHdwInst, MIDI_DISABLE_RECEIVE);  // use the hardware instance number
   check( rc, "MIDIDisableInstance\0" );                       // retrieved from MIDIQueryInstanceList

   rc = MIDIDeleteInstance(ulApplInst, 0);            // Delete application instance
   check( rc, "MIDIDeleteInstance\0" );


   // free up all memory allocated

   free((PBYTE)paClassInfo);
   free((PBYTE)paInstanceInfo);
   free((PBYTE)pMidiSetup);
   free((PBYTE)pBufferPtr);
   free((PBYTE)pMessageList);
   free((PBYTE)pNewMessageList);



   printf("Done.\n");

   DosExit( EXIT_PROCESS, 0 );

} /* end main */





/****************************************************************************
*
*     Function : check
*
*  Description : Used to check return codes and provide two levels of
*                feedback to the user (normal and debug modes).
*
*   Parameters : ULONG rc, CHAR *call_id
*
*      Returns : VOID
*
****************************************************************************/


VOID check( ULONG rc, CHAR *call_id )
{
   CHAR *szErrorString;
   CHAR *szSolutionString;
   CHAR *szDebugString;



   CHAR *UNRECOVERABLE    = "*the program cannot continue*\0";
   CHAR *CODE_ERROR       = "A code error has occured.\0";
   CHAR *SYSTEM_PROBLEM   = "The system has encountered a problem.\0";
   CHAR *HARDWARE_PROBLEM = "A hardware problem has been encountered.\0";


   switch( rc )
   {
      case TIMERERR_INVALID_PARAMETER:
         szDebugString    = "TIMERERR_INVALID_PARAMETER\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case TIMERERR_INTERNAL_SYSTEM:
         szDebugString    = "TIMERERR_INTERNAL_SYSTEM\0";
         szErrorString    = SYSTEM_PROBLEM;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_INVALID_PARAMETER:
         szDebugString    = "MIDIERR_INVALID_PARAMETER\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_INTERNAL_SYSTEM:
         szDebugString    = "MIDIERR_INTERNAL_SYSTEM\0";
         szErrorString    = SYSTEM_PROBLEM;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_INVALID_CLASS_NUMBER:
         szDebugString    = "MIDIERR_INVALID_CLASS_NUMBER\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_NO_DRIVER:
         szDebugString    = "MIDIERR_NO_DRIVER\0";
         szErrorString    = "A MIDI device driver was not found on this system.\0";
         szSolutionString = "*check to make sure the proper MIDI driver(s) are installed*\0";
         break;

      case MIDIERR_DUPLICATE_INSTANCE_NAME:
         szDebugString    = "MIDIERR_DUPLICATE_INSTANCE_NAME\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_INVALID_INSTANCE_NAME:
         szDebugString    = "MIDIERR_INVALID_INSTANCE_NAME\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_INVALID_INSTANCE_NUMBER:
         szDebugString    = "MIDIERR_INVALID_INSTANCE_NUMBER\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_INVALID_SETUP:
         szDebugString    = "MIDIERR_INVALID_SETUP\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_HARDWARE_FAILED:
         szDebugString    = "MIDIERR_HARDWARE_FAILED\0";
         szErrorString    = HARDWARE_PROBLEM;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_INVALID_FLAG:
         szDebugString    = "MIDIERR_INVALID_FLAG\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_RECEIVEONLY:
         szDebugString    = "MIDIERR_RECEIVEONLY\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_SENDONLY:
         szDebugString    = "MIDIERR_SENDONLY\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_NOT_ALLOWED:
         szDebugString    = "MIDIERR_NOT_ALLOWED\0";
         szErrorString    = CODE_ERROR;
         szSolutionString = UNRECOVERABLE;
         break;

      case MIDIERR_RESOURCE_NOT_AVAILABLE:
         szDebugString    = "MIDIERR_RESOURCE_NOT_AVAILABLE\0";
         szErrorString    = SYSTEM_PROBLEM;
         szSolutionString = UNRECOVERABLE;
         break;


                                       // These are locally defined program errors.

      case HARDWARE_NOT_FOUND:
         szDebugString    = "HARDWARE_NOT_FOUND\0";
         szErrorString    = "A hardware node (instance) couldn't be found.\0";
         szSolutionString = "*check to make sure an audio card with \nMPU-401 support is installed and enabled.*\0";
         break;

      case HARDWARE_UNABLE_TO_RECEIVE:
         szDebugString    = "HARDWARE_UNABLE_TO_RECEIVE\0";
         szErrorString    = "The hardware node is unable to receive MIDI messages.\0";
         szSolutionString = "*currently installed hardware does not support MIDI receive*\0";
         break;

      case MMIOOPEN_ERROR:
         szDebugString    = "MMIOOPEN_ERROR\0";
         szErrorString    = "There is a problem opening the file specified.\0";
         szSolutionString = "*check to make sure the file is a MIDI file (*.MID) \nin the MMOS2\\SOUNDS directory and that it is spelled correctly.*\0";
         break;

      default:
         szDebugString    = "Success\0";

   } /* end switch */


   if ( __MIDIDebug )
      printf("     ::%s (rc=%lu, %s)\n", call_id, rc, szDebugString);


   if ( rc )
   {
      printf( "\n\n%s\n%s\n\n", szErrorString, szSolutionString );
      exit(1);
   }

} /* end check */





/****************************************************************************
*
*     Function : spinbar
*
*  Description : Used to provide visual feedback on the progress of
*                functions while in debug mode.
*
*   Parameters : void
*
*      Returns : VOID
*
****************************************************************************/


VOID spinbar( void )
{
   static int counter = 0;
   char spin[] = "\\|/-";

   if ( counter >= 4 )
      counter = 0;
   else
   {
      putchar(8);
      putchar(spin[counter++]);
   }

} /* end spinbar */





/****************************************************************************
*
*     Function : CalcDelay
*
*  Description : Used to determine the number of microseconds per MIDI clock
*                based on the current tempo.  See notes.
*
*   Parameters : void
*
*      Returns : VOID
*
*        Notes :
*
*                   600,000,000 microseconds/10 minutes
*        -----------------------------------------------------------  ==   X microseconds/clock
*                                            usCPQNnum
*        (ulTempo beats/10 min) * ( 24 * ------------- clocks/beat )
*                                            usCPQNden
*
*
*              25,000,000 * usCPQNden
*        ==  --------------------------
*               ulTempo * usCPQNnum
*
*        where
*           usCPQNden = ((usCPQN & 0x3F) + 1) * 3
*           usCPQNnum = 1                                   if bit 6 of usCPQN is set
*
*           or
*
*           usCPQNden = 1
*           usCPQNnum = usCPQN + 1                          if bit 6 is not set
*
*
****************************************************************************/


VOID CalcDelay (void)
{
   ULONG ulTmp = 0;

                   // CPQN is Clocks Per Quarter Note
                   // bit 6 set if denominator

   if (usCPQN & 0x40)
   {
      ulTmp = 25000000 * ((usCPQN * 0x3F) + 1);
      ulPerClock = ulTmp / ulTempo;
      ulPerClock *= 3;
   }
   else
   {
      ulTmp = ulTempo * (usCPQN + 1);
      ulPerClock = 25000000 / ulTmp;
   }

} /* end CalcDelay */





/****************************************************************************
*
*     Function : QueryMMBASE
*
*  Description : Used to get the path for MMBASE.
*
*   Parameters : PCHAR pszMapFile
*
*      Returns : USHORT
*
****************************************************************************/


USHORT QueryMMBASE (PCHAR pszMapFile)
{
   PSZ pszEnv;
   PCHAR p ;

   if (!DosScanEnv ((PSZ)"MMBASE", &pszEnv))
   {
      strcpy (pszMapFile, (PCHAR)pszEnv);
      if ( (p = strchr(pszMapFile, ';')) != 0 ) *p = 0;        /* Remove After semicolon */
   }
   else
   {
      strcpy (pszMapFile, "C:\\");
   }

   return (0);

} /* end QueryMMBASE */


