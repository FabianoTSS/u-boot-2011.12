/*
 * Copyright(c) Realtek Semiconductor Corporation, 2012
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8208 PHY driver.
 *
 * Feature : RTL8295 PHY driver
 *
 */


/*
 * Include Files
 */
#include <rtk_switch.h>
#include <rtk_osal.h>
#include <rtk_debug.h>
#include <rtk_reg.h>
#include <rtk_phy.h>
#include <rtk/phy/conf/conftypes.h>
#include <rtk/phy/rtl8295.h>

/*
 * Symbol Definition
 */
#define PHY_8295_SDS_MODE_10GR                      0x1A    /* 10GBASE-R */
#define PHY_8295_SDS_MODE_OFF                       0x1F


#define PHY_8295_SDS_PAGE2                          0x2
#define PHY_8295_SDS_PAGE4                          0x4

/*
 * Data Declaration
 */
extern const rtk_mac_drv_t *gMacDrv;
extern const rtk_switch_model_t *gSwitchModel;
/*
 * Macro Declaration
 */
#define HWP_8390_50_FAMILY(unit)        (   (gSwitchModel->chip == RTK_CHIP_RTL8391M) || (gSwitchModel->chip == RTK_CHIP_RTL8392M) || \
                                            (gSwitchModel->chip == RTK_CHIP_RTL8393M) || (gSwitchModel->chip == RTK_CHIP_RTL8396M) || \
                                            (gSwitchModel->chip == RTK_CHIP_RTL8351M) || (gSwitchModel->chip == RTK_CHIP_RTL8352M) || \
                                            (gSwitchModel->chip == RTK_CHIP_RTL8353M))

//#define RTL8295R_DBG
#if defined(RTL8295R_DBG)
  #define PATCH_FLOW_PRINT(fmt, args...)    do {    printf("%s,%d : "fmt"\n", "8295R", __LINE__, ##args);   } while(0)
  #define PATCH_PRINT(fmt, args...)         do {    printf("%s,%d : "fmt"\n", "8295R", __LINE__, ##args);   } while(0)
  #define INFO_LOG(fmt, args...)            do {    printf("%s,%d : "fmt"\n", "8295R", __LINE__, ##args);   } while(0)
#else
  #define PATCH_FLOW_PRINT(fmt, args...)    do { } while(0)
  #define PATCH_PRINT(fmt, args...)         do { } while(0)
  #define INFO_LOG(fmt, args...)            do { } while(0)
#endif /* RTL8295R_DBG */

#define RT_ERR(_rv, _mod, fmt, args...)     do {    printf("(Err%x)%s,%d : "fmt"\n", _rv, "8295R",__LINE__, ##args); } while(0)


#define PHY_REG_WR(_unit, _macId, _phyPage, _phyReg, _data)                                         \
            do {                                                                                    \
                PATCH_FLOW_PRINT("WR-reg unit %u macId %u %u.%u 0x%X\n", _unit, _macId, _phyPage, _phyReg, _data);\
                phy_8295_phyReg_write(_unit, _macId, _phyPage, _phyReg, _data);                     \
            } while(0)

#define PHY_REG_FIELD_WR(_unit, _macId, _phyPage, _phyReg, _mBit, _lBit, _val)                      \
            do {                                                                                    \
                int32       _rv;                                                                    \
                uint32      _data;                                                                  \
                if ((_rv = phy_8295_phyReg_read(_unit, _macId, _phyPage, _phyReg, &_data)) != RT_ERR_OK)\
                {                                                                                   \
                    RT_ERR(_rv, (MOD_HAL), "unit %u macId %u %u.%u", _unit, _macId, _phyPage, _phyReg);\
                } else {                                                                            \
                    _data = REG32_FIELD_SET(_data, _val, _lBit, BITS_MASK(_mBit, _lBit));           \
                    PATCH_FLOW_PRINT("WR-reg unit %u macId %u %u.%u 0x%X\n", _unit, _macId, _phyPage, _phyReg, _data);\
                    phy_8295_phyReg_write(_unit, _macId, _phyPage, _phyReg, _data);                 \
                }                                                                                   \
            } while(0)

#define PHY_REG_RD(_unit, _macId, _phyPage, _phyReg, _pData)                                        \
            do {                                                                                    \
                int32      _rv;                                                                     \
                if ((_rv = phy_8295_phyReg_read(_unit, _macId, _phyPage, _phyReg, _pData)) != RT_ERR_OK) \
                {                                                                                   \
                    RT_ERR(_rv, (MOD_HAL), "unit %u macId %u RD %u.%u", _unit, _macId, _phyPage, _phyReg);\
                    *_pData = 0;                                                                    \
                }                                                                                   \
            } while(0)

#define PHY_REG_FIELD_RD(_unit, _macId, _phyPage, _phyReg, _mBit, _lBit, _pVal)                     \
            do {                                                                                    \
                int32       _rv;                                                                    \
                uint32      _data;                                                                  \
                if ((_rv = phy_8295_phyReg_read(_unit, _macId, _phyPage, _phyReg, &_data)) != RT_ERR_OK)\
                {                                                                                   \
                    RT_ERR(_rv, (MOD_HAL), "unit %u macId %u %u.%u", _unit, _macId, _phyPage, _phyReg);\
                    *_pVal = 0;                                                                     \
                } else {                                                                            \
                    *_pVal = REG32_FIELD_GET(_data, _lBit, BITS_MASK(_mBit, _lBit));                \
                }                                                                                   \
            } while(0)

#define PHY_SDS_WR(_unit, _macId, _sdsId, _pageIdx, _phyReg, _data)                                 \
            do {                                                                                    \
                PATCH_FLOW_PRINT("WR-sds unit %u macId %u S%u %u.%u 0x%X\n", _unit, _macId, _sdsId, (_pageIdx+PHY_8295_PAGE_BASE_OFFSET_S0), _phyReg, _data);\
                phy_8295_sdsReg_write(_unit, _macId, _sdsId, (_pageIdx+PHY_8295_PAGE_BASE_OFFSET_S0), _phyReg, _data);    \
            } while(0)

#define PHY_SDS_FIELD_WR(_unit, _macId, _sdsId, _pageIdx, _phyReg, _mBit, _lBit, _val)              \
            do {                                                                                    \
                int32       _rv;                                                                    \
                uint32      _data, _phyPage = _pageIdx + PHY_8295_PAGE_BASE_OFFSET_S0;              \
                if ((_rv = phy_8295_sdsReg_read(_unit, _macId, _sdsId, _phyPage, _phyReg, &_data)) != RT_ERR_OK)\
                {                                                                                   \
                    RT_ERR(_rv, (MOD_HAL), "unit %u macId %u S%u %u.%u", _unit, _macId, _sdsId, _phyPage, _phyReg);\
                } else {                                                                            \
                    _data = REG32_FIELD_SET(_data, _val, _lBit, BITS_MASK(_mBit, _lBit));           \
                    PATCH_FLOW_PRINT("WR-sds unit %u macId %u S%u %u.%u 0x%X\n", _unit, _macId, _sdsId, _phyPage, _phyReg, _data);\
                    phy_8295_sdsReg_write(_unit, _macId, _sdsId, _phyPage, _phyReg, _data);         \
                }                                                                                   \
            } while(0)

#define PHY_SDS_RD(_unit, _macId, _sdsId, _pageIdx, _phyReg, _pData)                                \
            do {                                                                                    \
                int32   _rv;                                                                        \
                uint32  _phyPage = _pageIdx + PHY_8295_PAGE_BASE_OFFSET_S0;                         \
                if ((_rv = phy_8295_sdsReg_read(_unit, _macId, _sdsId, _phyPage, _phyReg, _pData)) != RT_ERR_OK) \
                {                                                                                   \
                    RT_ERR(_rv, (MOD_HAL), "unit %u macId %u S%u RD %u.%u", _unit, _macId, _sdsId, _phyPage, _phyReg);\
                    *_pData = 0;                                                                    \
                }                                                                                   \
            } while(0)

#define PHY_SDS_FIELD_RD(_unit, _macId, _sdsId, _pageIdx, _phyReg, _mBit, _lBit, _pVal)             \
            do {                                                                                    \
                int32   _rv;                                                                        \
                uint32  _data, _phyPage = _pageIdx + PHY_8295_PAGE_BASE_OFFSET_S0;                  \
                if ((_rv = phy_8295_sdsReg_read(_unit, _macId, _sdsId, _phyPage, _phyReg, &_data)) != RT_ERR_OK) \
                {                                                                                   \
                    RT_ERR(_rv, (MOD_HAL), "unit %u macId %u S%u RD %u.%u", _unit, _macId, _sdsId, _phyPage, _phyReg);\
                    *_pVal = 0;                                                                     \
                } else {                                                                            \
                    *_pVal = REG32_FIELD_GET(_data, _lBit, BITS_MASK(_mBit, _lBit));                \
                }                                                                                   \
            } while(0)


/* sleep for milli-seconds */
#define PATCH_MSLEEP(_t)                                                                            \
            do {                                                                                    \
                udelay((_t) * 1000);                                                                \
            } while(0)


/*
 * Function Declaration
 */


int32
_phy_8295r_serdesModeReg_set(int32 port, uint32 mdxMacId, uint32 sdsId, uint32 mode)
{
    uint32      unit = 0;
    int32       ret;
    uint32      data, phyPage, phyReg, offset, mask;

    if ((ret = rtl8295_serdesModeRegOffsetMask_get(sdsId, &phyPage, &phyReg, &offset, &mask)) != RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }

    phy_8295_phyReg_read(unit, mdxMacId, phyPage, phyReg, &data);

    INFO_LOG("port=%u,mdx=%u S%u mode %X -> %X", port, mdxMacId, sdsId, (data & mask), mode);

    data = REG32_FIELD_SET(data, mode, offset, mask);

    phy_8295_phyReg_write(unit, mdxMacId, phyPage, phyReg, data);

    return RT_ERR_OK;
}


int32
_rtl8295r_phyPortPowerSet(int32 port, uint32 on)
{
    uint32      unit = 0;
    uint32      mdxMacId;
    uint32      sdsId = 1;
    uint32      data, pd_val;

    if (rtl8295_mdxMacId_get(port, &mdxMacId) != RT_ERR_OK)
        return RT_ERR_FAILED;

    if (on)
        pd_val = 0;
    else
        pd_val = 1;

    /* 10G reg */
    phy_8295_sdsReg_read(unit, mdxMacId, sdsId, RTL8295_SDS0_TGR_STD_0_REG00_PAGE, RTL8295_SDS0_TGR_STD_0_REG00_REG, &data);
    data = REG32_FIELD_SET(data, pd_val, PowerDown_OFFSET, PowerDown_MASK);
    phy_8295_sdsReg_write(unit, mdxMacId, sdsId, RTL8295_SDS0_TGR_STD_0_REG00_PAGE, RTL8295_SDS0_TGR_STD_0_REG00_REG, data);

    /* 1G reg */
    /* PHY_CONTROL_REG */
    phy_8295_sdsReg_read(unit, mdxMacId, sdsId, RTL8295_SDS0_FIB_REG00_PAGE, RTL8295_SDS0_FIB_REG00_REG, &data);
    data = REG32_FIELD_SET(data, pd_val, PowerDown_OFFSET, PowerDown_MASK);
    phy_8295_sdsReg_write(unit, mdxMacId, sdsId, RTL8295_SDS0_FIB_REG00_PAGE, RTL8295_SDS0_FIB_REG00_REG, data);

    if (on)
    {
        _phy_8295r_serdesModeReg_set(port, mdxMacId, sdsId, PHY_8295_SDS_MODE_10GR);
    }
    else
    {
        _phy_8295r_serdesModeReg_set(port, mdxMacId, sdsId, PHY_8295_SDS_MODE_OFF);
    }

    return RT_ERR_OK;
}

void
rtl8295r_phyPortPowerOff(int32 port)
{
    _rtl8295r_phyPortPowerSet(port, 0);
    return;
}

void
rtl8295r_phyPortPowerOn(int32 port)
{
    _rtl8295r_phyPortPowerSet(port, 1);
    return;
}


static void
_phy_8295_patch_bT_sds_rst(uint32 unit, uint32 port, uint32 mdxMacId)
{

    PHY_REG_WR(unit, mdxMacId, 1, 21, 0x6);
}

void
_phy_8295_patch_bT_init_sfp(uint32 unit, uint32 port, uint32 mdxMacId)
{
    /* <<< init_sfp >>> */



    PHY_REG_WR(unit, mdxMacId, 4, 20, 0x1000);

    PHY_REG_WR(unit, mdxMacId, 1, 20, 0x3898);

    //#inverse S1 RX P/N for SFP connection
    PHY_SDS_WR(unit, mdxMacId, 1, 0, 16, 0x1603);
    PHY_SDS_WR(unit, mdxMacId, 1, 24, 18, 0x2000);

    PHY_SDS_WR(unit, mdxMacId, 0, 24, 16, 0x0);
    PHY_SDS_WR(unit, mdxMacId, 1, 24, 16, 0x0);
    PHY_SDS_WR(unit, mdxMacId, 0, 125, 18, 0x0130);
    PHY_SDS_WR(unit, mdxMacId, 1, 125, 18, 0x0130);

    PHY_SDS_WR(unit, mdxMacId, 0, 0, 18, 0x30E0);
    PHY_SDS_WR(unit, mdxMacId, 1, 0, 18, 0x30E0);
    PHY_SDS_WR(unit, mdxMacId, 4, 0, 18, 0x30E0);
    PHY_SDS_WR(unit, mdxMacId, 5, 0, 18, 0x30E0);
    PHY_SDS_WR(unit, mdxMacId, 6, 0, 18, 0x30E0);
    PHY_SDS_WR(unit, mdxMacId, 7, 0, 18, 0x30E0);



    PHY_SDS_WR(unit, mdxMacId, 0, 132, 16, 0xE150);
    PHY_SDS_WR(unit, mdxMacId, 1, 132, 16, 0xE150);
    PHY_SDS_WR(unit, mdxMacId, 0, 190, 20, 0x41BC);
    PHY_SDS_WR(unit, mdxMacId, 0, 190, 21, 0x3DBC);
    PHY_SDS_WR(unit, mdxMacId, 1, 190, 20, 0x41BC);
    PHY_SDS_WR(unit, mdxMacId, 1, 190, 21, 0x3DBC);

    PHY_SDS_WR(unit, mdxMacId, 0, 132, 17, 0x9090);


    PHY_SDS_WR(unit, mdxMacId, 0, 132, 21, 0x4A8D);



    PHY_SDS_WR(unit, mdxMacId, 0, 184, 19, 0xF000);
    PHY_SDS_WR(unit, mdxMacId, 0, 185, 23, 0xBFDF);
    PHY_SDS_WR(unit, mdxMacId, 0, 186, 20, 0x1178);
    PHY_SDS_WR(unit, mdxMacId, 0, 187, 22, 0x07FF);
    PHY_SDS_WR(unit, mdxMacId, 0, 188, 18, 0x114F);
    PHY_SDS_WR(unit, mdxMacId, 0, 188, 21, 0x787C);
    PHY_SDS_WR(unit, mdxMacId, 0, 191, 16, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 0, 191, 17, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 0, 191, 19, 0x0000);

    PHY_SDS_WR(unit, mdxMacId, 0, 191, 21, 0x0019);
    PHY_SDS_WR(unit, mdxMacId, 0, 191, 23, 0xF0E0);


    PHY_SDS_WR(unit, mdxMacId, 0, 189, 19, 0x7C0F);

    PHY_SDS_WR(unit, mdxMacId, 1, 132, 17, 0x9090);

    PHY_SDS_WR(unit, mdxMacId, 1, 132, 21, 0x4A8D);


    PHY_SDS_WR(unit, mdxMacId, 1, 184, 19, 0xF000);
    PHY_SDS_WR(unit, mdxMacId, 1, 185, 23, 0xBFDF);
    PHY_SDS_WR(unit, mdxMacId, 1, 186, 20, 0x1178);
    PHY_SDS_WR(unit, mdxMacId, 1, 187, 22, 0x07FF);
    PHY_SDS_WR(unit, mdxMacId, 1, 188, 18, 0x114F);
    PHY_SDS_WR(unit, mdxMacId, 1, 188, 21, 0x787C);
    PHY_SDS_WR(unit, mdxMacId, 1, 191, 16, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 1, 191, 17, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 1, 191, 19, 0x0000);
    PHY_SDS_WR(unit, mdxMacId, 1, 191, 21, 0x0019);
    PHY_SDS_WR(unit, mdxMacId, 1, 191, 23, 0xF0E0);


    PHY_SDS_WR(unit, mdxMacId, 1, 189, 19, 0x7C0F);


    PHY_REG_WR(unit, mdxMacId, 5, 18, 0x1F);

    PHY_REG_WR(unit, mdxMacId, 5, 17, 0x1F);

    PHY_SDS_WR(unit, mdxMacId, 1, 189, 23, 0x0742);

    PHY_SDS_WR(unit, mdxMacId, 1, 190, 16, 0x1700);
    _phy_8295_patch_bT_sds_rst(unit, port, mdxMacId);

    PATCH_PRINT("Initialize %u Chip patch SFP+ and Enable DFE....Done!", mdxMacId);
}

void
_phy_8295_patch_a_init_rtl8295r(uint32 unit, uint32 port, uint32 mdxMacId)
{
    uint32  val;

    PHY_REG_FIELD_RD(unit, mdxMacId, 0, 19, 8, 4, &val);
    if (val == 0)
    {
        PHY_SDS_FIELD_WR(unit, mdxMacId, 0, 25, 21, 11, 8, 0xF);
        PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 25, 21, 11, 8, 0xF);
    }

    PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 26, 17, 13, 12, 3);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 125, 20, 2, 2, 1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 125, 21, 5, 5, 1);

    PHY_SDS_FIELD_WR(unit, mdxMacId, 0, 27, 17, 6, 6, 0);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 27, 17, 6, 6, 0);

    PHY_REG_WR(unit, mdxMacId, 1, 22, 0x5800);

    PHY_SDS_WR(unit, mdxMacId, 0, 132, 16, 0xE350);

    PHY_SDS_WR(unit, mdxMacId, 0, 132, 21, 0x420D);

    PHY_SDS_WR(unit, mdxMacId, 0, 186, 18, 0x2080);
    PHY_SDS_WR(unit, mdxMacId, 0, 186, 20, 0x1378);
    PHY_SDS_WR(unit, mdxMacId, 0, 189, 19, 0x7C07);
    PHY_SDS_WR(unit, mdxMacId, 0, 190, 20, 0x41B8);
    PHY_SDS_WR(unit, mdxMacId, 0, 190, 21, 0x3DB8);

    if (val == 0)
    {
        PHY_SDS_WR(unit, mdxMacId, 0, 188, 17, 0x91CC);
        PHY_SDS_WR(unit, mdxMacId, 0, 188, 18, 0x116F);
    }

    PHY_SDS_WR(unit, mdxMacId, 0, 187, 22, 0x07FA);

    PHY_SDS_WR(unit, mdxMacId, 0, 168, 19, 0xF000);
    PHY_SDS_WR(unit, mdxMacId, 0, 169, 23, 0xBFDF);
    PHY_SDS_WR(unit, mdxMacId, 0, 170, 18, 0x2100);
    PHY_SDS_WR(unit, mdxMacId, 0, 170, 20, 0x1378);
    PHY_SDS_WR(unit, mdxMacId, 0, 171, 22, 0x07FA);
    PHY_SDS_WR(unit, mdxMacId, 0, 172, 18, 0x114F);
    PHY_SDS_WR(unit, mdxMacId, 0, 172, 21, 0x787C);
    PHY_SDS_WR(unit, mdxMacId, 0, 175, 16, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 0, 175, 17, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 0, 175, 21, 0x0019);
    PHY_SDS_WR(unit, mdxMacId, 0, 175, 22, 0xF0E0);

    PHY_SDS_WR(unit, mdxMacId, 1, 132, 16, 0xE350);

    PHY_SDS_WR(unit, mdxMacId, 1, 132, 21, 0x420D);

    PHY_SDS_WR(unit, mdxMacId, 1, 186, 18, 0x2080);
    PHY_SDS_WR(unit, mdxMacId, 1, 186, 20, 0x1378);
    PHY_SDS_WR(unit, mdxMacId, 1, 189, 19, 0x7C07);
    PHY_SDS_WR(unit, mdxMacId, 1, 190, 20, 0x41B8);
    PHY_SDS_WR(unit, mdxMacId, 1, 190, 21, 0x3DB8);

    if (val == 0)
    {
        PHY_SDS_WR(unit, mdxMacId, 1, 188, 17, 0x91CC);
        PHY_SDS_WR(unit, mdxMacId, 1, 188, 18, 0x116F);
    }

    PHY_SDS_WR(unit, mdxMacId, 1, 187, 22, 0x07FA);

    PHY_SDS_WR(unit, mdxMacId, 1, 168, 19, 0xF000);
    PHY_SDS_WR(unit, mdxMacId, 1, 169, 23, 0xBFDF);
    PHY_SDS_WR(unit, mdxMacId, 1, 170, 18, 0x2100);
    PHY_SDS_WR(unit, mdxMacId, 1, 170, 20, 0x1378);
    PHY_SDS_WR(unit, mdxMacId, 1, 171, 22, 0x07FA);
    PHY_SDS_WR(unit, mdxMacId, 1, 172, 18, 0x114F);
    PHY_SDS_WR(unit, mdxMacId, 1, 172, 21, 0x787C);
    PHY_SDS_WR(unit, mdxMacId, 1, 175, 16, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 1, 175, 17, 0x8107);
    PHY_SDS_WR(unit, mdxMacId, 1, 175, 21, 0x0019);
    PHY_SDS_WR(unit, mdxMacId, 1, 175, 22, 0xF0E0);

    /* SDK: add inverse S1 RX polarity for specific model */
    if (HWP_8390_50_FAMILY(unit))
    {
        INFO_LOG("8396M inverse S1 RX polarity");
        /* inverse S1 RX polarity */
        PHY_SDS_WR(unit, mdxMacId, 1, 0, 16, 0x1603);
        PHY_SDS_WR(unit, mdxMacId, 1, 24, 18, 0x2000);
    }

}

void
_phy_8295_patch_8295r_config(uint32 unit, uint32 port, uint32 mdxMacId)
{
    /* disable FCTRL/EEE */
    PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 125, 19, 6, 5, 0x0);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 125, 19, 9, 9, 0x0);

    /* enable 1G auto nego */
    PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 8, 16, 12, 12, 0x1);
}

/* Function Name:
 *      rtl8295r_config
 * Description:
 *      configure 8295R
 * Input:
 *      basePort      base mac id of the PHY chip
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void rtl8295r_config(int32 basePort)
{
    uint32      unit = 0;
    uint32      mdxMacId;

    OSAL_PRINTF("### RTL8295R config - MAC ID = %d ###\n", basePort);
    rtl8295_mdxMacId_get(basePort, &mdxMacId);
    if (mdxMacId < 0)
    {
        INFO_LOG("port=%u err mdxMacId_get", basePort);
        return;
    }
    if (0 == phy_8295_get_model(basePort))    //RTK_PHYTYPE_RTL8295R_C22
    {
        _phy_8295_patch_bT_init_sfp(unit, basePort, mdxMacId);
    }
    else
    {
        _phy_8295_patch_a_init_rtl8295r(unit, basePort, mdxMacId);
    }

    _phy_8295_patch_8295r_config(unit, basePort, mdxMacId);
}


