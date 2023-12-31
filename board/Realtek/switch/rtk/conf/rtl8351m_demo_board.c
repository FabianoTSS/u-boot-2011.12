/*
 * Copyright(c) Realtek Semiconductor Corporation, 2010
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8353M DEMO board for U-Boot.
 *
 * Feature : RTL8353M DEMO board database
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
static const rtk_switch_model_t rtl8351m_demo = {
    .name = "RTL8351M_DEMO",
    .chip = RTK_CHIP_RTL8351M,

    /* LED layout settting */
    .led.led_if_sel = LED_IF_SEL_BI_COLOR_SCAN,
    .led.num = 3,
    .led.copr_pmask[0] = 0x0FFFFFFF,
    .led.copr_pmask[1] = 0x00000000,
    .led.fib_pmask[0] = 0x0F000000,
    .led.fib_pmask[1] = 0x00000000,
    .led.led_combo[0] = 0x0F000000,
    .led.led_combo[1] = 0x00000000,
    /* LED definition - set 0 */
    .led.led_definition_set[0].led[0] = 0xF,    /* 10/100Mbps link/act */
    .led.led_definition_set[0].led[1] = 0xA,    /* 1000Mbps link/act */
    .led.led_definition_set[0].led[2] = 0x6,    /* duplex mode */
    /* LED definition selection (per-port) */
    .led.led_copr_set_psel_bit0_pmask[0] = 0x00000000,
    .led.led_copr_set_psel_bit1_pmask[0] = 0x00000000,
    .led.led_copr_set_psel_bit0_pmask[1] = 0x00000000,
    .led.led_copr_set_psel_bit1_pmask[1] = 0x00000000,
    .led.led_fib_set_psel_bit0_pmask[0] = 0x00000000,
    .led.led_fib_set_psel_bit1_pmask[0] = 0x00000000,
    .led.led_fib_set_psel_bit0_pmask[1] = 0x00000000,
    .led.led_fib_set_psel_bit1_pmask[1] = 0x00000000,

    .port.count = 28,
    .port.list = {
        { .mac_id = 0,  .phy_idx = 0, .phy = 0 },
        { .mac_id = 1,  .phy_idx = 0, .phy = 1 },
        { .mac_id = 2,  .phy_idx = 0, .phy = 2 },
        { .mac_id = 3,  .phy_idx = 0, .phy = 3 },
        { .mac_id = 4,  .phy_idx = 0, .phy = 4 },
        { .mac_id = 5,  .phy_idx = 0, .phy = 5 },
        { .mac_id = 6,  .phy_idx = 0, .phy = 6 },
        { .mac_id = 7,  .phy_idx = 0, .phy = 7 },
        { .mac_id = 8,  .phy_idx = 1, .phy = 0 },
        { .mac_id = 9,  .phy_idx = 1, .phy = 1 },
        { .mac_id = 10, .phy_idx = 1, .phy = 2 },
        { .mac_id = 11, .phy_idx = 1, .phy = 3 },
        { .mac_id = 12, .phy_idx = 1, .phy = 4 },
        { .mac_id = 13, .phy_idx = 1, .phy = 5 },
        { .mac_id = 14, .phy_idx = 1, .phy = 6 },
        { .mac_id = 15, .phy_idx = 1, .phy = 7 },
        { .mac_id = 16, .phy_idx = 2, .phy = 0 },
        { .mac_id = 17, .phy_idx = 2, .phy = 1 },
        { .mac_id = 18, .phy_idx = 2, .phy = 2 },
        { .mac_id = 19, .phy_idx = 2, .phy = 3 },
        { .mac_id = 20, .phy_idx = 2, .phy = 4 },
        { .mac_id = 21, .phy_idx = 2, .phy = 5 },
        { .mac_id = 22, .phy_idx = 2, .phy = 6 },
        { .mac_id = 23, .phy_idx = 2, .phy = 7 },
        { .mac_id = 24, .phy_idx = 3, .phy = 0 },
        { .mac_id = 25, .phy_idx = 3, .phy = 1 },
        { .mac_id = 26, .phy_idx = 3, .phy = 2 },
        { .mac_id = 27, .phy_idx = 3, .phy = 3 },
    },  /* port.list */

    .serdes.count = 7,
    .serdes.list = {
        /* mii: SR4_CFG_FRC_SDS_MODE */
        { .sds_id = 0,  .phy_idx = 0},
        { .sds_id = 1,  .phy_idx = 0},
        { .sds_id = 2,  .phy_idx = 1},
        { .sds_id = 3,  .phy_idx = 1},
        { .sds_id = 4,  .phy_idx = 2},
        { .sds_id = 5,  .phy_idx = 2},
        { .sds_id = 6,  .phy_idx = 3},
    },  /* serdes.list */

    .phy.baseid = 0,
    .phy.count = 4,
    .phy.list = {
        [0] = { .chip = RTK_CHIP_RTL8208L, .mac_id = 0 , .phy_max = 8},
        [1] = { .chip = RTK_CHIP_RTL8208L, .mac_id = 8 , .phy_max = 8},
        [2] = { .chip = RTK_CHIP_RTL8208L, .mac_id = 16, .phy_max = 8},
        [3] = { .chip = RTK_CHIP_RTL8214B, .mac_id = 24, .phy_max = 4},
    },   /* .phy.list */
    .phy.rstGpioType = PHY_RSTGPIOTYPE_INTERNAL,
    .phy.rstGpio.rstIntGpio.pin = 14,
};

