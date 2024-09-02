/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-09-01 12:30:18
 * @LastEditors: L LC @amov
 * @LastEditTime: 2024-06-12 16:21:42
 * @FilePath: \SFP210-gimbal-powerControl\common\amovLink\src\amovlink.h
 */
#ifndef __AMOVLINK_H
#define __AMOVLINK_H

#include "stdint.h"
#include "stdbool.h"
#include "amovlink_message.h"

typedef void (*pAmovLinkCB)(AMOVLINK_SYS_MESSAGE_T *);

#define AMOVLINK_HEAD 0XAF
#define AMOVLINK_VERSION 0X02

#define AMOVLINK_LOCK
#define AMOVLINK_UNLOCK
typedef struct
{
    uint16_t (*getBytes)(uint8_t *, void *);
    uint16_t (*sendBytes)(uint8_t *, uint16_t, void *);
    void *userHandle;
    AMOVLINK_MESSAGE_T rxTemp;
    AMOVLINK_MESSAGE_STATE_T state;
    uint8_t dataLenght;
    uint8_t *pDataRx;
} AMOVLINK_IO_T;

void amovLinkInit(void);
bool amovLinkIoReg(AMOVLINK_IO_T *io,
                   uint16_t (*getBytes)(uint8_t *, void *),
                   uint16_t (*sendBytes)(uint8_t *, uint16_t, void *),
                   void *userHandle);
bool amovLinkIoDeReg(AMOVLINK_IO_T *io);
void amovLinkHandle(void);
bool amovLinkSendMsg(AMOVLINK_SYS_MESSAGE_T *msgOut);

#if defined(__linux__) || defined(_WIN32)
bool AMOVLINK_REG_MOUDLE_CB(char *name, pAmovLinkCB pFunCb);
#else
#define AMOVLINK_REG_MOUDLE_CB(name, pFunCb) pAmovLinkCB name##CB __attribute__((section(".AMOVLinkCBTab"))) = pFunCb
#endif

#endif