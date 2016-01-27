;***	VDMBIOSJ.INC
;
;
;	MVDM BIOSJ Data Definitions
;	Copyright (c) 1990 IBM Japan Corporation
;
;	For include file hierarchy, see MVDM.INC
;
;  CHANGE ACTIVITY =
;    DATE      FLAG	   APAR	  CHANGE DESCRIPTION
;    --------  ----------  -----  --------------------------------------
;    mm/dd/yy  @Vr.mpppxx  xxxxx  xxxxxxx
;    02/15/96  @V3.1MNH01 D148300 Merge DBCS-J (IBM J) source w/SBCS source
; *********************************************************************

EBIOS_BIOSMODE	EQU	00121h		;bios mode flag	      j-ea0000


BIOSJDATA_LEN	EQU	000800h		;reserve 2K


PS2_MODE	EQU	000h		;PS/2 mode BIOS	      (will be deleted later...) ;J-YF001
PS55_MODE	EQU	001h		;PS/55 DBCS mode BIOS (will be deleted later...) ;J-YF001
PCDOS_MODE	EQU	000h		;PC-DOS mode BIOS	    ;J-YF004
DOSJ_MODE	EQU	001h		;DOS-J mode BIOS	    ;J-YF004
DOSV_MODE	EQU	002h		;DOS/V mode BIOS	    ;J-YF004
DOSK_MODE	EQU	003h		;DOS/K mode BIOS      ;@K-BS1116A


;***	Int 16h Equates (AH values)
;

INT16J_BEEPON	EQU	003h					;ss0003
INT16J_BEEPOFF	EQU	004h					;ss0003
INT16J_SETSTATE		EQU	005h					;ss0003
INT16J_SETSTATE2	EQU	085h					;ss0003
INT16J_READCHGXTBL	EQU	006h					;ss0004
INT16J_CHGDISP	EQU	007h					;ss0003
INT16J_SETSHIFT		EQU	082h					;ss0003
INT16J_GETKBDFUNC	EQU	009h					;j-mi1110
INT16J_GETKBDHWID	EQU	00Ah					;j-mi1110
INT16J_GETKBDTYPE	EQU	00Bh					;ss0003
INT16J_READCHAR		EQU	0F0h					;ss0003
INT16J_PEEKCHAR		EQU	0F1h					;ss0003
INT16J_FLUSHBUFF	EQU	0F2h					;ss0003
INT16V_RETKBDID		EQU	00Ah	     ;DOSV return KBD ID   ;j-mh0001
INT16V_DBCSSHIFT	EQU	013h				     ;j-mh0001
INT16V_SETDSHIFT	EQU	000h	     ;INT16 AH=13 AL=00	   ;j-mh0001
INT16V_GETDSHIFT	EQU	001h	     ;INT16 AH=13 AL=01	   ;j-mh0001
INT16V_CHGDISP	EQU	014h				     ;j-mh0001
INT16V_ENADISP	EQU	000h	     ;INT16 AH=14 AL=00	   ;j-mh0001
INT16V_DISDISP	EQU	001h	     ;INT16 AH=14 AL=01	   ;j-mh0001
INT16V_QUEDISP	EQU	002h	     ;INT16 AH=14 AL=02	   ;j-mh0001
INT16_READCHARINTERIM	EQU	0F0h	     ;@IBM K-BS1116A
INT16_PEEKCHARINTERIM	EQU	0F1h	     ;@IBM K-BS1116A
INT16_FLUSHBUFFINTERIM	EQU	0F3h	     ;@IBM K-BS1116A


;***	MVDM BIOSJ Data Definitions - used by VDDs

;***	BIOSJ Data Area
;
;	This area cannot exceed BIOSJDATA_LEN
;

DefStruc ,drvtbl_ss
	_WORD	 max_cyl_m
	_BYTE	 max_head_m
	_WORD	 dummy1
	_WORD	 dummy2
	_WORD	 dummy3
	_BYTE	 dummy4,,7
	_BYTE	 dummy5
	_WORD	 start_chr_m
	_BYTE	 dummy6
	_BYTE	 dummy7
	_WORD	 dummy8
	_BYTE	 dummy9
	_BYTE	 dummy10
EndStruc DRVTBL_S

DefStruc ,ba_ss
	_BYTE	 normal
	_BYTE	 intensify
EndStruc BA_S

DefStruc ,lss_ss
	_BYTE	 ls_reset
	_BYTE	 ls_mode
	_BYTE	 ls_bank
EndStruc LSS_S

DefStruc ,lgs_ss
	_BYTE	 set_reset
	_BYTE	 enable_sr
	_BYTE	 color_compare
	_BYTE	 data_rotation
	_BYTE	 read_map_select
	_BYTE	 lg_mode
	_BYTE	 complement
	_BYTE	 color_dont_care
	_BYTE	 bit_mask_low
	_BYTE	 bit_mask_high
	_BYTE	 map_mask
	_BYTE	 lg_command
	_BYTE	 set_reset_2
EndStruc LGS_S

DefStruc ,bios_version_blk_ss
	_WORD	 bios_version
EndStruc BIOS_VERSION_BLK_S

DefStruc ,communication_blk_ss
	_BYTE	 cb_byte0
	_BYTE	 sdlc_bca_num
	_BYTE	 tca_num
EndStruc COMMUNICATION_BLK_S

DefStruc ,asyncronus_ss
	_BYTE	 ab_byte0
EndStruc ASYNCRONUS_BLK_S

DefStruc ,hdisk_blk_ss
	_BYTE	 int_hdisk_num
	_BYTE	 ext_hdisk_num
EndStruc HDISK_BLK_S

DefStruc ,keyboard_blk_ss
	_BYTE	 kbdid_1st
	_BYTE	 kbdid_2nd
	_BYTE	 kbdid_3rd
	_BYTE	 kbd_character
	_WORD	 table_offset
	_WORD	 table_segment
EndStruc KEYBOARD_BLK_S

DefStruc ,diskette_blk_ss
	_BYTE	 diskette_num
EndStruc DISKETTE_BLK_S

DefStruc ,printer_blk_ss
	_BYTE	 printer_num
	_BYTE	 dot_size
	_BYTE	 color_num
	_BYTE	 prn_character
	_WORD	 prn_type
EndStruc PRINTER_BLK_S

DefStruc ,timer_blk_ss
	_BYTE	 tb_byte0
EndStruc TIMER_BLK_S

DefStruc ,video_blk_ss
	_BYTE	 flags1
	_BYTE	 flags2
	_WORD	 regen_offset
	_WORD	 regen_segment
	_WORD	 regen_length
	_WORD	 shadow_offset
	_WORD	 shadow_segment
	_WORD	 video_page_info_offset
	_WORD	 video_page_info_segment
	_WORD	 video_mode_info_offset
	_WORD	 video_mode_info_segment
EndStruc VIDEO_BLK_S

DefStruc ,register_shadow_ss
	_WORD	 word0
	_WORD	 ls_offset
	_WORD	 ls_segment
	_WORD	 lg_offset
	_WORD	 lg_segment
EndStruc REGISTER_SHADOW_S

DefStruc ,cpu_blk_ss
	_BYTE	 cpu
EndStruc CPU_BLK_S

DefStruc ,jis_mode_blk_ss
	_BYTE	 jis_mode
EndStruc JIS_MODE_BLK_S

DefStruc ,extention_blk_ss
	_WORD	 ext,,16
EndStruc EXTENTION_BLK_S

DefStruc ,kbd_inf_blk_ss
	_BYTE	 rk_flag
	_DWORD	 roman_adr
	_DWORD	 kanakan_adr
	_DWORD	 dict_top_adr
	_DWORD	 mast_top_adr
	_DWORD	 c_top_adr
	_DWORD	 kmode_adr
	_DWORD	 hptr_adr
	_DWORD	 apkanji_adr
	_DWORD	 nodisp_mode_adr
	_DWORD	 phrase_conv_inf_adr
EndStruc KBD_INF_BLK_S

DefStruc ,video_inf_blk_ss
	_DWORD	 acorn_mode_adr
	_DWORD	 vib_common_disp_adr
EndStruc VIDEO_INF_BLK_S

DefStruc ,prt_inf_blk_ss
	_DWORD	 print_id_adr
	_DWORD	 must_release_adr
	_DWORD	 monitor_flag_adr
	_DWORD	 dma_check_flag_adr
	_DWORD	 reset_ecc_adr
	_DWORD	 ps_high_low_adr
EndStruc PRT_INF_BLK_S

DefStruc ,common_inf_blk_ss
	_DWORD	 beep_adr
	_DWORD	 service_adr
	_DWORD	 dict_loader_adr
	_DWORD	 click_click_adr
	_DWORD	 soft_kankan_adr
	_DWORD	 delay_adr
	_DWORD	 file_loader_adr
	_DWORD	 cib_common_disp_adr
EndStruc COMMON_INF_BLK_S

DefStruc ,d_d_inf_blk_ss
	_DWORD	 ps_flag_adr
	_WORD	 hdd_start
	_WORD	 hdd_end
	_WORD	 hdd_start_block
	_WORD	 hdd_end_block
	_WORD	 hdd_start_cyl
	_WORD	 hdd_end_cyl
	_WORD	 mf35_0CH_adr
	_WORD	 mf35_13H_adr
EndStruc D_D_INF_BLK_S

DefStruc ,bank_inf_blk_ss
	_BYTE	 bank0,,3
	_BYTE	 bank1,,3
	_BYTE	 bank2,,3
EndStruc BANK_INF_BLK_S

DefStruc ,sear_inf_blk_ss
	_BYTE	 sib_dummy
EndStruc SEAR_INF_BLK_S

DefStruc ,mem_ctl_blk_ss
	_BYTE	 mcb_dummy
EndStruc MEM_CTL_BLK_S

DefStruc ,video_page_information_ss			;j-ys0914
	_WORD	 vpinfo_nb					     ;j-ys0914
	_WORD	 vpinfo_number_of_pages				     ;j-ys0914
	_BYTE	 vpinfo_page_allocation_table,,8		     ;j-ys0914
EndStruc VIDEO_PAGE_INFORMATION_S								;j-ys0914

DefStruc ,videomode_info_ss			;j-ys0914
	_WORD	 vminfo_nb					     ;j-ys0914
	_BYTE	 vminfo_video_mode				     ;j-ys0914
	_BYTE	 vminfo_number_of_planes			     ;j-ys0914
	_BYTE	 vminfo_foreground_color			     ;j-ys0914
	_BYTE	 vminfo_background_color			     ;j-ys0914
	_BYTE	 vminfo_CPU_page				     ;j-ys0914
	_BYTE	 vminfo_CRT_page				     ;j-ys0914
EndStruc VIDEOMODE_INFO_S								;j-ys0914

DefStruc ,biosjdata_s

    ;BIOS-J Data Area Length
    _WORD    biosjlen

    ;Keyboard Area
    _WORD    fdabuffer_head
    _WORD    fdabuffer_tail
    _WORD    fdabuffer_len
    _WORD    fdabuffer_start
    _WORD    fdabuffer_end
    _BYTE    fdabios_break
    _WORD    fdareset_flag
    _BYTE    fdakbd_id,,3
    _BYTE    fdajkb_flag
    _BYTE    fdawork
    _BYTE    fdabeep_status
    _WORD    fdabeep_freq

    ;Diskette Area
    _BYTE    seek_status
    _BYTE    diskette_status
    _BYTE    nec_status,,7
    _BYTE    work_status
    _BYTE    nec_copy,,7
    _BYTE    dvt0,,4
    _WORD    motor_status
    _BYTE    chip_status
    _BYTE    adap_status
    _BYTE    int_status
    _BYTE    motor_flag
    _BYTE    status_status

    ;Disk Area
    _BYTE    ipldrv
    _BYTE    mode
    _WORD    flag_tati
    DRVTBL_S drv0_table
    DRVTBL_S drv1_table

    ;Video Area
    _BYTE    crt_mode
    _WORD    crt_cols
    _WORD    crt_len
    _BYTE    dmy_crt_reg,,16
    _BYTE    dmy_cmd_reg
    _BYTE    dmy_col_reg
    BA_S base_attr

    ;Video Area (Cursor Position)
    _WORD    cursor_posn
    _WORD    grph_posn
    _WORD    cursor_mode
    _WORD    gr_crs_mode
    _WORD    save_ccp
    _WORD    save_wrt
    _BYTE    tty_char_save
TAB_LENGTH	EQU	13
    _WORD    alt_posn
    _WORD    alt_grph_posn
    _WORD    alt_mode
    _WORD    alt_gr_crs
    _WORD    alt_save_ccp
    _WORD    alt_save_wrt
    _BYTE    alt_tty_save
    _WORD    cpp1
    _WORD    cpp2
    _WORD    cpp3
    _WORD    cpp4
    _WORD    cpp5
    _WORD    cpp6
    _WORD    cpp7
    _WORD    cpp8
    _BYTE    current_page
    _WORD    addr_6845
    _BYTE    crt_mode_set
    _BYTE    crt_palette

    ;Video Area (EGA)
    _BYTE    rows
    _WORD    bytesperchar
    _BYTE    ega_mode
    _BYTE    xswitch

    ;Phoenix Area (LS_PORT)
    LSS_S   ls_save					       ;@V3.1MNH01

    ;Phoenix Area (LG_PORT)
    LGS_S lg_save

    ;Timer Area
    _WORD    timer_low
    _WORD    timer_high

    ;Keyboard Buffer Area
    _WORD    fdakb_buffer,,240
    _BYTE    apl_flag

    ;Printer Area
    _BYTE    vlpt_data,,40

    ;Dummy _INT 18H (CDH, 18H)
    _BYTE    int18code,,40

    ;_INT 1FH Area
    BIOS_VERSION_BLK_S bios_version_blk

    COMMUNICATION_BLK_S communication_blk

    ASYNCRONUS_BLK_S asyncronus_blk

    HDISK_BLK_S hdisk_blk

    KEYBOARD_BLK_S keyboard_blk

    DISKETTE_BLK_S diskette_blk

    PRINTER_BLK_S printer_blk

    TIMER_BLK_S timer_blk

    VIDEO_BLK_S video_blk

    REGISTER_SHADOW_S register_shadow

    CPU_BLK_S cpu_blk

    JIS_MODE_BLK_S jis_mode_blk

    EXTENTION_BLK_S extention_blk

    KBD_INF_BLK_S kbd_inf_blk

    VIDEO_INF_BLK_S video_inf_blk

    PRT_INF_BLK_S prt_inf_blk

    COMMON_INF_BLK_S common_inf_blk

    D_D_INF_BLK_S d_d_inf_blk

    BANK_INF_BLK_S bank_inf_blk

    SEAR_INF_BLK_S sear_inf_blk

    MEM_CTL_BLK_S mem_ctl_blk

    VIDEO_PAGE_INFORMATION_S videopageinfo		     ;j-ys0914

    VIDEOMODE_INFO_S videomodeinfo			     ;j-ys0914


EndStruc BIOSJDATA

DefType PBIOSJDATA,dd

;J-YF004 08/27/91 DOS/V mode support
;J-YF005 10/15/91 Bug Fix (EBIOS length field is _BYTE)
