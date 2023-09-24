/*
 * Copyright(c) Realtek Semiconductor Corporation, 2020
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8218D PHY driver.
 *
 * Feature : RTL8218D PHY driver
 *
 */

#ifndef	__RTL8218D_H__
#define	__RTL8218D_H__

/*
 * Include Files
 */

/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */

/* PHY Transformer RJ45 Impedance */
typedef enum rtk_phy_xfmrRj45Impd_e
{
    RTK_PHY_XFMRRJ45IMPD_75_OHM = 0,
    RTK_PHY_XFMRRJ45IMPD_95_OHM,
    RTK_PHY_XFMRRJ45IMPD_END,
} rtk_phy_xfmrRj45Impd_t;

typedef enum rtk_port_phyTestModeChannel_s
{
    PHY_TESTMODE_CHANNEL_NONE          = 0,
    PHY_TESTMODE_CHANNEL_A,
    PHY_TESTMODE_CHANNEL_B,
    PHY_TESTMODE_CHANNEL_C,
    PHY_TESTMODE_CHANNEL_D,
    PHY_TESTMODE_CHANNEL_END
} rtk_port_phyTestModeChannel_t;

typedef struct rtk_port_phyTestMode_s
{
    int32                           mode;
    rtk_port_phyTestModeChannel_t   channel;
    uint32                          flags;
} rtk_port_phyTestMode_t;

/*
 * Macro Definition
 */
#define MY_UNIT                                                 0
#define HAL_MIIM_FIX_PAGE                                       gMacDrv->miim_max_page
#define PORT_NUM_IN_8218D                                       (0x8)
#define RTK_PORT_PHYTESTMODE_FLAG_ALL_PHY_PORTS                 0x00000001

#define RTK_MII_READ(unit,port,p,r,pVal)                        gMacDrv->drv_miim_read(port,p,r,pVal)
#define RTK_MII_WRITE(unit,port,p,r,val)                        gMacDrv->drv_miim_write(port,p,r,val)

#define PHY_CONSTRUCT_RV_WRITE(unit, port, crv)                 phy_construct_confcodeRv_write(unit, port, crv, (sizeof(crv)/sizeof(confcode_rv_t)))
#define PHY_CONSTRUCT_PRV_WRITE(unit, phy_base_macId, cprv)     phy_construct_confcodePrv_write(unit, phy_base_macId, cprv, (sizeof(cprv)/sizeof(confcode_prv_t)))

#define RTL8218D_BASEPORT_ID(port)                             (port - (port % PORT_NUM_IN_8218D))

#define PHY_8218D_GREEN_EN_OFFSET                               0
#define PHY_8218D_GREEN_EN_MASK                                 (0xFF << PHY_8218D_GREEN_EN_OFFSET)
#define PHY_8218D_GREEN_EN_PORT_OFFSET(phyPort)                 ((phyPort & 0x7) + PHY_8218D_GREEN_EN_OFFSET)
#define PHY_8218D_GREEN_EN_PORT_MASK(phyPort)                   (0x1 << PHY_8218D_GREEN_EN_PORT_OFFSET(phyPort))

extern const rtk_switch_model_t *gSwitchModel;
extern const rtk_mac_drv_t *gMacDrv;

/*
 * Function Declaration
 */
/* Function Name:
 *      rtl8218d_config
 * Description:
 *      Configuration code for RTL8218D.
 * Input:
 *      macId - the macId of PHY0 of the RTL8218D
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
extern void rtl8218d_config(uint32 unit, uint8 phy_base_macId);

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
extern int32
phy_8218d_ieeeTestMode_set(uint32 unit, rtk_port_t port, rtk_port_phyTestMode_t *pTestMode);


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
extern int32
phy_8218d_eeeEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

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
extern int32
phy_8218d_linkDownPowerSavingEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

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
extern int32
phy_8218d_greenEnable_set(uint32 unit, rtk_port_t port, rtk_enable_t enable);

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
extern int32
phy_8218d_crossOverMode_set(uint32 unit, rtk_port_t port, rtk_port_crossOver_mode_t mode);


extern void phy_8218d_bConf_init(uint32 unit, rtk_port_t basePort);
#endif	/* __RTL8218D_H__ */

