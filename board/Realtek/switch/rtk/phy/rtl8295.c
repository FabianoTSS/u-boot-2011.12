/*
 * Copyright(c) Realtek Semiconductor Corporation, 2012
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8208 PHY driver.
 *
 * Feature : Common API for RTL8295R and 8214QF
 *
 */


/*
 * Include Files
 */
#include <rtk_switch.h>
#include <rtk_osal.h>
#include <rtk_debug.h>
#include <rtk_reg.h>
#include <rtk/phy/conf/conftypes.h>
#include <rtk/phy/rtl8295.h>


/*
 * Symbol Definition
 */

/*
 * Data Declaration
 */
extern const rtk_switch_model_t *gSwitchModel;
extern const rtk_mac_drv_t *gMacDrv;


/*
 * Symbol Definition
 */
#define HAL_RTL8295_SDS_BCST                8
#define HAL_RTL8295_MAX_SDS_IDX             HAL_RTL8295_SDS_BCST

/*
 * Data Declaration
 */
/* <<< SerDes page offset >>> */
static uint32  sds_reg_page_offset[HAL_RTL8295_MAX_SDS_IDX+1] =
                    {256,   //SDS_S0 for Serdes S0
                     768,   //SDS_S2 for Serdes S1
                     512,   //SDS_S1
                     2304,  //SDS_S3
                     1024,  //SDS_S4 for Serdes S4
                     1280,  //SDS_S5 for Serdes S5
                     1536,  //SDS_S6 for Serdes S6
                     1792,  //SDS_S7 for Serdes S7
                     2048   //BCAST
                    };
#define PHY_8295_SDS_REG_PAGE_OFFSET_IDX_MAX            (sizeof(sds_reg_page_offset)/sizeof(uint32))

/*
 * Macro Declaration
 */
#define RTL8295_REG_ADDR_TO_PAGE(_addr)         ((_addr / 4) / 8)
#define RTL8295_REG_ADDR_TO_IDX(_addr)          (((_addr / 4) % 8) + 16)


/*
 * Function Declaration
 */

int32
rtl8295_port_base_macId_get(uint32 port)
{
    int32               i;
    for (i = 0; i < gSwitchModel->port.count; i++)
    {
        if (gSwitchModel->port.list[i].mac_id == port)
        {
            return gSwitchModel->phy.list[gSwitchModel->port.list[i].phy_idx].mac_id;
        }
    }
    return -1;
}


static int32
_rtl8295_miim_read(
    int32       mdxMacId,
    uint32      page,
    uint32      phy_reg,
    uint32      *pData)
{
    return gMacDrv->drv_miim_read(mdxMacId, page, phy_reg, pData);
}


static int32
_rtl8295_miim_write(
        int32       mdxMacId,
        uint32      page,
        uint32      phy_reg,
        uint32      data)
{
    return gMacDrv->drv_miim_write(mdxMacId, page, phy_reg, data);
}


/* Function Name:
 *      rtl8295_reg_read
 * Description:
 *      Read from register
 * Input:
 *      mdxMacId    - MAC ID for MDC/MDIO
 *      reg_addr    - register address
 * Output:
 *      pData       - register data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
rtl8295_reg_read(int32 mdxMacId, uint32 reg_addr, uint32 *pData)
{
    uint32      page;
    uint32      reg;
    int32       ret;

    page = RTL8295_REG_ADDR_TO_PAGE(reg_addr);
    reg = RTL8295_REG_ADDR_TO_IDX(reg_addr);

    if ((ret = _rtl8295_miim_read(mdxMacId, page, reg, pData)) != RT_ERR_OK)
    {
        return ret;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      rtl8295_reg_write
 * Description:
 *      Write to register
 * Input:
 *      mdxMacId    - MAC ID for MDC/MDIO
 *      reg_addr    - register address
 *      data        - data to write to register
 * Output:
 *      None
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
rtl8295_reg_write(int32 mdxMacId, uint32 reg_addr, uint32 data)
{
    uint32      page;
    uint32      reg;
    int32       ret;

    page = RTL8295_REG_ADDR_TO_PAGE(reg_addr);
    reg = RTL8295_REG_ADDR_TO_IDX(reg_addr);

    if ((ret = _rtl8295_miim_write(mdxMacId, page, reg, data)) != RT_ERR_OK)
    {
        return ret;
    }

    return RT_ERR_OK;
}


static int32
_sdsPageReg_to_chipPageReg(uint32 sdsId, uint32 page, uint32 reg, uint32 *pScPage, uint32 *pScReg)
{
    uint32      tmp;

    tmp = (page << 5) + reg ;
    *pScPage = (tmp / 8) + sds_reg_page_offset[sdsId];
    *pScReg  = (tmp % 8) + 16;

    return RT_ERR_OK;
}


/* Function Name:
 *      rtl8295_sds_read
 * Description:
 *      Read from SerDes register
 * Input:
 *      mdxMacId    - MAC ID for MDC/MDIO
 *      sdsId       - SerDes Id
 *      page        - page
 *      reg         - register index
 * Output:
 *      pData       - register data
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
rtl8295_sds_read(int32 mdxMacId, uint32 sdsId, uint32 page, uint32 reg, uint32 *pData)
{
    uint32      sc_page, sc_reg;
    int32       ret;

    if (sdsId >= HAL_RTL8295_MAX_SDS_IDX)
    {
        return RT_ERR_FAILED;
    }

    _sdsPageReg_to_chipPageReg(sdsId, page, reg, &sc_page, &sc_reg);

    if ((ret = _rtl8295_miim_read(mdxMacId, sc_page, sc_reg, pData)) != RT_ERR_OK)
    {
        return ret;
    }

    return RT_ERR_OK;
}


/* Function Name:
 *      rtl8295_sds_write
 * Description:
 *      Write to SerDes register
 * Input:
 *      mdxMacId    - MAC ID for MDC/MDIO
 *      sdsId       - SerDes Id
 *      page        - page
 *      reg         - register index
 *      data        - data to write to register
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
rtl8295_sds_write(int32 mdxMacId, uint32 sdsId, uint32 page, uint32 reg, uint32 data)
{
    uint32      sc_page, sc_reg;
    int32       ret;

    if (sdsId >= HAL_RTL8295_MAX_SDS_IDX)
    {
        return RT_ERR_FAILED;
    }

    _sdsPageReg_to_chipPageReg(sdsId, page, reg, &sc_page, &sc_reg);

    if ((ret = _rtl8295_miim_write(mdxMacId, sc_page, sc_reg, data)) != RT_ERR_OK)
    {
        return ret;
    }

    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8295_sdsField_write
 * Description:
 *      Write SerDes register field
 * Input:
 *      mdxMacId    - MAC ID for MDC/MDIO
 *      sdsId       - SerDes Id
 *      page        - page
 *      reg         - register index
 *      offset      - offset of the field
 *      mask        - mask of the field
 *      val        - data to write to register's field
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
phy_8295_sdsField_write(uint32 mdxMacId, uint32 sdsId, uint32 page, uint32 reg, uint32 offset, uint32 mask, uint32 val)
{
    int32           ret;
    uint32          data;

    if ((ret = rtl8295_sds_read(mdxMacId, sdsId, page, reg, &data)) != RT_ERR_OK)
    {
        return ret;
    }

    data = REG32_FIELD_SET(data, val, offset, mask);
    return rtl8295_sds_write(mdxMacId, sdsId, page, reg, data);
}




/* Function Name:
 *      rtl8295_mdxMacId_get
 * Description:
 *      Get MAC ID for accessing 8295R or 8214QF
 * Input:
 *      mdxMacId    - MAC ID for MDC/MDIO
 *      sdsId       - SerDes Id
 *      page        - page
 *      reg         - register index
 *      data        - data to write to register
 * Output:
 *      None.
 * Return:
 *      RT_ERR_OK     - OK
 *      RT_ERR_FAILED - Failed
 * Note:
 *      None
 */
int32
rtl8295_mdxMacId_get(int32 port, uint32 *pMdxMacId)
{
    if (gSwitchModel->chip == RTK_CHIP_RTL8396M)
    {
        if (port == 24)
        {
            *pMdxMacId = 25;
            return RT_ERR_OK;
        }
        else if (port == 36)
        {
            *pMdxMacId = 26;
            return RT_ERR_OK;
        }
    }

    *pMdxMacId = rtl8295_port_base_macId_get(port);

    if (*pMdxMacId < 0)
    {
        return RT_ERR_FAILED;
    }
    else
    {
        return RT_ERR_OK;
    }
}


/* Function Name:
 *      phy_8295_get_model
 * Description:
 *      Get model type
 * Input:
 *      unit        - unit id
 *      port        - port id
 *
 * Output:
 *      None.
 * Return:
 *      1: formal
 * Note:
 *      None
 */
int32
phy_8295_get_model(int32 port)
{
    uint32      mdxMacId;
    uint32      data;

    if (rtl8295_mdxMacId_get(port, &mdxMacId) == RT_ERR_FAILED)
        mdxMacId = port;

    gMacDrv->drv_miim_read(mdxMacId, 0, 3, &data);
    if (data == 0x0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/* Function Name:
 *      phy_8295_phyReg_read
 * Description:
 *      Read 8295 register using PHY page, reg format.
 *      When the MDC/MDIO is using Clause 45 format, this API will transfer the register to MMD.
 * Input:
 *      unit - unit ID
 *      mdxMacId - mac ID for access the chip
 *      phy_page - PHY page number
 *      phy_reg - PHY reg number
 * Output:
 *      pData - data value of the register
 * Return:
 *      None
 * Note:
 *      This API only valid for access registers that defined in register file.
 */
int32
phy_8295_phyReg_read(uint32 unit, uint32 mdxMacId, uint32 phy_page, uint32 phy_reg, uint32 *pData)
{
    if (unit); /* to avoid make warning */

    gMacDrv->drv_miim_read(mdxMacId, phy_page, phy_reg, pData);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8295_phyReg_write
 * Description:
 *      Write 8295 register using PHY page, reg format.
 *      When the MDC/MDIO is using Clause 45 format, this API will transfer the register to MMD.
 * Input:
 *      unit - unit ID
 *      mdxMacId - mac ID for access the chip
 *      phy_page - PHY page number
 *      phy_reg - PHY reg number
 *      data - data value set to the register
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      This API only valid for access registers that defined in register file.
 */
int32
phy_8295_phyReg_write(uint32 unit, uint32 mdxMacId, uint32 phy_page, uint32 phy_reg, uint32 data)
{
    if (unit); /* to avoid make warning */

    gMacDrv->drv_miim_write(mdxMacId, phy_page, phy_reg, data);
    return RT_ERR_OK;
}

/* Function Name:
 *      phy_8295_sdsReg_read
 * Description:
 *      Write 8295 serdes register
 *      When the MDC/MDIO is using Clause 45 format, this API will transfer the register to MMD.
 * Input:
 *      unit - unit ID
 *      mdxMacId - mac ID for access the chip
 *      s0_phy_page - using S0 phy page for indicating the what register to access.
 *              the function will use sdsId to find the real phy page of the serdes's register.
 *      phy_reg - PHY reg number
 * Output:
 *      pData - data value of the register
 * Return:
 *      None
 * Note:
 *      This API only valid for access registers that defined in register file.
 */
int32
phy_8295_sdsReg_read(uint32 unit, uint32 mdxMacId, uint32 sdsId, uint32 s0_phy_page, uint32 phy_reg, uint32 *pData)
{
    uint32  page_idx;

    if (sdsId >= PHY_8295_SDS_REG_PAGE_OFFSET_IDX_MAX)
    {
        return RT_ERR_FAILED;
    }

    if ((page_idx = s0_phy_page - PHY_8295_PAGE_BASE_OFFSET_S0) >= PHY_8295_SDS_PAGE_NUM_MAX)
    {
        return RT_ERR_FAILED;
    }

    return phy_8295_phyReg_read(unit, mdxMacId, (sds_reg_page_offset[sdsId] + page_idx), phy_reg, pData);
}

/* Function Name:
 *      phy_8295_sdsReg_write
 * Description:
 *      Write 8295 serdes register
 *      When the MDC/MDIO is using Clause 45 format, this API will transfer the register to MMD.
 * Input:
 *      unit - unit ID
 *      mdxMacId - mac ID for access the chip
 *      s0_phy_page - using S0 phy page for indicating the what register to access.
 *              the function will use sdsId to find the real phy page of the serdes's register.
 *      phy_reg - PHY reg number
 *      data - data value set to the register
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      This API only valid for access registers that defined in register file.
 */
int32
phy_8295_sdsReg_write(uint32 unit, uint32 mdxMacId, uint32 sdsId, uint32 s0_phy_page, uint32 phy_reg, uint32 data)
{
    uint32  page_idx;

    if (sdsId >= PHY_8295_SDS_REG_PAGE_OFFSET_IDX_MAX)
    {
        return RT_ERR_FAILED;
    }

    if ((page_idx = s0_phy_page - PHY_8295_PAGE_BASE_OFFSET_S0) >= PHY_8295_SDS_PAGE_NUM_MAX)
    {
        return RT_ERR_FAILED;
    }

    return phy_8295_phyReg_write(unit, mdxMacId, (sds_reg_page_offset[sdsId] + page_idx), phy_reg, data);
}


int32
rtl8295_serdesModeRegOffsetMask_get(uint32 sdsId, uint32 *pPhyPage, uint32 *pPhyReg, uint32 *pOffset, uint32 *pMask)
{
    switch (sdsId)
    {
      case 0:
        *pPhyPage = RTL8295_SDS_CTRL_SDS_CTRL_S0_PAGE;
        *pPhyReg  = RTL8295_SDS_CTRL_SDS_CTRL_S0_REG;
        *pOffset = RTL8295_SDS_CTRL_SDS_CTRL_S0_SERDES_MODE_S0_OFFSET;
        *pMask   = RTL8295_SDS_CTRL_SDS_CTRL_S0_SERDES_MODE_S0_MASK;
        break;
      case 1:
        *pPhyPage = RTL8295_SDS_CTRL_SDS_CTRL_S1_PAGE;
        *pPhyReg  = RTL8295_SDS_CTRL_SDS_CTRL_S1_REG;
        *pOffset = RTL8295_SDS_CTRL_SDS_CTRL_S1_SERDES_MODE_S1_OFFSET;
        *pMask   = RTL8295_SDS_CTRL_SDS_CTRL_S1_SERDES_MODE_S1_MASK;
        break;
      case 4:
        *pPhyPage = RTL8295_SDS_CTRL_SDS_CTRL_S4_PAGE;
        *pPhyReg  = RTL8295_SDS_CTRL_SDS_CTRL_S4_REG;
        *pOffset = RTL8295_SDS_CTRL_SDS_CTRL_S4_SERDES_MODE_S4_OFFSET;
        *pMask   = RTL8295_SDS_CTRL_SDS_CTRL_S4_SERDES_MODE_S4_MASK;
        break;
      case 5:
        *pPhyPage = RTL8295_SDS_CTRL_SDS_CTRL_S5_PAGE;
        *pPhyReg  = RTL8295_SDS_CTRL_SDS_CTRL_S5_REG;
        *pOffset = RTL8295_SDS_CTRL_SDS_CTRL_S5_SERDES_MODE_S5_OFFSET;
        *pMask   = RTL8295_SDS_CTRL_SDS_CTRL_S5_SERDES_MODE_S5_MASK;
        break;
      case 6:
        *pPhyPage = RTL8295_SDS_CTRL_SDS_CTRL_S6_PAGE;
        *pPhyReg  = RTL8295_SDS_CTRL_SDS_CTRL_S6_REG;
        *pOffset = RTL8295_SDS_CTRL_SDS_CTRL_S6_SERDES_MODE_S6_OFFSET;
        *pMask   = RTL8295_SDS_CTRL_SDS_CTRL_S6_SERDES_MODE_S6_MASK;
        break;
      case 7:
        *pPhyPage = RTL8295_SDS_CTRL_SDS_CTRL_S7_PAGE;
        *pPhyReg  = RTL8295_SDS_CTRL_SDS_CTRL_S7_REG;
        *pOffset = RTL8295_SDS_CTRL_SDS_CTRL_S7_SERDES_MODE_S7_OFFSET;
        *pMask   = RTL8295_SDS_CTRL_SDS_CTRL_S7_SERDES_MODE_S7_MASK;
        break;
      default:
        return RT_ERR_PORT_ID;
    }/* end switch */
    return RT_ERR_OK;
}

