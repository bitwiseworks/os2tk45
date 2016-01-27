/*
 * Copyright (c) International Business Machines Corporation 1994
 * WPIDL2XH.CMD
 *
 * Command file to emit XH header file from Workplace Shell IDL.
 * This command assumes the toolkit include paths are set accordingly.
 *
 * Invoke this command file on each Workplace Shell class's .idl file
 * from the OS/2 toolkit to re-generate headers for C++.  This is
 * only necessary when you upgrade to a new level of the SOMobject's
 * Toolkit.  Invoking the SOM compiler's xh emitter will not do this
 * for you, as the Workplace Shell classes currently only maintain
 * passthrus for .h files for C.
 *
 * Usage:
 *   wpidl2xh <inputfile>
 *
 * History:
 *   09/11/94 - Eric E. Osmann * Created
 */

parse upper arg inputfile
say 'WPIDL2XH - IBM Corporation (c) 1994, Version 1.0'
say ''

/*
 * Check for an input file.
 */
if( inputfile = '' ) then do
  say "no input file specified"
  exit 99
end

/*
 * Make sure the file exists
 */
if stream( inputfile, 'c', 'query exists' ) == '' then do
  say inputfile "file doesn't exist"
  exit 98
end

/*
 * Initialization of the temporary output files
 */
'@echo off'
outputfile1 = "$wptmp1.idl"
"erase" outputfile1 "2>nul >nul"
outputfile2 = "$wptmp2.idl"
"erase" outputfile2 "2>nul >nul"
say 'Processing 'inputfile

/*
 * Read every line from the file and parse it.
 * Change c_h to c_xh and SOMAny to SOMObject.
 * Pass one
 */
do while lines( inputfile )
  line = linein( inputfile )

  /*
   * Look for 'passthru C_h' combination
   */
  if pos( 'passthru C_h', line ) \= 0 then
  do
     cpos = pos( 'C_h', line )
     hpos = pos( 'h', line, cpos )
     line = insert( 'x', line, hpos - 1 )
     rc = lineout( outputfile1, line )
  end
  else if pos( 'SOMAny', line ) \= 0 then
  do
     spos = pos( 'SOMAny', line )        /* look for 'SOMAny'  */
     line = delstr( line, spos, 6 )
     line = insert( 'SOMObject', line, spos - 1 )
     rc = lineout( outputfile1, line )
  end
  else
     rc = lineout( outputfile1, line )
end  /* of do while */
rc = stream( outputfile1, c, "close" )

/*
 * Special case IDL files.
 * Pass two
 */
if( inputfile = 'WPOBJECT.IDL' ) then
do
   Say 'Special case processing for 'inputfile
   do while lines( outputfile1 )
      line = linein( outputfile1 )
      if pos('#ifndef M_WPObject',line ) \= 0 then
      do
         line = linein( outputfile1 )
         line = linein( outputfile1 )
         rc = lineout( outputfile2, '"      class M_WPObject;"' )
      end
      else if pos( '#ifndef WPObject', line ) \= 0 then
      do
         line = linein( outputfile1 )
         line = linein( outputfile1 )
         rc = lineout( outputfile2, '"      class WPObject;"' )
      end
      else if pos( '#define WPFolder SOMObject', line ) \= 0 then
      do
         /* -- Note ------------------------------------------- */
         /* SOM_WPFolder_xh if defined by wpfolder.xh and is    */
         /* subject to change with future major releases of     */
         /* SOM, so therefore the following statement may need  */
         /* to change accordingly.                              */
         /* --------------------------------------------------- */
         rc = lineout( outputfile2, '"      #ifndef SOM_WPFolder_xh"' )
         rc = lineout( outputfile2, '"        #define WPFolder WPObject"' )
         rc = lineout( outputfile2, '"      #endif"' )
      end
      else if pos( '#define OBJECT_FROM_PREC', line ) \= 0 then
      do
         pvoidpos = pos( 'PVOID', line )
         line = delstr( line, pvoidpos, 5 )
         line = insert( 'WPObject *', line, pvoidpos - 1 )
         rc = lineout( outputfile2, line )
      end
      else
      do
         rc = lineout( outputfile2, line )
      end
   end /* do while lines( outputfile1 ) */
end   /* if infile = WPOBJECT.IDL */

/*
 * WPFOLDER case
 */
else if( inputfile = 'WPFOLDER.IDL' ) then
do
   Say 'Special case processing for 'inputfile
   linein( outputfile1, 1, 0 )
   do while lines( outputfile1 )
      line = linein( outputfile1 )
      if pos( 'passthru C_xh', line ) \= 0 then
      do
         rc = lineout( outputfile2, 'passthru C_xh = ""' )
         rc = lineout( outputfile2, '"     #ifdef WPFolder"' )
         rc = lineout( outputfile2, '"         #undef WPFolder"' )
         rc = lineout( outputfile2, '"         class  WPFolder;"' )
         rc = lineout( outputfile2, '"     #endif"' )
         rc = lineout( outputfile2, '"";' )
         rc = lineout( outputfile2, '' )
         rc = lineout( outputfile2, 'passthru C_xh_after = ""' )
      end
      else
      do
         rc = lineout( outputfile2, line )
      end
   end  /* do while lines */
end     /* if inputfile = wpfolder.idl */

/*
 * WPCLSMGR.IDL case
 */
else if( inputfile = 'WPCLSMGR.IDL' ) then
do
   Say 'Special case processing for 'inputfile
   do while lines( outputfile1 )
      line = linein( outputfile1 )
      if pos( 'dllname = "pmwp.dll"', line ) \= 0 then
      do
         rc = lineout( outputfile2, '    dllname = "pmwp.dll";' )
         rc = lineout( outputfile2, '' )
         rc = lineout( outputfile2, 'passthru C_xh_after = ""' )
         rc = lineout( outputfile2, '"     #ifndef wpobject_xh"' )
         rc = lineout( outputfile2, '"         #include \"wpobject.xh\""' )
         rc = lineout( outputfile2, '"     #endif"' )
         rc = lineout( outputfile2, '"";' )
      end
      else
      do
         rc = lineout( outputfile2, line )
      end
   end  /* do while lines */
end

/*
 * WPMWV.IDL case
 */
else if( inputfile = 'WPMWV.IDL' ) then
do
   Say 'Special case processing for 'inputfile
   do while lines( outputfile1 )
      line = linein( outputfile1 )
      if pos( 'passthru C_xh', line ) \= 0 then
      do
         rc = lineout( outputfile2, 'passthru C_xh_after = ""' )
      end
      else if pos( '#ifndef WPObject', line ) \= 0 then
      do
         line = linein( outputfile1 )
         line = linein( outputfile1 )
      end
      else
      do
         rc = lineout( outputfile2, line )
      end
   end  /* do while lines */
end

/*
 * WPDISK.IDL case
 */
else if( inputfile = 'WPDISK.IDL' ) then
do
   Say 'Special case processing for 'inputfile
   do while lines( outputfile1 )
      line = linein( outputfile1 )
      if pos( 'passthru C_xh', line ) \= 0 then
      do
         rc = lineout( outputfile2, 'passthru C_xh_after = ""' )
         rc = lineout( outputfile2, '"     #ifndef wprootf_xh"' )
         rc = lineout( outputfile2, '"         #include \"wprootf.xh\""' )
         rc = lineout( outputfile2, '"     #endif"' )
      end
      else if pos( '#ifndef WPRootFolder', line ) \= 0 then
      do
         line = linein( outputfile1 )
         line = linein( outputfile1 )
      end
      else
      do
         rc = lineout( outputfile2, line )
      end
   end  /* do while lines */
end

/*
 * For all other cases, set the first pass output file to
 * second pass output file so that the SC compiler is happy.
 */
else
   do
      outputfile2 = outputfile1
   end

/*
 * Close streams before calling emitter.
 */
rc = stream( outputfile1, c, "close" )
rc = stream( outputfile2, c, "close" )

/*
 * Finally, emit the xh header file.
 */
say 'Emitting xh file for 'inputfile
sc '-C 128000 -S 128000 -mnoint -mnotc -s"xh" 'outputfile2
'erase' outputfile1 outputfile2 '>nul 2>nul'

/* End of command file */
