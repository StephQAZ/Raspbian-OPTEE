/****************************************************************************************/
/*                          COPYRIGHT INFORMATION                                       */
/*    All rights reserved:shuaifengyun@126.com.                                         */
/****************************************************************************************/
/*
 ****************************************************************************************
 *
 *               CryptoVerifyTaHandle.c
 *
 * Filename      : CryptoVerifyTaHandle.c
 * Author        : Shuai Fengyun
 * Mail          : shuai.fengyun@126.com
 * Create Time   : Fri 30 Oct 2015 12:41:17 AM EDT
 ****************************************************************************************
 */

#define MOUDLE_CRYPTO_TA_HANDLE_C_

/** @defgroup MODULE_NAME_INFOR
* @{
*/

/*
 *******************************************************************************
 *                                INCLUDE FILES
 *******************************************************************************
*/
#include "CryptoVerifyTaHandle.h"
#include "CryptoVerifyTaAes.h"
#include "CryptoVerifyTaDebug.h"
#include "CryptoVerifyTaHash.h"
#include "CryptoVerifyTaOther.h"
#include "CryptoVerifyTaPbkdf2.h"
#include "CryptoVerifyTaRsa.h"




/*
 *******************************************************************************
 *                         FUNCTIONS SUPPLIED BY THIS MODULE
 *******************************************************************************
*/
int g_CryptoTaHandle_Random(uint32_t paramTypes, TEE_Param params[4]);
int g_CryptoTaHandle_Sha(uint32_t paramTypes, TEE_Param params[4]);
int g_CryptoTaHandle_hmac(uint32_t paramTypes, TEE_Param params[4]);
int g_CryptoTaHandle_Aes(uint32_t paramTypes, TEE_Param params[4]);
int g_CryptoTaHandle_Pbkdf(uint32_t paramTypes, TEE_Param params[4]);
int g_CryptoTaHandle_Rsa(uint32_t paramTypes, TEE_Param params[4], UINT32 opMode, UINT32 padding);
int g_CryptoTaHandle_bn(uint32_t paramTypes, TEE_Param params[4]);
int g_CryptoTaHandle_base64(uint32_t paramTypes, TEE_Param params[4]);


/*
 *******************************************************************************
 *                          VARIABLES SUPPLIED BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                          FUNCTIONS USED ONLY BY THIS MODULE
 *******************************************************************************
*/





/*
 *******************************************************************************
 *                          VARIABLES USED ONLY BY THIS MODULE
 *******************************************************************************
*/
CHAR g_Aes128Key[] = {0x2FU, 0x58U, 0x7FU, 0xF0U, 0x43U, 0x83U, 0x95U, 0x3CU,
                      0x1DU, 0x44U, 0x05U, 0x2BU, 0x61U, 0x49U, 0x17U, 0xF8U};
CHAR g_Aes128Iv[] = {0x1DU, 0x44U, 0x05U, 0x2BU, 0x61U, 0x49U, 0x17U, 0xF8U,
                     0x58U, 0xE0U, 0x90U, 0x43U, 0x84U, 0xA1U, 0xC1U, 0x75U};



CHAR g_pwdBuf[] = "Password22icyshuai";
CHAR g_SaltBuf[] = "itsMEjessica22icyshuai";






CHAR g_Publickey1024_N[] = 
{
    0xab, 0x89, 0x1c, 0x98, 0xdf, 0xe8, 0x29, 0xa8, 0x26, 0x8f, 0x88, 0x51, 0x8b, 0x8c,
    0x6e, 0xa5, 0xd7, 0x60, 0x48, 0x07, 0xe1, 0xb6, 0x0d, 0x86, 0x32, 0xcb, 0x97, 0x91, 0xa8,
    0xb5, 0xf8, 0xc0, 0x4d, 0xf3, 0x3b, 0xb7, 0xf2, 0xe7, 0x40, 0xc6, 0x83, 0xe7, 0x31, 0xec,
    0x3f, 0x76, 0x3e, 0x14, 0x82, 0x5e, 0x35, 0xf9, 0x5b, 0x43, 0x79, 0x88, 0x1e, 0x1b, 0x7a,
    0x29, 0x5b, 0x93, 0xc4, 0x5d, 0xee, 0x60, 0x2d, 0x62, 0x21, 0x34, 0x69, 0x39, 0xa8, 0x82,
    0x8d, 0x20, 0x8c, 0xf8, 0xc8, 0x2c, 0x11, 0x3e, 0x56, 0x1b, 0xe8, 0xd1, 0x91, 0xad, 0x93,
    0x00, 0xcf, 0x10, 0xab, 0x2a, 0xc7, 0x24, 0x13, 0xde, 0x04, 0xeb, 0x71, 0x62, 0x2e, 0x35,
    0xa3, 0x4e, 0x24, 0xf2, 0xa0, 0x32, 0x07, 0xd1, 0x87, 0x49, 0x28, 0xf7, 0x82, 0x95, 0x3d,
    0x19, 0x12, 0x36, 0x0d, 0x78, 0x22, 0x51, 0x80, 0x15
};
UINT32 g_PublicKey1024Len_N = 128U;

CHAR g_Publickey1024_D[] = 
{
    0x8b, 0x7d, 0xae, 0x61, 0xab, 0x03, 0x32, 0x8c, 0xe8, 0xfe, 0xa3, 0x8e, 0x1c, 0x7b,
    0x10, 0xa7, 0x90, 0x23, 0x5d, 0xaf, 0xfb, 0x1e, 0x7e, 0x94, 0xf8, 0xcd, 0x92, 0xee, 0x02,
    0x52, 0x28, 0x7c, 0xc9, 0x66, 0xd8, 0xd3, 0x57, 0x96, 0x32, 0x98, 0xbc, 0x3e, 0xcf, 0xdc,
    0x1c, 0x27, 0x5d, 0x88, 0x74, 0x13, 0x84, 0xb4, 0xc8, 0x40, 0x70, 0x9d, 0xfe, 0x4d, 0xbd,
    0xe3, 0xcf, 0xf2, 0xb4, 0x25, 0x91, 0x90, 0xc4, 0xb5, 0x02, 0x49, 0x9b, 0xcc, 0xa9, 0xe9,
    0x16, 0xf5, 0x00, 0x65, 0xc0, 0x05, 0x24, 0x25, 0x40, 0x48, 0xf1, 0xcc, 0xef, 0xb6, 0x1e,
    0xf2, 0x2d, 0x7a, 0x35, 0xce, 0xc7, 0x52, 0x27, 0x5e, 0x0b, 0x99, 0x35, 0xf5, 0x67, 0xf1,
    0x37, 0x2b, 0x10, 0x34, 0xe1, 0x1d, 0xf6, 0xe6, 0x7f, 0x67, 0x5a, 0xdd, 0xd4, 0xde, 0x10,
    0x84, 0x3b, 0x0e, 0x75, 0xba, 0x6a, 0x0a, 0xe6, 0x41
};
UINT32 g_PublicKey1024Len_D = 128U;


CHAR g_Publickey1024_E[] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};
UINT32 g_PublicKey1024Len_E = 128U;




/*CHAR g_Publickey2048_N[] = 
{
    0xc6, 0x23, 0x15, 0x60, 0xd5, 0xf5, 0x34, 0x1a, 0xd9, 0xa0, 0x1a, 0x55, 0x4f, 0x04,
    0xfb, 0x2f, 0x83, 0x42, 0x90, 0x71, 0x73, 0xb0, 0xa3, 0xf1, 0x33, 0xbd, 0x21, 0x59, 0x9c, 
    0xff, 0x87, 0xd1, 0xda, 0x49, 0xdb, 0xe2, 0xa5, 0xd1, 0xb3, 0x88, 0x36, 0xdf, 0xea, 0x54, 
    0xc0, 0x53, 0x27, 0xae, 0x02, 0x5a, 0xce, 0x17, 0x40, 0xd7, 0x01, 0x44, 0xaf, 0xff, 0xbf, 
    0x28, 0x3b, 0x4c, 0xc9, 0x66, 0x56, 0x36, 0x02, 0xd0, 0x09, 0x15, 0x5e, 0x4c, 0x08, 0x84, 
    0x4c, 0xa5, 0x7a, 0x30, 0x8e, 0x68, 0xff, 0x8d, 0x5a, 0x66, 0x61, 0xcb, 0x16, 0xf3, 0x8b, 
    0x10, 0x6e, 0x5c, 0xff, 0xa6, 0xf3, 0xf3, 0xe9, 0xb3, 0x8f, 0xe7, 0x7d, 0x7d, 0xea, 0x4d, 
    0x98, 0x96, 0x39, 0x45, 0xe5, 0xcf, 0xb6, 0x69, 0x8a, 0xf1, 0x1a, 0xfd, 0xee, 0xb0, 0xa5, 
    0x4b, 0x15, 0x76, 0x1f, 0x7b, 0x95, 0x12, 0x9d, 0x9f, 0x52, 0x2e, 0x8b, 0x3d, 0x5c, 0x41, 
    0x94, 0xbc, 0x16, 0x64, 0xcf, 0x58, 0x61, 0xc8, 0x06, 0xdf, 0xca, 0xeb, 0xf4, 0x82, 0xd0, 
    0x43, 0x62, 0xbc, 0x1e, 0x1c, 0x83, 0xaa, 0xee, 0x8f, 0x47, 0x7f, 0x87, 0xb1, 0x58, 0xee, 
    0xb1, 0x49, 0x56, 0x95, 0x1c, 0xf9, 0x49, 0x8e, 0xa6, 0xa3, 0x5b, 0x77, 0xe6, 0xb4, 0x2e, 
    0xeb, 0x96, 0x69, 0x00, 0xb6, 0xc2, 0xbb, 0xbd, 0x50, 0xbf, 0x6a, 0x15, 0xb0, 0x35, 0xc9, 
    0x67, 0x70, 0x6c, 0xaf, 0xd5, 0xfa, 0x9f, 0xbf, 0x2d, 0xaa, 0x8e, 0x81, 0xed, 0x5e, 0x09, 
    0x17, 0x55, 0x32, 0x7d, 0xc7, 0x23, 0x0e, 0x2e, 0xd3, 0xa5, 0x36, 0xcf, 0xc1, 0x80, 0xab, 
    0x37, 0x62, 0x05, 0xb4, 0x8b, 0x10, 0xe7, 0x4e, 0x83, 0x80, 0x06, 0xf4, 0x2e, 0x91, 0x44, 
    0xff, 0x2c, 0x9a, 0xc9, 0x99, 0x6c, 0x44, 0x83, 0x65, 0x3e, 0xcb, 0xa5, 0x0d, 0x9f, 0x5f, 
    0xf1, 0x79
};*/
CHAR g_Publickey2048_N[] = 
{
    0xca, 0xaa, 0x20, 0x34, 0x4c, 0xa9, 0x91, 0x90, 0xc8, 0x53, 0x86, 0x28, 0xd4, 0x66,
    0xd3, 0xa6, 0x4e, 0xe5, 0x35, 0x4a, 0x34, 0xbd, 0x9f, 0x53, 0xe6, 0x58, 0xf7, 0x06, 0xf9,
    0x5d, 0x29, 0x80, 0xce, 0xc2, 0xa5, 0x9c, 0xa0, 0xe2, 0x63, 0x79, 0xc1, 0xdc, 0x17, 0x9d,
    0xfd, 0x95, 0x93, 0x3a, 0xbc, 0x9d, 0xd3, 0xf5, 0xfb, 0x7a, 0xee, 0x31, 0xb7, 0x39, 0x01,
    0x18, 0x87, 0xa7, 0x2c, 0x00, 0xd1, 0x2e, 0xfb, 0x04, 0xed, 0xa3, 0x92, 0xc9, 0xa2, 0x99,
    0xa6, 0x4d, 0x6b, 0x82, 0x14, 0x9a, 0x00, 0x06, 0xc6, 0x16, 0x56, 0x83, 0x48, 0xad, 0x27,
    0x0e, 0x27, 0x6b, 0x47, 0x2d, 0x84, 0xd4, 0xaf, 0xea, 0xb7, 0x03, 0x00, 0xbe, 0xcd, 0x91,
    0xd4, 0x33, 0x96, 0x41, 0x61, 0x43, 0xf9, 0x37, 0x18, 0x14, 0x67, 0xf0, 0x48, 0x68, 0xa1,
    0xfe, 0x50, 0x73, 0xda, 0xce, 0xbb, 0xff, 0x2e, 0x49, 0x0c, 0x17, 0x09, 0xdd, 0x62, 0x3f,
    0xbc, 0xef, 0x30, 0x45, 0xdb, 0x4d, 0xa9, 0x48, 0xe6, 0xaf, 0xe3, 0x44, 0x67, 0x32, 0xcf,
    0xa3, 0x6d, 0xf6, 0x61, 0x49, 0x97, 0x74, 0x58, 0xde, 0x57, 0xd5, 0xf3, 0x07, 0x96, 0x8e,
    0xb0, 0xae, 0x4d, 0xfc, 0xad, 0xf0, 0xfc, 0x49, 0xe2, 0x18, 0x43, 0x93, 0x7a, 0x21, 0x75,
    0x26, 0xd2, 0x7f, 0xa3, 0x1f, 0xe9, 0xb4, 0x2e, 0x43, 0x4d, 0xe9, 0x0e, 0x77, 0x0b, 0xe4,
    0x1c, 0x7b, 0x84, 0x57, 0x67, 0x43, 0xbc, 0xb4, 0x42, 0xed, 0x16, 0x38, 0xc3, 0x03, 0xfe,
    0x5a, 0x3d, 0xd6, 0x90, 0xc0, 0x15, 0x00, 0xa0, 0x2c, 0x11, 0x22, 0xdd, 0x31, 0x11, 0x2a,
    0x2e, 0xcb, 0x41, 0x2a, 0x54, 0x3b, 0x5e, 0xe0, 0x4a, 0xe7, 0x90, 0x99, 0x98, 0x06, 0x4e,
    0xad, 0x51, 0x36, 0x0c, 0xba, 0xa6, 0xb8, 0x63, 0xa4, 0x25, 0x9b, 0xb5, 0xd6, 0xe3, 0x84,
    0xd0, 0xaf
};
UINT32 g_PublicKey2048Len_N = 256U;
/*CHAR g_Publickey2048_D[] =
{
    0x7a, 0x5f, 0x9e, 0xcb, 0x91, 0x3a, 0x01, 0xb5, 0x77, 0xa5, 0xff, 0xbd, 0xa2, 0xb1, 0x63,
    0xe6, 0x63, 0x7e, 0x90, 0x31, 0xd2, 0x0f, 0x4e, 0x22, 0x22, 0x1f, 0x74, 0xe2, 0xa1, 0x29,
    0xdd, 0x9c, 0x09, 0xe3, 0x46, 0x30, 0x84, 0xd3, 0xb0, 0xbb, 0xb7, 0x90, 0xb2, 0x6d, 0x27,
    0xdf, 0xf4, 0x08, 0xf0, 0x21, 0x5f, 0x5a, 0x53, 0x4c, 0xb7, 0xd6, 0xd1, 0x90, 0xf6, 0x62,
    0x85, 0xc5, 0x96, 0x3a, 0x63, 0x92, 0xb6, 0x48, 0x00, 0xe4, 0x36, 0xba, 0x65, 0x24, 0x39,
    0x26, 0x97, 0x02, 0x38, 0x62, 0xb7, 0x3b, 0x79, 0x92, 0xf4, 0x61, 0x77, 0xca, 0x71, 0xa0,
    0x73, 0x59, 0x72, 0xf8, 0x8d, 0x81, 0x9f, 0x5c, 0xac, 0xcb, 0x7f, 0xe1, 0x6d, 0xfe, 0x00,
    0xf8, 0xff, 0x64, 0xa1, 0x5c, 0x99, 0xc0, 0x33, 0xf5, 0x58, 0x03, 0x70, 0x11, 0x9f, 0xf5,
    0x70, 0xca, 0xe5, 0x04, 0xf1, 0xfc, 0x6e, 0x66, 0xab, 0x1e, 0x6b, 0xff, 0x2f, 0xee, 0x70,
    0xc1, 0xc5, 0x19, 0xf9, 0x70, 0x5e, 0xcb, 0x62, 0xf3, 0x48, 0x8e, 0x6a, 0x9a, 0x2e, 0xea,
    0x23, 0x7b, 0xba, 0x09, 0x33, 0xeb, 0x43, 0xe2, 0xb4, 0x36, 0xae, 0x72, 0x78, 0x10, 0x11,
    0x04, 0x81, 0xcc, 0x49, 0xf2, 0x40, 0xef, 0xfe, 0x94, 0x27, 0xc3, 0x06, 0x2d, 0xb8, 0x7a,
    0x58, 0xdd, 0x3f, 0x47, 0x01, 0x6f, 0x5f, 0xb5, 0xc9, 0x29, 0x33, 0xc9, 0x07, 0x50, 0x04,
    0xcd, 0x69, 0x9a, 0x43, 0xd7, 0xb2, 0xff, 0x62, 0xe5, 0x30, 0x78, 0xe3, 0xb9, 0xde, 0x0f,
    0x4e, 0x71, 0xa3, 0x64, 0x5b, 0xb8, 0xf9, 0xf1, 0xa7, 0xf8, 0x67, 0xad, 0x99, 0x94, 0x39,
    0x3a, 0xd3, 0x98, 0x97, 0x5d, 0xe8, 0xfa, 0x92, 0xc8, 0x78, 0x79, 0xa3, 0x7b, 0x15, 0x3a,
    0x6b, 0x00, 0xfa, 0x9b, 0xe3, 0x58, 0x4c, 0x03, 0x62, 0x80, 0xf2, 0x50, 0x8b, 0xa5, 0x6b,
    0xa5
};*/
CHAR g_Publickey2048_D[] =
{
    0xc8, 0x0a, 0xcf, 0x48, 0xf0, 0x56, 0xfc, 0x31, 0x2e, 0x1b, 0x8b, 0xb4, 0x67, 0x9b,
    0xab, 0xd4, 0x5f, 0xd1, 0xcd, 0x1d, 0x17, 0x1c, 0x08, 0xd5, 0xb6, 0xec, 0x1f, 0xc1, 0x89,
    0x88, 0x94, 0xbb, 0xe8, 0xda, 0xa6, 0xc0, 0x08, 0xea, 0x0d, 0x88, 0x94, 0x7b, 0x1b, 0xf0,
    0xbd, 0xea, 0xd3, 0x6d, 0xc9, 0xd9, 0xfb, 0xeb, 0xb4, 0xb4, 0x2d, 0xc3, 0x72, 0xf8, 0x58,
    0x85, 0x8e, 0xf8, 0xca, 0xed, 0x6f, 0x8d, 0x71, 0x06, 0xf7, 0x4e, 0x5b, 0x48, 0x9e, 0x30,
    0x0d, 0xc0, 0x23, 0x9b, 0x3d, 0x9c, 0x98, 0xe4, 0x16, 0xc8, 0xdb, 0xd2, 0x30, 0x7d, 0x81,
    0xac, 0x6d, 0xea, 0x6d, 0x52, 0x97, 0xe7, 0x9c, 0x51, 0x11, 0x1e, 0x6c, 0xb0, 0xe9, 0x96,
    0xc5, 0x1f, 0xba, 0x73, 0x84, 0x7f, 0xa4, 0xd1, 0xe5, 0xf9, 0xd6, 0x72, 0x8d, 0xf6, 0x81,
    0xd7, 0xd6, 0x2f, 0xd5, 0x4d, 0xf4, 0xb1, 0x38, 0x4b, 0xec, 0xd1, 0x9d, 0xf3, 0x82, 0xd7,
    0x88, 0x65, 0x4e, 0xb0, 0xb8, 0xdf, 0x1c, 0x7c, 0x5c, 0x8e, 0xf4, 0xac, 0x13, 0x8f, 0x5a,
    0xe9, 0x4f, 0x97, 0x30, 0xab, 0x42, 0x27, 0xcb, 0x31, 0x9d, 0x51, 0xf8, 0x50, 0x95, 0xb9,
    0x75, 0xda, 0x80, 0x7e, 0x33, 0x0c, 0xc0, 0xfb, 0xd2, 0x7d, 0x22, 0xda, 0x8e, 0x2b, 0xe4,
    0x02, 0x04, 0x6f, 0xad, 0xa3, 0x0d, 0x4c, 0x73, 0xc7, 0x1d, 0x53, 0xc4, 0x09, 0x92, 0x97,
    0x65, 0x8b, 0xac, 0xb4, 0xfd, 0x9d, 0x3c, 0xc7, 0x1d, 0x9c, 0x42, 0x2c, 0x3d, 0xce, 0x65,
    0x78, 0x6b, 0x14, 0xf1, 0xdb, 0x17, 0xc3, 0x3a, 0x82, 0xd8, 0x31, 0x70, 0x29, 0xe3, 0x81,
    0x3f, 0xc7, 0x83, 0xbd, 0x66, 0x84, 0xab, 0x28, 0xde, 0x67, 0x55, 0x52, 0xd4, 0xd4, 0x25,
    0x6d, 0xb8, 0xeb, 0x79, 0x4e, 0x57, 0xa1, 0xe4, 0xd3, 0x16, 0x1c, 0xed, 0x88, 0x86, 0x4f,
    0xfc, 0xf1
};
UINT32 g_PublicKey2048Len_D = 256U;


CHAR g_Publickey2048_E[] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01
};
UINT32 g_PublicKey2048Len_E = 256U;







/*
 *******************************************************************************
 *                               FUNCTIONS IMPLEMENT
 *******************************************************************************
*/
/** @ingroup MOUDLE_NAME_C_
 *- #Description  This function for handle command.
 * @param   pMsg           [IN] The received request message
 *                               - Type: MBX_Msg *
 *                               - Range: N/A.
 *
 * @return     void
 * @retval     void
 *
 *
 */
static void l_CryptoTaHandle_SetAes128Key(AesOperation* aesOper)
{
    aesOper->key = g_Aes128Key;
    aesOper->iv = g_Aes128Iv;
    aesOper->keyLen = 128U;
    aesOper->ivLen = 16U;
}

static void l_CryptoTaHandle_SetAesAction(AesOperation* aesOper, AesOperModeInfo modeInfo)
{
    switch(modeInfo.active)
    {
        case EN_OP_AES_ENCRYPT:
            aesOper->operMode = TEE_MODE_ENCRYPT;
            break;
        case EN_OP_AES_DECRYPT:
            aesOper->operMode = TEE_MODE_DECRYPT;
            break;
        default:
            break;
    }

    switch(modeInfo.mode)
    {
        case EN_MODE_CBC:
            aesOper->algorithmId= TEE_ALG_AES_CBC_NOPAD;
            break;
        case EN_MODE_ECB:
            aesOper->algorithmId = TEE_ALG_AES_ECB_NOPAD;
            break;
        case EN_MODE_CTR:
            aesOper->algorithmId = TEE_ALG_AES_CTR;
            break;
        case EN_MODE_CBC_CTS:
            aesOper->algorithmId = TEE_ALG_AES_CTS;
            break;
        default:
            break;
    }
}



static void l_CryptoTaHandle_SetRsa1024Key(RsaKey* rsaKey, UINT32 padding,UINT32  opMode)
{
    rsaKey->Ddata = g_Publickey1024_D;
    rsaKey->Ndata = g_Publickey1024_N;
    rsaKey->Edata = g_Publickey1024_E;
    rsaKey->Dlen = g_PublicKey1024Len_D;
    rsaKey->Nlen = g_PublicKey1024Len_N;
    rsaKey->Elen = g_PublicKey1024Len_E;
    if(PADDING_NO != padding)
    {
        if(EM_OP_SIGN == opMode ||EM_OP_VERIFY== opMode)
        rsaKey->padding = TEE_ALG_RSASSA_PKCS1_V1_5_SHA1;
        else
        rsaKey->padding = TEE_ALG_RSAES_PKCS1_V1_5;
    }
    else
    {
        rsaKey->padding = TEE_ALG_RSA_NOPAD;
    }
	UNUSED(padding);
    TF("The padding is 0x%x\n", rsaKey->padding);
}

static void l_CryptoTaHandle_SetRsa2048Key(RsaKey* rsaKey, UINT32 padding,UINT32  opMode)
{
    rsaKey->Ddata = g_Publickey2048_D;
    rsaKey->Ndata = g_Publickey2048_N;
    rsaKey->Edata = g_Publickey2048_E;
    rsaKey->Dlen = g_PublicKey2048Len_D;
    rsaKey->Nlen = g_PublicKey2048Len_N;
    rsaKey->Elen = g_PublicKey2048Len_E;
	if(PADDING_NO != padding)
    {
        if(EM_OP_SIGN == opMode ||EM_OP_VERIFY== opMode)
        rsaKey->padding = TEE_ALG_RSASSA_PKCS1_V1_5_SHA1;
        else
        rsaKey->padding = TEE_ALG_RSAES_PKCS1_V1_5;
    }
    else
    {
        rsaKey->padding = TEE_ALG_RSA_NOPAD;
    }
	UNUSED(padding);
    TF("The padding is 0x%x\n", rsaKey->padding);
}


int g_CryptoTaHandle_Random(uint32_t paramTypes, TEE_Param params[4])
{
    UINT32 l_RandomLen = 0U;
    CHAR* l_pBuf = NULL;
    UNUSED(paramTypes);

    /**1) Get the request length & point of responding buffer */
    l_RandomLen = params[0].memref.size;
    l_pBuf = params[0].memref.buffer;
    
    g_CryptoTaOther_Random(l_RandomLen, l_pBuf);
    
    return OK;
}



int g_CryptoTaHandle_Sha(uint32_t paramTypes, TEE_Param params[4])
{
    EN_SHA_MODE l_shaMode;
    CHAR* l_InputData= NULL;
    CHAR* l_OutPut = NULL;
    UINT32 l_InputLen = 0U;
    UINT32 l_OutputLen = 0U;
    UNUSED(paramTypes);

    /**1) Get the sha mode, input data info & output info */
    l_InputData = params[0].memref.buffer;
    l_InputLen = params[0].memref.size;
    l_shaMode = params[1].value.a;
    l_OutPut = params[2].memref.buffer;
    l_OutputLen = params[2].memref.size;

    /**2) Do sha operation */
    g_CryptoTaHash_sha(l_shaMode, l_InputData, l_InputLen, l_OutPut, &l_OutputLen);
    
    return OK;
}


int g_CryptoTaHandle_hmac(uint32_t paramTypes, TEE_Param params[4])
{
    CHAR* l_OutPut = NULL;
    UINT32 l_OutputLen = 0U;
    UINT32 l_Count = 0U;
    UNUSED(paramTypes);

    /**1) Get the sha mode, input data info & output info */
    l_OutPut = params[0].memref.buffer;
    l_OutputLen = params[0].memref.size;
    l_Count = params[1].value.a;


    /**2) Do HMAC operation */
    g_CryptoTaHash_hmac(l_OutPut, l_OutputLen, g_pwdBuf, 18, g_SaltBuf, 22, l_Count);

    
    return OK;
}





int g_CryptoTaHandle_Aes(uint32_t paramTypes, TEE_Param params[4])
{
    AesOperation l_aesOper;
    AesOperModeInfo l_pAesModeInfo;
    CHAR test[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
    UNUSED(paramTypes);

    TF("Start to do AES operation!!!!\n");
    /**1) Get the sha mode, input data info & output info */
    l_pAesModeInfo.active = params[0].value.a;
    l_pAesModeInfo.mode = params[0].value.b;
    l_aesOper.inBuf = params[1].memref.buffer;
    l_aesOper.outBuf = params[2].memref.buffer;
    l_aesOper.dataLen = params[3].value.a;
    TEE_MemMove(l_aesOper.outBuf, test, sizeof(test));

    /**2) Put key & IV info into variable */
    l_CryptoTaHandle_SetAes128Key(&l_aesOper);

    /**3) Set the algorithm & mode */
    l_CryptoTaHandle_SetAesAction(&l_aesOper, l_pAesModeInfo);
    TF("ID: 0x%x, mode: 0x%x\n", l_aesOper.algorithmId, l_aesOper.operMode);
    
    /**4) Do AES operation */
    g_CryptoTaAes_AesOper(l_aesOper);

    return OK;
}






int g_CryptoTaHandle_Pbkdf(uint32_t paramTypes, TEE_Param params[4])
{
    CHAR* l_pResData= NULL;
    UINT32 l_ResLen = 0U;
    UNUSED(paramTypes);

    /**1) Get the sha mode, input data info & output info */
    l_pResData = params[0].memref.buffer;
    l_ResLen = params[0].memref.size;

    /**2) Do sha operation */
    g_CryptoTaPbkdf_PBKDF2(g_pwdBuf, 18, g_SaltBuf, 22, 1024, l_ResLen, l_pResData);
    
    return OK;
}




int g_CryptoTaHandle_Rsa(uint32_t paramTypes, TEE_Param params[4], UINT32 opMode, UINT32 padding)
{
    RsaKey l_rsaOper;
    CHAR* l_InputData= NULL;
    CHAR* l_OutPut = NULL;
    UINT32 l_InputLen = 0U;
    UINT32 l_OutputLen = 0U;
	EN_RSA_KEY_TYPE l_KeyType = EN_KEY_INVALID;
	UNUSED(paramTypes);

    /**1) Get the sha mode, input data info & output info */
    l_InputData = params[0].memref.buffer;
    l_InputLen = params[0].memref.size;
    l_OutPut = params[1].memref.buffer;
    l_OutputLen = params[1].memref.size;
    l_KeyType = params[2].value.a;

    if(EN_KEY_1024 == l_KeyType)
    {
        l_CryptoTaHandle_SetRsa1024Key(&l_rsaOper, padding,opMode);
        TF("The set pad is 0x%x\n", padding);
    }
    else if(EN_KEY_2048 == l_KeyType)
    {
        l_CryptoTaHandle_SetRsa2048Key(&l_rsaOper, padding,opMode);
        TF("111The set pad is 0x%x\n", padding);
        TF("The alg is :0x%x\n", l_rsaOper.padding);
    }
    else
    {
    }
    /**2) Do sha operation */
    switch(opMode)
    {
        case EM_OP_ENC:
            TF("ENC OPER\n");
            g_CryptoTaRsa_rsaEncOper(l_rsaOper, l_InputData, l_OutPut, l_InputLen, &l_OutputLen);
            break;
        case EM_OP_DEC:
            TF("DEC OPER\n");
            g_CryptoTaRsa_rsaDecOper(l_rsaOper, l_InputData, l_OutPut, l_InputLen, &l_OutputLen);
            break;
        case EM_OP_SIGN:
            TF("SIGN OPER\n");
            g_CryptoTaRsa_rsaSignOper(l_rsaOper, l_InputData, l_OutPut, l_InputLen, &l_OutputLen);
            break;
        case EM_OP_VERIFY:
            TF("VERIFY OPER\n");
            g_CryptoTaRsa_rsaVerifyOper(l_rsaOper, l_InputData, l_OutPut, l_InputLen, &l_OutputLen);
            break;
        default:
            break;
    }
    
    return OK;
}




int g_CryptoTaHandle_base64(uint32_t paramTypes, TEE_Param params[4])
{
    CHAR* l_pResData= NULL;
    UINT32 l_ResLen = 0U;
    CHAR* l_pInput = NULL;
    UINT32 l_LenIn = 0U;
    UINT32 l_Oper = 0U;
    UNUSED(paramTypes);

    /**1) Get the sha mode, input data info & output info */
    l_pInput = params[0].memref.buffer;
    l_LenIn = params[0].memref.size;
    l_pResData = params[1].memref.buffer;
    l_ResLen = params[1].memref.size;
    l_Oper = params[2].value.a;
    TF("The input buffer data just like follow(%d):\n", l_LenIn);
    g_TA_Printf(l_pInput, l_LenIn);

    /**2) Do sha operation */
    g_CryptoTaOther_base64(l_pInput, l_LenIn, l_pResData, l_ResLen, l_Oper);

    TF("The output buffer data just like follow:\n");
    g_TA_Printf(l_pResData, l_ResLen);


    return OK;
}









/**
 * @}
 */
