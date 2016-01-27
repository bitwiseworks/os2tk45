/*TOOLKIT.CH*/

/*In a more complicated sample it would be necessary to change paths*/
/*in the config.sys in order for the apps to run.  This is an example of*/
/*the file necessary to do this.*/

/*MERGE merges data into an existing statement in the Config.sys file.*/
/*You can also use the SET keyword following the first quote */
/*character.  The word set is ignored unless you are adding a new line */
/*to the config.sys.  The = # refers to the Control.scr file indicating the*/
/*destination directory.*/
/*  The  ".;" states to look in the current directory*/
/*This is why in this small example this file is not necessary*/
/*but if this did not exist in the config.sys already, this file would be*/
/*necessary.  There are other  keywords, refer to the programming*/
/*guide for further documentation*/


/*merge "SET LIB"=0*/
/*merge "SET INCLUDE"=4*/
/*merge "SET INCLUDE"=11*/
/*merge "SET INCLUDE"=".;"*/
