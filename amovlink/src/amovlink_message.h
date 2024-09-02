/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-09-01 11:40:49
 * @LastEditors: L LC @amov
 * @LastEditTime: 2023-10-08 17:11:57
 * @FilePath: \SFP210-gimbal-powerControl\common\amovLink\src\amovlink_message.h
 */
#ifndef _AMOVLINK_MESSAGE_H
#define _AMOVLINK_MESSAGE_H

#include "stdint.h"
#include "stdbool.h"

#pragma pack(1)

// 系统内部msg
typedef struct
{
    uint8_t cmd;
    uint8_t len;
    uint8_t target;
    uint8_t source;
    uint8_t data[64];
} AMOVLINK_SYS_MESSAGE_T;

// 外部msg定义
typedef struct
{
    uint8_t head;
    uint8_t version;
    uint8_t target;
    uint8_t source;
    uint8_t len;
    uint8_t command;
    uint8_t data[64];
    union
    {
        uint8_t f8[2];
        uint16_t f16;
    } crc;
} AMOVLINK_MESSAGE_T;
#pragma pack()

typedef enum
{
    AMOVLINK_MESSAGE_STATE_IDEL = 0,
    AMOVLINK_MESSAGE_STATE_HEAD_RCV,
    AMOVLINK_MESSAGE_STATE_VERSION_RCV,
    AMOVLINK_MESSAGE_STATE_TARGET_RCV,
    AMOVLINK_MESSAGE_STATE_SOURCE_RCV,
    AMOVLINK_MESSAGE_STATE_LENGHT_RCV,
    AMOVLINK_MESSAGE_STATE_DATA_RCV,
    AMOVLINK_MESSAGE_STATE_CRC_RCV1,
    AMOVLINK_MESSAGE_STATE_END,
} AMOVLINK_MESSAGE_STATE_T;

#endif
