/*
 * @Description: 
 * @Author: L LC @amov
 * @Date: 2023-09-04 09:18:06
 * @LastEditors: L LC @amov
 * @LastEditTime: 2023-09-18 11:36:07
 * @FilePath: \SFP210-gimbal-powerControl\common\amovLink\src\amovlink_coder.h
 */
#ifndef _AMOVLINK_CODER_H 
#define _AMOVLINK_CODER_H 

#include "amovlink.h"

uint16_t amovLinkCoder(AMOVLINK_IO_T *io, uint8_t source, AMOVLINK_SYS_MESSAGE_T *msgOut);

#endif
