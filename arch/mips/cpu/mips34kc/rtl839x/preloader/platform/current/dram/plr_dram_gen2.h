#ifndef PLR_DRAM_GEN2_H
#define PLR_DRAM_GEN2_H

/* Memory controller register address and related field constant */
#define MCR_A               (0xB8001000)
#define MCR_DRAMTYPE_MASK   (0xF0000000)
#define MCR_DRAMTYPE_DDR    (0x00000000)
#define MCR_DRAMTYPE_DDR2   (0x10000000)
#define MCR_DRAMTYPE_DDR3   (0x20000000)
#define MCR_IPREF_MASK 		(0x1 << DG2_REG_OFFSET_IPREF)
#define MCR_DPREF_MASK 		(0x1 << DG2_REG_OFFSET_DPREF)
#define MCR_PREF_MASK		(MCR_IPREF_MASK | MCR_DPREF_MASK)
#define MCR_FLASH_MAP1_DIS_FD_S (18)
#define MCR_FLASH_MAP1_DIS_MASK (0x1 << MCR_FLASH_MAP1_DIS_FD_S)

#define DCR_A               (0xB8001004)
#define DCR_MASK			(DCR_BANKCNT_MASK|DCR_DBUSWID_MASK|DCR_ROWCNT_MASK|DCR_COLCNT_MASK	\
							|DCR_DCHIPSEL_MASK|DCR_FAST_RX_MASK|DCR_BSTREF_MASK)
#define DTR0_A              (0xB8001008)
#define DTR1_A              (0xB800100C)
#define DTR2_A              (0xB8001010)
#define DMCR_A              (0xB800101C)
#define DMCR_MRS_BUSY       (0x80000000)
#define DMCR_MR_MODE_EN     (0x00100000)
#define DMCR_MRS_MODE_MR    (0x00000000)
#define DMCR_MRS_MODE_EMR1  (0x00010000)
#define DMCR_MRS_MODE_EMR2  (0x00020000)
#define DMCR_MRS_MODE_EMR3  (0x00030000)
#define DMCR_DIS_DRAM_REF_FD_S  (24)
#define DMCR_DIS_DRAM_REF_MASK  (1 << DMCR_DIS_DRAM_REF_FD_S)
#define DMCR_MR_MODE_EN_FD_S    (20)
#define DMCR_MR_MODE_EN_MASK    (1 << DMCR_MR_MODE_EN_FD_S)


/* DDR Mode register related definition */
#define DDR1_MR_BURST_2     (0x00000001)
#define DDR1_MR_BURST_4     (0x00000002)
#define DDR1_MR_BURST_8     (0x00000003)
#define DDR1_MR_BURST_SEQ   (0x00000000)
#define DDR1_MR_BURST_INTER (0x00000008)
#define DDR1_MR_CAS_2       (0x00000020)
#define DDR1_MR_CAS_3       (0x00000030)
#define DDR1_MR_CAS_25      (0x00000060)
#define DDR1_MR_OP_NOR      (0x00000000)
#define DDR1_MR_OP_RST_DLL  (0x00000100)
#define DDR1_MR_OP_TEST     (0x00000080)

#define DDR1_EMR1_DLL_EN    (0x00000000)
#define DDR1_EMR1_DLL_DIS   (0x00000001)
#define DDR1_EMR1_DRV_NOR   (0x00000000)
#define DDR1_EMR1_DRV_WEAK  (0x00000002)

/* DDR2 Mode register related definition */
#define DDR2_MR_BURST_4     (0x00000002)
#define DDR2_MR_BURST_8     (0x00000003)
#define DDR2_MR_BURST_SEQ   (0x00000000)
#define DDR2_MR_BURST_INTER     (0x00000008)
#define DDR2_MR_CAS_2       (0x00000020)
#define DDR2_MR_CAS_3       (0x00000030)
#define DDR2_MR_CAS_4       (0x00000040)
#define DDR2_MR_CAS_5       (0x00000050)
#define DDR2_MR_CAS_6       (0x00000060)
#define DDR2_MR_TM_NOR      (0x00000000)
#define DDR2_MR_TM_TEST     (0x00000080)
#define DDR2_MR_DLL_RESET_YES   (0x00000100)
#define DDR2_MR_DLL_RESET_NO    (0x00000000)
#define DDR2_MR_WR_2        (0x00000200)
#define DDR2_MR_WR_3        (0x00000400)
#define DDR2_MR_WR_4        (0x00000600)
#define DDR2_MR_WR_5        (0x00000800)
#define DDR2_MR_WR_6        (0x00000A00)
#define DDR2_MR_WR_7        (0x00000C00)
#define DDR2_MR_PD_FAST     (0x00000000)
#define DDR2_MR_PD_SLOW     (0x00001000)
#define DDR2_EMR1_DLL_EN    (0x00000000)
#define DDR2_EMR1_DLL_DIS   (0x00000001)
#define DDR2_EMR1_DIC_FULL  (0x00000000)
#define DDR2_EMR1_DIC_REDUCE    (0x00000002)
#define DDR2_EMR1_RTT_DIS   (0x00000000)
#define DDR2_EMR1_RTT_75    (0x00000004)
#define DDR2_EMR1_RTT_150   (0x00000040)
#define DDR2_EMR1_RTT_50    (0x00000044)
#define DDR2_EMR1_ADD_0     (0x00000000)
#define DDR2_EMR1_ADD_1     (0x00000008)
#define DDR2_EMR1_ADD_2     (0x00000010)
#define DDR2_EMR1_ADD_3     (0x00000018)
#define DDR2_EMR1_ADD_4     (0x00000020)
#define DDR2_EMR1_ADD_5     (0x00000028)
#define DDR2_EMR1_OCD_EX    (0x00000000)
#define DDR2_EMR1_OCD_D1    (0x00000080)
#define DDR2_EMR1_OCD_D0    (0x00000100)
#define DDR2_EMR1_OCD_AD    (0x00000200)
#define DDR2_EMR1_OCD_DEF   (0x00000380)
#define DDR2_EMR1_QOFF_EN   (0x00000000)
#define DDR2_EMR1_QOFF_DIS  (0x00001000)
#define DDR2_EMR1_NDQS_EN   (0x00000000)
#define DDR2_EMR1_NDQS_DIS  (0x00000400)
#define DDR2_EMR1_RDQS_EN   (0x00000800)
#define DDR2_EMR1_RDQS_DIS  (0x00000000)
#define DDR2_EMR2_HTREF_EN  (0x00000080)
#define DDR2_EMR2_HTREF_DIS (0x00000000)
#define DDR2_EMR2_DCC_DIS   (0x00000000)
#define DDR2_EMR2_DCC_EN    (0x00000008)
#define DDR2_EMR2_PASELF_FULL   (0x00000000)


/* DDR3 Mode register related definition */
#define DDR3_MR_BURST_8         (0x00000000)
#define DDR3_MR_BURST_BC4OR8        (0x00000001)
#define DDR3_MR_BURST_BC4       (0x00000002)
#define DDR3_MR_READ_BURST_NIBBLE   (0x00000000)
#define DDR3_MR_READ_BURST_INTER    (0x00000008)
#define DDR3_MR_CAS_5           (0x00000010)
#define DDR3_MR_CAS_6           (0x00000020)
#define DDR3_MR_CAS_7           (0x00000030)
#define DDR3_MR_CAS_8           (0x00000040)
#define DDR3_MR_CAS_9           (0x00000050)
#define DDR3_MR_CAS_10          (0x00000060)
#define DDR3_MR_CAS_11          (0x00000070)
#define DDR3_MR_TM_NOR          (0x00000000)
#define DDR3_MR_TM_TEST         (0x00000080)
#define DDR3_MR_DLL_RESET_YES       (0x00000100)
#define DDR3_MR_DLL_RESET_NO        (0x00000000)
#define DDR3_MR_WR_5            (0x00000200)
#define DDR3_MR_WR_6            (0x00000400)
#define DDR3_MR_WR_7            (0x00000600)
#define DDR3_MR_WR_8            (0x00000800)
#define DDR3_MR_WR_9            (0x00000A00)
#define DDR3_MR_WR_10           (0x00000C00)
#define DDR3_MR_WR_12           (0x00000E00)
#define DDR3_MR_PD_FAST         (0x00001000)
#define DDR3_MR_PD_SLOW         (0x00000000)
#define DDR3_EMR1_DLL_EN        (0x00000000)
#define DDR3_EMR1_DLL_DIS       (0x00000001)
#define DDR3_EMR1_DIC_RZQ_DIV_6     (0x00000000)
#define DDR3_EMR1_DIC_RZQ_DIV_7     (0x00000002)
#define DDR3_EMR1_RTT_NOM_DIS       (0x00000000)
#define DDR3_EMR1_RTT_NOM_RZQ_DIV4  (0x00000004)
#define DDR3_EMR1_RTT_NOM_RZQ_DIV2  (0x00000040)
#define DDR3_EMR1_RTT_NOM_RZQ_DIV6  (0x00000044)
#define DDR3_EMR1_RTT_NOM_RZQ_DIV12 (0x00000200)
#define DDR3_EMR1_RTT_NOM_RZQ_DIV8  (0x00000204)
#define DDR3_EMR1_ADD_0         (0x00000000)
#define DDR3_EMR1_ADD_CL_RD1        (0x00000008)
#define DDR3_EMR1_ADD_CL_RD2        (0x00000010)
#define DDR3_EMR1_WRITE_LEVEL_DIS   (0x00000000)
#define DDR3_EMR1_WRITE_LEVEL_EN    (0x00000080)
#define DDR3_EMR1_TDQS_DIS      (0x00000000)
#define DDR3_EMR1_TDQS_EN       (0x00000800)
#define DDR3_EMR1_QOFF_DIS      (0x00001000)
#define DDR3_EMR1_QOFF_EN       (0x00000000)
#define DDR3_EMR2_PASR_FULL     (0x00000000)
#define DDR3_EMR2_PASR_HALF_L       (0x00000001)
#define DDR3_EMR2_PASR_QUA      (0x00000002)
#define DDR3_EMR2_PASR_8TH_L        (0x00000003)
#define DDR3_EMR2_PASR_3_QUA        (0x00000004)
#define DDR3_EMR2_PASR_HALF_H       (0x00000005)
#define DDR3_EMR2_PASR_8TH_H        (0x00000007)
#define DDR3_EMR2_CWL_5         (0x00000000)
#define DDR3_EMR2_CWL_6         (0x00000008)
#define DDR3_EMR2_CWL_7         (0x00000010)
#define DDR3_EMR2_CWL_8         (0x00000018)
#define DDR3_EMR2_ASR_EN        (0x00000040)
#define DDR3_EMR2_ASR_DIS       (0x00000000)
#define DDR3_EMR2_SRT_NOR       (0x00000000)
#define DDR3_EMR2_SRT_EXT       (0x00000080)
#define DDR3_EMR2_RTT_WR_DIS        (0x00000000)
#define DDR3_EMR2_RTT_WR_RZQ_DIV_4  (0x00000200)
#define DDR3_EMR2_RTT_WR_RZQ_DIV_2  (0x00000400)
#define DDR3_EMR3_MPR_LOC_PRE_PAT   (0x00000000)
#define DDR3_EMR3_MPR_OP_NOR        (0x00000000)
#define DDR3_EMR3_MPR_OP_DATA       (0x00000004)

#define MPMR0_A             (0xB8001040)
#define MPMR1_A             (0xB8001044)
#define DIDER_A             (0xB8001050)
#define D23OSCR_A           (0xB800107C)
#define DACCR_A             (0xB8001500)
#define DACSPCR_A           (0xB8001504)
#define DACSPAR_A           (0xB8001508)
#define DACDQ0RR_A          (0xB8001510)
#define STATIC_CAL_DATA_BASE DACDQ0RR_A
#define STATIC_CAL_DATA_LEN (33)

/* Definitions for software calibration function */
#define MEMCTL_INVALID_CLKM_DELAY_VALUE (0xffffffff)
#define MEMCTL_CALI_FAIL          (1)
//#define MEMCTL_DDR1_SUPPORT
#define MEMCTL_DDR2_SUPPORT
#define MEMCTL_DDR3_SUPPORT
#define _memctl_debug_printf_I printf
//#define _memctl_debug_printf printf
#define _memctl_debug_printf(...)
//#define _memctl_debug_printf_I(...)


/* ToDo: Move the following setting to platform dependent files(Apollo) */
#define DDRCKODL_A          (0xB800021C)
#define REG_OFFSET_DDRCKM90_TAP  16
#define REG_OFFSET_DDRCKM_TAP    8 
#define REG_OFFSET_DDRCK_PHS_SEL 0


#define DRAMI (parameters.soc.dram_info)
#define GET_DDR_TYPE()          ((*((volatile u32_t *)MCR_A)>>28)&0xF)
#define DDR_TYPE_DDR1           0
#define DDR_TYPE_DDR2           1
#define DDR_TYPE_DDR3           2

#define NS2CLK_ROUNDUP(freq, val)    (freq*val+999)/1000

typedef union {
    struct {
        unsigned int dummy:10;
        unsigned int cfg_force:1;
        unsigned int e2:1;
        unsigned int nt:3;
        unsigned int pt:3;
        unsigned int plsb:2;
        unsigned int ttcn:3;
        unsigned int ttcp:3;
        unsigned int ttfn:3;
        unsigned int ttfp:3;
    } f;
    unsigned int v;
} zq_force_v1_t;

typedef union {
    struct {
        unsigned int dummy:22;
        unsigned int e2:1;
        unsigned int nt:3;
        unsigned int pt:3;
        unsigned int plsb:2;
        unsigned int cfg_force:1;
    } f;
    unsigned int v;
} zq_force_v2_t;

typedef union {
    struct {
        unsigned int dummy:1;
        unsigned int cali_done:1;
        unsigned int cali_err:1;
        unsigned int odtp_pull_up:2;
        unsigned int odtp_pull_down:2;
        unsigned int odtn_pull_up:2;
        unsigned int odtn_pull_down:2;
        unsigned int ttfp:3;
        unsigned int ttcp:3;
        unsigned int plsb_1:1;
        unsigned int ttfn:3;
        unsigned int ttcn:3;
        unsigned int pt:3;
        unsigned int plsb_0:1;
        unsigned int nt:3;
        unsigned int e2:1;
    } f;
    unsigned int v;
} zctrl_status_t;

typedef union {
    struct {
        unsigned int cas:4;
        unsigned int wr:4;
        unsigned int cwl:4;
        unsigned int rtp:4;
        unsigned int wtr:4;
        unsigned int refi:4;
        unsigned int refiunit:4;
        unsigned int dummy:4;
    } f;
    unsigned int v;
} memctrl_dtr0_t;

typedef union {
    struct {
        unsigned int dummy0:4;
        unsigned int rp:4;
        unsigned int dummy1:4;
        unsigned int rcd:4;
        unsigned int dummy2:4;
        unsigned int rrd:4;
        unsigned int dummy3:3;
        unsigned int fawg:5;
    } f;
    unsigned int v;
} memctrl_dtr1_t;

typedef union {
    struct {
        unsigned int dummy0:4;
        unsigned int rfc:8;
        unsigned int dummy1:2;
        unsigned int ras:6;
        unsigned int dummy2:12;
    } f;
    unsigned int v;
} memctrl_dtr2_t;

typedef union {
    struct {
        unsigned int tx_delay:5;
        unsigned int clkm_delay:5;
        unsigned int m90_delay:5;
        unsigned int dummy:17;
    } f;
    unsigned int v;
} memctrl_dcdr_t;

typedef union {
    struct {
        unsigned int dqr_ac_en:1;
        unsigned int dummy0:2;
        unsigned int dq_phase_shift_90:5;
        unsigned int dummy1:3;
        unsigned int dq_phase_max_tap:5;
        unsigned int dummy2:3;
        unsigned int dq_phase_cur_tap:5;
        unsigned int dummy3:3;
        unsigned int dq_phase_min_tap:5;
    } f;
    unsigned int v;
} memctrl_dacdq_t;

const unsigned int * memctl_get_size_array(unsigned int type, unsigned int buswidth);
void memctlc_set_refi (unsigned int *dtr_reg, unsigned int val);

#endif /* #ifndef PLR_DRAM_GEN2_H */
