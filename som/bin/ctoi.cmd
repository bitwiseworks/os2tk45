/*  [REXX]  */
/*
 *   COMPONENT_NAME: some
 *
 *   ORIGINS: 27
 *
 *
 *    25H7912  (C)  COPYRIGHT International Business Machines Corp. 1992,1994,1996 
 *   All Rights Reserved
 *   Licensed Materials - Property of IBM
 *   US Government Users Restricted Rights - Use, duplication or
 *   disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 */
/*  @(#) somc/ctoi.cmd 2.10 1/9/96 17:24:49 [7/30/96 14:44:48]
 *  Convert a .csc files to .idl files.
 */
nonomatch = "";
force = 0;

parse arg argv;

parse value argv with flag rest;
if flag = "-f" then do;
    force = 1;
    argv = rest;
end;

scflags = value("scflags",,"os2environment");
sc	= value("sc",,"os2environment");
if sc = "" then sc = "sc";

if scflags <> "" then
    cmd = "@" || sc "-mtypecheck -mrmstar -mconvert" scflags "-sidl";
else
    cmd = "@" || sc "-mtypecheck -mrmstar -mconvert -sidl";

do while argv <> "";
    parse value argv with i argv;
    iroot = root(i);
    if stream(i, "c", "query exists") <> "" then
	if stream(iroot || ".idl", "c", "query exists") <> "" then
	    say iroot || ".idl already exists"
	else do;
	    say i":";
	    cmd i;
	    if rc <> 0 then do;
		say "error in converting" i;
		"@del" iroot || ".id2 >nul 2>&1";
		if force = 0 then exit(-1);
	    end;
	    else do;
		"@copy" iroot || ".id2" iroot || ".idl";
		"@del"  iroot || ".id2";
            end;
	end;
    else do;
	say i "not found";
	exit(-1);
    end;
end;

"@del SMIDENTS.UNK >nul 2>&1";
exit(0);

root: procedure;
    parse arg fn;
    i = length(fn);
    root = fn;
    do while i>0 & root = fn
        if substr(fn,i,1) = '.' then
            root = LEFT(fn,i-1);
        i = i - 1
    end 
    return root;
