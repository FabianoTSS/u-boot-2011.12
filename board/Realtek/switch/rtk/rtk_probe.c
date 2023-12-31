/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : Related definition of the RTK board probe for U-Boot.
 *
 * Feature : RTK board probe function
 *
 */


/*
 * Include Files
 */
#include <config.h>
#include <rtk_osal.h>
#include <rtk_switch.h>
#if defined(CONFIG_RTK_BOARD)
  #if defined(CONFIG_RTL8328)
    #if defined(CONFIG_RTL8328M_8214F_DEMO)
    #include <rtk/conf/rtl8328m_8214f_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8328M_8214F_DEMO) */
    #if defined(CONFIG_RTL8328M_8214FB_DEMO)
    #include <rtk/conf/rtl8328m_8214fb_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8328M_8214FB_DEMO) */
    #if defined(CONFIG_RTL8328M_8212F_DEMO)
    #include <rtk/conf/rtl8328m_8212f_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8328M_8212F_DEMO) */
    #if defined(CONFIG_RTL8328S_2SFP_DEMO)
    #include <rtk/conf/rtl8328s_2sfp_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8328S_2SFP_DEMO) */
    #if defined(CONFIG_RTL8328L_8212B_DEMO)
    #include <rtk/conf/rtl8328l_8212b_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8328L_8212B_DEMO) */
    #if defined(CONFIG_RTL8328L_8212B_SERIAL_LED_DEMO)
    #include <rtk/conf/rtl8328l_8212b_serial_led_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8328L_8212B_SERIAL_LED_DEMO) */
  #endif /* end of #if defined(CONFIG_RTL8328) */
  #if defined(CONFIG_RTL8390)
    #if defined(CONFIG_RTL8396M_8214QF_8295R_ES_DEMO)
    #include <rtk/conf/rtl8396m_14qf_95r_es_demo_board.c>
    #endif/* end of #if defined(CONFIG_RTL8396M_8214QF_8295R_ES_DEMO) */
    #if defined(CONFIG_RTL8396M_8218B_8214QF_8295R_DEMO)
    #include <rtk/conf/rtl8396m_8218b_8214qf_8295r_demo.c>
    #endif/* end of #if defined(CONFIG_RTL8396M_8218B_8214QF_8295R_DEMO) */
    #if defined(CONFIG_RTL8396M_8214QF_8295R_DEMO)
    #include <rtk/conf/rtl8396m_8214qf_8295r_demo.c>
    #endif/* end of #if defined(CONFIG_RTL8396M_8214QF_8295R_DEMO) */
    #if defined(CONFIG_RTL8353M_QA)
    #include <rtk/conf/rtl8353m_qa_board.c>
    #endif /* end of #if defined(CONFIG_RTL8353M_QA) */
    #if defined(CONFIG_RTL8392M_QA)
    #include <rtk/conf/rtl8392m_qa_board.c>
    #endif /* end of #if defined(CONFIG_RTL8392M_QA) */
    #if defined(CONFIG_RTL8393M_QA)
    #include <rtk/conf/rtl8393m_qa_board.c>
    #endif /* end of #if defined(CONFIG_RTL8393M_QA) */
    #if defined(CONFIG_RTL8353M_DEMO)
    #include <rtk/conf/rtl8353m_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8353M_DEMO) */
    #if defined(CONFIG_RTL8353M_14B_DEMO)
    #include <rtk/conf/rtl8353m_14b_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8353M_14B_DEMO) */
    #if defined(CONFIG_RTL8351M_DEMO)
    #include <rtk/conf/rtl8351m_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8351M_DEMO) */
    #if defined(CONFIG_RTL8392M_DEMO)
    #include <rtk/conf/rtl8392m_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8392M_DEMO) */
    #if defined(CONFIG_RTL8393M_DEMO)
    #include <rtk/conf/rtl8393m_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8393M_DEMO) */
    #if defined(CONFIG_RTL8393M_18D)
    #include <rtk/conf/rtl8393m_8218d_board.c>
    #endif /* end of #if defined(CONFIG_RTL8393M_DEMO) */

    #include <rtk/conf/rtl8393m_8218b_8218fb_2sfp.c>
    #if defined(CONFIG_RTL8396M_DEMO)
    #include <rtk/conf/rtl8396m_demo_board.c>
    #include <rtk/conf/rtl8396m_demo2_board.c>
    #endif /* end of #if defined(CONFIG_RTL8396M_DEMO) */
    #if defined(CONFIG_RTL8391M_DEMO)
    #include <rtk/conf/rtl8391m_demo_board.c>
    #endif /* end of #if defined(CONFIG_RTL8391M_DEMO) */
    #if defined(CONFIG_RTL8391M_14C_DEMO)
    #include <rtk/conf/rtl8391m_14c_demo_board.c>
    #endif  /* end of CONFIG_RTL8391M_14C_DEMO */
	#if defined(CONFIG_RTL8391M_14QF_14QF_14FC_DEMO)
	#include <rtk/conf/rtl8391m_14qf_14qf_14fc_demo_board.c>
	#endif
    #if defined(CONFIG_RTL8393M_14C_DEMO)
    #include <rtk/conf/rtl8393m_14c_demo_board.c>
    #endif  /* end of CONFIG_RTL8393M_14C_DEMO */
    #if defined(CONFIG_RTL8393M_8218B_8214QF_DEMO)
    #include <rtk/conf/rtl8393m_8214b_8214qf_board.c>
    #endif  /* end of CONFIG_RTL8393M_8218B_8214QF_DEMO */	
  #endif /* end of #if defined(CONFIG_RTL8390) */

  #if defined(CONFIG_RTL8380)
    #if defined(CONFIG_RTL8382M_8218B_INTPHY_8218FB_DEMO)
    #include <rtk/conf/rtl8382m_8218b_intphy_8218fb_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_8214FC_DEMO)
    #include <rtk/conf/rtl8382m_8218b_intphy_8218b_8214fc_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_8214QF_DEMO)
    #include <rtk/conf/rtl8382m_8218b_intphy_8218b_8214qf_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8382M_8218D_INTPHY_8218D_8214FC_DEMO)
    #include <rtk/conf/rtl8382m_8218d_intphy_8218d_8214fc_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_8214B_DEMO)
    #include <rtk/conf/rtl8382m_8218b_intphy_8218b_8214b_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_2FIB_1G_DEMO)
    #include <rtk/conf/rtl8382m_8218b_intphy_8218b_2fib_1g_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8382M_8218D_INTPHY_8218D_2FIB_1G_DEMO)
    #include <rtk/conf/rtl8382m_8218d_intphy_8218d_2fib_1g_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8381M_INTPHY_8214FC_2FIB_1G_DEMO)
    #include <rtk/conf/rtl8381m_intphy_8214fc_2fib_1g_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8381M_INTPHY_8214C_2FIB_1G_DEMO)
    #include <rtk/conf/rtl8381m_intphy_8214c_2fib_1g_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8380M_INTPHY_2FIB_1G_DEMO)
    #include <rtk/conf/rtl8380m_intphy_2fib_1g_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8332M_8208L_INTPHY_8208L_8214B_DEMO)
    #include <rtk/conf/rtl8332m_8208l_intphy_8208l_8214b_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8332M_8208L_INTPHY_8208L_8214C_DEMO)
    #include <rtk/conf/rtl8332m_8208l_intphy_8208l_8214c_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8332M_8208L_INTPHY_8208L_8212B_DEMO)
    #include <rtk/conf/rtl8332m_8208l_intphy_8208l_8212b_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8330M_INTPHY_8212B_DEMO)
    #include <rtk/conf/rtl8330m_intphy_8212b_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8330M_INTPHY_2FIB_1G_DEMO)
    #include <rtk/conf/rtl8330m_intphy_2fib_1g_demo_board.c>
    #endif
    #if defined(CONFIG_RTL8330M_INTPHY_8212B_SGMII_DEMO)
    #include <rtk/conf/rtl8330m_intphy_8212b_sgmii_demo_board.c>
    #endif
  #endif




#endif /* end of #if defined(CONFIG_RTK_BOARD) */

/*
 * Symbol Definition
 */
#define ENV_BOARD_MODEL     "boardmodel"
#define RTL8328M_CHIP_ID    (0x83284800)
#define RTL8328S_CHIP_ID    (0x83289800)
#define RTL8330M_CHIP_ID    (0x83306800)
#define RTL8332M_CHIP_ID    (0x83326800)
#define RTL8380M_CHIP_ID    (0x83806800)
#define RTL8381M_CHIP_ID    (0x83816800)
#define RTL8382M_CHIP_ID    (0x83826800)

						


/*
 * Data Declaration
 */
typedef enum rtk_chiptype_e
{
    CHIPTYPE_RTL8328M = 0,
    CHIPTYPE_RTL8328S,
    CHIPTYPE_RTL8382M,
    CHIPTYPE_RTL8381M,
    CHIPTYPE_RTL8380M,
    CHIPTYPE_RTL8332M,
    CHIPTYPE_RTL8330M,
} rtk_chiptype_t;


/*
 * Macro Definition
 */
#define RTL8380_CHIP_INFO_ADDR                      (0xBB0000D8)
#define RTL8380_CHIP_INFO_CHIP_INFO_EN_OFFSET       (28)
#define RTL8380_CHIP_INFO_CHIP_INFO_EN_MASK         (0xF << RTL8380_CHIP_INFO_CHIP_INFO_EN_OFFSET)
#define RTL8380_CHIP_INFO_CHIP_VER_OFFSET           (16)
#define RTL8380_CHIP_INFO_CHIP_VER_MASK             (0x1F << RTL8380_CHIP_INFO_CHIP_VER_OFFSET)
#define RTL8380_CHIP_INFO_RL_ID_OFFSET              (0)
#define RTL8380_CHIP_INFO_RL_ID_MASK                (0xFFFF << RTL8380_CHIP_INFO_RL_ID_OFFSET)
#define RTL8380_MODEL_NAME_INFO_ADDR                (0xBB0000D4)
#define RTL8380_MODE_DEFINE_CTL_ADDR                (0xBB001024)

/*
 * Function Declaration
 */

/* Function Name:
 *      _rtk_chip_type_get
 * Description:
 *      Get the chip type
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0         - Success
 *      otherwise - Failure
 * Note:
 *      None
 */
static int _rtk_chip_type_get(rtk_chiptype_t *chip_type)
{
    unsigned int temp;
#if defined(CONFIG_RTL8380)
    unsigned int sub_version;
    unsigned int original_data_intRd;
    unsigned int original_data_chipRd;
#endif

    /* RTL8328M and RTL8328S Identify */
    temp = REG32(0xBB06FFF8);
    switch (temp)
    {
        case RTL8328M_CHIP_ID:
            *chip_type = CHIPTYPE_RTL8328M;
            return 0;
        case RTL8328S_CHIP_ID:
            *chip_type = CHIPTYPE_RTL8328S;
            return 0;
        default:
            break;
    }

#if defined(CONFIG_RTL8380)
    /* RTL8382M Identify */
    temp = REG32(0xBB000058);
    original_data_intRd = temp;
    temp &= ~(0x3);
    temp |= (0x3);

    /* Check sub-version */
    sub_version = REG32(RTL8380_MODE_DEFINE_CTL_ADDR);
    sub_version = sub_version & (1UL<<23);

    REG32(0xBB000058) = temp;

    temp = REG32(RTL8380_CHIP_INFO_ADDR);
    original_data_chipRd = temp;
    temp &= ~(RTL8380_CHIP_INFO_CHIP_INFO_EN_MASK);
    temp |= (0xA << RTL8380_CHIP_INFO_CHIP_INFO_EN_OFFSET);
    REG32(RTL8380_CHIP_INFO_ADDR) = temp;
    temp = REG32(RTL8380_MODEL_NAME_INFO_ADDR);

    REG32(RTL8380_CHIP_INFO_ADDR) = original_data_chipRd;
    REG32(0xBB000058) = original_data_intRd;

    switch (temp)
    {
        case RTL8382M_CHIP_ID:
            *chip_type = CHIPTYPE_RTL8382M;
            return 0;
        case RTL8380M_CHIP_ID:
            if(sub_version == 0)
            {
                *chip_type = CHIPTYPE_RTL8381M;
            }
            else
            {
                *chip_type = CHIPTYPE_RTL8380M;
            }
            return 0;
        case RTL8332M_CHIP_ID:
            *chip_type = CHIPTYPE_RTL8332M;
            return 0;
        case RTL8330M_CHIP_ID:
            *chip_type = CHIPTYPE_RTL8330M;
            return 0;
        default:
            break;
    }
#endif

    return -1;
} /* end of _rtk_chip_type_get */

/* Function Name:
 *      rtk_board_probe
 * Description:
 *      Probe the rtk board
 * Input:
 *      None
 * Output:
 *      None
 * Return:
 *      0 - Success
 * Note:
 *      None
 */
extern char* tpBoardInfoGet(void);

int rtk_board_probe(rtk_switch_model_t **pSwitchModel)
{
    rtk_chiptype_t  chip_type;
    char    *board_model;
	char    *board_model_getfrom_combo_info = NULL;

    board_model = getenv(ENV_BOARD_MODEL);
	
	board_model_getfrom_combo_info = tpBoardInfoGet();

	if(board_model_getfrom_combo_info != NULL)
	{
		printf("rtk_board_probe: board_model_getfrom_combo_info=%s\r\n", board_model_getfrom_combo_info);
		board_model = board_model_getfrom_combo_info;
	}
	else
	{
		printf("rtk_board_probe: board_model_getfrom_combo_info=NULL\r\n");
	}
	printf("rtk_board_probe: board_model=%s\r\n", board_model);
#if defined(CONFIG_RTL8328M_8214F_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8328M_8214F_DEMO"))))
    {   /* RTL8328M_8214F_DEMO */
        (*pSwitchModel) = &rtl8328m_8214f_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8328M_8214F_DEMO) */
#if defined(CONFIG_RTL8328M_8214FB_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8328M_8214FB_DEMO"))))
    {   /* RTL8328M_8214FB_DEMO */
        (*pSwitchModel) = &rtl8328m_8214fb_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8328M_8214FB_DEMO) */
#if defined(CONFIG_RTL8328M_8212F_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8328M_8212F_DEMO"))))
    {   /* RTL8328M_8212F_DEMO */
        (*pSwitchModel) = &rtl8328m_8212f_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8328M_8212F_DEMO) */
#if defined(CONFIG_RTL8328S_2SFP_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8328S_2SFP_DEMO"))))
    {   /* RTL8328M_8212F_DEMO */
        (*pSwitchModel) = &rtl8328s_2sfp_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8328M_8212F_DEMO) */
#if defined(CONFIG_RTL8328L_8212B_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8328L_8212B_DEMO"))))
    {   /* RTL8328L_8212B_DEMO */
        (*pSwitchModel) = &rtl8328l_8212b_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8328L_8212B_DEMO) */
#if defined(CONFIG_RTL8328L_8212B_SERIAL_LED_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8328L_8212B_SERIAL_LED_DEMO"))))
    {   /* RTL8328L_8212B_SERIAL_LED_DEMO */
        (*pSwitchModel) = &rtl8328l_8212b_serial_led_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8328L_8212B_SERIAL_LED_DEMO) */


#if defined(CONFIG_RTL8380)
#if defined(CONFIG_RTL8382M_8218B_INTPHY_8218FB_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8382M_8218B_INTPHY_8218FB_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8382m_8218b_intphy_8218fb_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_8214FC_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8382M_8218B_INTPHY_8218B_8214FC_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8382m_8218b_intphy_8218b_8214fc_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_8214QF_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8382M_8218B_INTPHY_8218B_8214QF_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8382m_8218b_intphy_8218b_8214qf_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8382M_8218D_INTPHY_8218D_8214FC_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8382M_8218D_INTPHY_8218D_8214FC_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8382m_8218d_intphy_8218d_8214fc_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_8214B_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8382M_8218B_INTPHY_8218B_8214B_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8382m_8218b_intphy_8218b_8214b_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8382M_8218B_INTPHY_8218B_2FIB_1G_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8382M_8218B_INTPHY_8218B_2FIB_1G_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8382m_8218b_intphy_8218b_2fib_1g_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8382M_8218D_INTPHY_8218D_2FIB_1G_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8382M_8218D_INTPHY_8218D_2FIB_1G_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8382m_8218d_intphy_8218d_2fib_1g_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8381M_INTPHY_8214FC_2FIB_1G_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8381M_INTPHY_8214FC_2FIB_1G_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8381m_intphy_8214fc_2fib_1g_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8381M_INTPHY_8214C_2FIB_1G_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8381M_INTPHY_8214C_2FIB_1G_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8381m_intphy_8214c_2fib_1g_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8380M_INTPHY_2FIB_1G_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8380M_INTPHY_2FIB_1G_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8380m_intphy_2fib_1g_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8332M_8208L_INTPHY_8208L_8214B_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8332M_8208L_INTPHY_8208L_8214B_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8332m_8208l_intphy_8208l_8214b_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8332M_8208L_INTPHY_8208L_8214C_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8332M_8208L_INTPHY_8208L_8214C_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8332m_8208l_intphy_8208l_8214c_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8330M_INTPHY_8212B_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8330M_INTPHY_8212B_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8330m_intphy_8212b_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8332M_8208L_INTPHY_8208L_8212B_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8332M_8208L_INTPHY_8208L_8212B_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8332m_8208l_intphy_8208l_8212b_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8330M_INTPHY_2FIB_1G_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8330M_INTPHY_2FIB_1G_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8330m_intphy_2fib_1g_demo;
        }
        else
#endif
#if defined(CONFIG_RTL8330M_INTPHY_8212B_SGMII_DEMO)
        if (board_model && ((0 == strcmp(board_model, "RTL8330M_INTPHY_8212B_SGMII_DEMO"))))
        {
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8330m_intphy_8212b_sgmii_demo;
        }
        else
#endif
#endif

#if defined(CONFIG_RTL8390)
#if defined(CONFIG_RTL8353M_QA)
    if (board_model && ((0 == strcmp(board_model, "RTL8353M_QA"))))
    {   /* RTL8353M_QA */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8353m_qa;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8353M_QA) */
#if defined(CONFIG_RTL8392M_QA)
    if (board_model && ((0 == strcmp(board_model, "RTL8392M_QA"))))
    {   /* RTL8392M_QA */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8392m_qa;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8392M_QA) */
#if defined(CONFIG_RTL8393M_QA)
    if (board_model && ((0 == strcmp(board_model, "RTL8393M_QA"))))
    {   /* RTL8393M_QA */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8393m_qa;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8393M_QA) */
#if defined(CONFIG_RTL8353M_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8353M_DEMO"))))
    {   /* RTL8353M_DEMO */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8353m_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8353M_DEMO) */
#if defined(CONFIG_RTL8353M_14B_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8353M_14B_DEMO"))))
    {   /* RTL8353M_DEMO */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8353m_14b_demo;
    }
    else
#endif  /* CONFIG_RTL8353M_14B_DEMO */
#if defined(CONFIG_RTL8351M_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8351M_DEMO"))))
    {   /* RTL8351M_DEMO */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8351m_demo;
    }
    else
#endif  /* CONFIG_RTL8351M_DEMO */
#if defined(CONFIG_RTL8392M_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8392M_DEMO"))))
    {   /* RTL8392M_DEMO */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8392m_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8392M_DEMO) */
#if defined(CONFIG_RTL8393M_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8393M_DEMO"))))
    {   /* RTL8393M_DEMO */
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8393m_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8393M_DEMO) */
#if defined(CONFIG_RTL8393M_18D)
        if (board_model && ((0 == strcmp(board_model, "RTL8393M_8218D"))))
        {   /* RTL8393M_8218D */
            (*pSwitchModel) = (rtk_switch_model_t *)&rtl8393m_8218d;
        }
        else
#endif /* end of #if defined(CONFIG_RTL8393M_18D) */
    if (board_model && ((0 == strcmp(board_model, "RTL8393M_8218B_8218FB_2SFP"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8393m_8218b_8218fb_2sfp;
    }
    else
#if defined(CONFIG_RTL8396M_8214QF_8295R_ES_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8396M_8292_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8396m_14qf_95r_es_demo;
    }
    else
#endif/* end of #if defined(CONFIG_RTL8396M_8295_DEMO) */
#if defined(CONFIG_RTL8396M_8218B_8214QF_8295R_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8396M_8218B_8214QF_8295R_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8396m_8218b_8214qf_8295r_demo;
    }
    else
#endif/* end of #if defined(CONFIG_RTL8396M_8295_DEMO) */
#if defined(CONFIG_RTL8396M_8214QF_8295R_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8396M_8214QF_8295R_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8396m_8214qf_8295r_demo;
    }
    else
#endif/* end of #if defined(CONFIG_RTL8396M_8214QF_8295R_DEMO) */
#if defined(CONFIG_RTL8396M_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8396M_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8396m_demo;
    }
    else if (board_model && ((0 == strcmp(board_model, "RTL8396M2_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8396m2_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8396M_DEMO) */

#if defined(CONFIG_RTL8391M_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8391M_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8391m_demo;
    }
    else
#endif /* end of #if defined(CONFIG_RTL8391M_DEMO) */
#if defined(CONFIG_RTL8391M_14C_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8391M_14C_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8391m_14c_demo;
    }
    else
#endif  /* end of CONFIG_RTL8391M_14C_DEMO */
#if defined(CONFIG_RTL8391M_14QF_14QF_14FC_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8391M_14QF_14QF_14FC_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8391m_14qf_14qf_14fc_demo;
    }
    else
#endif
#if defined(CONFIG_RTL8393M_14C_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8393M_14C_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8393m_14c_demo;
    }
    else
#endif  /* end of CONFIG_RTL8393M_14C_DEMO */
#if defined(CONFIG_RTL8393M_8218B_8214QF_DEMO)
    if (board_model && ((0 == strcmp(board_model, "RTL8393M_8218B_8214QF_DEMO"))))
    {
        (*pSwitchModel) = (rtk_switch_model_t *)&rtl8393m_8218b_14qf_demo;
    }
	else
#endif
#endif  /* end of #if defined(CONFIG_RTL8390) */
    {   /* Not match existing board model string, try to bind QA/demo board depend on chip type */
        if (_rtk_chip_type_get(&chip_type) != 0)
        {
            OSAL_PRINTF("Board probe failure due to can't get chip type");
            (*pSwitchModel) = NULL;
        }

        else
        {   /* default - demo board */
            switch (chip_type)
            {
#if defined(CONFIG_RTL8328M_8214F_DEMO)
                case CHIPTYPE_RTL8328M:
                case CHIPTYPE_RTL8328S:
                    (*pSwitchModel) = &rtl8328m_8214f_demo;
                    break;
#endif /* end of #if defined(CONFIG_RTL8328M_8214F_DEMO) */
#if defined(CONFIG_RTL8382M_8218B_8218FB_DEMO)
                case CHIPTYPE_RTL8382M:
                    (*pSwitchModel) = &rtl8382m_8218b_8218fb_demo;
                    break;
#endif /* end of #if defined(CONFIG_RTL8382M_8218B_8218FB_DEMO) */
                default:
                    (*pSwitchModel) = NULL;
                    break;
            }
        }

    }

    return 0;
} /* end of rtk_board_probe */



