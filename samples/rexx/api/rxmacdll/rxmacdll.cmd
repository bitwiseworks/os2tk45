/*********************************************************************/
/*   RXMACDLL.CMD - Builds macrospace access functions dynamic link  */
/*                  library for REXXSAA                              */
/*********************************************************************/


Say 'This program demonstrates the MacroSpace and NLS function '
Say 'ÿ libraries for REXX.'
Say
Say 'Required Programs:'
Say ' MACRO.DLL'
Say ' RXNLSINF.DLL'
Say ' NLSMONEY.CMD'
Say

Address CMD '@PAUSE'
Say 'The MacroSpace utility library contains the functions:'
Say
Say '       MACROLOAD     - Load a file into the MacroSpace'
Say '       MACROSAVE     - Save the MacroSpace to a Library'
Say '       MACROERASE    - Erase the Contents of the MacroSpace'
Say '       MACROQUERY    - Query a Function in the MacroSpace'
Say
Say 'The NLS utility library  contains the function:'
Say
Say '       RxNLSInfo     - Retrieve NLS information'
Say
Say 'Now this program will call the functions.'
Address CMD '@PAUSE'

D.1 = 'MacroLoad';          D.2 = 'MacroSave'
D.3 = 'MacroErase';         D.4 = 'MacroQuery'

/* To use the new functions from REXX, we must "register" them   */
/* by using RxFuncAdd.                                           */
Do I=1 to 4
   if RxFuncQuery(D.I) Then Call RxFuncAdd D.I, 'MACRO', D.I
End /* do */

amounts = '1000 1000.00 0 .12 .456 1000000 123456 123456.78'

call MacroLoad 'NLSMoney BEFORE NLSMoney'

numamt = words( amounts )

do i = 1 to numamt

  amount = word( amounts, i )
  fmtamt = NLSMoney( amount )

  say "The amount" amount "formats as" fmtamt || "."
end

call MacroErase
Do I = 1 to 4
   Call RxFuncDrop D.I
End
Call RxFuncDrop RxNLSInfo
Address CMD '@PAUSE'


