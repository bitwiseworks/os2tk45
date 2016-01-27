/********************** START OF SPE****************************
*
*   SOURCE FILE NAME:   ADMCFUNC.H
*
*   DESCRIPTIVE NAME: mci Waveform Driver include file.
*
*              Copyright (c) IBM Corporation  1991, 1993
*                        All Rights Reserved
*
*   STATUS: MM Extensions 1.0
*
*   FUNCTION: Component function Prototypes.
*
*
*   NOTES:
*
*       DEPENDENCIES:
*
*   EXTERNAL ENTRY POINTS: None.
*   INTERNAL ENTRY POINTS: None.
*
*   EXTERNAL REFERENCES (system):
*
*   MODIFICATION HISTORY:
*   DATE        DEVELOPER           CHANGE DESCRIPTION
*****************************************************************************/

/***********************************************
* MCI Message Support Functions
***********************************************/
RC   MCIStat (FUNCTION_PARM_BLOCK *);// Status
RC   MCIOpen (FUNCTION_PARM_BLOCK *);// Open
RC   MCICue  (FUNCTION_PARM_BLOCK *);// Cue
RC   MCIPlay (FUNCTION_PARM_BLOCK *);// Play
RC   MCIRecd (FUNCTION_PARM_BLOCK *);// Record
RC   MCIPaus (FUNCTION_PARM_BLOCK *);// Pause
RC   MCIClos (FUNCTION_PARM_BLOCK *);// Close
RC   MCISet  (FUNCTION_PARM_BLOCK *);// Set
RC   MCISetCuePoint (FUNCTION_PARM_BLOCK *);// Set Cue Point
RC   MCISetPositionAdvise (FUNCTION_PARM_BLOCK * );
RC   MCIMagic( FUNCTION_PARM_BLOCK *, ULONG );
RC   MCISeek (FUNCTION_PARM_BLOCK *);// Seek
RC   MCIStop (FUNCTION_PARM_BLOCK *);// Stop
RC   MCILoad (FUNCTION_PARM_BLOCK *);// Load
RC   MCIInfo (FUNCTION_PARM_BLOCK *);// Info
RC   MCIResume (FUNCTION_PARM_BLOCK *);// Resume
RC   MCISave (FUNCTION_PARM_BLOCK *);// Save
RC   MCIRest (FUNCTION_PARM_BLOCK *);// Restore
RC   MCISync (FUNCTION_PARM_BLOCK *);// Sync Offset
RC   MCISavf (FUNCTION_PARM_BLOCK *);// Save File
RC   MCIConnector (FUNCTION_PARM_BLOCK* );// Connectors
RC   MCISetPostionAdvise (FUNCTION_PARM_BLOCK *);// Set Position Advise
RC   MCISaveFile (FUNCTION_PARM_BLOCK *);// Save Element
RC   MCICaps (FUNCTION_PARM_BLOCK *);// Get Device Capabilities
#ifdef PTRFIX
RC   MCIExtCaps (FUNCTION_PARM_BLOCK *);// Get Extended Device Capabilities
#endif

/***************************************
* Post MCI Notification Messages
***************************************/

RC PostMDMMessage (ULONG ,USHORT,FUNCTION_PARM_BLOCK *);

/************************
* Element I/O
************************/
RC OpenFile (INSTANCE *, ULONG);      // Open Media Element

RC InstallIOProc (ULONG, MMIOINFO *); // Install IO Proc

RC GetAudioHeader (INSTANCE *, ULONG);       // Get Audio Header

RC SetAudioHeader (INSTANCE *);       // Set Audio Header

/************************************************
* Streaming Support Functions
***********************************************/

RC CreateStream ( INSTANCE * );

RC DestroyStream ( INSTANCE  *pInstance);

RC AssociatePlayList (INSTANCE * ); // PlayList Support

RC CreateToEvent(INSTANCE *, ULONG); // Enable Event

RC APIENTRY RecordEventRoutine (MEVCB *);// Record Stream EventProc

RC APIENTRY PlayEventRoutine (MEVCB *);// Play Stream EventProc

typedef VOID (* _Optlink ADMCTHREAD)(void *);

RC _Optlink StartRecord (VOID *); // Start Record Stream

RC _Optlink StartPlay (VOID *);   // Start Play Stream

RC _Optlink StartSave (VOID * ); // Start Save function

/******************************************
* Audio Device Support Functions
******************************************/

RC InitAudioDevice (INSTANCE *,ULONG ); // Open The Audio Device

RC SetAudioDevice (INSTANCE *, PMMAUDIOHEADER );// Change device

VOID SetWaveDeviceDefaults (INSTANCE *);// Set Wave device defaults

//VOID VSDInstToWaveSetParms (PMCI_WAVE_SET_PARMS, INSTANCE *); // Swap Data

//RC GetPDDName (ULONG , CHAR []);

ULONG ModifyConnection( INSTANCE   *pInstance );

ULONG GetMixerInfo( INSTANCE   *ulpInstance, PCONCB pconcb );


/********************************************
* Instance Management, Parameter Checking..
********************************************/

ULONG ObtainDefaults(  INSTANCE   *ulpInstance);

RC CheckFlags (ULONG); // Check MCI Flags

RC CleanUp (PVOID); // Release Memory


#pragma linkage( DBCSGenerateUniqueFile, system )

RC CopyParms (ULONG,FUNCTION_PARM_BLOCK *, ULONG);// Memory Allocation

RC CheckPlayFlags (FUNCTION_PARM_BLOCK *); // Error Checking

RC CheckRecordFlags (FUNCTION_PARM_BLOCK *); // Error Checking


RC CuePointAdd ( INSTANCE *, PMCI_CUEPOINT_PARMS );

RC CuePointDelete ( INSTANCE *, PMCI_CUEPOINT_PARMS );

void  GetNotifyAbortAccess ( INSTANCE *, PULONG );

ULONG CueAbortInProgressNotify( INSTANCE *, FUNCTION_PARM_BLOCK *, ULONG );

void GenericThreadAbort( INSTANCE *, FUNCTION_PARM_BLOCK *, ULONG );

void ThreadedStop( INSTANCE * );

void DestroySetStream ( INSTANCE * );

ULONG CueStream( INSTANCE *, ULONG );

ULONG CueSetup( FUNCTION_PARM_BLOCK *, ULONG );

ULONG GetOldStreamPosition( INSTANCE * ) ;

ULONG AbortInProgressNotify( INSTANCE *, FUNCTION_PARM_BLOCK  *, ULONG, ULONG );

ULONG PrepareAndCreateStream( INSTANCE *, ULONG, ULONG );

ULONG RememberStreamState( INSTANCE *, ULONG, BOOL );

ULONG ProcessFromToFlags( FUNCTION_PARM_BLOCK  *, INSTANCE *, ULONG, ULONG );

ULONG EnableEvents( INSTANCE * );

ULONG StopStream2( INSTANCE *, ULONG );

ULONG CloseFile( INSTANCE *ulpInstance );

ULONG CheckForValidElement( INSTANCE *, PSZ, ULONG);

ULONG LoadAbortNotifies( INSTANCE *, FUNCTION_PARM_BLOCK *, ULONG );

ULONG OpenHandle( INSTANCE  *, ULONG, HMMIO );

ULONG ProcessElement( INSTANCE *, ULONG, ULONG );

ULONG SetupTempFiles( INSTANCE   *, ULONG );

void OpenInit( INSTANCE  * );

ULONG DetermineConnections( INSTANCE *, MMDRV_OPEN_PARMS * );

ULONG ConnectToAmp( INSTANCE *, MMDRV_OPEN_PARMS *, ULONG );

ULONG StreamSetup ( void );

ULONG CreateSemaphores( FUNCTION_PARM_BLOCK * );

void NotifyWaitSetup( FUNCTION_PARM_BLOCK *, USHORT, ULONG, ULONG );

ULONG AllocateInstance( FUNCTION_PARM_BLOCK * );

ULONG UpdateFileHeader( INSTANCE * );

RC MCIPaste (FUNCTION_PARM_BLOCK *pFuncBlock);

RC MCICopy(FUNCTION_PARM_BLOCK *pFuncBlock, ULONG ulMessage );
RC MCIMajic(FUNCTION_PARM_BLOCK *pFuncBlock);

RC GetClipboardData( PVOID *, INSTANCE *, ULONG, PULONG, ULONG, PMMAUDIOHEADER );

RC _Optlink StartPaste( VOID * );

//RC StartPaste( FUNCTION_PARM_BLOCK * );
void PasteNotify ( FUNCTION_PARM_BLOCK *, ULONG, ULONG );

RC _Optlink StartCopy( VOID * );
//RC StartCopy( FUNCTION_PARM_BLOCK * );

void CopyNotify ( FUNCTION_PARM_BLOCK *, ULONG, ULONG );
RC AddNode ( INSTANCE *, ULONG );
RC RemoveUndoNodes( INSTANCE * );



RC CheckEditFlags(FUNCTION_PARM_BLOCK *pFuncBlock, ULONG );

RC ConvertFormat( PMMAUDIOHEADER, INSTANCE *, PVOID *, PULONG );

RC Convert16to8( PCHAR, ULONG, PCHAR, PULONG, PMMAUDIOHEADER );


RC PutClipboardData( PVOID *, INSTANCE *, ULONG, ULONG );

RC  GetFromToPosition(LONG, ULONG, MCI_EDIT_PARMS *, INSTANCE *, ULONG *, ULONG * );

RC ConvertSampleRates( USHORT, PULONG, USHORT, PVOID, PVOID *, ULONG );

RC ConvertLowerSampleRates( USHORT, PULONG, USHORT, PVOID, PVOID *, ULONG );

/* I wish this was C++ so I could just overload the operator- but alas,
   we are still using C 8-( */

void GetSample16( ULONG, ULONG, ULONG, PVOID, PSHORT, USHORT );

void PutSample16( ULONG, ULONG, ULONG, PVOID, SHORT, USHORT );

void MultiplyAcc16( PLONG, USHORT, LONG );

void GetSample8( ULONG, ULONG, ULONG, PVOID, PCHAR, USHORT );

void PutSample8( ULONG, ULONG, ULONG, PVOID, CHAR, USHORT );

void MultiplyAcc8( PUSHORT, USHORT, USHORT );

void Modulo( PSHORT  x, SHORT y, SHORT z );
void RModulo( PSHORT  x, SHORT y, SHORT z );

ULONG GetDefaults( PCHAR pOrigSource, INSTANCE *pInstance );

PCHAR GetToken(PCHAR    pToken, USHORT   LastToken);

ULONG PutInInstance( PLONG   pValue, PCHAR   pString );


// networking stuff
ULONG   BeginQualityofService ( INSTANCE *, LONG );
ULONG   EndQualityofService ( INSTANCE * );

// codec stuff

ULONG   SetupCodec( INSTANCE *pInstance, USHORT usFormatTag );

ULONG   DataTranslation( INSTANCE      *pInstance );
ULONG   InstallProtocol( INSTANCE      *pInstance );




/*********************************
* Time Format Support
*********************************/

RC ConvertTimeUnits (INSTANCE *, PULONG, ULONG); // To Current Base

RC ConvertToMM (INSTANCE *, PULONG, ULONG ); // To MMTIME

/*********************************
* Tasking Support
*********************************/
RC AbortWaitOperation (INSTANCE *); // Release Wait Block

ULONG MCD_EnterCrit (INSTANCE *);  // Acquire Mutex Sem

ULONG MCD_ExitCrit (INSTANCE *);  // Release Mutex Sem

ULONG GetSaveSem (INSTANCE * );

ULONG GiveUpSaveSem (INSTANCE * ulpInstance);

ULONG AcquireProcSem ( void );  // Acquire Global Proc Sem

ULONG ReleaseProcSem ( void);  // Release Global Proc Sem

/***********************************
* Debug information
************************************/


/***********************************
* SpiStreamCalls
************************************/

ULONG ADMCSeekStream ( INSTANCE *pInstance, ULONG ulFlags, LONG  lSeekPoint );
ULONG ADMCStopStream ( INSTANCE *pInstance, ULONG ulFlags );
ULONG ADMCAssociate ( HSTREAM hstream, HID hid, PACB pacb );
ULONG APIENTRY ADMCDisableEvent ( HEVENT hevent );
ULONG APIENTRY ADMCEnableEvent ( PEVCB pevcb, PHEVENT phevent );
//void SyncAudioHeader( INSTANCE * ulpInstance );
ULONG StartStream( INSTANCE *pInstance, ULONG ulStartFlags );
ULONG TestNetwork( INSTANCE *pInstance, PMCI_WAVE_GETDEVCAPS_PARMS pExtDevCaps );
ULONG ReleaseStreamingSemaphore( INSTANCE *pInstance );
ULONG GetStreamingSemaphore( INSTANCE *pInstance );
ULONG CheckForConnection( INSTANCE *pInstance );
ULONG CanPlayRecord( INSTANCE *pInstance, ULONG ulDirection );
ULONG FindAmp( INSTANCE *pInstance );






/*****************************
* MCI Driver Entry Point
*****************************/
ULONG APIENTRY  MM_mciDriverEntryADMC ( PVOID  ,  /* Instance Pointer  */
                                        USHORT ,  /* MCI Message       */
                                        ULONG  ,  /* MCI Flags         */
                                        ULONG  ,  /* Data For This Msg */
                                        USHORT);  /* User Parameter    */

USHORT ADMCEXIT ( void);  /* AudioMCD Exit Routine    */

ULONG StreamSetup ( void );


/*****************************
* Message cue entry point
*****************************/

MRESULT EXPENTRY ClipboardProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );

/* MMIO api entry */

ULONG APIENTRY DBCSGenerateUniqueFile( PSZ pszPathName,
                                       PULONG pulPathLength,
                                       PHFILE phfile );

ULONG StatusLevelEvent (INSTANCE * ulpInstance, MEVCB *pevcb);


