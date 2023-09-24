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
#define PHY_8295_SDS_MODE_FIB1G                     0x4     /* 1000BASE-X/FIB1G */
#define PHY_8295_SDS_MODE_OFF                       0x1F

/*
 * Data Declaration
 */
extern const rtk_mac_drv_t *gMacDrv;
extern const rtk_switch_model_t *gSwitchModel;


/*
 * Macro Declaration
 */
#define HWP_8380_30_FAMILY(unit)        (   (gSwitchModel->chip == RTK_CHIP_RTL8332M) || (gSwitchModel->chip == RTK_CHIP_RTL8382M) || \
                                            (gSwitchModel->chip == RTK_CHIP_RTL8330M) || (gSwitchModel->chip == RTK_CHIP_RTL8380M) || \
                                            (gSwitchModel->chip == RTK_CHIP_RTL8381M) )

//#define RTL8214QF_DBG
#if defined(RTL8214QF_DBG)
  #define PATCH_FLOW_PRINT(fmt, args...)    do {    printf("%s,%d : "fmt"\n", "8214QF", __LINE__, ##args);   } while(0)
  #define PATCH_PRINT(fmt, args...)         do {    printf("%s,%d : "fmt"\n", "8214QF", __LINE__, ##args);   } while(0)
  #define INFO_LOG(fmt, args...)            do {    printf("%s,%d : "fmt"\n", "8214QF", __LINE__, ##args);   } while(0)
#else
  #define PATCH_FLOW_PRINT(fmt, args...)    do { } while(0)
  #define PATCH_PRINT(fmt, args...)         do { } while(0)
  #define INFO_LOG(fmt, args...)            do { } while(0)
#endif /* RTL8214QF_DBG */


#define RT_ERR(_rv, _mod, fmt, args...)     do {    printf("(Err%x)%s,%d : "fmt"\n", _rv, "8214QF",__LINE__, ##args); } while(0)


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
phy_8214qf_sdsId_get(int32 port, uint32 mdxMacId, uint32 *pSdsId)
{

    *pSdsId = port - mdxMacId + 4;

    if ((*pSdsId < 4) || (*pSdsId > 7))
        return RT_ERR_FAILED;

    return RT_ERR_OK;
}

int32
_phy_8214qf_serdesModeReg_set(int32 port, uint32 mdxMacId, uint32 sdsId, uint32 mode)
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
rtl8214qf_phyPortPowerSet(int32 port, uint32 on)
{
    int32   ret;
    uint32  phyData;
    uint32  mdxMacId;
    uint32  sdsId;

    if (rtl8295_mdxMacId_get(port, &mdxMacId) != RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }

    if (phy_8214qf_sdsId_get(port, mdxMacId, &sdsId) != RT_ERR_OK)
    {
        return RT_ERR_FAILED;
    }

    if (on == 0)
    {
        _phy_8214qf_serdesModeReg_set(port, mdxMacId, sdsId, PHY_8295_SDS_MODE_OFF);
    }

    if ((ret = gMacDrv->drv_miim_read(port, PHY_PAGE_0, PHY_CONTROL_REG, &phyData)) != RT_ERR_OK)
    {
        return ret;
    }

    if (on) /* power on */
    {
        phyData &= ~(PowerDown_MASK);
    }
    else    /* power off */
    {
        phyData &= ~(PowerDown_MASK);
        phyData |= (1 << PowerDown_OFFSET);
    }

    if ((ret = gMacDrv->drv_miim_write(port, PHY_PAGE_0, PHY_CONTROL_REG, phyData)) != RT_ERR_OK)
    {
        return ret;
    }

    if (on)
    {
        _phy_8214qf_serdesModeReg_set(port, mdxMacId, sdsId, PHY_8295_SDS_MODE_FIB1G);
    }

    return RT_ERR_OK;
}

void
rtl8214qf_phyPortPowerOff(int32 port)
{
    rtl8214qf_phyPortPowerSet(port, 0);
    return;
}


void
rtl8214qf_phyPortPowerOn(int32 port)
{
    rtl8214qf_phyPortPowerSet(port, 1);
    return;
}


int32
_rtl8214qf_serdesListIdx_get(uint32 basePort)
{
    int sdsIdx, phyIdx;

    for (sdsIdx = 0; sdsIdx < gSwitchModel->serdes.count; sdsIdx++)
    {
        if (gSwitchModel->serdes.list[sdsIdx].mii == RTK_MII_NONE) /* workaround disabled serdes */
            continue;
        phyIdx = gSwitchModel->serdes.list[sdsIdx].phy_idx;
        if (gSwitchModel->phy.list[phyIdx].mac_id == basePort)
        {
            return sdsIdx;
        }
    }

    return -1;
}


static void
_phy_8295_patch_bT_sds_rst(uint32 unit, uint32 port, uint32 mdxMacId)
{

    PHY_REG_WR(unit, mdxMacId, 1, 21, 0x6);
}


void
_phy_8295_patch_bT_init_chip(uint32 unit, uint32 port, uint32 mdxMacId)
{
    /* <<< init_chip >>> */


    PHY_REG_WR(unit, mdxMacId, 1, 20, 0x3898);
    PHY_SDS_WR(unit, mdxMacId, 0, 24, 16, 0x0);
    PHY_SDS_WR(unit, mdxMacId, 1, 24, 16, 0x0);
    PHY_SDS_WR(unit, mdxMacId, 0, 125, 18, 0x0130);
    PHY_SDS_WR(unit, mdxMacId, 1, 125, 18, 0x0130);


    PHY_REG_WR(unit, mdxMacId, 28, 19, 0x8000);
    PHY_REG_WR(unit, mdxMacId, 33, 23, 0x8000);
    PHY_REG_WR(unit, mdxMacId, 44, 23, 0x8000);


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



    PHY_SDS_WR(unit, mdxMacId, 0, 175, 19, 0x0000);
    PHY_SDS_WR(unit, mdxMacId, 0, 175, 21, 0x0011);
    PHY_SDS_WR(unit, mdxMacId, 0, 175, 23, 0x0F0F);

    PHY_SDS_WR(unit, mdxMacId, 4, 132, 17, 0xF807);
    PHY_SDS_WR(unit, mdxMacId, 4, 144, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 4, 146, 16, 0xF807);
    PHY_SDS_WR(unit, mdxMacId, 4, 160, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 4, 162, 16, 0xFA07);

    PHY_SDS_WR(unit, mdxMacId, 5, 144, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 5, 146, 16, 0xF807);
    PHY_SDS_WR(unit, mdxMacId, 5, 160, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 5, 162, 16, 0xFA07);

    PHY_SDS_WR(unit, mdxMacId, 6, 132, 17, 0xFA07);
    PHY_SDS_WR(unit, mdxMacId, 6, 144, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 6, 146, 16, 0xF807);
    PHY_SDS_WR(unit, mdxMacId, 6, 160, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 6, 162, 16, 0xFA07);

    PHY_SDS_WR(unit, mdxMacId, 7, 144, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 7, 146, 16, 0xF807);
    PHY_SDS_WR(unit, mdxMacId, 7, 160, 21, 0x981B);
    PHY_SDS_WR(unit, mdxMacId, 7, 162, 16, 0xFA07);

    PHY_SDS_FIELD_WR(unit, mdxMacId, 4, 144, 19, 6, 2, 0x18);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 5, 144, 19, 6, 2, 0x18);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 6, 144, 19, 6, 2, 0x18);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 7, 144, 19, 6, 2, 0x18);

    PHY_SDS_FIELD_WR(unit, mdxMacId, 4, 145, 20, 15, 12, 0xE);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 5, 145, 20, 15, 12, 0xE);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 6, 145, 20, 15, 12, 0xE);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 7, 145, 20, 15, 12, 0xE);

    /* set SP_CFG_EN_LINK_FIB1G for enable fiberNwayForceLink */
    PHY_SDS_FIELD_WR(unit, mdxMacId, 4, 0, 20, 2, 2, 1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 5, 0, 20, 2, 2, 1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 6, 0, 20, 2, 2, 1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 7, 0, 20, 2, 2, 1);

    _phy_8295_patch_bT_sds_rst(unit, port, mdxMacId);

    PATCH_PRINT("Initialize Chip patch....Done!");
}


void
_phy_8295_patch_a_init_rtl8214qf(uint32 unit, uint32 port, uint32 mdxMacId)
{
    /* init_rtl8214qf */
    uint32      val;

    PHY_REG_FIELD_RD(unit, mdxMacId, 0, 19, 8, 4, &val);
    if (val == 0)
    {
        PHY_SDS_FIELD_WR(unit, mdxMacId, 0, 25, 21, 11, 8, 0xF);
        PHY_SDS_FIELD_WR(unit, mdxMacId, 1, 25, 21, 11, 8, 0xF);
    }

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

    PHY_SDS_FIELD_WR(unit, mdxMacId, 4, 145, 20, 15, 12, 0xe);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 5, 145, 20, 15, 12, 0xe);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 6, 145, 20, 15, 12, 0xe);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 7, 145, 20, 15, 12, 0xe);

    PHY_SDS_FIELD_WR(unit, mdxMacId, 4, 144, 19, 6, 2, 0x16);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 5, 144, 19, 6, 2, 0x16);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 6, 144, 19, 6, 2, 0x16);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 7, 144, 19, 6, 2, 0x16);

    /* set SP_CFG_EN_LINK_FIB1G for enable fiberNwayForceLink */
    PHY_SDS_FIELD_WR(unit, mdxMacId, 4, 0, 20, 2, 2, 1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 5, 0, 20, 2, 2, 1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 6, 0, 20, 2, 2, 1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 7, 0, 20, 2, 2, 1);

    PHY_SDS_WR(unit, mdxMacId, 0, 174, 20, 0x4144);
    PHY_SDS_WR(unit, mdxMacId, 0, 174, 21, 0x3d44);
    PHY_SDS_WR(unit, mdxMacId, 0, 128, 16, 0x400);
    PHY_SDS_WR(unit, mdxMacId, 0, 128, 16, 0xc00);
    PHY_SDS_WR(unit, mdxMacId, 0, 128, 16, 0x0);

}


void
_phy_8295_patch_8214qf_config(uint32 unit, uint32 basePort, uint32 mdxMacId)
{
    uint32      sdsId;

    if (0 == phy_8295_get_model(basePort))    //RTK_PHYTYPE_RTL8214QF_NC5
    {
        /* disable serdes */
        PHY_REG_WR(unit, mdxMacId, 5, 20, 0x1F);
        PHY_REG_WR(unit, mdxMacId, 5, 21, 0x1F);
        PHY_REG_WR(unit, mdxMacId, 5, 22, 0x1F);
        PHY_REG_WR(unit, mdxMacId, 5, 23, 0x1F);
    }

    /* enable 1G auto nego */
    PHY_SDS_FIELD_WR(unit, mdxMacId, 4, 8, 16, 12, 12, 0x1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 5, 8, 16, 12, 12, 0x1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 6, 8, 16, 12, 12, 0x1);
    PHY_SDS_FIELD_WR(unit, mdxMacId, 7, 8, 16, 12, 12, 0x1);

    if ((sdsId = _rtl8214qf_serdesListIdx_get(basePort)) >= 0)
    {
        PATCH_FLOW_PRINT("sdsIdx:%u polarity RX:%d TX:%d", sdsId, gSwitchModel->serdes.list[sdsId].rx_polarity, gSwitchModel->serdes.list[sdsId].tx_polarity);
        if (SERDES_POLARITY_CHANGE == gSwitchModel->serdes.list[sdsId].rx_polarity)
        {
            PHY_SDS_FIELD_WR(unit, mdxMacId, 0, 0, 16, 9, 9, 0x1);
        }
        if (SERDES_POLARITY_CHANGE == gSwitchModel->serdes.list[sdsId].tx_polarity)
        {
            PHY_SDS_FIELD_WR(unit, mdxMacId, 0, 0, 16, 8, 8, 0x1);
        }
    }

    if (HWP_8380_30_FAMILY(unit))
    {
        PHY_SDS_FIELD_WR(unit, mdxMacId, 0, 0, 16, 15, 15, 0x1);
    }
}




/* Function Name:
 *      rtl8214qf_config
 * Description:
 *      configure 8214QF
 * Input:
 *      base_macid      base mac id of the PHY chip
 * Output:
 *      None
 * Return:
 *      None
 * Note:
 *      None
 */
void
rtl8214qf_config(int32 basePort)
{
    uint32      unit = 0;
    uint32      mdxMacId;

    OSAL_PRINTF("### RTL8214QF config - MAC ID = %d ###\n", basePort);
    rtl8295_mdxMacId_get(basePort, &mdxMacId);
    if (mdxMacId < 0)
    {
        INFO_LOG("port=%u err mdxMacId_get", basePort);
        return;
    }

    if (0 == phy_8295_get_model(basePort))    //RTK_PHYTYPE_RTL8214QF_NC5
    {
        _phy_8295_patch_bT_init_chip(unit, basePort, mdxMacId);
    }
    else
    {
        _phy_8295_patch_a_init_rtl8214qf(unit, basePort, mdxMacId);
    }

    _phy_8295_patch_8214qf_config(unit, basePort, mdxMacId);
}






