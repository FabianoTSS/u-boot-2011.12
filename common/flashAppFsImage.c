/******************************************************************************
*
* Copyright (c) 2010 TP-LINK Technologies CO.,LTD.
* All rights reserved.
*
* file name  :   flashAppFsImage.c
* version    :   V1.0
* description:   �����������Ľӿ�.
*
* author     :   LiuZenglin <LiuZenglin@tp-link.net>
* data       :   06/25/2013
*
* history    :
* 01   06/25/2013  LiuZenglin     Create.
*
* refresh option:
* 1������switch.tp �� webImage.z
* 2����ɾ��ԭ�ڲ������ֱ��д�뾵���е��ڲ��������򸲸ǣ��ײ�ᴦ��
*    profile��mac��RSA�ģ�

* 4��ɾ���ڲ����еĳ�gev��profile���ɾ���Ǳ����gev���д�뾵���е��ڲ��
*
* �Ǿ����а汾��ΪV1 A.B.C, flash�а汾��ΪV2 A.B.C
* if ((A1 == A1) && (B1 <= B2) && (C1 <= C2))   ִ�в���1
* if ((A1 == A1) && (B1 <= B2) && (C1 > C2))    ִ�в���1�Ͳ���2
* if ((A1 == A1) && (B1 > B2))                  ִ�в���2�Ͳ���3
* if (A1 != A1)                                 ִ�в���3�Ͳ���4
*
******************************************************************************/
#include<common.h>
#include<vsprintf.h>
#include <tplink/flashApp.h>
#include <tplink/flashAppInner.h>
#include <tplink/flashAppInnerGev.h>
#include <tplink/flashAppInnerProfile.h>
#include <tplink/flashAppFsImage.h>
#include <linux/ctype.h>
#include <tplink/flashUtil.h>
#include <tplink/md5_tp.h>
#include "malloc.h"

int flash_fs_image_debug = 0;
    
#define DBG(arg...) do{if(0){printf("%s,%d:",__FUNCTION__, __LINE__);printf(arg);}}while(0)


/* ǩ����Ϣ���� */
#define RSA_SIGN_LEN	(128)
/* ���ڼ��������ļ�ǩ���Ĺ�Կ�ļ� */
//static char pubKey[] = "BgIAAACkAABSU0ExAAQAAAEAAQBhVXK3wF79A6cXXFu0Y0wKz0dPQWi2dWPE7p8eY9e6PAqc5BBTQPxi2/N1OotrUN11Q6cBXA0gmflRusUiJtdARng43tSWz2pZueskCC5kH9/+1/ACi2ZY1WlK5TVu5Bh0YCzAfvlmsbuPjk/W4S3Jco+ISDOrpF5wwuxlCHI2vQ==";
/*
 * �ú������ڼ�������ļ���ǩ���Ƿ���ȷ
 * note:
 * 1,ǩ����Ϣλ�������ļ������128bytes
 * 2,ǩ����Ϣ���ɹ���:�����������ļ�����md5,����md5ֵ����128bytes��RSAǩ����Ϣ
 * 
*/



