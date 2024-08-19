/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-08-31 18:36:27
 * @LastEditors: L LC @amov
 * @LastEditTime: 2023-10-08 17:22:55
 * @FilePath: \SFP210-gimbal-powerControl\common\iap\src\iap.h
 */
#ifndef _IAP_H
#define _IAP_H

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    IAP_COMMAND_JUMP = 80,
    IAP_COMMAND_FLASH_ERASE,
    IAP_COMMAND_BOLCK_INFO,
    IAP_COMMAND_BLOCK_WRITE,
    IAP_COMMAND_SOFT_INFO,
    IAP_COMMAND_HARDWARE_INFO,
    IAP_COMMAND_BLOCK_START,
    IAP_COMMAND_BLOCK_END = 117,
    IAP_COMMAND_CHECK_ALL,
    IAP_COMMAND_END = 127,
} IAP_COMMAND_T;

typedef enum
{
    IAP_STATE_FAILD = 0,
    IAP_STATE_SUCCEED,
} IAP_STATE_T;

#pragma pack(1)
typedef struct
{
    uint8_t type;
    uint8_t len;
    uint8_t reserve[2]; // reserve 2 bytes to aligned
    uint8_t data[64];
} IAP_MESSAGE_T;
#pragma pack()

void iapInit(bool (*getMessage)(void *),
             bool (*sendMessage)(void *),
             void (*debug)(const char *, ...),
             void (*custom)(void)
#ifdef BOOT
                 ,
             uint32_t (*getSysTickMs)(void),
             IAP_STATE_T (*blockWrite)(uint32_t, uint8_t *, uint32_t),
             IAP_STATE_T (*blockErase)(uint32_t, uint32_t)
#endif
);
void iapHandle(void);

#endif