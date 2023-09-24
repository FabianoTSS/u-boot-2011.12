/*
 * Copyright(c) Realtek Semiconductor Corporation, 2016
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8396M with RTL8295 DEMO board for U-Boot.
 *
 * Feature : RTL8396M with RTL8295 DEMO board database
 *
 */


/*
 * Include Files
 */


/*
 * Symbol Definition
 */


/*
 * Data Declaration
 */
static const rtk_switch_model_t rtl8396m_8214qf_8295r_demo = {
    .name = "RTL8396M_8214QF_8295R_DEMO",
    .chip = RTK_CHIP_RTL8396M,

    /* LED layout settting */
    .led.led_if_sel = LED_IF_SEL_BI_COLOR_SCAN,
    .led.num = 3,
    .led.copr_pmask[0] = 0x00000000,
    .led.copr_pmask[1] = 0x00000000,
    .led.fib_pmask[0]  = 0x01000F0F,
    .led.fib_pmask[1]  = 0x00000010,
    .led.led_combo[0]  = 0x00000000,
    .led.led_combo[1]  = 0x00000000,
    /* LED definition - set 0 */
    .led.led_definition_set[0].led[0] = 0xA,    /* 1000Mbps link/act */
    .led.led_definition_set[0].led[1] = 0xF,    /* 10/100Mbps link/act */
    .led.led_definition_set[0].led[2] = 0x6,    /* duplex mode */
    /* LED definition - set 1 */
    .led.led_definition_set[1].led[0] = 0x15,   /* 10G link/act */
    .led.led_definition_set[1].led[1] = 0xD,    /* 1000/100Mbps link/act */
    .led.led_definition_set[1].led[2] = 0x6,    /* duplex mode */
    /* LED definition selection (per-port) */
    .led.led_copr_set_psel_bit0_pmask[0] = 0x01000000,
    .led.led_copr_set_psel_bit1_pmask[0] = 0x00000000,
    .led.led_copr_set_psel_bit0_pmask[1] = 0x00000010,
    .led.led_copr_set_psel_bit1_pmask[1] = 0x00000000,
    .led.led_fib_set_psel_bit0_pmask[0]  = 0x01000000,
    .led.led_fib_set_psel_bit1_pmask[0]  = 0x00000000,
    .led.led_fib_set_psel_bit0_pmask[1]  = 0x00000010,
    .led.led_fib_set_psel_bit1_pmask[1]  = 0x00000000,

    .port.count = 10,
    .port.list = {
        { .mac_id = 0,  .phy_idx = 0, .phy = 0 },
        { .mac_id = 1,  .phy_idx = 0, .phy = 1 },
        { .mac_id = 2,  .phy_idx = 0, .phy = 2 },
        { .mac_id = 3,  .phy_idx = 0, .phy = 3 },
        { .mac_id = 8,  .phy_idx = 1, .phy = 0 },
        { .mac_id = 9,  .phy_idx = 1, .phy = 1 },
        { .mac_id = 10, .phy_idx = 1, .phy = 2 },
        { .mac_id = 11, .phy_idx = 1, .phy = 3 },
        { .mac_id = 24, .phy_idx = 3, .phy = 0 },
        { .mac_id = 36, .phy_idx = 4, .phy = 0 },
    },  /* port.list */

    .serdes.count = 4,
    .serdes.list = {
        { .sds_id = 0,  .phy_idx = 0, .mii = RTK_MII_QSGMII },
        { .sds_id = 2,  .phy_idx = 1, .mii = RTK_MII_QSGMII },
        { .sds_id = 8,  .phy_idx = 2, .mii = RTK_MII_10GR },
        { .sds_id = 12, .phy_idx = 3, .mii = RTK_MII_10GR },
    },  /* serdes.list */

    .phy.baseid = 0,
    .phy.count = 4,
    .phy.list = {
        [0] = { .chip = RTK_CHIP_RTL8214QF, .mac_id = 0 , .phy_max = 4},
        [1] = { .chip = RTK_CHIP_RTL8214QF, .mac_id = 8 , .phy_max = 4},
        [2] = { .chip = RTK_CHIP_RTL8295R,  .mac_id = 24, .phy_max = 1},
        [3] = { .chip = RTK_CHIP_RTL8295R,  .mac_id = 36, .phy_max = 1},
    },   /* .phy.list */
    .phy.rstGpioType = PHY_RSTGPIOTYPE_INTERNAL,
    .phy.rstGpio.rstIntGpio.pin = 23,
};

