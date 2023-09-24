/*
 * Copyright(c) Realtek Semiconductor Corporation, 2013
 * All rights reserved.
 *
 * Purpose : Related switch common util.
 *
 * Feature : switch util function
 *
 */


/*
 * Include Files
 */
#include <rtk_osal.h>
#include <rtk_switch.h>
#include <common/util.h>

#if (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218D))
  #include <rtk/phy/rtl8218b.h>
#endif/* (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218D)) */
#if (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B))
  #include <rtk/phy/rtl8214f.h>
#endif/* (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B)) */
#if (defined(CONFIG_RTL8214C))
  #include <rtk/phy/rtl8214c.h>
#endif/* defined(CONFIG_RTL8214C) */
#if (defined(CONFIG_RTL8218D) || defined(CONFIG_RTL8218B))
  #include <rtk/phy/rtl8218d.h>
#endif/* defined(CONFIG_RTL8218D) */

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
 *      rtk_portIdxFromMacId
 * Description:
 *      Get PHY index from MAC port id
 * Input:
 *      macId   - MAC port id
 * Output:
 *      portIdx - port index relates MAC port id
 * Return:
 *      None
 * Note:
 *      None
 */
int rtk_portIdxFromMacId(int macId, int *portIdx)
{
    int idx;

    if (gSwitchModel == NULL)
        return -1;

    if (gMacDrv == NULL)
        return -1;

    for (idx = 0; idx < gSwitchModel->port.count; ++idx)
    {
        if (gSwitchModel->port.list[idx].mac_id == macId)
        {
            *portIdx = idx;
            return 0;
        }
    }

    return -1;
}

int rtk_phyBaseMacIdFromMacId(int macId, int *baseMacId)
{
    int     portIdx, phyIdx;


    if (rtk_portIdxFromMacId(macId, &portIdx) < 0)
        return -1;

    if ((phyIdx = gSwitchModel->port.list[portIdx].phy_idx) >= gSwitchModel->phy.count)
        return -1;

    return gSwitchModel->phy.list[phyIdx].mac_id;
}


void rtk_phyPatchBit_set(int port, int page, int reg, unsigned char endBit,
    unsigned char startBit, unsigned int inVal)
{
    unsigned char   len;
    unsigned char   i;
    unsigned int    val, mask;

    len = endBit - startBit + 1;

    if (16 == len)
        val = inVal;
    else
    {
        mask = 0;
        for (i = startBit; i <= endBit; ++i)
            mask |= (1 << i);

        gMacDrv->drv_miim_read(port, page, reg, &val);
        val &= ~(mask);
        val |= (inVal << startBit);
    }
    gMacDrv->drv_miim_write(port, page, reg, (int)val);
}

#if (defined(CONFIG_RTL8214FC) || defined(CONFIG_RTL8218FB))
/* Function Name:
 *      rtl8214fc_media_set
 * Description:
 *      Set meida of port.
 * Input:
 *      portid - Port number
 *      media  - Media
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8214fc_media_set(int portid, int media)
{
    int basePort;
    int phy_idx, portIdx;
    unsigned int val;
    int reg;
    unsigned int power;

    if (gSwitchModel == NULL)
        return;

    if (gMacDrv == NULL)
        return;

    if ((media != MEDIATYPE_COPPER) && (media != MEDIATYPE_FIBER))
        return;

    if (rtk_portIdxFromMacId(portid, &portIdx) < 0)
        return;

    phy_idx = gSwitchModel->port.list[portIdx].phy_idx;

    switch (gSwitchModel->phy.list[phy_idx].chip)
    {
        case RTK_CHIP_RTL8214FC:
            basePort = portid - (portid % 4);
            break;
        case RTK_CHIP_RTL8218FB:
            if (0 == gSwitchModel->port.list[portid].phy / 4)
                return;
            basePort = portid - (portid % 8);
            break;
        default:
            return;
    }

    switch (portid % 4)
    {
        case 0:
            reg = 16;
            break;
        case 1:
            reg = 19;
            break;
        case 2:
            reg = 20;
            break;
        case 3:
            reg = 21;
            break;
        default:
            return ;
    }

    gMacDrv->drv_miim_write(basePort, gMacDrv->miim_max_page, 29, 8);
    gMacDrv->drv_miim_read(basePort, 0x266, reg, &val);

    switch (media)
    {
        case MEDIATYPE_COPPER:
            val |= (1<<10);
            val |= (1<<11);
            /* power off fiber */
            gMacDrv->drv_miim_write(portid, gMacDrv->miim_max_page, 29, 3);
            gMacDrv->drv_miim_read(portid, 0, 16, &power);
            if (!(power & (1 << 11)))
            {
                power |= (1 << 11);
                gMacDrv->drv_miim_write(portid, 0, 16, power);
            }
            break;
        case MEDIATYPE_FIBER:
            val |= (1<<10);
            val &= ~(1<<11);
            /* power off copper */
            gMacDrv->drv_miim_write(portid, gMacDrv->miim_max_page, 29, 1);
            gMacDrv->drv_miim_read(portid, 0xa40, 16, &power);
            if (!(power & (1 << 11)))
            {
                power |= (1 << 11);
                gMacDrv->drv_miim_write(portid, 0xa40, 16, power);
            }
            break;
        case MEDIATYPE_COPPER_AUTO:
            val &= ~(1<<10);
            val |= (1<<11);

            break;
        case MEDIATYPE_FIBER_AUTO:
            val &= ~(1<<10);
            val &= ~(1<<11);
            break;
        default:
            return;
    }

    gMacDrv->drv_miim_write(basePort, gMacDrv->miim_max_page, 29, 8);
    gMacDrv->drv_miim_write(basePort, 0x266, reg, val);
    gMacDrv->drv_miim_write(basePort, gMacDrv->miim_max_page, 29, 0);

    /* power on fiber */
    if (media != MEDIATYPE_COPPER)
    {
        gMacDrv->drv_miim_write(portid, gMacDrv->miim_max_page, 29, 3);
        gMacDrv->drv_miim_read(portid, 0, 16, &power);
        if (power & (1 << 11))
        {
            power &= ~(1 << 11);
            gMacDrv->drv_miim_write(portid, 0, 16, power);
        }
    }

    /* power on copper */
    if (media != MEDIATYPE_FIBER)
    {
        gMacDrv->drv_miim_write(portid, gMacDrv->miim_max_page, 29, 1);
        gMacDrv->drv_miim_read(portid, 0xa40, 16, &power);
        if (power & (1 << 11))
        {
            power &= ~(1 << 11);
            gMacDrv->drv_miim_write(portid, 0xa40, 16, power);
        }
    }

    gMacDrv->drv_miim_write(portid, gMacDrv->miim_max_page, 29, 0);

    return;
} /* end of rtl8214fc_media_set */

/* Function Name:
 *      rtl8214fc_media_get
 * Description:
 *      Get meida of port.
 * Input:
 *      portid - Port number (0~28)
 *      media  - Media
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8214fc_media_get(int portid, int *media)
{
    int basePort;
    int phy_idx, portIdx;
    unsigned int val;
    unsigned int reg;

    if (gSwitchModel == NULL)
        return;

    if (gMacDrv == NULL)
        return;

    if (NULL == media)
        return;

    if (rtk_portIdxFromMacId(portid, &portIdx) < 0)
        return;

    phy_idx = gSwitchModel->port.list[portIdx].phy_idx;

    switch (gSwitchModel->phy.list[phy_idx].chip)
    {
        case RTK_CHIP_RTL8214FC:
            basePort = portid - (portid % 4);
            break;
        case RTK_CHIP_RTL8218FB:
            if (0 == gSwitchModel->port.list[portid].phy / 4)
                return;
            basePort = portid - (portid % 8);
            break;
        default:
            return;
    }

    switch (portid % 4)
    {
        case 0:
            reg = 16;
            break;
        case 1:
            reg = 19;
            break;
        case 2:
            reg = 20;
            break;
        case 3:
            reg = 21;
            break;
        default:
            return ;
    }

    gMacDrv->drv_miim_write(basePort, gMacDrv->miim_max_page, 29, 8);
    gMacDrv->drv_miim_read(basePort, 0x266, reg, &val);
    gMacDrv->drv_miim_write(basePort, gMacDrv->miim_max_page, 29, 0);

    if (val & (1 << 10))
    {
        if (val & (1 << 11))
            *media = MEDIATYPE_COPPER;
        else
            *media = MEDIATYPE_FIBER;
    }
    else
    {
        if (val & (1 << 11))
            *media = MEDIATYPE_COPPER_AUTO;
        else
            *media = MEDIATYPE_FIBER_AUTO;
    }

    return;
} /* end of rtl8214fc_media_get */

void rtl8214fc_fiber_watchdog(int port)
{
    int media;
    unsigned int val, backup_reg10_data;
    int chk_loop, chk_error = 0;

    rtl8214fc_media_get(port, &media);
    /* fiber media */
    if (MEDIATYPE_FIBER == media)
    {
        /* only work for giga */
        gMacDrv->drv_miim_read(port, 0, 0, &val);
        if (0 != ((val >> 13) & 0x1) || 1 != ((val >> 6) & 0x1))
            return;

        gMacDrv->drv_miim_read(port, 0, 1, &val);
        gMacDrv->drv_miim_read(port, 0, 1, &val);

        if((val & 0x4) == 0) /*Checking Port is Link Down*/
        {
            gMacDrv->drv_miim_write(port, 0, 30, 0x3); /*Write page 0 reg 30 = 0x3*/
            gMacDrv->drv_miim_write(port, 0xf, 0x10, 0x10); /*Write page 0xf reg 0x10 = 0x10*/

            /*Check Error counter three times*/
            for(chk_loop = 0; chk_loop < 3; chk_loop++)
            {
                gMacDrv->drv_miim_read(port, 0xf, 0x11, &val); /*Read Error counter*/
                if(val == 0xffff)
                    chk_error++;
            }

            if(chk_error > 2)
            { /*Error happen*/
                /*Reset RX*/
                gMacDrv->drv_miim_write(port, 0, 30, 0x3); /*Write page 0 reg 30 = 0x3*/
                gMacDrv->drv_miim_read(port, 0x8, 0x10, &backup_reg10_data);
                val = (backup_reg10_data & (0xffffffd));
                gMacDrv->drv_miim_write(port, 0x8, 0x10, val); /*Set bit1 to 0 to reset RX*/
                val = (backup_reg10_data | 0x2);
                gMacDrv->drv_miim_write(port, 0x8, 0x10, val); /*Set bit1 to 1 to reset RX*/
                //gMacDrv->drv_miim_write(unit, port_index, 0, 30, 0x0); /*Write page 0 reg 30 = 0x0*/
            }
        }
    }

    return;
}
#endif

/* Function Name:
 *      rtk_port_linkDownPowerSavingEnable_set
 * Description:
 *      Link down power saving (LDPS) enable/disable
 * Input:
 *      port - port ID (MAC ID)
 *      enable - ENABLE/DISABLE
 * Output:
 *      None
 * Return:
 *      0 - success.
 *      < 0 - Fail.
 * Note:
 *      For RTL8214C
 */
int rtk_port_linkDownPowerSavingEnable_set(int portid, int enable)
{
    int phy_idx, portIdx;
    int ret = -1;

    if (gSwitchModel == NULL)
    {
        printf("MAC driver not found!\n");
        return -1;
    }

    if (rtk_portIdxFromMacId(portid, &portIdx) < 0)
    {
        printf("Port %d not exist!\n", portid);
        return -1;
    }

    phy_idx = gSwitchModel->port.list[portIdx].phy_idx;
    switch (gSwitchModel->phy.list[phy_idx].chip)
    {
    #if (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC))
      case RTK_CHIP_RTL8218B:
      case RTK_CHIP_RTL8218FB:
      case RTK_CHIP_RTL8214FC:
        ret = rtl8218b_linkDownPowerSavingEnable_set(portid, enable);
        break;
    #endif/* (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC)) */
    #if (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B))
      case RTK_CHIP_RTL8214FB:
      case RTK_CHIP_RTL8214B:
      case RTK_CHIP_RTL8212B:
        ret = rtl8214fb_linkDownPowerSavingEnable_set(portid, enable);
        break;
    #endif/* (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B)) */
    #if (defined(CONFIG_RTL8214C))
      case RTK_CHIP_RTL8214C:
        ret = rtl8214c_linkDownPowerSavingEnable_set(portid, enable);
        break;
    #endif/* defined(CONFIG_RTL8214C)) */
    #if (defined(CONFIG_RTL8218D) || defined(CONFIG_RTL8218B))
      case RTK_CHIP_RTL8218D:
        ret = phy_8218d_linkDownPowerSavingEnable_set(MY_UNIT, portid, enable);
        break;
    #endif/* defined(CONFIG_RTL8218D)) */
      default:
        printf("port %d feature not support\n", portid);
        return -1;
    }/* switch */

    return ret;
}


/* Function Name:
 *      rtl8214c_greenEnable_set
 * Description:
 *      Set the status of link-up green feature of the specific port
 * Input:
 *      port   - port id (MAC ID)
 *      enable - ENABLE/DISABLE
 * Output:
 *      None
 * Return:
 *      0: success
 *      < 0: failed
 * Note:
 *      None.
 */
int rtk_port_greenEnable_set(int portid, int enable)
{
    int phy_idx, portIdx;
    int ret = -1;


    if (gSwitchModel == NULL)
    {
        printf("MAC driver not found!\n");
        return -1;
    }

    if (rtk_portIdxFromMacId(portid, &portIdx) < 0)
    {
        printf("Port %d not exist!\n", portid);
        return -1;
    }

    phy_idx = gSwitchModel->port.list[portIdx].phy_idx;
    switch (gSwitchModel->phy.list[phy_idx].chip)
    {
    #if (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC))
      case RTK_CHIP_RTL8218B:
      case RTK_CHIP_RTL8218FB:
      case RTK_CHIP_RTL8214FC:
        ret = rtl8218b_greenEnable_set(portid, enable);
        break;
    #endif/* (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC)) */
    #if (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B))
      case RTK_CHIP_RTL8214FB:
      case RTK_CHIP_RTL8214B:
      case RTK_CHIP_RTL8212B:
        ret = rtl8214fb_greenEnable_set(portid, enable);
        break;
    #endif/* (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B)) */
    #if (defined(CONFIG_RTL8214C))
      case RTK_CHIP_RTL8214C:
        ret = rtl8214c_greenEnable_set(portid, enable);
        break;
    #endif/* defined(CONFIG_RTL8214C)) */
    #if (defined(CONFIG_RTL8218D) || defined(CONFIG_RTL8218B))
      case RTK_CHIP_RTL8218D:
        ret = phy_8218d_greenEnable_set(MY_UNIT, portid, enable);
        break;
    #endif/* defined(CONFIG_RTL8218D)) */
      default:
        printf("port %d feature not support\n", portid);
        return -1;
    }/* switch */

    return ret;
}

/* Function Name:
 *      rtk_port_phyCrossOverMode_set
 * Description:
 *      Set cross over mode on a specified port
 * Input:
 *      port   - port id (MAC ID)
 *      mode - Auto/Force-MDI/Force-MDIX
 * Output:
 *      None
 * Return:
 *      0: success
 *      < 0: failed
 * Note:
 *      None.
 */
int rtk_port_phyCrossOverMode_set(int portid, rtk_port_crossOver_mode_t mode)
{
    int phy_idx, portIdx;
    int ret = -1;


    if (gSwitchModel == NULL)
    {
        printf("MAC driver not found!\n");
        return -1;
    }

    if (rtk_portIdxFromMacId(portid, &portIdx) < 0)
    {
        printf("Port %d not exist!\n", portid);
        return -1;
    }

    phy_idx = gSwitchModel->port.list[portIdx].phy_idx;
    switch (gSwitchModel->phy.list[phy_idx].chip)
    {
    #if (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC))
      case RTK_CHIP_RTL8218B:
      case RTK_CHIP_RTL8218FB:
      case RTK_CHIP_RTL8214FC:
        ret = rtl8218b_crossOverMode_set(portid, mode);
        break;
    #endif/* (defined(CONFIG_RTL8218B) || defined(CONFIG_RTL8218FB) || defined(CONFIG_RTL8214FC)) */
    #if (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B))
      case RTK_CHIP_RTL8214FB:
      case RTK_CHIP_RTL8214B:
      case RTK_CHIP_RTL8212B:
        ret = rtl8214fb_crossOverMode_set(portid, mode);
        break;
    #endif/* (defined(CONFIG_RTL8214FB) || defined(CONFIG_RTL8214B) || defined(CONFIG_RTL8212B)) */
    #if (defined(CONFIG_RTL8214C))
      case RTK_CHIP_RTL8214C:
        ret = rtl8214c_crossOverMode_set(portid, mode);
        break;
    #endif/* defined(CONFIG_RTL8214C)) */
    #if (defined(CONFIG_RTL8218D) || defined(CONFIG_RTL8218B))
      case RTK_CHIP_RTL8218D:
        ret = phy_8218d_crossOverMode_set(MY_UNIT, portid, mode);
        break;
    #endif/* defined(CONFIG_RTL8218D)) */
      default:
        printf("port %d feature not support\n", portid);
        return -1;
    }/* switch */

    return ret;
}


