/***    VDMBIOSJ.H
 *
 *      SCCSID = @(#)vdmbiosj.h    xx.xx 90/05/28
 *
 *      MVDM BIOSJ Data Definitions
 *      Copyright (c) 1990 IBM Japan Corporation
 *
 *      For include file hierarchy, see MVDM.H
 *
 * CHANGE ACTIVITY =
 *   DATE      FLAG        APAR   CHANGE DESCRIPTION
 *   --------  ----------  -----  --------------------------------------
 *   mm/dd/yy  @Vr.mpppxx  xxxxx  xxxxxxx
 *   02/15/96  @V3.1MNH01 D148300 Merge DBCS-J (IBM J) source w/SBCS source
 ***********************************************************************/

#define EBIOS_BIOSMODE  0x0121          // bios mode flag       j-ea0000


#define BIOSJDATA_LEN   0x00800         // reserve 2K
//c-begin
#define BIOSJDATA_PAGES PAGESFROMBYTES(BIOSJDATA_LEN)
//c-end

//c-begin
#define EBIOSDATA_SEG   (*(PWORD)(0x40E))
#define EBIOSDATA_ADR   (EBIOSDATA_SEG << 4)
//J-YF005#define BIOSJDATA_ADR   (EBIOSDATA_ADR + (*(PWORD)(EBIOSDATA_ADR) * 1024))
#define BIOSJDATA_ADR   (EBIOSDATA_ADR + (*(PBYTE)(EBIOSDATA_ADR) * 1024))      //J-YF005
#define BIOSJDATA_SEG   (BIOSJDATA_ADR >> 4)
//c-end

//c-begin
#define BIOSMODE        (*(PBYTE)(EBIOSDATA_ADR + EBIOS_BIOSMODE))
//c-end
#define PS2_MODE        0x00            // PS/2 mode BIOS       (will be deleted later...) //J-YF001
#define PS55_MODE       0x01            // PS/55 DBCS mode BIOS (will be deleted later...) //J-YF001
#define PCDOS_MODE      0x00            // PC-DOS mode BIOS           //J-YF004
#define DOSJ_MODE       0x01            // DOS-J mode BIOS            //J-YF004
#define DOSV_MODE       0x02            // DOS/V mode BIOS            //J-YF004
#define DOSK_MODE       0x03            // DOS/K mode BIOS


/***    Int 16h Equates (AH values)
 */

#define INT16J_BEEPON           0x03                                    //ss0003
#define INT16J_BEEPOFF          0x04                                    //ss0003
#define INT16J_SETSTATE         0x05                                    //ss0003
#define INT16J_SETSTATE2        0x85                                    //ss0003
#define INT16J_READCHGXTBL      0x06                                    //ss0004
#define INT16J_CHGDISP          0x07                                    //ss0003
#define INT16J_SETSHIFT         0x82                                    //ss0003
#define INT16J_GETKBDFUNC       0x09                                    //j-mi1110
#define INT16J_GETKBDHWID       0x0A                                    //j-mi1110
#define INT16J_GETKBDTYPE       0x0B                                    //ss0003
#define INT16J_READCHAR         0xF0                                    //ss0003
#define INT16J_PEEKCHAR         0xF1                                    //ss0003
#define INT16J_FLUSHBUFF        0xF2                                    //ss0003
#define INT16V_RETKBDID         0x0A         // DOSV return KBD ID   //j-mh0001
#define INT16V_DBCSSHIFT        0x13                                 //j-mh0001
#define INT16V_SETDSHIFT        0x00         // INT16 AH=13 AL=00    //j-mh0001
#define INT16V_GETDSHIFT        0x01         // INT16 AH=13 AL=01    //j-mh0001
#define INT16V_CHGDISP          0x14                                 //j-mh0001
#define INT16V_ENADISP          0x00         // INT16 AH=14 AL=00    //j-mh0001
#define INT16V_DISDISP          0x01         // INT16 AH=14 AL=01    //j-mh0001
#define INT16V_QUEDISP          0x02         // INT16 AH=14 AL=02    //j-mh0001


/***ET+ MVDM BIOSJ Data Definitions - used by VDDs */

/***    BIOSJ Data Area
 *
 *      This area cannot exceed BIOSJDATA_LEN
 */

typedef struct drvtbl_ss {
        WORD    max_cyl_m;
        BYTE    max_head_m;
        WORD    dummy1;
        WORD    dummy2;
        WORD    dummy3;
        BYTE    dummy4[7];
        BYTE    dummy5;
        WORD    start_chr_m;
        BYTE    dummy6;
        BYTE    dummy7;
        WORD    dummy8;
        BYTE    dummy9;
        BYTE    dummy10;
} DRVTBL_S;
//c-begin
#define TABLE_LEN sizeof(DRVTBL_S)
//c-end

typedef struct ba_ss {
        BYTE    normal;
        BYTE    intensify;
} BA_S;

typedef struct lss_ss {
        BYTE    ls_reset;
        BYTE    ls_mode;
        BYTE    ls_bank;
} LSS_S;

typedef struct lgs_ss {
        BYTE    set_reset;
        BYTE    enable_sr;
        BYTE    color_compare;
        BYTE    data_rotation;
        BYTE    read_map_select;
        BYTE    lg_mode;
        BYTE    complement;
        BYTE    color_dont_care;
        BYTE    bit_mask_low;
        BYTE    bit_mask_high;
        BYTE    map_mask;
        BYTE    lg_command;
        BYTE    set_reset_2;
} LGS_S;

typedef struct bios_version_blk_ss {
        WORD    bios_version;
} BIOS_VERSION_BLK_S;
//c-begin
#define bios_version_blk_len sizeof(BIOS_VERSION_BLK_S)
//c-end

typedef struct communication_blk_ss {
        BYTE    cb_byte0;
        BYTE    sdlc_bca_num;
        BYTE    tca_num;
} COMMUNICATION_BLK_S;

typedef struct asyncronus_ss {
        BYTE    ab_byte0;
} ASYNCRONUS_BLK_S;

typedef struct hdisk_blk_ss {
        BYTE    int_hdisk_num;
        BYTE    ext_hdisk_num;
} HDISK_BLK_S;

typedef struct keyboard_blk_ss {
        BYTE    kbdid_1st;
        BYTE    kbdid_2nd;
        BYTE    kbdid_3rd;
        BYTE    kbd_character;
        WORD    table_offset;
        WORD    table_segment;
} KEYBOARD_BLK_S;

typedef struct diskette_blk_ss {
        BYTE    diskette_num;
} DISKETTE_BLK_S;

typedef struct printer_blk_ss {
        BYTE    printer_num;
        BYTE    dot_size;
        BYTE    color_num;
        BYTE    prn_character;
        WORD    prn_type;
} PRINTER_BLK_S;

typedef struct timer_blk_ss {
        BYTE    tb_byte0;
} TIMER_BLK_S;

typedef struct video_blk_ss {
        BYTE    flags1;
        BYTE    flags2;
        WORD    regen_offset;
        WORD    regen_segment;
        WORD    regen_length;
        WORD    shadow_offset;
        WORD    shadow_segment;
        WORD    video_page_info_offset;
        WORD    video_page_info_segment;
        WORD    video_mode_info_offset;
        WORD    video_mode_info_segment;
} VIDEO_BLK_S;

typedef struct register_shadow_ss {
        WORD    word0;
        WORD    ls_offset;
        WORD    ls_segment;
        WORD    lg_offset;
        WORD    lg_segment;
} REGISTER_SHADOW_S;

typedef struct cpu_blk_ss {
        BYTE    cpu;
} CPU_BLK_S;

typedef struct jis_mode_blk_ss {
        BYTE    jis_mode;
} JIS_MODE_BLK_S;

typedef struct extention_blk_ss {
        WORD    ext[16];
} EXTENTION_BLK_S;

typedef struct kbd_inf_blk_ss {
        BYTE    rk_flag;
        DWORD   roman_adr;
        DWORD   kanakan_adr;
        DWORD   dict_top_adr;
        DWORD   mast_top_adr;
        DWORD   c_top_adr;
        DWORD   kmode_adr;
        DWORD   hptr_adr;
        DWORD   apkanji_adr;
        DWORD   nodisp_mode_adr;
        DWORD   phrase_conv_inf_adr;
} KBD_INF_BLK_S;

typedef struct video_inf_blk_ss {
        DWORD   acorn_mode_adr;
        DWORD   vib_common_disp_adr;
} VIDEO_INF_BLK_S;

typedef struct prt_inf_blk_ss {
        DWORD   print_id_adr;
        DWORD   must_release_adr;
        DWORD   monitor_flag_adr;
        DWORD   dma_check_flag_adr;
        DWORD   reset_ecc_adr;
        DWORD   ps_high_low_adr;
} PRT_INF_BLK_S;

typedef struct common_inf_blk_ss {
        DWORD   beep_adr;
        DWORD   service_adr;
        DWORD   dict_loader_adr;
        DWORD   click_click_adr;
        DWORD   soft_kankan_adr;
        DWORD   delay_adr;
        DWORD   file_loader_adr;
        DWORD   cib_common_disp_adr;
} COMMON_INF_BLK_S;

typedef struct d_d_inf_blk_ss {
        DWORD   ps_flag_adr;
        WORD    hdd_start;
        WORD    hdd_end;
        WORD    hdd_start_block;
        WORD    hdd_end_block;
        WORD    hdd_start_cyl;
        WORD    hdd_end_cyl;
        WORD    mf35_0CH_adr;
        WORD    mf35_13H_adr;
} D_D_INF_BLK_S;

typedef struct bank_inf_blk_ss {
        BYTE    bank0[3];
        BYTE    bank1[3];
        BYTE    bank2[3];
} BANK_INF_BLK_S;

typedef struct sear_inf_blk_ss {
        BYTE    sib_dummy;
} SEAR_INF_BLK_S;

typedef struct mem_ctl_blk_ss {
        BYTE    mcb_dummy;
} MEM_CTL_BLK_S;

typedef struct video_page_information_ss {                                 //j-ys0914
        WORD    vpinfo_nb;                                           //j-ys0914
        WORD    vpinfo_number_of_pages;                              //j-ys0914
        BYTE    vpinfo_page_allocation_table[8];                     //j-ys0914
} VIDEO_PAGE_INFORMATION_S;                                                               //j-ys0914

typedef struct videomode_info_ss {                                        //j-ys0914
        WORD    vminfo_nb;                                           //j-ys0914
        BYTE    vminfo_video_mode;                                   //j-ys0914
        BYTE    vminfo_number_of_planes;                             //j-ys0914
        BYTE    vminfo_foreground_color;                             //j-ys0914
        BYTE    vminfo_background_color;                             //j-ys0914
        BYTE    vminfo_CPU_page;                                     //j-ys0914
        BYTE    vminfo_CRT_page;                                     //j-ys0914
} VIDEOMODE_INFO_S;                                                               //j-ys0914

typedef struct biosjdata_s {

    /* BIOS-J Data Area Length */
    WORD    biosjlen;

    /* Keyboard Area */
    WORD    fdabuffer_head;
    WORD    fdabuffer_tail;
    WORD    fdabuffer_len;
    WORD    fdabuffer_start;
    WORD    fdabuffer_end;
    BYTE    fdabios_break;
    WORD    fdareset_flag;
    BYTE    fdakbd_id[3];
    BYTE    fdajkb_flag;
    BYTE    fdawork;
    BYTE    fdabeep_status;
    WORD    fdabeep_freq;

    /* Diskette Area */
    BYTE    seek_status;
    BYTE    diskette_status;
    BYTE    nec_status[7];
    BYTE    work_status;
    BYTE    nec_copy[7];
    BYTE    dvt0[4];
    WORD    motor_status;
    BYTE    chip_status;
    BYTE    adap_status;
    BYTE    int_status;
    BYTE    motor_flag;
    BYTE    status_status;

    /* Disk Area */
    BYTE    ipldrv;
    BYTE    mode;
    WORD    flag_tati;
    DRVTBL_S drv0_table;
    DRVTBL_S drv1_table;

    /* Video Area */
    BYTE    crt_mode;
    WORD    crt_cols;
    WORD    crt_len;
    BYTE    dmy_crt_reg[16];
    BYTE    dmy_cmd_reg;
    BYTE    dmy_col_reg;
    BA_S base_attr;

    /* Video Area (Cursor Position) */
    WORD    cursor_posn;
    WORD    grph_posn;
    WORD    cursor_mode;
    WORD    gr_crs_mode;
    WORD    save_ccp;
    WORD    save_wrt;
    BYTE    tty_char_save;
#define TAB_LENGTH 13
    WORD    alt_posn;
    WORD    alt_grph_posn;
    WORD    alt_mode;
    WORD    alt_gr_crs;
    WORD    alt_save_ccp;
    WORD    alt_save_wrt;
    BYTE    alt_tty_save;
    WORD    cpp1;
    WORD    cpp2;
    WORD    cpp3;
    WORD    cpp4;
    WORD    cpp5;
    WORD    cpp6;
    WORD    cpp7;
    WORD    cpp8;
    BYTE    current_page;
    WORD    addr_6845;
    BYTE    crt_mode_set;
    BYTE    crt_palette;

    /* Video Area (EGA) */
    BYTE    rows;
    WORD    bytesperchar;
    BYTE    ega_mode;
    BYTE    xswitch;

    /* Phoenix Area (LS_PORT) */
    LSS_S   ls_save;                                            /*@V3.1MNH01*/

    /* Phoenix Area (LG_PORT) */
    LGS_S lg_save;

    /* Timer Area */
    WORD    timer_low;
    WORD    timer_high;

    /* Keyboard Buffer Area */
    WORD    fdakb_buffer[240];
//c-begin
#define FDAKB_BUFFER_LEN sizeof(fdakb_buffer)
//c-end
    BYTE    apl_flag;

    /* Printer Area */
    BYTE    vlpt_data[40];

    /* Dummy INT 18H (CDH, 18H) */
    BYTE    int18code[40];

    /* INT 1FH Area */
    BIOS_VERSION_BLK_S bios_version_blk;

    COMMUNICATION_BLK_S communication_blk;

    ASYNCRONUS_BLK_S asyncronus_blk;

    HDISK_BLK_S hdisk_blk;

    KEYBOARD_BLK_S keyboard_blk;

    DISKETTE_BLK_S diskette_blk;

    PRINTER_BLK_S printer_blk;

    TIMER_BLK_S timer_blk;

    VIDEO_BLK_S video_blk;

    REGISTER_SHADOW_S register_shadow;

    CPU_BLK_S cpu_blk;

    JIS_MODE_BLK_S jis_mode_blk;

    EXTENTION_BLK_S extention_blk;

    KBD_INF_BLK_S kbd_inf_blk;

    VIDEO_INF_BLK_S video_inf_blk;

    PRT_INF_BLK_S prt_inf_blk;

    COMMON_INF_BLK_S common_inf_blk;

    D_D_INF_BLK_S d_d_inf_blk;

    BANK_INF_BLK_S bank_inf_blk;

    SEAR_INF_BLK_S sear_inf_blk;

    MEM_CTL_BLK_S mem_ctl_blk;

    VIDEO_PAGE_INFORMATION_S videopageinfo;                   //j-ys0914

    VIDEOMODE_INFO_S videomodeinfo;                           //j-ys0914

//c-begin
    BYTE    b0;      //test test test                           /*@V3.1MNH01*/
    BYTE    b1;      //test test test                           /*@V3.1MNH01*/
    BYTE    b2;      //test test test                           /*@V3.1MNH01*/
    BYTE    b3;      //test test test                           /*@V3.1MNH01*/
    BYTE    b4;      //test test test                           /*@V3.1MNH01*/
    BYTE    b5;      //test test test                           /*@V3.1MNH01*/
//c-end

} BIOSJDATA;

typedef BIOSJDATA *PBIOSJDATA;

//J-YF004 08/27/91 DOS/V mode support
//J-YF005 10/15/91 Bug Fix (EBIOS length field is BYTE)
