/*
 * Copyright(c) Realtek Semiconductor Corporation, 2012
 * All rights reserved.
 *
 * Purpose : Related implementation of the RTL8208 PHY driver.
 *
 * Feature : Common API for RTL8295R and 8214QF
 *
 */

#ifndef	__RTL8208_H__
#define	__RTL8208_H__

/*
 * Include Files
 */
#include <rtk/phy/rtl8295_reg_def.h>


/*
 * Symbol Definition
 */
#define PHY_8295_PAGE_BASE_OFFSET_S0                        256
#define PHY_8295_PAGE_BASE_OFFSET_S1                        768
#define PHY_8295_PAGE_BASE_OFFSET_S0_SLV                    512
#define PHY_8295_PAGE_BASE_OFFSET_S1_SLV                    2304
#define PHY_8295_PAGE_BASE_OFFSET_S4                        1024
#define PHY_8295_PAGE_BASE_OFFSET_S5                        1280
#define PHY_8295_PAGE_BASE_OFFSET_S6                        1536
#define PHY_8295_PAGE_BASE_OFFSET_S7                        1792
#define PHY_8295_PAGE_BASE_OFFSET_BCAST                     2048
#define PHY_8295_SDS_PAGE_NUM_MAX                           256 /* maximum page number per serdes */

/* <<< reg definition >>> */
#define RTL8295_SDS_CTRL_S0_ADDR                                                                                (0xA4)
  #define RTL8295_SDS_CTRL_S0_SERDES_MODE_S0_OFFSET                                                             (0)
  #define RTL8295_SDS_CTRL_S0_SERDES_MODE_S0_MASK                                                               (0x1F << RTL8295_SDS_CTRL_S0_SERDES_MODE_S0_OFFSET)

#define RTL8295_SDS_CTRL_S1_ADDR                                                                                (0xA8)
  #define RTL8295_SDS_CTRL_S1_SERDES_MODE_S1_OFFSET                                                             (0)
  #define RTL8295_SDS_CTRL_S1_SERDES_MODE_S1_MASK                                                               (0x1F << RTL8295_SDS_CTRL_S1_SERDES_MODE_S1_OFFSET)

#define RTL8295_SDS_CTRL_S4_ADDR                                                                                (0xB0)
  #define RTL8295_SDS_CTRL_S4_SERDES_MODE_S4_OFFSET                                                             (0)
  #define RTL8295_SDS_CTRL_S4_SERDES_MODE_S4_MASK                                                               (0x1F << RTL8295_SDS_CTRL_S4_SERDES_MODE_S4_OFFSET)

#define RTL8295_SDS_CTRL_S5_ADDR                                                                                (0xB4)
  #define RTL8295_SDS_CTRL_S5_SERDES_MODE_S5_OFFSET                                                             (0)
  #define RTL8295_SDS_CTRL_S5_SERDES_MODE_S5_MASK                                                               (0x1F << RTL8295_SDS_CTRL_S5_SERDES_MODE_S5_OFFSET)

#define RTL8295_SDS_CTRL_S6_ADDR                                                                                (0xB8)
  #define RTL8295_SDS_CTRL_S6_SERDES_MODE_S6_OFFSET                                                             (0)
  #define RTL8295_SDS_CTRL_S6_SERDES_MODE_S6_MASK                                                               (0x1F << RTL8295_SDS_CTRL_S6_SERDES_MODE_S6_OFFSET)

#define RTL8295_SDS_CTRL_S7_ADDR                                                                                (0xBC)
  #define RTL8295_SDS_CTRL_S7_SERDES_MODE_S7_OFFSET                                                             (0)
  #define RTL8295_SDS_CTRL_S7_SERDES_MODE_S7_MASK                                                               (0x1F << RTL8295_SDS_CTRL_S7_SERDES_MODE_S7_OFFSET)

/*
 * Data Declaration
 */

/*
 * Macro Definition
 */
#define BITS_MASK(mBit, lBit)           (uint32)((((uint64)0x1 << (mBit - lBit + 1)) - 1) << lBit)
#define DATA_BITS_GET(data, mBit, lBit)         REG32_FIELD_GET(data, lBit, BITS_MASK(mBit, lBit))
#define DATA_BITS_SET(data, val, mBit, lBit)    REG32_FIELD_SET(data, val, lBit, BITS_MASK(mBit, lBit))

/*
 * Function Declaration
 */
extern int32 rtl8295_reg_read(int32 mdxMacId, uint32 reg_addr, uint32 *pData);
extern int32 rtl8295_reg_write(int32 mdxMacId, uint32 reg_addr, uint32 data);
extern int32 rtl8295_sds_read(int32 mdxMacId, uint32 sdsId, uint32 page, uint32 reg, uint32 *pData);
extern int32 rtl8295_sds_write(int32 mdxMacId, uint32 sdsId, uint32 page, uint32 reg, uint32 data);
extern int32 phy_8295_sdsField_write(uint32 mdxMacId, uint32 sdsId, uint32 page, uint32 reg, uint32 offset, uint32 mask, uint32 val);
extern int32 rtl8295_mdxMacId_get(int32 port, uint32 *pMdxMacId);
extern int32 phy_8295_get_model(int32 port);
extern int32 phy_8295_phyReg_read(uint32 unit, uint32 mdxMacId, uint32 phy_page, uint32 phy_reg, uint32 *pData);
extern int32 phy_8295_phyReg_write(uint32 unit, uint32 mdxMacId, uint32 phy_page, uint32 phy_reg, uint32 data);
extern int32 phy_8295_sdsReg_read(uint32 unit, uint32 mdxMacId, uint32 sdsId, uint32 s0_phy_page, uint32 phy_reg, uint32 *pData);
extern int32 phy_8295_sdsReg_write(uint32 unit, uint32 mdxMacId, uint32 sdsId, uint32 s0_phy_page, uint32 phy_reg, uint32 data);
extern int32 rtl8295_serdesModeRegOffsetMask_get(uint32 sdsId, uint32 *pPhyPage, uint32 *pPhyReg, uint32 *pOffset, uint32 *pMask);
#endif	/* __RTL8208_H__ */

