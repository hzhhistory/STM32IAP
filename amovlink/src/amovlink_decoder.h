/*
 * @Description: 
 * @Author: L LC @amov
 * @Date: 2023-09-04 09:18:06
 * @LastEditors: L LC @amov
 * @LastEditTime: 2023-09-04 09:27:01
 * @FilePath: \amovLink\src\amovlink_decoder.h
 */
#ifndef _AMOVLINK_DECODER_H 
#define _AMOVLINK_DECODER_H 

#include "amovlink.h"

void amovLinkDecoderInit(AMOVLINK_IO_T *io);
bool amovLinkDecoder(AMOVLINK_IO_T *io, uint8_t byte, AMOVLINK_SYS_MESSAGE_T *msgOut);

#endif
