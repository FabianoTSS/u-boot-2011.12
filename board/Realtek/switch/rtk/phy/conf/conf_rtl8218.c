
#ifdef  CONFIG_4L_PCB /* for 4 layers board */

/* 2010-0304 */
#define phy_0_serdes_0_624e  0x8803
#define phy_0_serdes_0_6252  0xd410
#define phy_0_serdes_0_6253  0x9060
#define phy_0_serdes_0_6254  0xfc00
#define phy_0_serdes_0_6255  0xc62c
#define phy_0_serdes_0_6256  0x2e31
#define phy_0_serdes_0_6257  0x7f10
#define phy_0_serdes_0_625a  0x948c
#define phy_0_serdes_0_6250  0x2299
#define phy_0_serdes_0_6251  0x469B
#define phy_0_serdes_0_625b  0x5b88
#define phy_0_serdes_0_6258  0x0968
#define phy_0_serdes_1_644e  0x8803
#define phy_0_serdes_1_6452  0xd410
#define phy_0_serdes_1_6453  0x9060
#define phy_0_serdes_1_6454  0xfc00
#define phy_0_serdes_1_6455  0xc62c
#define phy_0_serdes_1_6456  0x2e31
#define phy_0_serdes_1_6457  0x7f10
#define phy_0_serdes_1_645a  0x948c
#define phy_0_serdes_1_6450  0x2299
#define phy_0_serdes_1_6451  0x469B
#define phy_0_serdes_1_645b  0x5b88
#define phy_0_serdes_1_6458  0x0968

#else /* for 2 layers board */

/* 2010-0318 */
#define phy_0_serdes_0_624e	0x8803
#define phy_0_serdes_0_6252	0xd410
#define phy_0_serdes_0_6253	0x8060
#define phy_0_serdes_0_6254	0xfc00
#define phy_0_serdes_0_6255	0xc63c
#define phy_0_serdes_0_6256	0x2e31
#define phy_0_serdes_0_6257	0x7f10
#define phy_0_serdes_0_625a	0x944e
#define phy_0_serdes_0_6250	0x329a
#define phy_0_serdes_0_6251	0x469B
#define phy_0_serdes_0_625b	0x5b88
#define phy_0_serdes_0_6258	0x0968
#define phy_0_serdes_1_644e	0x8803
#define phy_0_serdes_1_6452	0xd410
#define phy_0_serdes_1_6453	0x8060
#define phy_0_serdes_1_6454	0xfc00
#define phy_0_serdes_1_6455	0xc63c
#define phy_0_serdes_1_6456	0x2e31
#define phy_0_serdes_1_6457	0x7f10
#define phy_0_serdes_1_645a	0x944e
#define phy_0_serdes_1_6450	0x329a
#define phy_0_serdes_1_6451	0x469B
#define phy_0_serdes_1_645b	0x5b88
#define phy_0_serdes_1_6458	0x0968

#endif


confcode_pprmv_t rtl8218_serdes_perchip[] = { \
    /* First */
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x624e},
    {0, 31, 24, 0xffff, phy_0_serdes_0_624e},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6252},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6252},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6253},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6253},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6254},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6254},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6255},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6255},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6256},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6256},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6257},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6257},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x625a},
    {0, 31, 24, 0xffff, phy_0_serdes_0_625a},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6250},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6250},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6251},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6251},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x625b},
    {0, 31, 24, 0xffff, phy_0_serdes_0_625b},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6258},
    {0, 31, 24, 0xffff, phy_0_serdes_0_6258},
    {0, 31, 21, 0xffff, 0x0003},
    /* Second */
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x644e},
    {0, 31, 24, 0xffff, phy_0_serdes_1_644e},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6452},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6452},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6453},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6453},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6454},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6454},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6455},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6455},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6456},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6456},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6457},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6457},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x645a},
    {0, 31, 24, 0xffff, phy_0_serdes_1_645a},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6450},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6450},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6451},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6451},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x645b},
    {0, 31, 24, 0xffff, phy_0_serdes_1_645b},
    {0, 31, 21, 0xffff, 0x0003},
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x6458},
    {0, 31, 24, 0xffff, phy_0_serdes_1_6458},
    {0, 31, 21, 0xffff, 0x0003},

#if 0   /* can not support get and write with phyid */
    /* Enable PHY (if chip is powerdown by default) */
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x130f},
    {0, 31, 24, 0xffe0, 0x0840}, //0x0840 | (rtl8218_phy0_macid << 0)},
    {0, 31, 21, 0xffff, 0x0003},

#undef RTL8218_PKTGEN
#ifdef RTL8218_PKTGEN
    /* ###### Green off ########### */
    {0, 31, 31, 0xffff, 0x000e},
    {0, 31, 23, 0xffff, 0x130f},
    {0, 31, 24, 0xffe0, 0x0800}, //0x0800 | (rtl8218_phy0_macid << 0)},
    {0, 31, 21, 0xffff, 0x0003},
#endif
#endif

}; /* end of rtl8218_perchip[] */


#if 0   /* using the new config for RTL8218 formal version */
    if (val == 0x94eb)
    {
        /* GPHY setting */
        gMacDrv->drv_miim_write(rtl8218_macid,0,31,5);
        gMacDrv->drv_miim_write(rtl8218_macid,5,1,0x0700);
        gMacDrv->drv_miim_write(rtl8218_macid,5,5,0x8B82);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x05CB);
        
        gMacDrv->drv_miim_write(rtl8218_macid,5,31,2);
        gMacDrv->drv_miim_write(rtl8218_macid,2,4,0x80C2);
        gMacDrv->drv_miim_write(rtl8218_macid,2,5,0x0938);
        
        gMacDrv->drv_miim_write(rtl8218_macid,2,31,3);
        gMacDrv->drv_miim_write(rtl8218_macid,3,18,0xC4D2);
        gMacDrv->drv_miim_write(rtl8218_macid,3,13,0x0207);
        
        gMacDrv->drv_miim_write(rtl8218_macid,3,31,1);
        gMacDrv->drv_miim_write(rtl8218_macid,1,4,0x267E);
        gMacDrv->drv_miim_write(rtl8218_macid,1,28,0xE5F7);
        gMacDrv->drv_miim_write(rtl8218_macid,1,27,0x0424);
        
        gMacDrv->drv_miim_write(rtl8218_macid,1,31,5);
        gMacDrv->drv_miim_write(rtl8218_macid,5,5,0xfff6);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x0080);
        gMacDrv->drv_miim_write(rtl8218_macid,5,5,0x8000);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0xf8e0);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0xe000);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0xe1e0);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x01ac);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x2408);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0xe08b);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x84f7);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x20e4);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x8b84);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0xfc05);
        gMacDrv->drv_miim_write(rtl8218_macid,5,5,0x8b90);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x8000);
        gMacDrv->drv_miim_write(rtl8218_macid,5,5,0x8b92);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x8000);
        gMacDrv->drv_miim_write(rtl8218_macid,5,8,0xfffa);
        gMacDrv->drv_miim_write(rtl8218_macid,5,2,0x3265);
        gMacDrv->drv_miim_write(rtl8218_macid,5,5,0xfff6);
        gMacDrv->drv_miim_write(rtl8218_macid,5,6,0x00f3);
        
        gMacDrv->drv_miim_write(rtl8218_macid,5,31,0);
        
        gMacDrv->drv_miim_write(rtl8218_macid,0,31,7);
        gMacDrv->drv_miim_write(rtl8218_macid,7,30,0x0042);
        gMacDrv->drv_miim_write(rtl8218_macid,7,24,0x0000);
        
        gMacDrv->drv_miim_write(rtl8218_macid,7,31,0);
    }
#endif
confcode_rv_t rtl8218_perport[] = {\
	{0x1f, 0x0007}, {0x1e, 0x0048}, {0x19, 0x4012}, {0x1f, 0x0003}, \
	{0x01, 0x3554}, {0x02, 0x63e8}, {0x03, 0x99c2}, {0x04, 0x0113}, \
	{0x05, 0x303e}, {0x0d, 0x0207}, {0x0e, 0xe100}, {0x1f, 0x0007}, \
	{0x1e, 0x0020}, {0x1b, 0x2f4a}, {0x15, 0x0000}, {0x1e, 0x0040}, \
	{0x18, 0x0000}, {0x1f, 0x0007}, {0x1e, 0x002c}, {0x18, 0x008b}, \
	{0x1f, 0x0005}, {0x05, 0x8b6e}, {0x06, 0x0000}, {0x0f, 0x0100}, \
	{0x05, 0xfff6}, {0x06, 0x0080}, {0x1f, 0x0007}, {0x1e, 0x002d}, \
	{0x18, 0xf030}, {0x1e, 0x0023}, {0x16, 0x0005}, {0x15, 0x005c}, \
	{0x19, 0x0068}, {0x15, 0x0082}, {0x19, 0x000a}, {0x15, 0x00a1}, \
	{0x19, 0x0081}, {0x15, 0x00af}, {0x19, 0x0080}, {0x15, 0x00d4}, \
	{0x19, 0x0000}, {0x15, 0x00e4}, {0x19, 0x0081}, {0x15, 0x00e7}, \
	{0x19, 0x0080}, {0x15, 0x010d}, {0x19, 0x0083}, {0x15, 0x0118}, \
	{0x19, 0x0083}, {0x15, 0x0120}, {0x19, 0x0082}, {0x15, 0x019c}, \
	{0x19, 0x0081}, {0x15, 0x01a4}, {0x19, 0x0080}, {0x15, 0x01cd}, \
	{0x19, 0x0000}, {0x15, 0x01dd}, {0x19, 0x0081}, {0x15, 0x01e0}, \
	{0x19, 0x0080}, {0x15, 0x0147}, {0x19, 0x0096}, {0x16, 0x0000}, \
	{0x1e, 0x002d}, {0x18, 0xf010}, {0x1f, 0x0005}, {0x05, 0x8000}, \
	{0x06, 0x0280}, {0x06, 0x2bf7}, {0x06, 0x00e0}, {0x06, 0xfff7}, \
	{0x06, 0xa080}, {0x06, 0x02ae}, {0x06, 0xf602}, {0x06, 0x806e}, \
	{0x06, 0x0201}, {0x06, 0x5002}, {0x06, 0x0163}, {0x06, 0x0281}, \
	{0x06, 0x03e0}, {0x06, 0x8b8c}, {0x06, 0xe18b}, {0x06, 0x8d1e}, \
	{0x06, 0x01e1}, {0x06, 0x8b8e}, {0x06, 0x1e01}, {0x06, 0xa000}, \
	{0x06, 0xe4ae}, {0x06, 0xd8bf}, {0x06, 0x8097}, {0x06, 0xd785}, \
	{0x06, 0x80d0}, {0x06, 0x6c02}, {0x06, 0x2971}, {0x06, 0xee8b}, \
	{0x06, 0x8601}, {0x06, 0xee85}, {0x06, 0xfd00}, {0x06, 0xee8b}, \
	{0x06, 0x6750}, {0x06, 0xee8b}, {0x06, 0x6803}, {0x06, 0xee8b}, \
	{0x06, 0x6902}, {0x06, 0xee8b}, {0x06, 0x6c24}, {0x06, 0xee8b}, \
	{0x06, 0x6d24}, {0x06, 0xee8a}, {0x06, 0xfc07}, {0x06, 0xee8a}, \
	{0x06, 0xfd73}, {0x06, 0xee8b}, {0x06, 0x7000}, {0x06, 0xee8b}, \
	{0x06, 0x71ee}, {0x06, 0x0282}, {0x06, 0x16ee}, {0x06, 0xfff6}, \
	{0x06, 0x00ee}, {0x06, 0xfff7}, {0x06, 0xfc04}, {0x06, 0xf8fa}, \
	{0x06, 0xef69}, {0x06, 0xe08b}, {0x06, 0x86ad}, {0x06, 0x201a}, \
	{0x06, 0xbf85}, {0x06, 0x80d0}, {0x06, 0x6c02}, {0x06, 0x2978}, \
	{0x06, 0xe0e0}, {0x06, 0xe4e1}, {0x06, 0xe0e5}, {0x06, 0x5806}, \
	{0x06, 0x68c0}, {0x06, 0xd1d2}, {0x06, 0xe4e0}, {0x06, 0xe4e5}, \
	{0x06, 0xe0e5}, {0x06, 0xef96}, {0x06, 0xfefc}, {0x06, 0x0425}, \
	{0x06, 0x0807}, {0x06, 0x2640}, {0x06, 0x7227}, {0x06, 0x267e}, \
	{0x06, 0x2804}, {0x06, 0xb729}, {0x06, 0x2576}, {0x06, 0x2a68}, \
	{0x06, 0xe52b}, {0x06, 0xad00}, {0x06, 0x2cdb}, {0x06, 0xf02d}, \
	{0x06, 0x67bb}, {0x06, 0x2e7b}, {0x06, 0x0f2f}, {0x06, 0x7365}, \
	{0x06, 0x31ac}, {0x06, 0xcc32}, {0x06, 0x2300}, {0x06, 0x332d}, \
	{0x06, 0x1734}, {0x06, 0x7f52}, {0x06, 0x3510}, {0x06, 0x0036}, \
	{0x06, 0x1000}, {0x06, 0x3710}, {0x06, 0x0038}, {0x06, 0x7fce}, \
	{0x06, 0x3ce5}, {0x06, 0xf73d}, {0x06, 0x3da4}, {0x06, 0x6530}, \
	{0x06, 0x3e67}, {0x06, 0x0053}, {0x06, 0x69d2}, {0x06, 0x0f6a}, \
	{0x06, 0x012c}, {0x06, 0x6c2b}, {0x06, 0x136e}, {0x06, 0xe100}, \
	{0x06, 0x6f12}, {0x06, 0xf771}, {0x06, 0x006b}, {0x06, 0x7306}, \
	{0x06, 0xeb74}, {0x06, 0x94c7}, {0x06, 0x7698}, {0x06, 0x0a77}, \
	{0x06, 0x5000}, {0x06, 0x788a}, {0x06, 0x1579}, {0x06, 0x7f6f}, \
	{0x06, 0x7a06}, {0x06, 0xa6f8}, {0x06, 0xe08b}, {0x06, 0x8ead}, \
	{0x06, 0x201d}, {0x06, 0xf620}, {0x06, 0xe48b}, {0x06, 0x8e02}, \
	{0x06, 0x260e}, {0x06, 0x0226}, {0x06, 0xe802}, {0x06, 0x044c}, \
	{0x06, 0x0281}, {0x06, 0xd502}, {0x06, 0x0259}, {0x06, 0x0282}, \
	{0x06, 0x9b02}, {0x06, 0x8159}, {0x06, 0x0281}, {0x06, 0x86e0}, \
	{0x06, 0x8b8e}, {0x06, 0xad23}, {0x06, 0x05f6}, {0x06, 0x23e4}, \
	{0x06, 0x8b8e}, {0x06, 0xe08b}, {0x06, 0x8ead}, {0x06, 0x2408}, \
	{0x06, 0xf624}, {0x06, 0xe48b}, {0x06, 0x8e02}, {0x06, 0x27b1}, \
	{0x06, 0xe08b}, {0x06, 0x8ead}, {0x06, 0x260b}, {0x06, 0xf626}, \
	{0x06, 0xe48b}, {0x06, 0x8e02}, {0x06, 0x05c0}, {0x06, 0x021c}, \
	{0x06, 0xf702}, {0x06, 0x03c2}, {0x06, 0x0204}, {0x06, 0x14fc}, \
	{0x06, 0x04f8}, {0x06, 0xf9e0}, {0x06, 0x8b87}, {0x06, 0xad20}, \
	{0x06, 0x22e0}, {0x06, 0xe200}, {0x06, 0xe1e2}, {0x06, 0x01ad}, \
	{0x06, 0x2011}, {0x06, 0xe2e0}, {0x06, 0x22e3}, {0x06, 0xe023}, \
	{0x06, 0xad39}, {0x06, 0x085a}, {0x06, 0xc09f}, {0x06, 0x04f7}, \
	{0x06, 0x24ae}, {0x06, 0x02f6}, {0x06, 0x24e4}, {0x06, 0xe200}, \
	{0x06, 0xe5e2}, {0x06, 0x01fd}, {0x06, 0xfc04}, {0x06, 0xf8fa}, \
	{0x06, 0xef69}, {0x06, 0xe08b}, {0x06, 0x86ad}, {0x06, 0x2140}, \
	{0x06, 0xe0e0}, {0x06, 0x22e1}, {0x06, 0xe023}, {0x06, 0x58c0}, \
	{0x06, 0x5902}, {0x06, 0x1e01}, {0x06, 0xe18b}, {0x06, 0x651f}, \
	{0x06, 0x109e}, {0x06, 0x2de4}, {0x06, 0x8b65}, {0x06, 0xad21}, \
	{0x06, 0x1ead}, {0x06, 0x2724}, {0x06, 0xd400}, {0x06, 0x01bf}, \
	{0x06, 0x309b}, {0x06, 0x022a}, {0x06, 0x16bf}, {0x06, 0x309e}, \
	{0x06, 0x022a}, {0x06, 0x54e0}, {0x06, 0x8b67}, {0x06, 0x1b10}, \
	{0x06, 0xaa0e}, {0x06, 0xe18b}, {0x06, 0x68ae}, {0x06, 0x03e1}, \
	{0x06, 0x8b69}, {0x06, 0xbf83}, {0x06, 0x6302}, {0x06, 0x2a16}, \
	{0x06, 0xef96}, {0x06, 0xfefc}, {0x06, 0x04f8}, {0x06, 0xe08b}, \
	{0x06, 0x85ad}, {0x06, 0x2728}, {0x06, 0xe0e0}, {0x06, 0x36e1}, \
	{0x06, 0xe037}, {0x06, 0xe18b}, {0x06, 0x731f}, {0x06, 0x109e}, \
	{0x06, 0x1be4}, {0x06, 0x8b73}, {0x06, 0xac20}, {0x06, 0x08ac}, \
	{0x06, 0x210a}, {0x06, 0xac27}, {0x06, 0x0cae}, {0x06, 0x0d02}, \
	{0x06, 0x8206}, {0x06, 0xae08}, {0x06, 0x0282}, {0x06, 0x0bae}, \
	{0x06, 0x0302}, {0x06, 0x8212}, {0x06, 0xfc04}, {0x06, 0xee8b}, \
	{0x06, 0x6a00}, {0x06, 0x0402}, {0x06, 0x821f}, {0x06, 0x0282}, \
	{0x06, 0x6204}, {0x06, 0x0282}, {0x06, 0x8304}, {0x06, 0xee8b}, \
	{0x06, 0x6a00}, {0x06, 0xee8b}, {0x06, 0x6b02}, {0x06, 0x04f8}, \
	{0x06, 0xf9e0}, {0x06, 0x8b85}, {0x06, 0xad26}, {0x06, 0x38d0}, \
	{0x06, 0x0b02}, {0x06, 0x28c1}, {0x06, 0x5882}, {0x06, 0x7882}, \
	{0x06, 0x9f2d}, {0x06, 0xe08b}, {0x06, 0x6ae1}, {0x06, 0x8b6b}, \
	{0x06, 0x1f10}, {0x06, 0x9ec8}, {0x06, 0x10e4}, {0x06, 0x8b6a}, \
	{0x06, 0xe0e0}, {0x06, 0x00e1}, {0x06, 0xe001}, {0x06, 0xf727}, \
	{0x06, 0xe4e0}, {0x06, 0x00e5}, {0x06, 0xe001}, {0x06, 0xe2e0}, \
	{0x06, 0x20e3}, {0x06, 0xe021}, {0x06, 0xad30}, {0x06, 0xf7f6}, \
	{0x06, 0x27e4}, {0x06, 0xe000}, {0x06, 0xe5e0}, {0x06, 0x01fd}, \
	{0x06, 0xfc04}, {0x06, 0xf8fa}, {0x06, 0xef69}, {0x06, 0xe08b}, \
	{0x06, 0x86ad}, {0x06, 0x2212}, {0x06, 0xe0e0}, {0x06, 0x14e1}, \
	{0x06, 0xe015}, {0x06, 0xad26}, {0x06, 0x9ce1}, {0x06, 0x8b6c}, \
	{0x06, 0xbf83}, {0x06, 0x6602}, {0x06, 0x2a16}, {0x06, 0xef96}, \
	{0x06, 0xfefc}, {0x06, 0x04f8}, {0x06, 0xfaef}, {0x06, 0x69e0}, \
	{0x06, 0x8b86}, {0x06, 0xad22}, {0x06, 0x09e1}, {0x06, 0x8b6d}, \
	{0x06, 0xbf83}, {0x06, 0x6602}, {0x06, 0x2a16}, {0x06, 0xef96}, \
	{0x06, 0xfefc}, {0x06, 0x04f8}, {0x06, 0xf9fa}, {0x06, 0xef69}, \
	{0x06, 0xfae0}, {0x06, 0x8b85}, {0x06, 0xad25}, {0x06, 0x22e0}, \
	{0x06, 0xe022}, {0x06, 0xe1e0}, {0x06, 0x23e2}, {0x06, 0xe036}, \
	{0x06, 0xe3e0}, {0x06, 0x375a}, {0x06, 0xc40d}, {0x06, 0x0158}, \
	{0x06, 0x021e}, {0x06, 0x20e3}, {0x06, 0x8ae7}, {0x06, 0xac31}, \
	{0x06, 0x0bac}, {0x06, 0x3a0e}, {0x06, 0xac3e}, {0x06, 0x52ae}, \
	{0x06, 0x06af}, {0x06, 0x8359}, {0x06, 0xaf83}, {0x06, 0x47af}, \
	{0x06, 0x8356}, {0x06, 0xad37}, {0x06, 0x10e0}, {0x06, 0x8ad0}, \
	{0x06, 0x10e4}, {0x06, 0x8ad0}, {0x06, 0xe18a}, {0x06, 0xd11b}, \
	{0x06, 0x109e}, {0x06, 0x1eae}, {0x06, 0x71ad}, {0x06, 0x322f}, \
	{0x06, 0xe08b}, {0x06, 0x70e1}, {0x06, 0x8b71}, {0x06, 0xef64}, \
	{0x06, 0xe08a}, {0x06, 0xd2e1}, {0x06, 0x8ad3}, {0x06, 0x14e4}, \
	{0x06, 0x8ad2}, {0x06, 0xe58a}, {0x06, 0xd31f}, {0x06, 0x649f}, \
	{0x06, 0x55d1}, {0x06, 0x00bf}, {0x06, 0x28be}, {0x06, 0x022a}, \
	{0x06, 0x16ee}, {0x06, 0x8ad0}, {0x06, 0x00d1}, {0x06, 0x01bf}, \
	{0x06, 0x8369}, {0x06, 0x022a}, {0x06, 0x16ae}, {0x06, 0x3fae}, \
	{0x06, 0x24ad}, {0x06, 0x3621}, {0x06, 0xe08a}, {0x06, 0xfce1}, \
	{0x06, 0x8afd}, {0x06, 0xef64}, {0x06, 0xe08a}, {0x06, 0xd2e1}, \
	{0x06, 0x8ad3}, {0x06, 0x1f64}, {0x06, 0x9f02}, {0x06, 0xae26}, \
	{0x06, 0x14e4}, {0x06, 0x8ad2}, {0x06, 0xe58a}, {0x06, 0xd31f}, \
	{0x06, 0x649f}, {0x06, 0x1bae}, {0x06, 0x0dee}, {0x06, 0x8ad2}, \
	{0x06, 0x00ee}, {0x06, 0x8ad3}, {0x06, 0x00ae}, {0x06, 0x0fac}, \
	{0x06, 0x390c}, {0x06, 0xd101}, {0x06, 0xbf28}, {0x06, 0xbe02}, \
	{0x06, 0x2a16}, {0x06, 0xee8a}, {0x06, 0xd000}, {0x06, 0xe68a}, \
	{0x06, 0xe7fe}, {0x06, 0xef96}, {0x06, 0xfefd}, {0x06, 0xfc04}, \
	{0x06, 0xcce2}, {0x06, 0x0064}, {0x06, 0xe48c}, {0x06, 0x70e0}, \
	{0x06, 0x7699}, {0x06, 0xe000}, {0x05, 0xe142}, {0x06, 0x0701}, \
	{0x05, 0xe140}, {0x06, 0x0405}, {0x0f, 0x0000}, {0x1f, 0x0000}, \
	{0x1f, 0x0005}, {0x05, 0x8b84}, {0x06, 0x0042}, {0x1f, 0x0000}, \
	{0x0d, 0x0003}, {0x0e, 0x0015}, {0x0d, 0x4003}, {0x0e, 0x0006}, \
	{0x1f, 0x0007}, {0x1e, 0x002c}, {0x19, 0x0504}, {0x1f, 0x0000}, \
	\
	};

confcode_prv_t rtl8218_perchip[] = {\
	{3, 0x1f, 0x0007}, {3, 0x1e, 0x000b}, {3, 0x16, 0x1a00}, {3, 0x1f, 0x0000}, \
	{2, 0x1f, 0x0007}, {2, 0x1e, 0x000a}, {2, 0x19, 0x0000}, {2, 0x1a, 0x0000}, \
	{2, 0x1b, 0x0000}, {2, 0x1c, 0x0000}, {2, 0x1e, 0x000b}, {2, 0x15, 0x0500}, \
	{2, 0x16, 0x0000}, {2, 0x17, 0x0000}, {2, 0x18, 0x0000}, {2, 0x1f, 0x0000}, \
	{0, 0x1f, 0x000e}, {0, 0x17, 0x1362}, {0, 0x18, 0x0115}, {0, 0x15, 0x0003}, \
	{0, 0x1f, 0x000e}, {0, 0x17, 0x1363}, {0, 0x18, 0x0002}, {0, 0x15, 0x0003}, \
	{0, 0x1f, 0x000e}, {0, 0x17, 0x1363}, {0, 0x18, 0x0000}, {0, 0x15, 0x0003}, \
	{0, 0x1f, 0x000e}, {0, 0x17, 0x1309}, {0, 0x18, 0x0080}, {0, 0x15, 0x0003}, \
	{0, 0x1f, 0x000e}, {0, 0x17, 0x6242}, {0, 0x18, 0x5e86}, {0, 0x15, 0x0003}, \
	{0, 0x1f, 0x000e}, {0, 0x17, 0x6442}, {0, 0x18, 0x5e86}, {0, 0x15, 0x0003} \
	\
	};


