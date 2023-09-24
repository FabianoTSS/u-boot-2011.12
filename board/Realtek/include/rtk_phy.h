#ifndef __RTK_PHY_H__
#define __RTK_PHY_H__

/* PHY Page Definition */
#define PHY_PAGE_0                      (0)
#define PHY_PAGE_1                      (1)
#define PHY_PAGE_8                      (8)
#define PHY_PAGE_31                     (31)


/* PHY register definition */
#define PHY_CONTROL_REG                         0
#define PHY_STATUS_REG                          1
#define PHY_IDENTIFIER_1_REG                    2
#define PHY_IDENTIFIER_2_REG                    3
#define PHY_AN_ADVERTISEMENT_REG                4
#define PHY_AN_LINKPARTNER_REG                  5
#define PHY_1000_BASET_CONTROL_REG              9
#define PHY_1000_BASET_STATUS_REG               10
#define PHY_EXTENDED_STATUS_REG                 15
#define PHY_PAGE_SELECTION_REG                  31

/* PHY MII register */

/* Register 0: Control Register */
#define Reset_OFFSET                     (15)
#define Reset_MASK                       (0x1U<<Reset_OFFSET)
#define Loopback_OFFSET                  (14)
#define Loopback_MASK                    (0x1U<<Loopback_OFFSET)
#define SpeedSelection0_OFFSET           (13)
#define SpeedSelection0_MASK             (0x1U<<SpeedSelection0_OFFSET)
#define AutoNegotiationEnable_OFFSET     (12)
#define AutoNegotiationEnable_MASK       (0x1U<<AutoNegotiationEnable_OFFSET)
#define PowerDown_OFFSET                 (11)
#define PowerDown_MASK                   (0x1U<<PowerDown_OFFSET)
#define Isolate_OFFSET                   (10)
#define Isolate_MASK                     (0x1U<<Isolate_OFFSET)
#define RestartAutoNegotiation_OFFSET    (9)
#define RestartAutoNegotiation_MASK      (0x1U<<RestartAutoNegotiation_OFFSET)
#define DuplexMode_OFFSET                (8)
#define DuplexMode_MASK                  (0x1U<<DuplexMode_OFFSET)
#define CollisionTest_OFFSET             (7)
#define CollisionTest_MASK               (0x1U<<CollisionTest_OFFSET)
#define SpeedSelection1_OFFSET           (6)
#define SpeedSelection1_MASK             (0x1U<<SpeedSelection1_OFFSET)

/* Register 1: Status Register */
#define _100Base_T4_OFFSET               (15)
#define _100Base_T4_MASK                 (0x1U<<_100Base_T4_OFFSET)
#define _100Base_TX_FD_OFFSET            (14)
#define _100Base_TX_FD_MASK              (0x1U<<_100Base_TX_FD_OFFSET )
#define _100Base_TX_HD_OFFSET            (13)
#define _100Base_TX_HD_MASK              (0x1U<<_100Base_TX_HD_OFFSET)
#define _10Base_T_FD_OFFSET              (12)
#define _10Base_T_FD_MASK                (0x1U<<_10Base_T_FD_OFFSET)
#define _10Base_T_HD_OFFSET              (11)
#define _10Base_T_HD_MASK                (0x1U<<_10Base_T_HD_OFFSET)
#define _100Base_T2_FD_OFFSET            (10)
#define _100Base_T2_FD_MASK              (0x1U<<_100Base_T2_FD_OFFSET)
#define _100Base_T2_HD_OFFSET            (9)
#define _100Base_T2_HD_MASK              (0x1U<<_100Base_T2_HD_OFFSET)
#define ExtendStatus_OFFSET              (8)
#define ExtendStatus_MASK                (0x1U<<ExtendStatus_OFFSET)
#define MFPreambleSuppression_OFFSET     (6)
#define MFPreambleSuppression_MASK       (0x1U<<MFPreambleSuppression_OFFSET)
#define AutoNegotiationComplete_OFFSET   (5)
#define AutoNegotiationComplete_MASK     (0x1U<<AutoNegotiationComplete_OFFSET )
#define RemoteFault_OFFSET               (4)
#define RemoteFault_MASK                 (0x1U<<RemoteFault_OFFSET)
#define AutoNegotiationAbility_OFFSET    (3)
#define AutoNegotiationAbility_MASK      (0x1U<<AutoNegotiationAbility_OFFSET )
#define LinkStatus_OFFSET                (2)
#define LinkStatus_MASK                  (0x1U<<LinkStatus_OFFSET)
#define JabberDetect_OFFSET              (1)
#define JabberDetect_MASK                (0x1U<<JabberDetect_OFFSET)
#define ExtendedCapability_OFFSET        (0)
#define ExtendedCapability_MASK          (0x1U<<ExtendedCapability_OFFSET)





#endif /* end of __RTK_PHY_H__ */

