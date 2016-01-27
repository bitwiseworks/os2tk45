ORD_INCLUDED	EQU	1


RECT1S	STRUC
rcs_xLeft	DW	?
rcs_yBottom	DW	?
rcs_xRight	DW	?
rcs_yTop	DW	?
RECT1S	ENDS

ODPOINT	STRUC
odpt_dx	DB	?
odpt_dy	DB	?
ODPOINT	ENDS

SIZES	STRUC
sizs_cx	DW	?
sizs_cy	DW	?
SIZES	ENDS

SWPUSHORT	STRUC
swpus_HiByte	DB	?
swpus_LoByte	DB	?
SWPUSHORT	ENDS
OCODE_GNOP1	EQU	00H             
OCODE_GESD	EQU	0FFH             
OCODE2_1	EQU	80H 
OCODE2_2	EQU	88H 

ORDER	STRUC
ord_idCode	DB	?
ord_uchData	DB	?
ORDER	ENDS
OCODE_GBAR	EQU	68H             
OCODE_GCFIG	EQU	7DH             
OCODE_GEEL	EQU	49H             
OCODE_GEPTH	EQU	7FH             
OCODE_GEPROL	EQU	3EH             
OCODE_GPOP	EQU	3FH             
OCODE_GSBMX	EQU	0DH             
OCODE_GPSBMX	EQU	4DH             
OCODE_GSCD	EQU	3AH             
OCODE_GPSCD	EQU	7AH             
OCODE_GSCR	EQU	39H             
OCODE_GPSCR	EQU	79H             
OCODE_GSCS	EQU	38H             
OCODE_GPSCS	EQU	78H             
OCODE_GSCOL	EQU	0AH             
OCODE_GPSCOL	EQU	4AH             
OCODE_GSLE	EQU	1AH             
OCODE_GPSLE	EQU	5AH             
OCODE_GSLJ	EQU	1BH             
OCODE_GPSLJ	EQU	5BH             
OCODE_GSLT	EQU	18H             
OCODE_GPSLT	EQU	58H             
OCODE_GSLW	EQU	19H             
OCODE_GPSLW	EQU	59H             
OCODE_GSMP	EQU	3BH             
OCODE_GPSMP	EQU	7BH             
OCODE_GSMS	EQU	3CH             
OCODE_GPSMS	EQU	7CH             
OCODE_GSMT	EQU	29H             
OCODE_GPSMT	EQU	69H             
OCODE_GSMX	EQU	0CH             
OCODE_GPSMX	EQU	4CH             
OCODE_GSPS	EQU	08H             
OCODE_GPSPS	EQU	48H             
OCODE_GSPT	EQU	28H             
OCODE_GPSPT	EQU	09H             
GBAR_RESERVED	EQU	80H 
GBAR_BOUNDARY	EQU	0C0H 
GBAR_NOBOUNDARY	EQU	80H 
GBAR_WINDING	EQU	0A0H 
GBAR_ALTERNATE	EQU	80H 
GSCR_PRECISION	EQU	0FH 
OCODE_VLONG	EQU	0FEH 
LORDER_ML	EQU	253 

LORDER	STRUC
lord_idCode	DB	?
lord_uchLength	DB	?
lord_uchData	DB	LORDER_ML DUP (?)
LORDER	ENDS
OCODE_GEAR	EQU	60H             
OCODE_GEIMG	EQU	93H             
OCODE_GCCHST	EQU	83H             
GCCHST_MC	EQU	255              
OCODE_GCHST	EQU	0C3H             
GCHST_SMC	EQU	251              
GCHST_LMC	EQU	247              
OCODE_GCCHSTM	EQU	0B1H             
GCCHSTM_MC	EQU	255              
OCODE_GCHSTM	EQU	0F1H             
GCHSTM_SMC	EQU	251              
GCHSTM_LMC	EQU	247              
OCODE_GCOMT	EQU	01H             
GCOMT_ML	EQU	255              
OCODE_GIMD	EQU	92H             
GIMD_ML	EQU	255              
OCODE_GCFARC	EQU	87H             
OCODE_GFARC	EQU	0C7H             
OCODE_GLABL	EQU	0D3H             
OCODE_GSCP	EQU	21H             
OCODE_GPSCP	EQU	61H             
OCODE_GCBEZ	EQU	0A5H             
GCBEZ_SMB	EQU	21               
GCBEZ_LMB	EQU	10               
OCODE_GBEZ	EQU	0E5H             
GBEZ_SMB	EQU	20               
GBEZ_LMB	EQU	10               
OCODE_GCFLT	EQU	85H             
GCFLT_SMP	EQU	63               
GCFLT_LMP	EQU	31               
OCODE_GFLT	EQU	0C5H             
GFLT_SMP	EQU	62               
GFLT_LMP	EQU	30               
OCODE_GCLINE	EQU	81H             
GCLINE_SMP	EQU	63               
GCLINE_LMP	EQU	31               
OCODE_GLINE	EQU	0C1H             
GLINE_SMP	EQU	62               
GLINE_LMP	EQU	30               
OCODE_GCMRK	EQU	82H             
GCMRK_SMP	EQU	63               
GCMRK_LMP	EQU	31               
OCODE_GMRK	EQU	0C2H             
GMRK_SMP	EQU	62               
GMRK_LMP	EQU	30               
OCODE_GCRLINE	EQU	0A1H             
GCRLINE_MP	EQU	127              
OCODE_GRLINE	EQU	0E1H             
GRLINE_SMP	EQU	125              
GRLINE_LMP	EQU	123              
OCODE_GSBCOL	EQU	25H             
OCODE_GPSBCOL	EQU	65H             
OCODE_GSECOL	EQU	26H             
OCODE_GPSECOL	EQU	66H             
SECOL_DEFAULT0	EQU	0000H 
SECOL_DEFAULT1	EQU	0FF00H 
SECOL_NEUTRAL	EQU	0FF07H 
SECOL_RESET	EQU	0FF08H 
OCODE_GSCA	EQU	34H             
OCODE_GPSCA	EQU	74H             
OCODE_GSCH	EQU	35H             
OCODE_GPSCH	EQU	75H             
OCODE_GSFLW	EQU	11H             
OCODE_GPSFLW	EQU	51H             
OCODE_GSPIK	EQU	43H             
OCODE_GPSPIK	EQU	23H             
OCODE_GCARC	EQU	86H             
OCODE_GARC	EQU	0C6H             

ORDERS_GCARC	STRUC
osgcarc_ptInter	DB	SIZE POINTS DUP (?)
osgcarc_ptEnd	DB	SIZE POINTS DUP (?)
ORDERS_GCARC	ENDS

ORDERL_GCARC	STRUC
olgcarc_ptInter	DB	SIZE POINTL DUP (?)
olgcarc_ptEnd	DB	SIZE POINTL DUP (?)
ORDERL_GCARC	ENDS
OCODE_GBEL	EQU	0D2H             
GBEL_DL	EQU	251 

ORDER_GBEL	STRUC
ogbel_lElementType	DD	?
ogbel_achDesc	DB	GBEL_DL DUP (?)
ORDER_GBEL	ENDS
OCODE_GCBIMG	EQU	91H             
OCODE_GBIMG	EQU	0D1H             

ORDER_GCBIMG	STRUC
ogbimg_uchFormat	DB	?
ogbimg_uchReserved	DB	?
ogbimg_cx	DB	SIZE SWPUSHORT DUP (?)
ogbimg_cy	DB	SIZE SWPUSHORT DUP (?)
ORDER_GCBIMG	ENDS
OCODE_GBPTH	EQU	0D0H             

ORDER_GBPTH	STRUC
ogbpth_usReserved	DW	?
ogbpth_idPath	DD	?
ORDER_GBPTH	ENDS
OCODE_GCBOX	EQU	80H             
OCODE_GBOX	EQU	0C0H             

ORDERS_GCBOX	STRUC
osgcbox_fbFlags	DB	?
osgcbox_uchReserved	DB	?
osgcbox_ptCorner	DB	SIZE POINTS DUP (?)
osgcbox_hAxis	DW	?
osgcbox_vAxis	DW	?
ORDERS_GCBOX	ENDS

ORDERL_GCBOX	STRUC
olgcbox_fbFlags	DB	?
olgcbox_uchReserved	DB	?
olgcbox_ptCorner	DB	SIZE POINTL DUP (?)
olgcbox_hAxis	DD	?
olgcbox_vAxis	DD	?
ORDERL_GCBOX	ENDS
GCBOX_FILL	EQU	40H 
GCBOX_BOUNDARY	EQU	20H 
OCODE_GCALLS	EQU	07H             

ORDER_GCALLS	STRUC
ogcalls_sReserved	DW	?
ogcalls_idSegment	DD	?
ORDER_GCALLS	ENDS
OCODE_GFPTH	EQU	0D7H             

ORDER_GFPTH	STRUC
ogfpth_fbFlags	DB	?
ogfpth_uchReserved	DB	?
ogfpth_idPath	DD	?
ORDER_GFPTH	ENDS
GFPTH_ALTERNATE	EQU	00H 
GFPTH_WINDING	EQU	40H 
GFPTH_MODIFY	EQU	20H 
OCODE_GOPTH	EQU	0D4H             

ORDER_GOPTH	STRUC
ogopth_fbFlags	DB	?
ogopth_uchReserved	DB	?
ogopth_idPath	DD	?
ORDER_GOPTH	ENDS
OCODE_GMPTH	EQU	0D8H                

ORDER_GMPTH	STRUC
ogmpth_uchMode	DB	?
ogmpth_uchReserved	DB	?
ogmpth_idPath	DD	?
ORDER_GMPTH	ENDS
GMPTH_STROKE	EQU	06H 
OCODE_GCPARC	EQU	0A3H             
OCODE_GPARC	EQU	0E3H             

ORDERS_GCPARC	STRUC
osgcparc_ptCenter	DB	SIZE POINTS DUP (?)
osgcparc_ufx88Multiplier	DW	?
osgcparc_usStartAngle	DD	?
osgcparc_usSweepAngle	DD	?
ORDERS_GCPARC	ENDS

ORDERL_GCPARC	STRUC
olgcparc_ptCenter	DB	SIZE POINTL DUP (?)
olgcparc_ufxMultiplier	DD	?
olgcparc_usStartAngle	DD	?
olgcparc_usSweepAngle	DD	?
ORDERL_GCPARC	ENDS
OCODE_GSCPTH	EQU	0B4H             

ORDER_GSCPTH	STRUC
ogscpth_fbFlags	DB	?
ogscpth_uchReserved	DB	?
ogscpth_idPath	DD	?
ORDER_GSCPTH	ENDS
GSCPTH_ALTERNATE	EQU	00H 
GSCPTH_WINDING	EQU	40H 
GSCPTH_RESET	EQU	00H 
GSCPTH_INTERSECT	EQU	20H 
OCODE_GSAP	EQU	22H             
OCODE_GPSAP	EQU	62H             

ORDERS_GSAP	STRUC
osgsap_p	DW	?
osgsap_q	DW	?
osgsap_r	DW	?
osgsap_s	DW	?
ORDERS_GSAP	ENDS

ORDERL_GSAP	STRUC
olgsap_p	DD	?
olgsap_q	DD	?
olgsap_r	DD	?
olgsap_s	DD	?
ORDERL_GSAP	ENDS
OCODE_GSBICOL	EQU	0A7H             
OCODE_GPSBICOL	EQU	0E7H             
OCODE_GSICOL	EQU	0A6H             
OCODE_GPSICOL	EQU	0E6H             

ORDER_GSBICOL	STRUC
ogbicol_fbFlags	DB	?
ogbicol_auchColor	DB	3 DUP (?)
ORDER_GSBICOL	ENDS
SICOL_SPECIFY	EQU	00H 
SICOL_SPECIAL	EQU	40H 
SICOL_DEFAULT	EQU	80H 
SICOL_BLACK	EQU	1 
SICOL_WHITE	EQU	2 
SICOL_ONES	EQU	4 
SICOL_ZEROES	EQU	5 
OCODE_GSCC	EQU	33H             
OCODE_GPSCC	EQU	03H             

ORDERS_GSCC	STRUC
osgscc_cxInt	DW	?
osgscc_cyInt	DW	?
osgscc_cxFract	DW	?
osgscc_cyFract	DW	?
osgscc_fbFlags	DB	?
osgscc_uchReserved	DB	?
ORDERS_GSCC	ENDS

ORDERL_GSCC	STRUC
olgscc_cxInt	DD	?
olgscc_cyInt	DD	?
olgscc_cxFract	DW	?
olgscc_cyFract	DW	?
olgscc_fbFlags	DB	?
olgscc_uchReserved	DB	?
ORDERL_GSCC	ENDS
GSCC_ZERODEF	EQU	00H 
GSCC_ZEROZERO	EQU	80H 
OCODE_GSMC	EQU	37H             
OCODE_GPSMC	EQU	77H             

ORDERS_GSMC	STRUC
osgsmc_cx	DW	?
osgsmc_cy	DW	?
osgsmc_fbFlags	DB	?
osgsmc_uchReserved	DB	?
ORDERS_GSMC	ENDS

ORDERL_GSMC	STRUC
olgsmc_cx	DD	?
olgsmc_cy	DD	?
olgsmc_fbFlags	DB	?
olgsmc_uchReserved	DB	?
ORDERL_GSMC	ENDS
GSMC_ZERODEF	EQU	00H 
GSMC_ZEROZERO	EQU	80H 
OCODE_GSPRP	EQU	0A0H             
OCODE_GPSPRP	EQU	0E0H             

ORDERS_GSPRP	STRUC
osgsprp_fbFlags	DB	?
osgsprp_uchReserved	DB	?
osgsprp_ptPos	DB	SIZE POINTS DUP (?)
ORDERS_GSPRP	ENDS

ORDERL_GSPRP	STRUC
olgsprp_fbFlags	DB	?
olgsprp_uchReserved	DB	?
olgsprp_ptPos	DB	SIZE POINTL DUP (?)
ORDERL_GSPRP	ENDS
GSPRP_DEFAULT	EQU	80H 
GSPRP_SPECIFY	EQU	00H 
OCODE_GSIA	EQU	14H             
OCODE_GPSIA	EQU	54H             
GSIA_VL	EQU	3 

ORDER_GSIA	STRUC
ogsia_uchAttrType	DB	?
ogsia_uchPrimType	DB	?
ogsia_fbFlags	DB	?
ogsia_auchValue	DB	GSIA_VL DUP (?)
ORDER_GSIA	ENDS
GSIA_COLOR	EQU	01H 
GSIA_BCOLOR	EQU	02H 
GSIA_MIX	EQU	03H 
GSIA_BMIX	EQU	04H 
GSIA_LINE	EQU	01H 
GSIA_CHAR	EQU	02H 
GSIA_MARKER	EQU	03H 
GSIA_PATTERN	EQU	04H 
GSIA_IMAGE	EQU	05H 
GSIA_SPECIFY	EQU	00H 
GSIA_SPECIAL	EQU	40H 
GSIA_DEFAULT	EQU	80H 
GSIA_BLACK	EQU	1 
GSIA_WHITE	EQU	2 
GSIA_ONES	EQU	4 
GSIA_ZEROES	EQU	5 
OCODE_GSTM	EQU	24H             
OCODE_GPSTM	EQU	64H             
OCODE_GSTV	EQU	31H             
GSTM_ML	EQU	16 

ORDERS_GSTM	STRUC
osgstm_uchReserved	DB	?
osgstm_fbFlags	DB	?
osgstm_fsMask	DW	?
osgstm_asMatrix	DW	GSTM_ML DUP (?)
ORDERS_GSTM	ENDS

ORDERL_GSTM	STRUC
olgstm_uchReserved	DB	?
olgstm_fbFlags	DB	?
olgstm_fsMask	DW	?
olgstm_alMatrix	DD	GSTM_ML DUP (?)
ORDERL_GSTM	ENDS
GSTM_M11	EQU	8000H 
GSTM_M12	EQU	4000H 
GSTM_M13	EQU	2000H 
GSTM_M14	EQU	1000H 
GSTM_M21	EQU	0800H 
GSTM_M22	EQU	0400H 
GSTM_M23	EQU	0200H 
GSTM_M24	EQU	0100H 
GSTM_M31	EQU	0080H 
GSTM_M32	EQU	0040H 
GSTM_M33	EQU	0020H 
GSTM_M34	EQU	0010H 
GSTM_M41	EQU	0008H 
GSTM_M42	EQU	0004H 
GSTM_M43	EQU	0002H 
GSTM_M44	EQU	0001H 
GSTM_UNITY	EQU	00H 
GSTM_AFTER	EQU	01H 
GSTM_BEFORE	EQU	02H 
GSTM_OVERWRITE	EQU	03H 
GSTV_OVERWRITE	EQU	00H 
GSTV_AFTER	EQU	04H 
OCODE_GSSB	EQU	32H             
OCODE_GSVW	EQU	27H             
OCODE_GPSVW	EQU	67H             
GSSB_ML	EQU	4 

ORDERS_GSSB	STRUC
osgssb_fbFlags	DB	?
osgssb_fbMask	DB	?
osgssb_alMatrix	DW	GSSB_ML DUP (?)
ORDERS_GSSB	ENDS

ORDERL_GSSB	STRUC
olgssb_fbFLags	DB	?
olgssb_fbMask	DB	?
olgssb_alMatrix	DD	GSSB_ML DUP (?)
ORDERL_GSSB	ENDS
GSSB_XLEFT	EQU	20H 
GSSB_XRIGHT	EQU	10H 
GSSB_YBOTTOM	EQU	08H 
GSSB_YTOP	EQU	04H 
GSVW_INTERSECT	EQU	00H 
GSVW_REPLACE	EQU	80H 
OCODE_GSGCH	EQU	04H             
GSGCH_ML	EQU	254 

ORDER_GSGCH	STRUC
ogsgch_uchIdent	DB	?
ogsgch_auchData	DB	GSGCH_ML DUP (?)
ORDER_GSGCH	ENDS
OCODE_GSSLW	EQU	15H             
OCODE_GPSSLW	EQU	55H             

ORDERS_GSSLW	STRUC
osgsslw_fbFlags	DB	?
osgsslw_uchReserved	DB	?
osgsslw_LineWidth	DW	?
ORDERS_GSSLW	ENDS

ORDERL_GSSLW	STRUC
olgsslw_fbFlags	DB	?
olgsslw_uchReserved	DB	?
olgsslw_LineWidth	DD	?
ORDERL_GSSLW	ENDS
GSSLW_DEFAULT	EQU	80H 
GSSLW_SPECIFY	EQU	00H 
OCODE_GCSFLT	EQU	0A4H             
OCODE_GSFLT	EQU	0E4H             
GCSFLT_SMF	EQU	21 
GSFLT_SMF	EQU	20 

ORDERS_GCSFLT	STRUC
osgcsflt_apt	DB	SIZE POINTS * 2*GCSFLT_SMF DUP (?)
osgcsflt_afxSharpness	DD	GCSFLT_SMF DUP (?)
ORDERS_GCSFLT	ENDS
GCSFLT_LMF	EQU	12 
GSFLT_LMF	EQU	12 

ORDERL_GCSFLT	STRUC
olgcsflt_apt	DB	SIZE POINTL * 2*GCSFLT_SMF DUP (?)
olgcsflt_afxSharpness	DD	GCSFLT_SMF DUP (?)
ORDERL_GCSFLT	ENDS
OCODE_GBBLT	EQU	0D6H             

ORDERS_GBBLT	STRUC
osgbblt_fsFlags	DW	?
osgbblt_usMix	DW	?
osgbblt_hbmSrc	DD	?
osgbblt_lOptions	DD	?
osgbblt_rcsTargetRect	DB	SIZE RECT1S DUP (?)
osgbblt_rclSourceRect	DB	SIZE RECTL DUP (?)
ORDERS_GBBLT	ENDS

ORDERL_GBBLT	STRUC
olgbblt_fsFlags	DW	?
olgbblt_usMix	DW	?
olgbblt_hbmSrc	DD	?
olgbblt_lOptions	DD	?
olgbblt_rclTargetRect	DB	SIZE RECTL DUP (?)
olgbblt_rclSourceRect	DB	SIZE RECTL DUP (?)
ORDERL_GBBLT	ENDS
OCODE_GSCE	EQU	17H             
OCODE_GPSCE	EQU	57H             
OCODE_GSCBE	EQU	05H             
OCODE_GPSCBE	EQU	45H             

ORDER_GSCBE	STRUC
osgsce_fbFlags	DB	?
osgsce_uchReserved	DB	?
osgsce_ufxextra	DD	?
ORDER_GSCBE	ENDS
OCODE_GESCP	EQU	0D5H             
GESCP_ML	EQU	253 

ORDER_GESCP	STRUC
ogescp_uchType	DB	?
ogescp_uchIdent	DB	?
ogescp_auchData	DB	GESCP_ML DUP (?)
ORDER_GESCP	ENDS
GESCP_REG	EQU	80H             
GEBB_REGID	EQU	02H             
ETYPE_GEBB	EQU	800200D5H 
GEBB_LMP	EQU	29 

ORDERL_GEBB	STRUC
olgebb_fbFlags	DB	?
olgebb_usMix	DW	?
olgebb_cPoints	DB	?
olgebb_hbmSrc	DD	?
olgebb_lReserved	DD	?
olgebb_lOptions	DD	?
olgebb_aptPoints	DB	SIZE POINTL * GEBB_LMP DUP (?)
ORDERL_GEBB	ENDS
GEPEL_REGID	EQU	01H             
ETYPE_GEPEL	EQU	800100D5H 
GEDB_REGID	EQU	04H           
ETYPE_GEDB	EQU	800400D5H 

ORDERL_GEDB	STRUC
olgedb_fsFlags	DW	?
olgedb_usMix	DW	?
olgedb_pBits	DD	?
olgedb_pbmi	DD	?
olgedb_lOptions	DD	?
olgedb_rclTargetRect	DB	SIZE RECTL DUP (?)
olgedb_rclSourceRect	DB	SIZE RECTL DUP (?)
ORDERL_GEDB	ENDS
GEFF_REGID	EQU	03H           
ETYPE_GEFF	EQU	800300D5H 

ORDERL_GEFF	STRUC
olgeff_fsFlags	DB	?
olgeff_auchColor	DB	3 DUP (?)
ORDERL_GEFF	ENDS
ETYPE_LINEBUNDLE	EQU	0000FD01H 
ETYPE_CHARBUNDLE	EQU	0000FD02H 
ETYPE_MARKERBUNDLE	EQU	0000FD03H 
ETYPE_AREABUNDLE	EQU	0000FD04H 
ETYPE_IMAGEBUNDLE	EQU	0000FD05H 
VORDER_ML	EQU	65531 

VORDER	STRUC
vord_idCode	DB	?
vord_uchQualifier	DB	?
vord_uchLength	DB	SIZE SWPUSHORT DUP (?)
vord_uchData	DB	VORDER_ML DUP (?)
VORDER	ENDS
OCODEQ_GCCHSTE	EQU	0B0H            
OCODEQ_GCHSTE	EQU	0F0H            
OCODEQ_GTCHSPA	EQU	0F4H            
ETYPE_GCCHSTE	EQU	0000FEB0H 
ETYPE_GCHSTE	EQU	0000FEF0H 

ORDERS_GCCHSTE	STRUC
osgcchste_fbFlags	DB	?
osgcchste_uchReserved	DB	?
osgcchste_ptRect	DB	SIZE POINTS * 2 DUP (?)
osgcchste_cchString	DB	SIZE SWPUSHORT DUP (?)
osgcchste_achString	DB	1 DUP (?)
osgcchste_adx	DW	1 DUP (?)
ORDERS_GCCHSTE	ENDS

ORDERL_GCCHSTE	STRUC
olgcchste_fbFlags	DB	?
olgcchste_uchReserved	DB	?
olgcchste_ptRect	DB	SIZE POINTL * 2 DUP (?)
olgcchste_cchString	DB	SIZE SWPUSHORT DUP (?)
olgcchste_achString	DB	1 DUP (?)
olgcchste_adx	DD	1 DUP (?)
ORDERL_GCCHSTE	ENDS

ORDERL_GTCHSPA	STRUC
olgcchspa_fbFlags	DB	?
olgcchspa_uchReserved	DB	?
olgcchspa_ptRect	DB	SIZE POINTL * 2 DUP (?)
olgcchspa_cchString	DB	SIZE SWPUSHORT DUP (?)
olgcchspa_achString	DB	1 DUP (?)
olgcchspa_adx	DD	2 DUP (?)
olgcchspa_tabs	DD	1 DUP (?)
ORDERL_GTCHSPA	ENDS
GCCHSTE_DRAWRECT	EQU	80H 
GCCHSTE_NORECT	EQU	00H 
GCCHSTE_CLIP	EQU	40H 
GCCHSTE_NOCLIP	EQU	00H 
GCCHSTE_DEEMPHASIZE	EQU	20H             
GCCHSTE_NODEEMPHASIZE	EQU	00H 
GCCHSTE_LEAVEPOS	EQU	10H 
GCCHSTE_MOVEPOS	EQU	00H 
GCCHSTE_UNDERSCORE	EQU	08H 
GCCHSTE_NOUNDERSCORE	EQU	00H 
GCCHSTE_STRIKEOUT	EQU	04H 
GCCHSTE_NOSTRIKEOUT	EQU	00H 
GTCHSPA_STARTPOS	EQU	02H 
GTCHSPA_NOSTARTPOS	EQU	00H 
OCODEQ_GEESCP	EQU	0D5H            
GEESCP_ML	EQU	65533 

ORDER_GEESCP	STRUC
ogeescp_uchType	DB	?
ogeescp_uchIdent	DB	?
ogeescp_auchData	DB	GEESCP_ML DUP (?)
ORDER_GEESCP	ENDS
