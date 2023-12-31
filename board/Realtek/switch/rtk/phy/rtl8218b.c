/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8214F PHY driver.
 *
 * Feature : RTL8214F PHY driver
 *
 */


/*
 * Include Files
 */
#include <rtk_osal.h>
#include <rtk_debug.h>
#include <rtk_switch.h>
#include <common/util.h>
#include <rtk/phy/conf/conftypes.h>
#include <rtk/phy/conf/conf_rtl8218b.c>
#include <rtk/mac/rtl8390/rtl8390_drv.h>
#include <config.h>
#include <rtk_reg.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
extern const rtk_switch_model_t *gSwitchModel;
extern const rtk_mac_drv_t *gMacDrv;

/*
 * Macro Definition
 */

#define RTL8218B_CHIP_ID            0x6276

#define RTL8218B_TCA_CHIP_ID        0x0455
#define RTL8218B_INT_TCA_CHIP_ID    0x0477
#define RTL8218B_INT_MP_A_CHIP_ID   0x6276
#define RTL8218B_INT_MP_B_CHIP_ID   0x6275
#define RTL8218B_TCA_CUT_A          0x0
#define RTL8218B_TCA_CUT_B          0x1
/*PHY: 8218FB*/
#define RTL8218FB_TCA_CHIP_ID       0x0455
#define RTL8218FB_TCA_CUT_A         0x0
#define RTL8218FB_TCA_CUT_B         0x1

/*
 * Function Declaration
 */



/* Function Name:
 *      rtl8218d_eFuse_disable
 * Description:
 *      Configuration code for RTL8218d.
 * Input:
 *      phy_base_macId - the macId of PHY0 of the PHY
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
rtl8218d_eFuse_disable(uint8 phy_base_macId)
{
    uint32      data;

    gMacDrv->drv_miim_write(phy_base_macId, gMacDrv->miim_max_page, 30, 0x0008);
    gMacDrv->drv_miim_write(phy_base_macId, gMacDrv->miim_max_page, 31, 0x0260);
    gMacDrv->drv_miim_read(phy_base_macId, gMacDrv->miim_max_page, 0x14, &data);
    data = REG32_FIELD_SET(data, 0, 0x0, (0x1 << 0));
    gMacDrv->drv_miim_write(phy_base_macId, gMacDrv->miim_max_page, 0x14, data);
    gMacDrv->drv_miim_write(phy_base_macId, gMacDrv->miim_max_page, 30, 0x0000);

    return;
}


#if defined(CONFIG_RTL8390)
#define CHECK_TIMES     (150000)

/* Function Name:
 *      rtl8218b_rtl8390_config
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8218b_rtl8390_config(Tuint8 macId)
{
    unsigned long long rtl8218_macid_pm = (0xFFULL << macId);
    int rtl8218_phy0_macid = macId - (macId % 8);
    unsigned int val;
    int i, port_id, base_id;
    unsigned int rl_no = 0, ver_no = 0, cut_id, romId;

    base_id = rtl8218_phy0_macid;

    gMacDrv->drv_miim_read(base_id, 0, 3, &val);

    if ((val & 0x3FF) == 0x180)
    {
        OSAL_PRINTF("### RTL8218B config - MAC ID = %d ###\n", base_id);
        //OSAL_PRINTF("DBG: rtl8218_macid_pm = 0x%08X %08X\n", *(((unsigned int *)&rtl8218_macid_pm) + 0), *(((unsigned int *)&rtl8218_macid_pm) + 1));

        /* Enable PHY */
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 31, 0x000e);
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 23, 0x130f);
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 24, 0x0840 | ((rtl8218_phy0_macid % 24) << 0));
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 21, 0x0003);

        /* Serdes (Per-Chip) */
        for (i=0; i<(sizeof(rtl8218b_rtl8390_serdes_perchip)/sizeof(confcode_prv_t)); i++)
        {
#if 0
            int readback_val;
#endif
            gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_rtl8390_serdes_perchip[i].phy, \
                gMacDrv->miim_max_page, \
                (int)rtl8218b_rtl8390_serdes_perchip[i].reg, \
                (int)rtl8218b_rtl8390_serdes_perchip[i].val);

            OSAL_MDELAY(1);

#if 0
            /* read back and check the value */
            gMacDrv->drv_miim_read(rtl8218_phy0_macid + rtl8218b_rtl8390_serdes_perchip[i].phy, \
                gMacDrv->miim_max_page, \
                (int)rtl8218b_rtl8390_serdes_perchip[i].reg, \
                &readback_val);
            if (readback_val != (int)rtl8218b_rtl8390_serdes_perchip[i].val)
            {
                printf("WARN: rtl8218b_rtl8390_serdes_perchip: mac %u reg 0x%04X != 0x%04X (real: 0x%04X)\n", \
                    rtl8218_phy0_macid + rtl8218b_rtl8390_serdes_perchip[i].phy,
                    (int)rtl8218b_rtl8390_serdes_perchip[i].reg, \
                    (int)rtl8218b_rtl8390_serdes_perchip[i].val, \
                    readback_val);
            }
#endif
        }

        /* check RTL8218B */
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, 5, 5, 0x3ffe);
        gMacDrv->drv_miim_read(rtl8218_phy0_macid, 5, 6, &val);

        if (val == 0x2104)
        {
            /* per-port */
            for (i=0; i<(sizeof(rtl8218b_rtl8390_perport)/sizeof(confcode_rv_t)); i++)
            {
#if 0
                int readback_val;
#endif

                gMacDrv->drv_miim_portmask_write(rtl8218_macid_pm, \
                    gMacDrv->miim_max_page,
                    (int)rtl8218b_rtl8390_perport[i].reg,
                    (int)rtl8218b_rtl8390_perport[i].val);
                OSAL_MDELAY(1);

#if 0
                /* read back and check the value */
                gMacDrv->drv_miim_read(rtl8218_phy0_macid, \
                    gMacDrv->miim_max_page, \
                    (int)rtl8218b_rtl8390_perport[i].reg, \
                    &readback_val);
                if (readback_val != (int)rtl8218b_rtl8390_perport[i].val)
                {
                    printf("WARN: rtl8218b_rtl8390_perport: mac %u reg 0x%04X != 0x%04X (real: 0x%04X)\n", \
                        rtl8218_phy0_macid, \
                        (int)rtl8218b_rtl8390_perport[i].reg, \
                        (int)rtl8218b_rtl8390_perport[i].val, \
                        readback_val);
                }
#endif
            }

            /* per-chip */
            for (i=0; i<(sizeof(rtl8218b_rtl8390_perchip)/sizeof(confcode_prv_t)); i++)
            {
#if 0
                int readback_val;
#endif

                gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_rtl8390_perchip[i].phy, \
                    gMacDrv->miim_max_page,
                    (int)rtl8218b_rtl8390_perchip[i].reg,
                    (int)rtl8218b_rtl8390_perchip[i].val);
                OSAL_MDELAY(1);

#if 0
                /* read back and check the value */
                gMacDrv->drv_miim_read(rtl8218_phy0_macid + rtl8218b_rtl8390_perchip[i].phy, \
                    gMacDrv->miim_max_page, \
                    (int)rtl8218b_rtl8390_perchip[i].reg, \
                    &readback_val);
                if (readback_val != (int)rtl8218b_rtl8390_perchip[i].val)
                {
                    printf("WARN: rtl8218b_rtl8390_perchip: mac %u reg 0x%04X != 0x%04X (real: 0x%04X)\n", \
                        rtl8218_phy0_macid + rtl8218b_rtl8390_perchip[i].phy, \
                        (int)rtl8218b_rtl8390_perchip[i].reg, \
                        (int)rtl8218b_rtl8390_perchip[i].val, \
                        readback_val);
                }
#endif
            }
        }
    }
    else
    {
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 29, 0x0001);
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 31, 0x0a43);
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 19, 0x0002);
        gMacDrv->drv_miim_read(rtl8218_phy0_macid, gMacDrv->miim_max_page, 20, &rl_no);

        if (RTL8218B_CHIP_ID == rl_no)
        {
            /* Version */
            gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 19, 0x0004);
            gMacDrv->drv_miim_read(rtl8218_phy0_macid, gMacDrv->miim_max_page, 20, &ver_no);

            gMacDrv->drv_miim_write(base_id, gMacDrv->miim_max_page, 27, 0x0004);
            gMacDrv->drv_miim_read(base_id, gMacDrv->miim_max_page, 28, &romId);

            OSAL_PRINTF("### RTL8218B config - MAC ID = %d ###\n", base_id);

            /* Per-Chip */
            for (i = 0; i < (sizeof(rtl8218b_6276A_rtl8390_perchip)/sizeof(confcode_prv_t)); ++i)
            {
                gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_6276A_rtl8390_perchip[i].phy, \
                        gMacDrv->miim_max_page, \
                        (int)rtl8218b_6276A_rtl8390_perchip[i].reg,  \
                        rtl8218b_6276A_rtl8390_perchip[i].val);
            }

            for (i = 0; i < (sizeof(rtl8218b_6276A_rtl8390_perchip2)/sizeof(confcode_phy_patch_t)); ++i)
            {
                PHY_PATCH_SET(rtl8218_phy0_macid, gMacDrv->miim_max_page, rtl8218b_6276A_rtl8390_perchip2[i]);
            }

            for (i = 0; i < (sizeof(rtl8218b_6276A_rtl8390_perchip3)/sizeof(confcode_prv_t)); ++i)
            {
                gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_6276A_rtl8390_perchip3[i].phy, \
                        gMacDrv->miim_max_page, \
                        (int)rtl8218b_6276A_rtl8390_perchip3[i].reg,  \
                        rtl8218b_6276A_rtl8390_perchip3[i].val);
            }

            for (port_id = 0; port_id < 8; ++port_id)
            {
                int pid = rtl8218_phy0_macid + port_id;

                for (i = 0; i < (sizeof(rtl8218b_6276A_rtl8390_perport)/sizeof(confcode_rv_t)); ++i)
                    gMacDrv->drv_miim_write(pid, gMacDrv->miim_max_page, \
                            (int)rtl8218b_6276A_rtl8390_perport[i].reg, \
                            (int)rtl8218b_6276A_rtl8390_perport[i].val);

                #ifdef CONFIG_RTL8218B_HW_ESD_IMPROVEMENT
                if (0 != romId)
                {
                    for (i = 0; i < (sizeof(rtl8218b_6276B_hwEsd_perport)/sizeof(confcode_rv_t)); ++i)
                        gMacDrv->drv_miim_write(pid, gMacDrv->miim_max_page, \
                                (int)rtl8218b_6276B_hwEsd_perport[i].reg, \
                                (int)rtl8218b_6276B_hwEsd_perport[i].val);
                }
                #endif  /* CONFIG_RTL8218B_HW_ESD_IMPROVEMENT */
            }

            gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 30, 8);
            gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0x26e, 17, 0xb);
            gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0x26e, 16, 0x2);
            OSAL_MDELAY(1);
            gMacDrv->drv_miim_read(rtl8218_phy0_macid, 0x26e, 19, &val);
            gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0, 30, 0);

            val = (val >> 4) & 0xf;
            if (1 == val)
            {
                for (port_id = 0; port_id < 8; ++port_id)
                {
                    int pid = rtl8218_phy0_macid + port_id;

                    for (i = 0; i < (sizeof(rtl8218b_6276C_rtl8390_IPD_perport)/sizeof(confcode_rv_t)); ++i)
                        gMacDrv->drv_miim_write(pid, gMacDrv->miim_max_page, \
                                (int)rtl8218b_6276C_rtl8390_IPD_perport[i].reg, \
                                (int)rtl8218b_6276C_rtl8390_IPD_perport[i].val);
                }

                for (i = 0; i < (sizeof(rtl8218b_6276C_rtl8390_IPD_perchip)/sizeof(confcode_prv_t)); ++i)
                {
                    gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_6276C_rtl8390_IPD_perchip[i].phy, \
                            gMacDrv->miim_max_page, \
                            (int)rtl8218b_6276C_rtl8390_IPD_perchip[i].reg,  \
                            rtl8218b_6276C_rtl8390_IPD_perchip[i].val);
                }
            }
        }
        else
        {
            /*Switch to right TOP address*/
            gMacDrv->drv_miim_write(base_id, 0, 29, 0x0000);
            gMacDrv->drv_miim_write(base_id, 0, 31, 0x0a42);

            /*Check the 8218B internal chip ID*/
            gMacDrv->drv_miim_write(base_id, 31, 27, 0x0002);
            gMacDrv->drv_miim_read(base_id, 31, 28, &val);

            /*Check the 8218B internal cut ID*/
            gMacDrv->drv_miim_write(base_id, 31, 27, 0x0004);
            gMacDrv->drv_miim_read(base_id, 31, 28, &cut_id);

            /*Check the 8218B status*/
            gMacDrv->drv_miim_write(base_id, 31, 31, 0xa42);
            gMacDrv->drv_miim_read(base_id, 31, 16, &val);

            //OSAL_PRINTF("### RTL8218B config - MAC ID = %d, Mode = 0x%08x ###\n", base_id, val);
            OSAL_PRINTF("### RTL8218B config - MAC ID = %d ###\n", base_id);

            if(cut_id == RTL8218B_TCA_CUT_A)
            {
                if(val == 2) /* ext init mode*/
                {
                    for (i=0; i<(sizeof(rtl8218b_0455A_mode2_perchip)/sizeof(confcode_prv_t)); i++)
                    {
                        gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_0455A_mode2_perchip[i].phy, \
                            gMacDrv->miim_max_page, (int)rtl8218b_0455A_mode2_perchip[i].reg, rtl8218b_0455A_mode2_perchip[i].val);
                    }

                    for(port_id = 0; port_id < 8; port_id++)
                    {
                        for (i=0; i<(sizeof(rtl8218b_0455A_mode2_perport)/sizeof(confcode_rv_t)); i++)
                        {
                            if((i == 2))
                                udelay(500*1000);  /*Wait for 8018B leave the command state*/
                            gMacDrv->drv_miim_write(rtl8218_phy0_macid + port_id, \
                            gMacDrv->miim_max_page, (int)rtl8218b_0455A_mode2_perport[i].reg, rtl8218b_0455A_mode2_perport[i].val);
                        }
                    }
                }
                else if((val == 3) || (val == 5)) /* lan mode*/
                {
                    for (i=0; i<(sizeof(rtl8218b_0455A_mode3_perchip)/sizeof(confcode_prv_t)); i++)
                    {
                        gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_0455A_mode3_perchip[i].phy, \
                            gMacDrv->miim_max_page, (int)rtl8218b_0455A_mode3_perchip[i].reg, rtl8218b_0455A_mode3_perchip[i].val);
                    }

                    for(port_id = 0; port_id < 8; port_id++)
                    {
                        for (i=0; i<(sizeof(rtl8218b_0455A_mode3_perport)/sizeof(confcode_rv_t)); i++)
                        {
                            if((i == 2))
                                udelay(500*1000);  /*Wait for 8018B leave the command state*/
                            gMacDrv->drv_miim_write(rtl8218_phy0_macid + port_id, \
                            gMacDrv->miim_max_page, (int)rtl8218b_0455A_mode3_perport[i].reg, rtl8218b_0455A_mode3_perport[i].val);
                        }
                    }

                    /*Check the 8218B verA Patch Result*/
                    gMacDrv->drv_miim_write(0, 31, 27, 0x8012); /*Reg 27 is PHY internal SRAM address*/
                    gMacDrv->drv_miim_read(0, 31, 28, &val); /*Reg 28 is PHY internal SRAM address value*/
                    if(val != 0x3f0f)
                        DBG_PRINT(0, "RTL8218B TCA Patch Failed 1!!!\n");
                    gMacDrv->drv_miim_write(0, 31, 27, 0x80d1);
                    gMacDrv->drv_miim_read(0, 31, 28, &val);
                    if(val != 0xa5aa)
                        DBG_PRINT(0, "RTL8218B TCA Patch Failed 2!!!\n");
                } else {
                    OSAL_PRINTF("### RTL8218B TCA in ERROR status %d\n", val);
                    return;
                }
            }
            else if(cut_id == RTL8218B_TCA_CUT_B)
            {
                /* Per-Chip */
                for (i=0; i<(sizeof(rtl8218b_0455B_rtl8390_perchip)/sizeof(confcode_prv_t)); i++)
                {
    #if 0
                    int readback_val;
    #endif

                    gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218b_0455B_rtl8390_perchip[i].phy, \
                        gMacDrv->miim_max_page, \
                        (int)rtl8218b_0455B_rtl8390_perchip[i].reg, \
                        (int)rtl8218b_0455B_rtl8390_perchip[i].val);

    #if 0
                    /* read back and check the value */
                    gMacDrv->drv_miim_read(rtl8218_phy0_macid + rtl8218b_0455B_rtl8390_perchip[i].phy, \
                        gMacDrv->miim_max_page, \
                        (int)rtl8218b_0455B_rtl8390_perchip[i].reg, \
                        &readback_val);
                    if (readback_val != (int)rtl8218b_0455B_rtl8390_perchip[i].val)
                    {
                        printf("WARN: rtl8218b_0455B_rtl8390_perchip: mac %u reg 0x%04X != 0x%04X (real: 0x%04X)\n", \
                            rtl8218_phy0_macid + rtl8218b_0455B_rtl8390_perchip[i].phy, \
                            (int)rtl8218b_0455B_rtl8390_perchip[i].reg, \
                            (int)rtl8218b_0455B_rtl8390_perchip[i].val, \
                            readback_val);
                    }
    #endif
                }

                /* Per-Port */
                for(port_id = 0; port_id < 8; port_id++)
                {
                    for (i=0; i<(sizeof(rtl8218b_0455B_rtl8390_perport)/sizeof(confcode_rv_t)); i++)
                    {
    #if 0
                        int readback_val;
    #endif

                        gMacDrv->drv_miim_write(rtl8218_phy0_macid + port_id, \
                        gMacDrv->miim_max_page, \
                        (int)rtl8218b_0455B_rtl8390_perport[i].reg, \
                        (int)rtl8218b_0455B_rtl8390_perport[i].val);

    #if 0
                        /* read back and check the value */
                        gMacDrv->drv_miim_read(rtl8218_phy0_macid + port_id, \
                            gMacDrv->miim_max_page, \
                            (int)rtl8218b_0455B_rtl8390_perport[i].reg, \
                            &readback_val);
                        if (readback_val != (int)rtl8218b_0455B_rtl8390_perport[i].val)
                        {
                            printf("WARN: rtl8218b_0455B_rtl8390_perport: mac %u reg 0x%04X != 0x%04X (real: 0x%04X)\n", \
                                rtl8218_phy0_macid + port_id, \
                                (int)rtl8218b_0455B_rtl8390_perport[i].reg, \
                                (int)rtl8218b_0455B_rtl8390_perport[i].val, \
                                readback_val);
                        }
    #endif
                    }
                }
            }
        }
    }

    return;
} /* end of rtl8218b_rtl8390_config */

#if defined(CONFIG_RTL8218FB)
/* Function Name:
 *      rtl8218fb_rtl8390_config
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8218fb_rtl8390_config(Tuint8 macId)
{
    int rtl8218_phy0_macid = macId - (macId % 8);
    unsigned int val;
    int i, port_id, base_id;
    unsigned int rl_no = 0, cut_id, romId;

    base_id = rtl8218_phy0_macid;

    gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 29, 0x0001);
    gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 31, 0x0a43);
    gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 19, 0x0002);
    gMacDrv->drv_miim_read(rtl8218_phy0_macid, gMacDrv->miim_max_page, 20, &rl_no);

    if (RTL8218B_CHIP_ID == rl_no)
    {
        OSAL_PRINTF("### RTL8218FB config - MAC ID = %d ###\n", base_id);

        gMacDrv->drv_miim_write(base_id, gMacDrv->miim_max_page, 27, 0x0004);
        gMacDrv->drv_miim_read(base_id, gMacDrv->miim_max_page, 28, &romId);

        for (i = 0; i < (sizeof(rtl8218fb_6276A_rtl8390_perchip_serdes)/sizeof(confcode_prv_t)); ++i)
        {
            gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218fb_6276A_rtl8390_perchip_serdes[i].phy, \
                    gMacDrv->miim_max_page, \
                    (int)rtl8218fb_6276A_rtl8390_perchip_serdes[i].reg, \
                    (int)rtl8218fb_6276A_rtl8390_perchip_serdes[i].val);
        }

        for (i = 0; i < (sizeof(rtl8218b_6276A_rtl8390_perchip2)/sizeof(confcode_phy_patch_t)); ++i)
        {
            PHY_PATCH_SET(rtl8218_phy0_macid, gMacDrv->miim_max_page, rtl8218b_6276A_rtl8390_perchip2[i]);
        }

        for (i = 0; i < (sizeof(rtl8218fb_6276A_rtl8390_perchip_serdes3)/sizeof(confcode_prv_t)); ++i)
        {
            gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218fb_6276A_rtl8390_perchip_serdes3[i].phy, \
                    gMacDrv->miim_max_page, \
                    (int)rtl8218fb_6276A_rtl8390_perchip_serdes3[i].reg, \
                    (int)rtl8218fb_6276A_rtl8390_perchip_serdes3[i].val);
        }

        for (port_id = 0; port_id < 8; ++port_id)
        {
            int pid = rtl8218_phy0_macid + port_id;

            for (i = 0; i < (sizeof(rtl8218fb_6276A_rtl8390_perport)/sizeof(confcode_rv_t)); ++i)
                gMacDrv->drv_miim_write(pid, gMacDrv->miim_max_page, \
                        (int)rtl8218fb_6276A_rtl8390_perport[i].reg, \
                        (int)rtl8218fb_6276A_rtl8390_perport[i].val);
        }

        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, \
                0x1e, 0x8);
        gMacDrv->drv_miim_read(rtl8218_phy0_macid, 0x260, 0x13, &val);
        val &= (0x1F << 8);
        val |= 0x4020;
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0x260, 0x13, val);
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, \
                0x1e, 0x0);

        for (i = 0; i < (sizeof(rtl8218fb_6276A_rtl8390_fiber_perchip)/sizeof(confcode_prv_t)); ++i)
        {
            gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218fb_6276A_rtl8390_fiber_perchip[i].phy, \
                    gMacDrv->miim_max_page, \
                    (int)rtl8218fb_6276A_rtl8390_fiber_perchip[i].reg, \
                    (int)rtl8218fb_6276A_rtl8390_fiber_perchip[i].val);
        }

        if (romId <= 2)
        {
            for (port_id = 0; port_id < 8; ++port_id)
            {
                int pid = rtl8218_phy0_macid + port_id;

                /*  #patch key sram 0x8146 */
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x001f, 15, 0, 0x0a43);
                /*  #patch key sram 0x8146 */
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0013, 15, 0, 0x8146);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0014, 15, 0, (0x7600 + romId));
                /* #patch lock page 0xb82, reg.23.b0 */
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0013, 15, 0, 0xb82e);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0014, 15, 0, 0x0001);

                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x001f, 15, 0, 0x0a43);

                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xB820);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x0090);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xA012);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x0000);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xA014);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c04);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3f);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3f);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3f);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd70c);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6111);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6090);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd076);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd188);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c13);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd076);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd188);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c13);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6090);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd077);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd188);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c13);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd078);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd188);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd03b);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd198);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd700);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x3220);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x3c3e);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd024);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd18b);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd012);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd19b);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x3231);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x4c3e);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd70c);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6212);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6111);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6090);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd07a);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd07b);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6090);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd07c);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd07d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6111);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6090);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd07e);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd07f);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x6090);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd080);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2c3d);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd081);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0xd189);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x2511);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x251b);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xA01A);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x0000);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xA006);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x0fff);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xA004);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x0fff);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xA002);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x0fff);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xA000);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x1504);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x13, 15, 0, 0xB820);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x14, 15, 0, 0x0010);

                /* ####clear patch_key & patch_lock##### */
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0013, 15, 0, 0x0000);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0014, 15, 0, 0x0000);
                /*  #patch lock page 0xb82, reg.23.b0 */
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x001f, 15, 0, 0x0b82);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0017, 15, 0, 0x0000);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x001f, 15, 0, 0x0a43);
                /* #patch key sram 0x8146 */
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0013, 15, 0, 0x8146);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0014, 15, 0, 0x0000);
            }

            for (port_id = 0; port_id < 8; ++port_id)
            {
                int pid = rtl8218_phy0_macid + port_id;

                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x001f, 15, 0, 0x0a45);
                rtk_phyPatchBit_set(pid, gMacDrv->miim_max_page, 0x0016, 2, 0, port_id);
            }
        }
    }
    else
    {
        /*Switch to right TOP address*/
        gMacDrv->drv_miim_write(base_id, 0, 29, 0x0000);
        gMacDrv->drv_miim_write(base_id, 0, 31, 0x0a42);

        /*Check the 8218B internal chip ID*/
        gMacDrv->drv_miim_write(base_id, 31, 27, 0x0002);
        gMacDrv->drv_miim_read(base_id, 31, 28, &val);

        /*Check the 8218B internal cut ID*/
        gMacDrv->drv_miim_write(base_id, 31, 27, 0x0004);
        gMacDrv->drv_miim_read(base_id, 31, 28, &cut_id);

        /*Check the 8218B status*/
        gMacDrv->drv_miim_write(base_id, 31, 31, 0xa42);
        gMacDrv->drv_miim_read(base_id, 31, 16, &val);

        //OSAL_PRINTF("### RTL8218B config - MAC ID = %d, Mode = 0x%08x ###\n", base_id, val);
        OSAL_PRINTF("### RTL8218FB config - MAC ID = %d ###\n", base_id);

        if(cut_id == RTL8218FB_TCA_CUT_A)
        {
            OSAL_PRINTF("### RTL8218FB (A-cut) NOT USE RIGHT NOW!\n");
            return;
        }
        else if(cut_id == RTL8218FB_TCA_CUT_B)
        {
            /* Per-Chip */
            for (i=0; i<(sizeof(rtl8218fb_0455B_rtl8390_perchip)/sizeof(confcode_prv_t)); i++)
            {
                gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218fb_0455B_rtl8390_perchip[i].phy, \
                    gMacDrv->miim_max_page, \
                    (int)rtl8218fb_0455B_rtl8390_perchip[i].reg, \
                    (int)rtl8218fb_0455B_rtl8390_perchip[i].val);
            }

            /* Per-Port */
            for(port_id = 0; port_id < 8; port_id++)
            {
                for (i=0; i<(sizeof(rtl8218fb_0455B_rtl8390_perport)/sizeof(confcode_rv_t)); i++)
                {
                    gMacDrv->drv_miim_write(rtl8218_phy0_macid + port_id, \
                    gMacDrv->miim_max_page, \
                    (int)rtl8218fb_0455B_rtl8390_perport[i].reg, \
                    (int)rtl8218fb_0455B_rtl8390_perport[i].val);
                }
            }
        }
    }

    return;
} /* end of rtl8218fb_rtl8390_config */
#endif  /* defined(CONFIG_RTL8218FB) */
#endif


#if (defined(CONFIG_RTL8218D) && defined(CONFIG_RTL8390))
void rtl8218d_rtl8390_config(Tuint8 phyid)
{
    /* 18D_TODO */

}
#endif /* CONFIG_RTL8218D && CONFIG_RTL8390 */



#if (defined(CONFIG_RTL8218D) && defined(CONFIG_RTL8380))
void rtl8218d_rtl8380_config(Tuint8 phyid)
{
    uint8 phy_base_macId = phyid - (phyid % 8);
    uint32 portnum = 8;
    uint32 port;
    uint32 i = 0;
    uint32 temp = 0;
    uint32 patchready = 0;
    uint32 gphy_status = 0;
    uint32 regdata0,regdata1;
    uint32 romId;

    OSAL_PRINTF("**** RTL8218D config - MAC ID = %d ****\n", phy_base_macId);

    rtl8218d_eFuse_disable(phy_base_macId);
    OSAL_MDELAY(60);

    for (i = 0; i < sizeof(rtl8218d_rtl8380_perchip)/sizeof(confcode_prv_t); i++)
    {
        gMacDrv->drv_miim_write(phy_base_macId + rtl8218d_rtl8380_perchip[i].phy, \
            gMacDrv->miim_max_page, (int)rtl8218d_rtl8380_perchip[i].reg, rtl8218d_rtl8380_perchip[i].val);
    }

    /*Get ROMID*/
    gMacDrv->drv_miim_write(phy_base_macId, 0, 0x11, 0x4);
    gMacDrv->drv_miim_read(phy_base_macId, 0, 0x12, &romId);

    for (port =0; port <portnum; port++)
    {
        gMacDrv->drv_miim_read(phy_base_macId + i, 0xbc0, 0x12, &regdata0);
        gMacDrv->drv_miim_read(phy_base_macId + i, 0xbc0, 0x13, &regdata1);

        /* disable tx waveform */
        gMacDrv->drv_miim_write(phy_base_macId + i, 0xbc0, 0x12, 0x0);
        gMacDrv->drv_miim_write(phy_base_macId + i, 0xbc0, 0x13, 0x1d0);

        /* phy power on */
        gMacDrv->drv_miim_write(phy_base_macId + i, 0, 0, 0x1140);

        /*polling GPHY status is LAN_ON(3) */
        for (temp = 0; temp < 200; temp++)
        {
            gMacDrv->drv_miim_read(phy_base_macId + i, 0xa46, 0x15, &gphy_status);
            gphy_status = (gphy_status >> 8) & 0x7;
            if(3 == gphy_status)
            {
                break;
            }
            if (temp >= 200)
            {
                OSAL_PRINTF("Check GPHY status(%d) fail!\n", gphy_status);
                return;
            }
        }

        /*patch request*/
        gMacDrv->drv_miim_write(phy_base_macId+i, 0xb82, 0x10, 0x0010);
        /*check patch ready*/
        for (temp = 0; temp < 100; temp++)
        {
            gMacDrv->drv_miim_read(phy_base_macId + i, 0xb80, 0x10, &patchready);
            if(patchready & 0x40)
            {
                break;
            }
        }
        if (temp >= 100)
        {
            gMacDrv->drv_miim_read(phy_base_macId + i, 0x0b80, 0x10, &patchready);
            printf("8218D phy port %d not ready! patchready:0x%x\n", phy_base_macId+i, patchready);
            return;
        }

        /*set patch key*/
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x11, 0x8028);
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x12, 0x1900 + romId);

        /*set patch lock*/
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x11, 0xb82e);
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x12, 0x1);

        /*patch GPHY parameter */
        for (i = 0; i < sizeof(rtl8218d_rtl8380_perport) / sizeof(confcode_rv_t); i++)
        {
            gMacDrv->drv_miim_write(phy_base_macId + port, gMacDrv->miim_max_page,
                rtl8218d_rtl8380_perport[i].reg, rtl8218d_rtl8380_perport[i].val);
        }

        /*clear patch lock*/
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x11, 0xb82e);
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x12, 0x0);

        /*clear patch key*/
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x11, 0x8028);
        gMacDrv->drv_miim_write(phy_base_macId+i, 0, 0x12, 0x0);

        /*clear patch request*/
        gMacDrv->drv_miim_write(phy_base_macId+i, 0xb82, 0x10, 0);

        /*enable phy power off*/
        gMacDrv->drv_miim_write(phy_base_macId + i, 0, 0, 0x1940);

        gMacDrv->drv_miim_write(phy_base_macId + i, 0xbc0, 0x12, regdata0);
        gMacDrv->drv_miim_write(phy_base_macId + i, 0xbc0, 0x13, regdata1);
    }

    return;
}
#endif /* CONFIG_RTL8218D && CONFIG_RTL8380 */

/*
 * Function Declaration
 */
/* Function Name:
 *      rtl8218b_rtl8380_config
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
#if defined(CONFIG_RTL8380)
void rtl8218b_rtl8380_config(Tuint8 phyid)
{
    unsigned int rtl8218_macid_pm = (0xFF << phyid);
    int rtl8218_phy0_macid = phyid - (phyid % 8);
    unsigned int val, cut_id;
    int i, port_id, base_id;
    int loop;
    unsigned int intphy_ipd_flg = 0;
    unsigned int rtl8218b_ipd_flg = 0;

    base_id = rtl8218_phy0_macid;

    OSAL_PRINTF("**************************************************\n");

    gMacDrv->drv_miim_read(rtl8218_phy0_macid, 0, 3, &val);
    if ((val & 0x3FF) == 0x180)
    {
        OSAL_PRINTF("**** RTL8218B config - MAC ID = %d ****\n", base_id);

        /* Enable PHY */
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 31, 0x000e);
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 23, 0x130f);
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 24, 0x0840 | ((rtl8218_phy0_macid % 24) << 0));
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, 21, 0x0003);

        /* Serdes (Per-Chip) */
        for (i=0; i<(sizeof(rtl8218B_rtl8380_serdes_perchip)/sizeof(confcode_prv_t)); i++)
        {
            gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218B_rtl8380_serdes_perchip[i].phy, \
                gMacDrv->miim_max_page, (int)rtl8218B_rtl8380_serdes_perchip[i].reg, rtl8218B_rtl8380_serdes_perchip[i].val);
            OSAL_MDELAY(1);
        }

        /* check RTL8218B */
        gMacDrv->drv_miim_write(rtl8218_phy0_macid, 5, 5, 0x3ffe);
        gMacDrv->drv_miim_read(rtl8218_phy0_macid, 5, 6, &val);

        if (val == 0x2104)
        {
            /* per-port */
            for (i=0; i<(sizeof(rtl8218B_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
            {
                gMacDrv->drv_miim_portmask_write(rtl8218_macid_pm, \
                    gMacDrv->miim_max_page, (int)rtl8218B_rtl8380_perport[i].reg, rtl8218B_rtl8380_perport[i].val);
                OSAL_MDELAY(1);
            }

            /* per-chip */
            for (i=0; i<(sizeof(rtl8218B_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
            {
                gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218B_rtl8380_perchip[i].phy, \
                    gMacDrv->miim_max_page, (int)rtl8218B_rtl8380_perchip[i].reg, (int)rtl8218B_rtl8380_perchip[i].val);
                OSAL_MDELAY(1);
            }
        }
    }
    else
    {
    /*Check the 8218B internal chip ID*/
    gMacDrv->drv_miim_write(phyid, 31, 27, 0x0002);
    gMacDrv->drv_miim_read(phyid, 31, 28, &val);

    if((val != RTL8218B_TCA_CHIP_ID) && (val != RTL8218B_INT_TCA_CHIP_ID) \
            && (val != RTL8218B_INT_MP_A_CHIP_ID) && (val != RTL8218B_CHIP_ID) && (val != RTL8218B_INT_MP_B_CHIP_ID)){
        OSAL_PRINTF("### Error chip ID = 0x%08x ###\n", val);
        return;
    }

       OSAL_PRINTF("#### RTL8218B config - MAC ID = %d ####\n", base_id);

       if(RTL8218B_TCA_CHIP_ID == val)
       {
            /*Check the 8218B internal cut ID*/
            gMacDrv->drv_miim_write(base_id, 31, 27, 0x0004);
            gMacDrv->drv_miim_read(base_id, 31, 28, &cut_id);


            /*Check the 8218B status*/
            gMacDrv->drv_miim_write(base_id, 31, 31, 0xa42);
            gMacDrv->drv_miim_read(base_id, 31, 16, &val);

            if(RTL8218B_TCA_CUT_A == cut_id)
            {
                /*Not maintain right now*/
            }
              else if(cut_id == RTL8218B_TCA_CUT_B)
            {
                OSAL_PRINTF("Now External 8218B B-CUT\n");
                /* Per-Chip */
                for (i=0; i<(sizeof(rtl8218B_0455B_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
                {
                    gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218B_0455B_rtl8380_perchip[i].phy, \
                        gMacDrv->miim_max_page, (int)rtl8218B_0455B_rtl8380_perchip[i].reg, rtl8218B_0455B_rtl8380_perchip[i].val);
                }

                /*Enable Phy*/
                for(port_id = 0; port_id < 8; port_id++)
                {
                   gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
                   gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x00, 0x1140);
               }
               OSAL_MDELAY(100);

                /*Patch request*/
               for(port_id = 0; port_id < 8; port_id++)
                    {
                   gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                   gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
                    }
                OSAL_MDELAY(300);

               /*Patch ready check*/
               for(port_id = 0; port_id < 8; port_id++)
               {
                    for(loop = 0; loop < 100; loop++)
                    {
                        gMacDrv->drv_miim_read(rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                        if(val & 0x40)break;
                    }

                    if(loop >= 100)
                    {
                        OSAL_PRINTF("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                        return;
                }
            }

               /*Use broadcast ID method to pathc internal phy*/
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x16, 0xff00+rtl8218_phy0_macid);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
               OSAL_MDELAY(1);

                /* Per-Port */
                    for (i=0; i<(sizeof(rtl8218B_0455B_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
            {
                    gMacDrv->drv_miim_write(rtl8218_phy0_macid, \
                        gMacDrv->miim_max_page, (int)rtl8218B_0455B_rtl8380_perport[i].reg, rtl8218B_0455B_rtl8380_perport[i].val);
                    }

               /*Disable broadcast ID*/
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x16, 0+rtl8218_phy0_macid);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
               gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
               OSAL_MDELAY(1);
            }
               else
            {
            /*right now nothing to do*/
            }

       }
       else if(val == RTL8218B_INT_TCA_CHIP_ID)
       {    /*INTERNAL PHY*/
            OSAL_PRINTF("Now 8218B Internal phy!\n");

            /*Enable Phy*/
            MEM32_WRITE(0xbb00a100, 0x000080a8);
            OSAL_MDELAY(100);

            /*Patch request*/
            for(port_id = 0; port_id < 8; port_id++)
            {
                gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
            }
            OSAL_MDELAY(300);

           /*Patch ready check*/
           for(port_id = 0; port_id < 8; port_id++)
           {
                for(loop = 0; loop < 100; loop++)
                {
                    gMacDrv->drv_miim_read(rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                    if(val & 0x40)break;
                }

                if(loop >= 100)
                     {
                    OSAL_PRINTF("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                    return;
                }
            }
           /*Use broadcast ID method to pathc internal phy*/
           MEM32_WRITE(0xbb000120, 0xff2810);
           OSAL_MDELAY(1);

            /* Per-Port */
            for (i=0; i<(sizeof(rtl838x_intPhy_0477B_perport)/sizeof(confcode_rv_t)); i++)
            {
            gMacDrv->drv_miim_write(rtl8218_phy0_macid, \
                gMacDrv->miim_max_page, (int)rtl838x_intPhy_0477B_perport[i].reg, rtl838x_intPhy_0477B_perport[i].val);
            }

           /*Disable broadcast ID*/
           MEM32_WRITE(0xbb000120, 0xff0010);
           OSAL_MDELAY(1);
        }
       else if((val == RTL8218B_INT_MP_A_CHIP_ID) || (val == RTL8218B_CHIP_ID) || (val == RTL8218B_INT_MP_B_CHIP_ID))
       {
                /*MP Chip internal PHY AND external 8218b MP have the same RLNO: 6276,but at last internal phy will be 6275*/
                /*note that the definition of RTL8218B_INT_MP_CHIP_ID should be 0x6275 in B-cut, mantis#0012977*/
                if((rtl8218_phy0_macid == 0) || (rtl8218_phy0_macid == 16))
                {
                        /*External PHY*/
                        OSAL_PRINTF("Now External 8218B\n");

              /*CHIP Version*/
                    gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0);
                    gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1b, 0x4);
              gMacDrv->drv_miim_read(rtl8218_phy0_macid, 0xfff, 0x1c, &cut_id);

                        /* Per-Chip */
              if(0 == cut_id)
             {
                            for (i=0; i<(sizeof(rtl8218B_6276A_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
                            {
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218B_6276A_rtl8380_perchip[i].phy, \
                                    gMacDrv->miim_max_page, (int)rtl8218B_6276A_rtl8380_perchip[i].reg, rtl8218B_6276A_rtl8380_perchip[i].val);
                            }
             }
             else
             {
                            for (i=0; i<(sizeof(rtl8218B_6276B_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
                            {
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid + rtl8218B_6276B_rtl8380_perchip[i].phy, \
                                    gMacDrv->miim_max_page, (int)rtl8218B_6276B_rtl8380_perchip[i].reg, rtl8218B_6276B_rtl8380_perchip[i].val);
                            }
             }

                        /*Enable Phy*/
                        for(port_id = 0; port_id < 8; port_id++)
                        {
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x00, 0x1140);
                       }
                       OSAL_MDELAY(100);

                        /*Patch request*/
                       for(port_id = 0; port_id < 8; port_id++)
                       {
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
                       }
                        OSAL_MDELAY(300);

                       /*Patch ready check*/
                       for(port_id = 0; port_id < 8; port_id++)
                       {
                            for(loop = 0; loop < 100; loop++)
                            {
                                gMacDrv->drv_miim_read(rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                                if(val & 0x40)break;
                            }

                            if(loop >= 100)
                            {
                                OSAL_PRINTF("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                                return;
                            }
                       }

                       /*Use broadcast ID method to pathc internal phy*/
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x16, 0xff00+rtl8218_phy0_macid);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
                       OSAL_MDELAY(1);


                        /* Per-Port */
              if(0 == cut_id)
              {
                            for (i=0; i<(sizeof(rtl8218B_6276A_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
                           {
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, \
                                gMacDrv->miim_max_page, (int)rtl8218B_6276A_rtl8380_perport[i].reg, rtl8218B_6276A_rtl8380_perport[i].val);
                                 }
                                /*Disable broadcast ID*/
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x16, 0+rtl8218_phy0_macid);
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);
                        }
                        else
                        {
                    /*IPD FLAG*/
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 30, 8);
                             gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0x26e, 17, 0xb);
                             gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0x26e, 16, 0x2);
                             OSAL_MDELAY(1);
                             gMacDrv->drv_miim_read(rtl8218_phy0_macid, 0x26e, 19, &rtl8218b_ipd_flg);
                             gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0, 30, 0);
                       rtl8218b_ipd_flg = (((rtl8218b_ipd_flg >> 4) & 0xf)==0x1) ? 0x1 : 0x0;

                   if(rtl8218b_ipd_flg)
                   {
                                     for (i=0; i<(sizeof(rtl8218B_6276B_ipd_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
                                    {
                                         gMacDrv->drv_miim_write(rtl8218_phy0_macid, \
                                         gMacDrv->miim_max_page, (int)rtl8218B_6276B_ipd_rtl8380_perport[i].reg, rtl8218B_6276B_ipd_rtl8380_perport[i].val);
                            }
              }
              else
              {
                           for (i=0; i<(sizeof(rtl8218B_6276B_rtl8380_perport)/sizeof(confcode_rv_t)); i++)
                           {
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid, \
                                gMacDrv->miim_max_page, (int)rtl8218B_6276B_rtl8380_perport[i].reg, rtl8218B_6276B_rtl8380_perport[i].val);
                           }
                           #ifdef CONFIG_RTL8218B_HW_ESD_IMPROVEMENT
                           for (i = 0; i < (sizeof(rtl8218b_6276B_hwEsd_perport)/sizeof(confcode_rv_t)); ++i)
                               gMacDrv->drv_miim_write(rtl8218_phy0_macid, gMacDrv->miim_max_page, \
                                       (int)rtl8218b_6276B_hwEsd_perport[i].reg, \
                                       (int)rtl8218b_6276B_hwEsd_perport[i].val);
                           #endif  /* CONFIG_RTL8218B_HW_ESD_IMPROVEMENT */
              }

                       /*Disable broadcast ID*/
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001d, 0x0008);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x001f, 0x266);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x16, 0+rtl8218_phy0_macid);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1f, 0x0000);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid, 0xfff, 0x1d, 0x0000);

                   /* IPD_ENHANCE */
                   if(rtl8218b_ipd_flg)
                   {
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+0, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+0, 0xfff, 0x16, 0x0f91);

                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+1, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+1, 0xfff, 0x16, 0x0fd1);

                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+2, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+2, 0xfff, 0x16, 0x0fd1);

                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+3, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+3, 0xfff, 0x16, 0x0f51);

                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+4, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+4, 0xfff, 0x16, 0x0f91);

                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+5, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+5, 0xfff, 0x16, 0x0fd1);

                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+6, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+6, 0xfff, 0x16, 0x0fd1);

                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+7, 0xfff, 0x1f, 0x0a42);
                       gMacDrv->drv_miim_write(rtl8218_phy0_macid+7, 0xfff, 0x16, 0x0f51);
                   }

                        }

                       OSAL_MDELAY(1);
                }
                else if(rtl8218_phy0_macid == 8)
                {
                        if(val == RTL8218B_INT_MP_A_CHIP_ID)
                        {
                            /*Internal PHY*/
                             OSAL_PRINTF("Now Internal PHY-A\n");

                            /*Enable Phy*/
                            val = MEM32_READ(0xbb00a100);
                            val |= 1 << 15;
                            MEM32_WRITE(0xbb00a100, val);
                            OSAL_MDELAY(100);

                            /*Patch request*/
                           for(port_id = 0; port_id < 8; port_id++)
                           {
                               gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                               gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
                           }
                            OSAL_MDELAY(300);

                           /*Patch ready check*/
                           for(port_id = 0; port_id < 8; port_id++)
                           {
                                for(loop = 0; loop < 100; loop++)
                                {
                                    gMacDrv->drv_miim_read(rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                                    if(val & 0x40)break;
                                }

                                if(loop >= 100)
                                {
                                    OSAL_PRINTF("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                                    return;
                                }
                           }

                            /*phy per-port patch*/
                           for(port_id = 0; port_id < 8; port_id++)
                           {
                                /* Per-Port */
                                for (i=0; i<(sizeof(rtl838x_6275A_intPhy_perport)/sizeof(confcode_rv_t)); i++)
                                {
                                gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, \
                                        gMacDrv->miim_max_page, (int)rtl838x_6275A_intPhy_perport[i].reg, rtl838x_6275A_intPhy_perport[i].val);
                                }
                           }
                        }
                        else if(val == RTL8218B_INT_MP_B_CHIP_ID)
                        {
                            /*Internal PHY*/
                             OSAL_PRINTF("Now Internal PHY\n");

                        /*Check Internal PHY IPD Flag*/
                           intphy_ipd_flg = (MEM32_READ(0xBB003B28) == 0x1) ? 0x1 : 0x0;

                            /*Enable Phy*/
                            val = MEM32_READ(0xbb00a100);
                            val |= 1 << 15;
                            MEM32_WRITE(0xbb00a100, val);
                            OSAL_MDELAY(100);

                            /*Patch request*/
                           for(port_id = 0; port_id < 8; port_id++)
                           {
                               gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
                               gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
                           }
                            OSAL_MDELAY(300);

                           /*Patch ready check*/
                           for(port_id = 0; port_id < 8; port_id++)
                           {
                                for(loop = 0; loop < 100; loop++)
                                {
                                    gMacDrv->drv_miim_read(rtl8218_phy0_macid + port_id, 0xb80, 0x10, &val);
                                    if(val & 0x40)break;
                                }

                                if(loop >= 100)
                                {
                                    OSAL_PRINTF("Internal phy port %d not ready!\n",port_id+rtl8218_phy0_macid);
                                    return;
                                }
                           }

                            /*phy per-port patch*/
                           for(port_id = 0; port_id < 8; port_id++)
                           {
                                /* Per-Port */
                            if(intphy_ipd_flg)
                            {
                                            for (i=0; i<(sizeof(rtl838x_6275B_intPhy_ipd_perport)/sizeof(confcode_rv_t)); i++)
                                            {
                                                gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, \
                                                    gMacDrv->miim_max_page, (int)rtl838x_6275B_intPhy_ipd_perport[i].reg, rtl838x_6275B_intPhy_ipd_perport[i].val);
                                            }
                            }
                            else
                            {
                                for (i=0; i<(sizeof(rtl838x_6275B_intPhy_perport)/sizeof(confcode_rv_t)); i++)
                                {
                                    gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, \
                                        gMacDrv->miim_max_page, \
                                        (int)rtl838x_6275B_intPhy_perport[i].reg, \
                                        rtl838x_6275B_intPhy_perport[i].val);


                                }
                                #ifdef CONFIG_RTL8218B_HW_ESD_IMPROVEMENT
                                if (0 != cut_id)
                                {
                                    for (i = 0; i < (sizeof(rtl8218b_6276B_hwEsd_perport)/sizeof(confcode_rv_t)); ++i)
                                        gMacDrv->drv_miim_write(rtl8218_phy0_macid+port_id, gMacDrv->miim_max_page, \
                                                (int)rtl8218b_6276B_hwEsd_perport[i].reg, \
                                                (int)rtl8218b_6276B_hwEsd_perport[i].val);
                                }
                                #endif  /* CONFIG_RTL8218B_HW_ESD_IMPROVEMENT */
                           }
                                   }

                         if(intphy_ipd_flg)
                             {
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+0, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+0, 0xfff, 0x16, 0x0f91);

                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+1, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+1, 0xfff, 0x16, 0x0fd1);

                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+2, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+2, 0xfff, 0x16, 0x0fd1);

                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+3, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+3, 0xfff, 0x16, 0x0f51);

                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+4, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+4, 0xfff, 0x16, 0x0f91);

                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+5, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+5, 0xfff, 0x16, 0x0fd1);

                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+6, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+6, 0xfff, 0x16, 0x0fd1);

                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+7, 0xfff, 0x1f, 0x0a42);
                           gMacDrv->drv_miim_write(rtl8218_phy0_macid+7, 0xfff, 0x16, 0x0f51);
                           }

                        }
                        else
                        {
                            /*Do nothing*/
                        }
             }
             else
             {
                        /*right now nothing to do*/
             }
        }
      }

    return;
} /* end of rtl8218b_rtl8380_config */

void rtl8218fb_rtl8380_config(Tuint8 phyid)
{
    int rtl8218fb_phy0_macid = phyid - (phyid % 8);
    unsigned int val, cut_id;
    int i, port_id, base_id;
    int loop;

    base_id = rtl8218fb_phy0_macid;

    OSAL_PRINTF("///////////////////////////////////////////////////////////\n");

    OSAL_PRINTF("Now External 8218FB\n");

    /*Check the 8218FB internal chip ID*/
    gMacDrv->drv_miim_write(phyid, 31, 27, 0x0002);
    gMacDrv->drv_miim_read(phyid, 31, 28, &val);


    if(val != 0x6276)
    {
        OSAL_PRINTF("### Error chip ID = 0x%08x ###\n", val);
        return;
    }

     /*Check the 8218B internal cut ID*/
     gMacDrv->drv_miim_write(base_id, 31, 27, 0x0004);
     gMacDrv->drv_miim_read(base_id, 31, 28, &cut_id);

    /*Current not check CUT-ID,all use version-c patch*/

    /* Per-Chip */
    for (i=0; i<(sizeof(rtl8218fB_revB_rtl8380_perchip)/sizeof(confcode_prv_t)); i++)
    {
        gMacDrv->drv_miim_write(rtl8218fb_phy0_macid + rtl8218fB_revB_rtl8380_perchip[i].phy, \
            gMacDrv->miim_max_page, (int)rtl8218fB_revB_rtl8380_perchip[i].reg, rtl8218fB_revB_rtl8380_perchip[i].val);
    }

    /************************************** 8 UTP **************************************/
     /*Force Copper*/
    for(port_id = 0; port_id < 8; port_id++)
     {
        gMacDrv->drv_miim_write(rtl8218fb_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
        gMacDrv->drv_miim_write(rtl8218fb_phy0_macid+port_id, 0xfff, 0x1e, 0x0001);
    }

     /*Enable Phy*/
     for(port_id = 0; port_id < 8; port_id++)
     {
        gMacDrv->drv_miim_write(rtl8218fb_phy0_macid+port_id, 0xfff, 0x1f, 0x0000);
        gMacDrv->drv_miim_write(rtl8218fb_phy0_macid+port_id, 0xfff, 0x00, 0x1140);
    }
    OSAL_MDELAY(100);

    /*Patch request*/
   for(port_id = 0; port_id < 8; port_id++)
   {
       gMacDrv->drv_miim_write(rtl8218fb_phy0_macid+port_id, 0xfff, 0x1f, 0x0b82);
       gMacDrv->drv_miim_write(rtl8218fb_phy0_macid+port_id, 0xfff, 0x10, 0x0010);
   }
    OSAL_MDELAY(300);

   /*Patch ready check*/
   for(port_id = 0; port_id < 8; port_id++)
   {
        for(loop = 0; loop < 100; loop++)
        {
            gMacDrv->drv_miim_read(rtl8218fb_phy0_macid + port_id, 0xb80, 0x10, &val);
            if(val & 0x40)break;
        }

        if(loop >= 100)
        {
            OSAL_PRINTF("18fb phy port %d not ready!\n",port_id+rtl8218fb_phy0_macid);
            return;
        }
   }

   /*Use broadcast ID method to pathc internal phy*/
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x16, 0xff00+rtl8218fb_phy0_macid);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);
   OSAL_MDELAY(1);


    /* Per-Port - UTP*/
    for(port_id = 0; port_id < 8; port_id++)
    {
                for (i=0; i<(sizeof(rtl8218fB_revB_rtl8380_utp_perport)/sizeof(confcode_rv_t)); i++)
        {
            gMacDrv->drv_miim_write(rtl8218fb_phy0_macid + port_id, \
                    gMacDrv->miim_max_page, (int)rtl8218fB_revB_rtl8380_utp_perport[i].reg, rtl8218fB_revB_rtl8380_utp_perport[i].val);
         }
    }

   /*Disable broadcast ID*/
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x16, 0+rtl8218fb_phy0_macid);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);


    /************************************** 4 Fiber  **************************************/
   /*Use broadcast ID method to pathc internal phy*/
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x16, 0xf000+rtl8218fb_phy0_macid+4);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);
   OSAL_MDELAY(1);

    /* Per-Port - Fiber*/
     for (i=0; i<(sizeof(rtl8218fB_revB_rtl8380_fiber_perport)/sizeof(confcode_rv_t)); i++)
    {
        gMacDrv->drv_miim_write(rtl8218fb_phy0_macid + 4, \
                gMacDrv->miim_max_page, (int)rtl8218fB_revB_rtl8380_fiber_perport[i].reg, rtl8218fB_revB_rtl8380_fiber_perport[i].val);
     }

   /*Disable broadcast ID*/
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001d, 0x0008);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x001f, 0x266);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x16, 0+rtl8218fb_phy0_macid+4);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1f, 0x0000);
   gMacDrv->drv_miim_write(rtl8218fb_phy0_macid, 0xfff, 0x1d, 0x0000);


    return;
} /* end of rtl8218b_rtl8380_config */

#endif

/*
 * Function Declaration
 */
/* Function Name:
 *      rtl8218b_phyPowerOff
 * Description:
 *      Configuration code for RTL8218b.
 * Input:
 *      macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8218b_phyPowerOff(int macId)
{
    int i, base_id;
    unsigned int val, cut_id;

    /*Check the 8218B internal cut ID*/
    base_id = macId - (macId % 8);
    gMacDrv->drv_miim_write(base_id, 31, 27, 0x0004);
    gMacDrv->drv_miim_read(base_id, 31, 28, &cut_id);

       i = macId;
    if(cut_id == RTL8218B_TCA_CUT_A)
    {
        if((i%8) == 0) /*For Base ID only*/
        {
            /* Disable the 8218B TCA ESD Polliing*/
            val = 0x0008;
            gMacDrv->drv_miim_write(i, gMacDrv->miim_max_page, 29, val);
            val = 0x1330;
            gMacDrv->drv_miim_write(i, gMacDrv->miim_max_page, 31, val);
            val = 0x0003;
            gMacDrv->drv_miim_write(i, gMacDrv->miim_max_page, 18, val);
            val = 0x0000;
            gMacDrv->drv_miim_write(i, gMacDrv->miim_max_page, 29, val);
            /* End of Disable the 8218B ESD Polliing*/
        }
    }
}

void rtl8218fb_media_set(int portid, int media)
{
    unsigned int _RTL8218FB_Phy0Id, reg;
    int phy_idx;
    unsigned int val;

    if (gSwitchModel == NULL)
        return;

    if (gMacDrv == NULL)
        return;

    if ((media != MEDIATYPE_COPPER) && (media != MEDIATYPE_FIBER))
        return;

    phy_idx = gSwitchModel->port.list[portid].phy_idx;
    if (gSwitchModel->phy.list[phy_idx].chip != RTK_CHIP_RTL8218FB)
        return;

    _RTL8218FB_Phy0Id = portid - gSwitchModel->port.list[portid].phy;

    switch (portid % 8)
    {
        case 4:
            reg = 16;
            break;
        case 5:
            reg = 19;
            break;
        case 6:
            reg = 20;
            break;
        case 7:
            reg = 21;
            break;
        default:
            return;
    }

    gMacDrv->drv_miim_write(_RTL8218FB_Phy0Id, 0xa42, 29, 0x0008);
    gMacDrv->drv_miim_read(_RTL8218FB_Phy0Id, 0x266, reg, &val);

    switch (media)
    {
        case MEDIATYPE_COPPER:
            val |= (1<<10);
            val |= (1<<11);
            break;
        case MEDIATYPE_FIBER:
            val |= (1<<10);
            val &= ~(1<<11);
            break;
        default:
            break;
    }

    gMacDrv->drv_miim_write(_RTL8218FB_Phy0Id, 0x266, reg, val);
    gMacDrv->drv_miim_write(_RTL8218FB_Phy0Id, 0xa42, 29, 0x0000);

    return;
} /* end of rtl8218fb_media_set */


/* Function Name:
 *      rtl8218b_linkDownPowerSavingEnable_set
 * Description:
 *      Link down power saving (LDPS) enable/disable
 * Input:
 *      port - port ID
 *      enable - ENABLE/DISABLE
 * Output:
 *      None
 * Return:
 *      0 - success.
 *      < 0 - Fail.
 * Note:
 *      For RTL8218B/RTL8218FB/RTL8214FC.
 */
int rtl8218b_linkDownPowerSavingEnable_set(int port, int enable)
{
    uint32  value;
    int32   ret = RT_ERR_FAILED;
    int     media = MEDIATYPE_COPPER;

    if (gMacDrv == NULL)
    {
        printf("MAC driver error!\n");
        return -1;
    }

  #if (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB))
    rtl8214fc_media_get(port, &media);
  #endif/* (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB)) */
    if (media == MEDIATYPE_FIBER)
    {
        /* register is in copper page and can't access when fiber is linkup. */
        printf("Error! port %d is in Fiber mode.\n", port);
        return -1;
    }
    else
    {
        /* For Link-Down Power Saving (per-port) */
        if ((ret = gMacDrv->drv_miim_read(port, 0xa43, 24, &value)) != 0)
            return ret;

        value &= ~(0x1 << 2);
        if (ENABLE == enable)
        {
            value |= (0x1 << 2);
        }
        if ((ret = gMacDrv->drv_miim_write(port, 0xa43, 24, value)) != 0)
            return ret;
    }

    return 0;
} /* end of rtl8218b_linkDownPowerSavingEnable_set */


/* Function Name:
 *      rtl8218b_greenEnable_set
 * Description:
 *      Set the status of link-up green feature of the specific port
 * Input:
 *      port - port ID
 *      enable - ENABLE/DISABLE
 * Output:
 *      None
 * Return:
 *      0 - success.
 *      < 0 - Fail.
 * Note:
 *      For RTL8218B/RTL8218FB/RTL8214FC.
 */
int rtl8218b_greenEnable_set(int port, int enable)
{
    uint32          value, fixed_page;
    int32           ret;
    int             media = MEDIATYPE_COPPER;
    //uint32          link_sts;

    if (gMacDrv == NULL)
    {
        printf("MAC driver error!\n");
        return -1;
    }

  #if (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB))
    rtl8214fc_media_get(port, &media);
  #endif/* (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB)) */
    if (media == MEDIATYPE_FIBER)
    {
        /* register is in copper page and can't access when fiber is linkup. */
        printf("Error! port %d is in Fiber mode.\n", port);
        return -1;
    }
    else
    {
        /* Configure the GPHY page to copper */
        if ((ret = gMacDrv->drv_miim_write(port, 0xa42, 29, 0x0001)) != 0)
            return ret;

        fixed_page = 0;

        /* get value from CHIP*/
        if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 27, 0x8011)) != 0)
            return ret;
        if ((ret = gMacDrv->drv_miim_read(port, fixed_page, 28, &value)) != 0)
            return ret;

        if (ENABLE == enable)
        {
            value |= (1 << 9);
            if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 27, 0x8011)) != 0)
                return ret;
            if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 28, value)) != 0)
                return ret;
        }
        else
        {
            value &= ~(1 << 9);
            if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 27, 0x8011)) != 0)
                return ret;
            if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 28, value)) != 0)
                return ret;
        }

        /* Configure the GPHY page to auto */
        if ((ret = gMacDrv->drv_miim_write(port, 0xa42, 29, 0x0000)) != 0)
            return ret;
    }
    return 0;
} /* end of rtl8218b_greenEnable_set */

/* Function Name:
 *      rtl8218b_crossOverMode_set
 * Description:
 *      Set cross over mode on a specified port
 * Input:
 *      port   - port id (MAC ID)
 *      mode - rtk_port_crossOver_mode_t
 * Output:
 *      None
 * Return:
 *      0: success
 *      < 0: failed
 * Note:
 *      None.
 */
int rtl8218b_crossOverMode_set(int port, int mode)
{
    int32   ret = RT_ERR_FAILED;
    uint32  phyData;
    int     media = MEDIATYPE_COPPER;
    //uint32              link_sts;

    if (gMacDrv == NULL)
    {
        printf("MAC driver error!\n");
        return -1;
    }

  #if (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB))
    rtl8214fc_media_get(port, &media);
  #endif/* (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB)) */
    if (media == MEDIATYPE_FIBER)
    {
        /* register is in copper page and can't access when fiber is linkup. */
        printf("Error! port %d is in Fiber mode.\n", port);
        return -1;
    }
    else
    {
        /* Configure the GPHY page to copper */
        if ((ret = gMacDrv->drv_miim_write(port, 0xa42, 29, 0x0001)) != 0)
            return ret;

        /* get value from CHIP*/
        if ((ret = gMacDrv->drv_miim_read(port, 0xa43, 24, &phyData)) != 0)
            return ret;

        switch (mode)
        {
            case PORT_CROSSOVER_MODE_AUTO:
                phyData &= ~(1 << 9);
                break;
            case PORT_CROSSOVER_MODE_MDI:
                phyData |= (1 << 9);
                phyData |= (1 << 8);
                break;
            case PORT_CROSSOVER_MODE_MDIX:
                phyData |= (1 << 9);
                phyData &= ~(1 << 8);
                break;
            default:
                return RT_ERR_INPUT;
        }

        if ((ret = gMacDrv->drv_miim_write(port, 0xa43, 24, phyData)) != 0)
            return ret;

        /* Configure the GPHY page to auto */
        if ((ret = gMacDrv->drv_miim_write(port, 0xa42, 29, 0x0000)) != 0)
            return ret;
    }
    return 0;
} /* end of rtl8218b_crossOverMode_set */


