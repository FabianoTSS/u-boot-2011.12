/*! Copyright(c) 2008-2010 Shenzhen TP-LINK Technologies Co.Ltd.
 *
 *\file		md5.h
 *\brief		
 *\details	
 *
 *\author	Cai Peifeng
 *\version	
 *\date		07Apr10
 *
 *\warning	
 *
 *\history \arg	07Apr10, Cai Peifeng, Create the file
 */
#ifndef __MD5_H__
#define __MD5_H__

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************/
/*                                      CONFIGURATIONS                                            */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      INCLUDE_FILES                                             */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      DEFINES                                                   */
/**************************************************************************************************/

/**************************************************************************************************/
/*                                      TYPES                                                     */
/**************************************************************************************************/
/*!
 *\typedef	POINTER
 *\brief	define a generic pointer type
*/
typedef unsigned char *POINTER;

/*!
 *\typedef	UINT2
 *\brief	define a two bye word
*/
typedef unsigned short int UINT2;

/*!
 *\typedef	UINT4
 *\brief	define a four byte word	
*/
typedef unsigned int UINT4;

/*!
 *\struct	SWMD5_CTX
 *\brief	the md5 context	
*/

typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        							/* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
}SWMD5_CTX;

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
/*!
 *\fn 	 		void md5_calc( unsigned char *output, unsigned char *input, 
unsigned int inlen);
 *\brief   		caculate the MD5 digest for the input string
 *
 *\param[in]   	input	the buffer
 *\param[in]	inlen	the length of the buffer
 *\param[out]  	ouput 	the ouput md5
 *
 *\return  		N/A
 *\retval  		N/A
 *
 *\note		  
 */
void md5_calc( unsigned char *output, unsigned char *input, unsigned int inlen
);

/**************************************************************************************************/
/*                                      GLOBAL_FUNCTIONS                                          */
/**************************************************************************************************/


#ifdef __cplusplus
}
#endif

#endif	/* __MD5_H__ */
