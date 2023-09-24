/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : Related definition of the customer MAC init for U-Boot.
 *
 * Feature : customer MAC init function
 *
 */


/*
 * Include Files
 */
#include <config.h>
#include <rtk_type.h>
#include <rtk_osal.h>
#include <rtk_debug.h>
#include <rtk_switch.h>
#include <rtk/phy/rtl8214f.h>
#include <rtk/phy/rtl8218.h>
#include <rtk/drv/rtl8231/rtl8380_8231_drv.h>

/*
 * Symbol Definition
 */
#define CHIPVER_B_CUT       (0)                   /* B-CUT */


/*
 * Macro Definition
 */
#ifndef VALUE_CHG
#define VALUE_CHG(var,mask,val)     do { var = ((var & ~(mask)) | ((val) & (mask))); } while(0)
#endif
#ifndef VALUE_CHK
#define VALUE_CHK(var,val)          do { \
    if (var != val) \
        printf("Error:%d - Var 0x%08X != 0x%08X\n", __LINE__, var, val); \
        } while(0)
#endif
#ifndef REG32_CHG
#define REG32_CHG(reg,mask,val)     do { REG32(reg) = ((REG32(reg) & ~(mask)) | ((val) & (mask))); } while(0)
#endif
#ifndef REG32_CHK
#define REG32_CHK(reg,val)          do { \
    if (REG32(reg) != val) \
        printf("Error:%d - Reg(0x%08X), 0x%08X != 0x%08X\n", __LINE__, reg, REG32(reg), val); \
        } while(0)
#endif
#ifndef CHIPVER_CHAR
#define CHIPVER_CHAR(ver)           (ver + 0x41 + 1)
#endif
#ifndef PORTMASK_NUM
#define PORTMASK_NUM(port_num)      (0x1 << (port_num))
#endif
#ifndef PORTMASK_COUNT
#define PORTMASK_COUNT(port_count)  ((0x1 << (port_count)) - 1)
#endif

/*
 * Data Declaration
 */
extern const rtk_mac_drv_t *gMacDrv;

/*
 * Function Declaration
 */

/* Function Name:
 *      customer_mac_config_init
 * Description:
 *      Mac Configuration code for customer specified
 * Input:
 *      pModel - pointer to switch model of platform
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void customer_mac_config_init(const rtk_switch_model_t *pModel)
{
    if (pModel == NULL)
    {
        OSAL_PRINTF("%s():%d - pModel is NULL\n", __FUNCTION__, __LINE__);
        return;
    }

    /* Wait for customer to implement their configuration */

    return;
} /* end of customer_mac_config_init */

/* Function Name:
 *      customer_phy_config_init
 * Description:
 *      PHY Configuration code for customer specified
 * Input:
 *      pModel - pointer to switch model of platform
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void customer_phy_config_init(const rtk_switch_model_t *pModel)
{
    unsigned int phy_idx;

    if (pModel == NULL)
    {
        OSAL_PRINTF("%s():%d - pModel is NULL\n", __FUNCTION__, __LINE__);
        return;
    }

    /* Wait for customer to implement their configuration */
#if defined(CONFIG_GFIRST_16G8FX)
    if (0 == strcmp(pModel->name, "Gfirst_16G_8FX"))
    {
        for (phy_idx=0; phy_idx<pModel->phy.count; phy_idx++)
        {
            switch (pModel->phy.list[phy_idx].chip)
            {
    #if (defined(CONFIG_RTL8214FC))
                case RTK_CHIP_RTL8214FC:
                    {
                        Tuint8 rtl8214fc_phy_macid = pModel->phy.list[phy_idx].mac_id - pModel->phy.baseid;

                         /*Force 14FC Fiber*/
                        gMacDrv->drv_miim_write(rtl8214fc_phy_macid, 0, 29, 8);
                        gMacDrv->drv_miim_write(rtl8214fc_phy_macid, 0x266, 16, 0x795);
                        gMacDrv->drv_miim_write(rtl8214fc_phy_macid, 0x266, 19, 0x795);
                        gMacDrv->drv_miim_write(rtl8214fc_phy_macid, 0x266, 20, 0x795);
                        gMacDrv->drv_miim_write(rtl8214fc_phy_macid, 0x266, 21, 0x795);
                        gMacDrv->drv_miim_write(rtl8214fc_phy_macid, 0, 29, 0);
                    }
                    break;
    #endif
    #if (defined(CONFIG_RTL8214FB))
                case RTK_CHIP_RTL8214FB:
                    break;
    #endif
    #if (defined(CONFIG_RTL8218B))
                case RTK_CHIP_RTL8218B:
                    break;
    #endif
    #if (defined(CONFIG_RTL8218FB))
                case RTK_CHIP_RTL8218FB:
                    break;
    #endif
    #if (defined(CONFIG_RTL8208))
                case RTK_CHIP_RTL8208L:
                    break;
    #endif
                default:
                    break;
            }
        }
    }
#endif

    return;
} /* end of customer_phy_config_init */



void customer_mac_misc_config_init(const rtk_switch_model_t *pModel)
{
    if (pModel == NULL)
    {
        OSAL_PRINTF("%s():%d - pModel is NULL\n", __FUNCTION__, __LINE__);
        return;
    }

    /* Wait for customer to implement their configuration */

    return;
} /* end of customer_mac_misc_config_init */
