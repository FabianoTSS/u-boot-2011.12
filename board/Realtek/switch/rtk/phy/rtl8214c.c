/*
 * Copyright(c) Realtek Semiconductor Corporation, 2015
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8214F PHY driver.
 *
 * Feature : RTL8214C PHY driver
 *
 */


/*
 * Include Files
 */
#include <rtk_osal.h>
#include <rtk_debug.h>
#include <rtk_switch.h>
#include <rtk_phy.h>
#include <common/util.h>
#include <rtk/phy/conf/conftypes.h>
//#include <rtk/phy/phy_drv.h>
#include <config.h>


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

/*
 * Function Declaration
 */

/* Function Name:
 *      _rtl8214c_indirect_read
 * Description:
 *      Get PHY register.
 * Input:
 *      portid - Port number (0~51)
 *      page   - PHY page (0~127)
 *      reg    - PHY register (0~31)
 *      val    - data to write
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
int _rtl8214c_indirect_read(int port, int page, int reg, unsigned int *val)
{
    unsigned int addr, data;

    /* bit [10:5] page; bit [4:0] register */
    addr = (page << 5) | reg;
    gMacDrv->drv_miim_write(port, 0, 29, 0x6601);
    gMacDrv->drv_miim_write(port, 0, 30, addr);

    gMacDrv->drv_miim_write(port, 0, 29, 0x6600);
    gMacDrv->drv_miim_write(port, 0, 30, 0x80);

    do
    {
        gMacDrv->drv_miim_write(port, 0, 29, 0x6600);
        gMacDrv->drv_miim_read(port, 0, 30, &data);
    } while (((data >> 8) & 0x1) != 0);

    gMacDrv->drv_miim_write(port, 0, 29, 0x6602);
    gMacDrv->drv_miim_read(port, 0, 30, val);

    return 0;
}   /* end of _rtl8214c_indirect_read */

/* Function Name:
 *      _rtl8214c_indirect_write
 * Description:
 *      Set PHY register.
 * Input:
 *      portid - Port number (0~51)
 *      page   - PHY page (0~127)
 *      reg    - PHY register (0~31)
 *      val    - data to write
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
int _rtl8214c_indirect_write(int port, int page, int reg, unsigned int val)
{
    unsigned int addr, data;

    /* bit [10:5] page; bit [4:0] register */
    addr = (page << 5) | reg;

    gMacDrv->drv_miim_write(port, 0, 29, 0x6601);
    gMacDrv->drv_miim_write(port, 0, 30, addr);

    gMacDrv->drv_miim_write(port, 0, 29, 0x6602);
    gMacDrv->drv_miim_write(port, 0, 30, val);

    gMacDrv->drv_miim_write(port, 0, 29, 0x6600);
    gMacDrv->drv_miim_write(port, 0, 30, 0xC0);

    do
    {
        gMacDrv->drv_miim_write(port, 0, 29, 0x6600);
        gMacDrv->drv_miim_read(port, 0, 30, &data);

    } while (((data >> 8) & 0x1) != 0);

    return 0;
}   /* end of _rtl8214c_indirect_write */
#if defined(CONFIG_RTL8380)
static void _rtl8214c_serdes_rtl8380_config(Tuint8 phy0_macid)
{
    _rtl8214c_indirect_write(phy0_macid, 0x2a, 0, 0x84D2);
    _rtl8214c_indirect_write(phy0_macid, 0x2a, 1, 0xF995);
    _rtl8214c_indirect_write(phy0_macid, 0x2a, 2, 0x3DA1);
    _rtl8214c_indirect_write(phy0_macid, 0x2a, 3, 0x3960);
    _rtl8214c_indirect_write(phy0_macid, 0x2a, 4, 0x9728);
    _rtl8214c_indirect_write(phy0_macid, 0x2a, 5, 0xf83f);
    _rtl8214c_indirect_write(phy0_macid, 0x21, 3, 0x9D85);
    _rtl8214c_indirect_write(phy0_macid, 0x21, 4, 0xD810);

    _rtl8214c_indirect_write(phy0_macid, 0x21, 2, 0xb682);
    _rtl8214c_indirect_write(phy0_macid, 0x21, 2, 0xb602);
    _rtl8214c_indirect_write(phy0_macid, 0, 3, 0x7146);
    _rtl8214c_indirect_write(phy0_macid, 0, 3, 0x7106);
}   /* end of _rtl8214c_serdes_rtl8380_config */
#endif /* end of #if defined(CONFIG_RTL8380) */
static void _rtl8214c_serdes_config(Tuint8 phy0_macid)
{
    _rtl8214c_indirect_write(phy0_macid, 0x2a, 2, 0x2da1);
    _rtl8214c_indirect_write(phy0_macid, 0x21, 2, 0xb682);
    _rtl8214c_indirect_write(phy0_macid, 0x21, 2, 0xb602);
    _rtl8214c_indirect_write(phy0_macid, 0, 3, 0x7146);
    _rtl8214c_indirect_write(phy0_macid, 0, 3, 0x7106);
}   /* end of _rtl8214c_serdes_config */

/* Function Name:
 *      rtl8214c_config
 * Description:
 *      Configuration code for RTL8218c.
 * Input:
 *      macId - the macId of PHY0 of the RTL8218b
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8214c_config(Tuint8 macId)
{
    int phy0_macid = macId - (macId % 4);
    int idx, phyIdx;
    unsigned int val;

    OSAL_PRINTF("### RTL8214C config - MAC ID = %d ###\n", phy0_macid);

    switch (gSwitchModel->chip)
    {
        #if defined(CONFIG_RTL8380)
        case RTK_CHIP_RTL8382M:
        case RTK_CHIP_RTL8381M:
        case RTK_CHIP_RTL8332M:
        case RTK_CHIP_RTL8380M:
        case RTK_CHIP_RTL8330M:
            _rtl8214c_serdes_rtl8380_config(phy0_macid);
            break;
        #endif /* end of #if defined(CONFIG_RTL8380) */
        default:
            _rtl8214c_serdes_config(phy0_macid);
            break;
    }

    for (idx = 0; idx < gSwitchModel->phy.count; ++idx)
    {
        if (phy0_macid == gSwitchModel->phy.list[idx].mac_id)
        {
            break;
        }
    }

    if (idx == gSwitchModel->phy.count)
    {
        printf("Invalid MAC id %d %d\n", phy0_macid, macId);
        return;
    }

    phyIdx = idx;

    for (idx = 0; idx < gSwitchModel->serdes.count; ++idx)
    {
        if (phyIdx == gSwitchModel->serdes.list[idx].phy_idx)
        {
            break;
        }
    }

    if (idx == gSwitchModel->serdes.count)
    {
        printf("Invalid PHY index %d\n", phyIdx);
        return;
    }

    _rtl8214c_indirect_read(phy0_macid, 0, 0, &val);

    if (SERDES_POLARITY_CHANGE == gSwitchModel->serdes.list[idx].rx_polarity)
    {
        val |= (1 << 9);
    }

    if (SERDES_POLARITY_CHANGE == gSwitchModel->serdes.list[idx].tx_polarity)
    {
        val |= (1 << 8);
    }

    _rtl8214c_indirect_write(phy0_macid, 0, 0, val);

    return;
} /* end of rtl8214c_config */

/* Function Name:
 *      rtl8214c_linkDownPowerSavingEnable_set
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
 *      For RTL8214C
 */
int rtl8214c_linkDownPowerSavingEnable_set(int port, int enable)
{
    uint32  value;
    int32   ret;

    if (gMacDrv == NULL)
    {
        printf("Port %d driver error!\n", port);
        return -1;
    }

    /* For Link-Down Power Saving (per-port) */
    if ((ret = gMacDrv->drv_miim_read(port, 0xa43, 24, &value)) != RT_ERR_OK)
        return ret;

    value &= ~(0x1 << 2);
    if (ENABLE == enable)
    {
        value |= (0x1 << 2);
    }
    if ((ret = gMacDrv->drv_miim_write(port, 0xa43, 24, value)) != RT_ERR_OK)
        return ret;

    return 0;

} /* end of rtl8214c_linkDownPowerSavingEnable_set */

/* Function Name:
 *      rtl8214c_greenEnable_set
 * Description:
 *      Set the status of link-up green feature of the specific port
 * Input:
 *      port   - port id
 *      enable - ENABLE/DISABLE
 * Output:
 *      None
 * Return:
 *      0: success
 *      < 0: failed
 * Note:
 *      For RTL8214c
 */
int rtl8214c_greenEnable_set(int port, int enable)
{
    uint32  value, fixed_page;
    int32   ret = RT_ERR_FAILED;
    uint32  phyData0, restore_phyData0, restore_flag;

    if (gMacDrv == NULL)
    {
        printf("Port %d driver error!\n", port);
        return -1;
    }

    fixed_page = 0;
    restore_flag = 0;

    if ((ret = gMacDrv->drv_miim_read(port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    restore_phyData0 = phyData0;

    /* If the port is link up, link down at first */
    if ((phyData0 & (PowerDown_MASK)) == 0)
    {
        restore_flag = 1;
        phyData0 |= (1 << PowerDown_OFFSET);
        if ((ret = gMacDrv->drv_miim_write(port, PHY_PAGE_0, PHY_CONTROL_REG, phyData0)) != RT_ERR_OK)
            return ret;
    }

    /* get value from CHIP*/
    if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 27, 0x8011)) != RT_ERR_OK)
        return ret;
    if ((ret = gMacDrv->drv_miim_read(port, fixed_page, 28, &value)) != RT_ERR_OK)
        return ret;

    if (ENABLE == enable)
    {
        value |= (1 << 9);
        if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 27, 0x8011)) != RT_ERR_OK)
            return ret;
        if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 28, value)) != RT_ERR_OK)
            return ret;
    }
    else
    {
        value &= ~(1 << 9);
        if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 27, 0x8011)) != RT_ERR_OK)
            return ret;
        if ((ret = gMacDrv->drv_miim_write(port, fixed_page, 28, value)) != RT_ERR_OK)
            return ret;
    }

    /* To restore Reg 0*/
    if (restore_flag == 1)
    {
        if ((ret = gMacDrv->drv_miim_write(port, PHY_PAGE_0, PHY_CONTROL_REG, restore_phyData0)) != RT_ERR_OK)
            return ret;
    }

    return RT_ERR_OK;
} /* end of rtl8214c_greenEnable_set */


/* Function Name:
 *      rtk_port_phyCrossOverMode_set
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
 *      For RTL8214C
 */
int rtl8214c_crossOverMode_set(int port, int mode)
{
    int32   ret = -1;
    uint32  phyData;

    if (gMacDrv == NULL)
    {
        printf("Port %d driver error!\n", port);
        return -1;
    }

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
            return -1;
    }

    if ((ret = gMacDrv->drv_miim_write(port, 0xa43, 24, phyData)) != 0)
        return ret;

    return 0;

} /* end of rtl8214c_crossOverMode_set */



