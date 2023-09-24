/*
 * Copyright(c) Realtek Semiconductor Corporation, 2008
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8218D PHY driver.
 *
 * Feature : RTL8218D PHY driver
 *
 */


/*
 * Include Files
 */
#include <rtk_osal.h>
#include <rtk_debug.h>
#include <rtk_switch.h>
#include <rtk_reg.h>
#include <rtk_phy.h>
#include <common/util.h>
#include <rtk/phy/conf/conftypes.h>
#include <rtk/phy/rtl8218d.h>
#include <rtk/phy/conf/conf_rtl8218d.c>
#include <rtk/mac/rtl8390/rtl8390_drv.h>
#include <config.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */


/* Function Name:
 *      phy_construct_confcodeRv_write
 * Description:
 *      Construct the PHY with config code
 * Input:
 *      unit - unit id
 *      port - port id
 *      confcode_rv_t   - config code
 *      enty_num - entry number of the patch code
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
phy_construct_confcodeRv_write(uint32 unit, uint32 port, confcode_rv_t *pCc, uint32 enty_num)
{
    int32       i;

    for (i = 0; i < enty_num; i++)
    {
        RTK_MII_WRITE(unit, port, HAL_MIIM_FIX_PAGE, pCc[i].reg, pCc[i].val);
    }
}


/* Function Name:
 *      phy_construct_confcodePrv_write
 * Description:
 *      Construct the PHY with chip based config code
 * Input:
 *      unit - unit id
 *      phy_base_macId - base MAC ID of the PHY
 *      confcode_prv_t   - PHY config code
 *      enty_num - entry number of the patch code
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
phy_construct_confcodePrv_write(uint32 unit, uint32 phy_base_macId, confcode_prv_t *pPcc, uint32 enty_num)
{
    int32       i;

    for (i = 0; i < enty_num; i++)
    {
        RTK_MII_WRITE(unit, (phy_base_macId + pPcc[i].phy), HAL_MIIM_FIX_PAGE, pPcc[i].reg, pPcc[i].val);
    }
}

/*
 * Function Declaration
 */

/* Function Name:
 *      _rtl8218d_serdesMode_get
 * Description:
 *      Get SerDes mode of the PHY
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218d
 * Output:
 *      pMode - serdes mode
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAIED
 * Note:
 *      None
 */
int32
_rtl8218d_serdesMode_get(uint32 unit, uint8 phy_base_macId, rtk_mii_t *pMode)
{
    uint32      data, blk;

    RTK_MII_READ(unit, phy_base_macId, 0, 30, &blk);
    RTK_MII_WRITE(unit, phy_base_macId, 0, 30, 8);
    RTK_MII_READ(unit, phy_base_macId, 0x260, 18, &data);
    RTK_MII_WRITE(unit, phy_base_macId, 0, 30, blk);

    switch (data & 0xF0)
    {
      case 0xD0:
        *pMode = RTK_MII_QSGMII;
        break;
      case 0xB0:
        *pMode = RTK_MII_XSGMII;
        break;
      default:
        RTK_DBG_PRINT(0, "unit %u 8218D phy port %u serdes mode: %X unknown type.\n", unit, (uint32)phy_base_macId, (data & 0xF0));
        return RT_ERR_FAILED;
    }
    return RT_ERR_OK;
}


void
_rtl8218d_sdsBasic_config(uint32 unit, uint8 phy_base_macId)
{
    rtk_mii_t       mode;
    int32           ret;

    if ((ret = _rtl8218d_serdesMode_get(unit, phy_base_macId, &mode)) != RT_ERR_OK)
        return;

    switch(mode)
    {
        case RTK_MII_QSGMII:
            PHY_CONSTRUCT_PRV_WRITE(unit, phy_base_macId, rtl8218d_a_qsgmii_basic_perchip);
            break;
        case RTK_MII_XSGMII:
            PHY_CONSTRUCT_PRV_WRITE(unit, phy_base_macId, rtl8218d_a_xsgmii_basic_perchip);
            break;
        default:
            return;
    }

    return;
}


#if defined(CONFIG_RTL8390)

/* Function Name:
 *      _rtl8218d_rtl8390_config
 * Description:
 *      Configuration code for RTL8218d.
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218d
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
_rtl8218d_rtl8390_config(uint32 unit, uint8 phy_base_macId)
{
    _rtl8218d_sdsBasic_config(unit, phy_base_macId);
}
#endif

#if defined(CONFIG_RTL8380)
/* Function Name:
 *      _rtl8218d_rtl8380_config
 * Description:
 *      Configuration code for RTL8218d.
 * Input:
 *      unit
 *      phy_base_macId - the macId of PHY0 of the RTL8218d
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void _rtl8218d_rtl8380_config(uint32 unit, uint8 phy_base_macId)
{
    PHY_CONSTRUCT_PRV_WRITE(unit, phy_base_macId, rtl8218d_a_qsgmii_rtl8380_perchip);
}
#endif


/* Function Name:
 *      phy_8218d_chipRevId_get
 * Description:
 *      Get chip revision ID
 * Input:
 *      unit   - unit id
 *      port   - port id
 * Output:
 *      pChip_rev_id - chip revision ID
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      None
 */
int32
phy_8218d_chipRevId_get(uint32 unit, uint32 port, uint16 *chip_rev_id)
{
    uint32              basePort, blk;
    int32               ret = RT_ERR_OK;
    uint32              data;

//    basePort = HWP_PHY_BASE_MACID(unit, port);
    basePort = port;
    if ((ret = RTK_MII_READ(unit, basePort, 0, 30, &blk)) != RT_ERR_OK)
    {
        return ret;
    }

    RTK_MII_WRITE(unit, basePort, 0, 30, 8);
    RTK_MII_READ(unit, basePort, 0x327, 0x15, &data);
    RTK_MII_WRITE(unit, basePort, 0, 30, blk);

    *chip_rev_id = (uint16) REG32_FIELD_GET(data, 0, 0x3F);
    return RT_ERR_OK;
}


void
_rtl8218d_phy_config(uint32 unit, uint8 phy_base_macId)
{
    uint32      portnum, port;
    uint32      i = 0;
    uint32      value;
    uint16      chip_rev_id;
    int32       ret;

    if ((ret = phy_8218d_chipRevId_get(unit, phy_base_macId, &chip_rev_id)) != RT_ERR_OK)
    {
        return;
    }

//    portnum = HWP_PHY_BASE_PHY_MAX(unit, phy_base_macId);
    portnum = PORT_NUM_IN_8218D;

    switch (chip_rev_id)
    {
      case 0:
        {
            for (i = 0; i < portnum; i++)
            {
                port = phy_base_macId + i;
                /* backup reg 0 value */
                RTK_MII_READ(unit, port, HAL_MIIM_FIX_PAGE, 0x0, &value);

                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_1000FM_INRX_perport);
                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_100F_INRX_perport);

                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_green_perport);

                 /* restore reg 0 value */
                RTK_MII_WRITE(unit, port, HAL_MIIM_FIX_PAGE, 0x0, value);

                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_open_ini_perport);
                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_mismatch_ini_perport);
                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_lndrv_ini_perport);
                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_normal_cablen_perport);
            }/* end for */
        }
        break;
      case 1:
      default:
        {
            for (i = 0; i < portnum; i++)
            {
                port = phy_base_macId + i;

                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_open_ini_perport);
                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_mismatch_ini_perport);
                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_lndrv_ini_perport);
                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_a_gphy_rtct_y_normal_cablen_perport);

                PHY_CONSTRUCT_RV_WRITE(unit, port, rtl8218d_b_gphy_1000FM_INRX_perport);
             }/* end for */
             PHY_CONSTRUCT_PRV_WRITE(unit, phy_base_macId, rtl8218d_b_gphy_gdac_ib_dn_10_perchip);

            phy_8218d_bConf_init(unit, phy_base_macId);
        }
        break;
    }/* end switch */
}




/* Function Name:
 *      rtl8218d_config
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
 void rtl8218d_config(uint32 unit, uint8 phy_base_macId)
 {

    OSAL_PRINTF("### RTL8218D config - MAC ID = %d ###\n", phy_base_macId);
#if defined(CONFIG_RTL8380)
    _rtl8218d_rtl8380_config(unit,phy_base_macId);
#elif defined(CONFIG_RTL8390)
    _rtl8218d_rtl8390_config(unit,phy_base_macId);
#else
    _rtl8218d_sdsBasic_config(unit,phy_base_macId);
#endif

    _rtl8218d_phy_config(unit, phy_base_macId);

}

void
_phy_8218d_ieeeTestModeAllPhyPort_disable(uint32 unit, uint32 phyBasePort, uint32 phyPortCnt)
{
    uint32      phyPort;

    for (phyPort = 0; phyPort < phyPortCnt; phyPort++)
    {
        RTK_MII_WRITE(unit, phyBasePort+phyPort,0xA41,9,0x0E00);  // Disable Test mode
    }
}

/* Function Name:
 *      phy_8218d_ieeeTestMode_set
 * Description:
 *      Set test mode for Giga PHY transmitter test
 * Input:
 *      unit  - unit id
 *      port  - port id
 *      pTestMode->mode  - test mode 1 ~ 4 which is defined in IEEE 40.6.1.1.2
 *      pTestMode->channel  - Channel A, B, C, D, or none
 *      pTestMode->flags -
 *          RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS -
 *              apply the test on all ports of the PHY.
 *              To use this feature, the "port" parameter shall set to the first port of the PHY.
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_NOT_ALLOWED - The operation is not allowed
 *      RT_ERR_PORT_NOT_SUPPORTED - test mode is not supported
 * Note:
 *      RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS only support in mode 4
 */
int32
phy_8218d_ieeeTestMode_set(uint32 unit, uint32 port, rtk_port_phyTestMode_t *pTestMode)
{
    uint32      phyBasePort = RTL8218D_BASEPORT_ID(port);
    uint32      phyPortCnt = 0;
    uint32      val, chn;

    OSAL_PRINTF("RTL8218D Test Mode (PHYID: %u)\n", port);

    if ((pTestMode->flags & RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS) && (pTestMode->mode != 4))
    {
        OSAL_PRINTF("mode %u not support all-phy-port operation.\n", pTestMode->mode);
        return RT_ERR_NOT_ALLOWED;
    }

    phyPortCnt = PORT_NUM_IN_8218D;

    switch (pTestMode->mode)
    {
      case 1:
        {
            _phy_8218d_ieeeTestModeAllPhyPort_disable(unit, phyBasePort, phyPortCnt);

            RTK_MII_WRITE(unit, port,0xA41,9,0x2E00); // Test mode 1
            return RT_ERR_OK;
        }
        break;

      case 4:
        {
            uint32      phyPort;

            //0=A,1=B,2=C,3=D
            chn = pTestMode->channel - PHY_TESTMODE_CHANNEL_A;
            if (chn > 3)
            {
                OSAL_PRINTF(" Error test channel: %u\n", chn);
                return RT_ERR_INPUT;
            }

            OSAL_PRINTF(" test channel: %u\n", chn);

            if (!(pTestMode->flags & RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS))
            {
                /* single port test */
                for (phyPort = 0; phyPort < phyPortCnt; phyPort++)
                {
                    RTK_MII_WRITE(unit, phyBasePort+phyPort,0xBC0,18,0x0000);
                    RTK_MII_WRITE(unit, phyBasePort+phyPort,0xBC0,19,0x01C0);
                    RTK_MII_WRITE(unit, phyBasePort+phyPort,0xA41,9,0x0E00);// Disable Test mode
                }

                val = 0x1 << chn;
                RTK_MII_WRITE(unit, port,0xBC0,18, val);
                RTK_MII_WRITE(unit, port,0xBC0,19,0x01D0);/*Enable get power control*/
                RTK_MII_WRITE(unit, port,0xA41,9,0x8E00);/*Enable Test Mode 4*/

                return RT_ERR_OK;
            }
            else
            {
                OSAL_PRINTF("Not support all-phy-port operation.\n");
                return RT_ERR_CHIP_NOT_SUPPORTED;
            }
        }
        break;

    default:
        OSAL_PRINTF("The mode (%u) is not suppoted.\n", pTestMode->mode);
        return RT_ERR_CHIP_NOT_SUPPORTED;
    }

}


/* Function Name:
 *      phy_common_autoNegoEnable_get
 * Description:
 *      Get auto-negotiation enable status of the specific port
 * Input:
 *      unit - unit id
 *      port - port id
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - invalid parameter
 * Note:
 *      None
 */
int32
phy_common_autoNegoEnable_get(uint32 unit, rtk_port_t port, rtk_enable_t *pEnable)
{
    int32   ret;
    uint32  phyData0;

    /* get value from CHIP*/
    if ((ret = RTK_MII_READ(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData0)) != RT_ERR_OK)
        return ret;

    if (phyData0 & AutoNegotiationEnable_MASK)
    {
        *pEnable = ENABLED;
    }
    else
    {
        *pEnable = DISABLED;
    }

    return ret;
} /* end of phy_common_autoNegoEnable_get */


/* Function Name:
 *      phy_8218_eeeEnable_set
 * Description:
 *      Set enable status of EEE function in the specified port.
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - enable status of EEE
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_NOT_INIT - The module is not initial
 *      RT_ERR_UNIT_ID  - invalid unit id
 *      RT_ERR_PORT_ID  - invalid port id
 *      RT_ERR_INPUT    - invalid input parameter
 * Note:
 *      None
 */
int32
phy_8218d_eeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  phyData, val;
    int32   ret = RT_ERR_FAILED;
    rtk_enable_t     an_enable;

    /* Configure the GPHY page to copper */
    if ((ret = RTK_MII_WRITE(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    phy_common_autoNegoEnable_get(unit, port, &an_enable);

    if ((ret = RTK_MII_READ(unit, port, 0xa5d, 16, &phyData)) != RT_ERR_OK)
    {
        return ret;
    }

    if (enable == ENABLED)
        val = 1;
    else
        val = 0;

    phyData = REG32_FIELD_SET(phyData, val, 2, (0x1 << 2)); /* 1000M EEE ability */
    phyData = REG32_FIELD_SET(phyData, val, 1, (0x1 << 1)); /* 100M EEE ability */
    ret = RTK_MII_WRITE(unit, port, 0xa5d, 16, phyData);
    if ((ret != RT_ERR_OK)&&(ret != RT_ERR_CHIP_NOT_SUPPORTED))
    {
        return ret;
    }

    if ((ret = RTK_MII_READ(unit, port, 0xa42, 20, &phyData)) != RT_ERR_OK)
    {
        return ret;
    }

    phyData = REG32_FIELD_SET(phyData, val, 7, (0x1 << 7)); /* 500M EEE ability */
    /* the enable status will depending on the chip support 500M or not */
    if ((ret = RTK_MII_WRITE(unit, port, 0xa42, 20, phyData)) != RT_ERR_OK)
    {
        return ret;
    }

    /* Force re-autonegotiation if AN is on*/
    if (ENABLED == an_enable)
    {
        if ((ret = RTK_MII_READ(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
            return ret;

        phyData = REG32_FIELD_SET(phyData, 1, RestartAutoNegotiation_OFFSET, RestartAutoNegotiation_MASK);
        if ((ret = RTK_MII_WRITE(unit, port, PHY_PAGE_0, PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
            return ret;
    }

    /* Configure the GPHY page to auto */
    if ((ret = RTK_MII_WRITE(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_eeeEnable_set */

/* Function Name:
 *      phy_8218d_linkDownPowerSavingEnable_set
 * Description:
 *      Set the status of link-down power saving of the specific port in the specific unit
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - status of link-down power saving
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port link-down power saving
 */
int32
phy_8218d_linkDownPowerSavingEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  value;
    int32   ret = RT_ERR_FAILED;

    /* For Link-Down Power Saving (per-port) */
    if ((ret = RTK_MII_READ(unit, port, 0xa43, 24, &value)) != RT_ERR_OK)
        return ret;

    value &= ~(0x1 << 2);
    if (ENABLED == enable)
    {
        value |= (0x1 << 2);
    }
    if ((ret = RTK_MII_WRITE(unit, port, 0xa43, 24, value)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218d_linkDownPowerSavingEnable_set */

/* Function Name:
 *      phy_8218d_greenEnable_set
 * Description:
 *      Set the status of link-up green feature of the specific port in the specific unit
 * Input:
 *      unit   - unit id
 *      port   - port id
 *      enable - status of link-up  green feature
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 * Note:
 *      1. The RTL8218d is supported the per-port link-up green feature.
 */
int32
phy_8218d_greenEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable)
{
    uint32  data, data0, data1, data2, phy_port, base_port;
    int32   ret = RT_ERR_FAILED;


    base_port = RTL8218D_BASEPORT_ID(port);
    phy_port = port - base_port;
    if (phy_port >= PORT_NUM_IN_8218D)
    {
        return RT_ERR_PORT_ID;
    }


    if (ENABLED == enable)
    {
        if ((ret = RTK_MII_WRITE(unit, base_port, 0, 30, 0x8)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, base_port, 0x360, 0x13, &data)) != RT_ERR_OK)
            return ret;
        data = REG32_FIELD_SET(data, 1, PHY_8218D_GREEN_EN_PORT_OFFSET(phy_port), PHY_8218D_GREEN_EN_PORT_MASK(phy_port));
        if ((ret = RTK_MII_WRITE(unit, base_port, 0x360, 0x13, data)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, base_port, 0x360, 0x10, &data0)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, base_port, 0x360, 0x11, &data1)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, base_port, 0x360, 0x12, &data2)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, base_port, 0, 30, 0x0)) != RT_ERR_OK)
            return ret;

        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x805D)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 28, data0)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x8061)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 28, data1)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x804D)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 28, data2)) != RT_ERR_OK)
            return ret;
    }
    else
    {
        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x8065)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, port, 0, 28, &data0)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x8069)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, port, 0, 28, &data1)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x8055)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, port, 0, 28, &data2)) != RT_ERR_OK)
            return ret;

        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x805D)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 28, data0)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x8061)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 28, data1)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 27, 0x804D)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, port, 0, 28, data2)) != RT_ERR_OK)
            return ret;

        if ((ret = RTK_MII_WRITE(unit, base_port, 0, 30, 0x8)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_READ(unit, base_port, 0x360, 0x13, &data)) != RT_ERR_OK)
            return ret;
        data = REG32_FIELD_SET(data, 0, PHY_8218D_GREEN_EN_PORT_OFFSET(phy_port), PHY_8218D_GREEN_EN_PORT_MASK(phy_port));
        if ((ret = RTK_MII_WRITE(unit, base_port, 0x360, 0x13, data)) != RT_ERR_OK)
            return ret;
        if ((ret = RTK_MII_WRITE(unit, base_port, 0, 30, 0x0)) != RT_ERR_OK)
            return ret;
    }
        return RT_ERR_OK;

} /* end of phy_8218d_greenEnable_set */

/* Function Name:
 *      phy_8218d_crossOverMode_set
 * Description:
 *      Set cross over mode in the specified port.
 * Input:
 *      unit - unit id
 *      port - port id
 *      mode - cross over mode
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK
 *      RT_ERR_FAILED
 *      RT_ERR_UNIT_ID - invalid unit id
 *      RT_ERR_PORT_ID - invalid port id
 *      RT_ERR_INPUT   - invalid input parameter
 * Note:
 *      Following value is valid
 *      - PORT_CROSSOVER_MODE_AUTO
 *      - PORT_CROSSOVER_MODE_MDI
 *      - PORT_CROSSOVER_MODE_MDIX
 */
int32
phy_8218d_crossOverMode_set(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t mode)
{
    int32   ret = RT_ERR_FAILED;
    uint32  phyData;


    /* Configure the GPHY page to copper */
    if ((ret = RTK_MII_WRITE(unit, port, 0xa42, 30, 0x0001)) != RT_ERR_OK)
        return ret;

    /* get value from CHIP*/
    if ((ret = RTK_MII_READ(unit, port, 0xa43, 24, &phyData)) != RT_ERR_OK)
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

    if ((ret = RTK_MII_WRITE(unit, port, 0xa43, 24, phyData)) != RT_ERR_OK)
        return ret;

    /* Configure the GPHY page to auto */
    if ((ret = RTK_MII_WRITE(unit, port, 0xa42, 30, 0x0000)) != RT_ERR_OK)
        return ret;

    return RT_ERR_OK;
} /* end of phy_8218_crossOverMode_set */


