/*! Copyright(c) 2008-2010 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 *\file		flashApi.h
 *\brief		
 *\details	
 *
 *\author	Cai Peifeng
 *\version	
 *\date		16Apr10
 *
 *\warning	
 *
 *\history \arg	16Apr10, Cai Peifeng, Create the file
 */
#ifndef __FLASHUTIL_H__
#define __FLASHUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/*                                      CONFIGURATIONS                                            */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      INCLUDE_FILES                                             */
/**************************************************************************************************/

#include"flashConfig.h"
#include "flashAppFsImage.h"
/**************************************************************************************************/
/*                                      DEFINES                                                   */
/**************************************************************************************************/

#define IMAGE_ADDR (0xb4000000 + sysGetFlAddr("sys"))//0xb4000000 is base add of T1500

#define CHECKREGION(nRegion) \
        if( (1 != nRegion) && (2 != nRegion)) \
        { \
            printf("region is error %d\n", nRegion); \
            return ERROR; \
        }
        

/**************************************************************************************************/
/*                                      TYPES                                                     */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      EXTERN_PROTOTYPES                                         */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      LOCAL_PROTOTYPES                                          */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      VARIABLES                                                 */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      LOCAL_FUNCTIONS                                           */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      PUBLIC_FUNCTIONS                                          */
/**************************************************************************************************/


STATUS lockFlash(void);
	 

STATUS unlockFlash(void);


UINT32 l_writeFlash(char *source,unsigned int size, unsigned int flashOffset,int prompt);


UINT32 l_readFlash(char *source, unsigned int size, unsigned int flashOffset);

/**************************************************************************************************/
/*                                      GLOBAL_FUNCTIONS                                          */
/**************************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif	/* __FLASHAPI_H__ */

