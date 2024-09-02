/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-09-01 11:25:55
 * @LastEditors: L LC @amov
 * @LastEditTime: 2024-06-12 16:30:14
 * @FilePath: \SFP210-gimbal-powerControl\common\amovLink\src\amovlink_route.c
 */
#include "stddef.h"
#include "amovlink.h"
#include "amovlink_decoder.h"
#include "amovlink_coder.h"
#include "string.h"
#include"SEGGER_RTT.h"
typedef struct
{
    AMOVLINK_IO_T *io;
    uint8_t sourceId;
} AMOVLINK_ROUTE_LIST_T;
// 存放通道信息
static AMOVLINK_IO_T *amovLinkIos[AMOVLINK_MAX_CHANNEL];
// 路由表信息
static AMOVLINK_ROUTE_LIST_T amovRouteList[AMOVLINK_MAX_NODE_NUM];

/**
 * The function initializes several arrays and variables used in the amovLink module.
 */
void amovLinkInit(void)
{
    uint8_t i = 0;
    for (i = 0; i < AMOVLINK_MAX_CHANNEL; i++)
    {
        amovLinkIos[i] = NULL;
    }
    for (i = 0; i < AMOVLINK_MAX_NODE_NUM; i++)
    {
        amovRouteList[i].io = NULL;
        amovRouteList[i].sourceId = 0XFF;
    }
}

/**
 * The function `amovLinkIoReg` registers an `AMOVLINK_IO_T` object in an array of `amovLinkIos` and
 * returns `true` if successful.
 *
 * @param io The parameter "IO" is a pointer to a structure of type "AMOVLINK_IO_T".
 *
 * @return a boolean value.
 */
bool amovLinkIoReg(AMOVLINK_IO_T *io,
                   uint16_t (*getBytes)(uint8_t *, void *),
                   uint16_t (*sendBytes)(uint8_t *, uint16_t, void *),
                   void *userHandle)
{
    uint8_t i = 0;
    bool ret = false;

    io->getBytes = getBytes;
    io->sendBytes = sendBytes;
    io->userHandle = userHandle;

    AMOVLINK_LOCK
    for (i = 0; i < AMOVLINK_MAX_CHANNEL; i++)
    {
        if (amovLinkIos[i] == NULL)
        {
            amovLinkIos[i] = io;
            amovLinkDecoderInit(amovLinkIos[i]);
            ret = true;
            break;
        }
    }
    AMOVLINK_UNLOCK
    return ret;
}

/**
 * The function `amovLinkIoDeReg` removes a specified AMOVLINK_IO_T structure from an array and updates
 * related data structures accordingly.
 *
 * @param io The `io` parameter in the `amovLinkIoDeReg` function is a pointer to an `AMOVLINK_IO_T`
 * structure. This function is designed to remove this `io` pointer from the `amovLinkIos` array and
 * also update the corresponding entries in the
 *
 * @return The function `amovLinkIoDeReg` returns a boolean value indicating whether the operation was
 * successful or not. If the specified `AMOVLINK_IO_T *io` was found and removed from the `amovLinkIos`
 * array and `amovRouteList` array, then the function returns `true`. Otherwise, it returns `false`.
 */
bool amovLinkIoDeReg(AMOVLINK_IO_T *io)
{
    uint8_t i = 0, j = 0;
    bool ret = false;

    for (i = 0; i < AMOVLINK_MAX_CHANNEL; i++)
    {
        if (amovLinkIos[i] == io)
        {
            // remove amovLinkIos
            memmove(amovLinkIos + i, amovLinkIos + i + 1, (AMOVLINK_MAX_CHANNEL - 1 - i) * sizeof(AMOVLINK_IO_T *));
            amovLinkIos[AMOVLINK_MAX_CHANNEL - 1] = NULL;
            // remove amovRouteList
            for (j = 0; j < AMOVLINK_MAX_NODE_NUM; j++)
            {
                if (amovRouteList[i].io == io)
                {
                    amovRouteList[i].io = NULL;
                    amovRouteList[i].sourceId = 0XFF;
                    memmove(amovRouteList + i, amovRouteList + 1 + i, (AMOVLINK_MAX_NODE_NUM - 1 - j) * sizeof(AMOVLINK_ROUTE_LIST_T));
                }
            }
            ret = true;
            break;
        }
    }
    return ret;
}

/**
 * The function `amovLinkRouteCheck` assigns an `AMOVLINK_IO_T` pointer to a specific index in the
 * `amovRouteList` array based on the `sourceId` parameter.
 *
 * @param io A pointer to an AMOVLINK_IO_T structure.
 * @param sourceId The sourceId parameter is an 8-bit unsigned integer that represents the ID of the
 * source for which the route is being checked.
 */
static void amovLinkRouteCheck(AMOVLINK_IO_T *io, uint8_t sourceId)
{
    uint8_t i = 0;
    for (i = 0; i < AMOVLINK_MAX_NODE_NUM; i++)
    {
        // updata route list
        if (amovRouteList[i].sourceId == sourceId)
        {
            amovRouteList[i].io = io;
            break;
        }
        // reg route list
        if (amovRouteList[i].sourceId == 0XFF)
        {
            amovRouteList[i].sourceId = sourceId;
            amovRouteList[i].io = io;
            break;
        }
    }
}

/**
 * The function "amovLinkRouteScan" scans a list of AMOVLINK nodes and returns the AMOVLINK_IO_T
 * pointer associated with the targetId.
 *
 * @param targetId The targetId parameter is an 8-bit unsigned integer that represents the ID of the
 * target node for which we want to find the corresponding AMOVLINK_IO_T structure.
 *
 * @return a pointer to an AMOVLINK_IO_T structure.
 */
static AMOVLINK_IO_T *amovLinkRouteScan(uint8_t targetId)
{
    uint8_t i = 0;
    AMOVLINK_IO_T *io = NULL;
    for (i = 0; i < AMOVLINK_MAX_NODE_NUM; i++)
    {
        if (amovRouteList[i].sourceId == targetId)
        {
            io = amovRouteList[i].io;
            break;
        }
        else if (amovRouteList[i].sourceId == 0XFF)
        {
            break;
        }
    }
    // 如果没有其他iap节点 则第一个io作为默认iap io使用
    if (io == NULL && targetId == 0X01)
    {
        io = amovLinkIos[0];
    }
    return io;
}

#if defined(__linux__) || defined(_WIN32)
#include "malloc.h"
typedef struct amovlink_cb
{
    pAmovLinkCB callback;
    struct amovlink_cb *next;
} AMOVLINK_CB_LIST_T;
static AMOVLINK_CB_LIST_T head = {.next = NULL, .callback = NULL};
bool AMOVLINK_REG_MOUDLE_CB(char *name, pAmovLinkCB pFunCb)
{
    AMOVLINK_CB_LIST_T *temp = &head;
    bool ret = false;
    while (temp->next != NULL)
    {
        if (temp->callback == pFunCb)
        {
            return ret;
        }
        temp = temp->next;
    }
    // creat new callback
    // head nod
    if (temp == &head)
    {
        head.callback = pFunCb;
        ret = true;
    }
    else
    {
        temp->next = (AMOVLINK_CB_LIST_T *)calloc(1, sizeof(AMOVLINK_CB_LIST_T));
        if (temp->next != NULL)
        {
            temp->next->callback = pFunCb;
            ret = true;
        }
        else
        {
            ret = false;
        }
    }

    return ret;
}
#else
extern const int __AMOVLinkCBTab_start;
extern const int __AMOVLinkCBTab_end;
#endif
/**
 * The function amovLinkCbScan iterates through a table of function pointers and calls each function
 * with a given message.
 *
 * @param msg The parameter "msg" is a pointer to an AMOVLINK_SYS_MESSAGE_T structure.
 */
static void amovLinkCbScan(AMOVLINK_SYS_MESSAGE_T *msg)
{
#if defined(__linux__) || defined(_WIN32)

    AMOVLINK_CB_LIST_T *temp = &head;

    while (temp->callback != NULL)
    {
        temp->callback(msg);
        temp = temp->next;
        if (temp == NULL)
        {
            return;
        }
    }

#else
    int i = (int)&__AMOVLinkCBTab_start;
    int end = (int)&__AMOVLinkCBTab_end;
SEGGER_RTT_printf(0,"%d,%d\r\n",i,end);

    while (i < end)
    {
        (**((pAmovLinkCB *)i))(msg);
        i += sizeof(pAmovLinkCB);
    }
#endif
}

/**
 * The function `amovLinkHandle` handles incoming messages in a communication system and performs
 * routing and forwarding based on the target of the message.
 */

void amovLinkHandle(void)
{
    uint8_t i = 0, j = 0, k = 0;
    AMOVLINK_SYS_MESSAGE_T msgTemp;
    static uint8_t temp[AMOVLINK_MAX_CACHE];
    uint16_t getCount = 0;
    for (i = 0; amovLinkIos[i] != NULL; i++)
    {
        getCount = amovLinkIos[i]->getBytes(temp, amovLinkIos[i]->userHandle);

        for (k = 0; k < getCount; k++)
        {
            if (amovLinkDecoder(amovLinkIos[i], temp[k], &msgTemp))
            {
                // 处理回环信息 同端口接收的udp会存在回环问题
                if (msgTemp.source == AMOVLINK_LOCAL_ID)
                {
                    continue;
                }

                // 建立路由表
                amovLinkRouteCheck(amovLinkIos[i], msgTemp.source);

                // 处理本地信息
                if (msgTemp.target == AMOVLINK_LOCAL_ID)
                {
                    amovLinkCbScan(&msgTemp);
                    SEGGER_RTT_printf(0,"1\r\n");
                }
                else if (msgTemp.target == 0XFF)
                {
                    // 处理广播信息
                    amovLinkCbScan(&msgTemp);
                    // 转发至所有非来源IO
                    for (j = 0; j < AMOVLINK_MAX_CHANNEL; j++)
                    {
                        if (j == i)
                        {
                            continue;
                        }
                        amovLinkCoder(amovLinkIos[j], msgTemp.source, &msgTemp);
                    }
                }
                // 通过路由表转发
                else
                {
                    amovLinkCoder(amovLinkRouteScan(msgTemp.target), msgTemp.source, &msgTemp);
                }
            }
        }
    }
}

/**
 * The function amovLinkSendMsg sends a message to a target using the amovLinkCoder function.
 *
 * @param msgOut The parameter `msgOut` is a pointer to a structure of type `AMOVLINK_SYS_MESSAGE_T`.
 *
 * @return a boolean value.
 */
bool amovLinkSendMsg(AMOVLINK_SYS_MESSAGE_T *msgOut)
{
    bool ret = false;
    if (msgOut->target == 0XFF)
    {
        uint8_t i = 0;
        for (i = 0; amovLinkIos[i] != NULL; i++)
        {
            amovLinkCoder(amovLinkIos[i], AMOVLINK_LOCAL_ID, msgOut);
        }
        ret = true;
    }
    else if (amovLinkCoder(amovLinkRouteScan(msgOut->target), AMOVLINK_LOCAL_ID, msgOut))
    {
        ret = true;
    }

    return ret;
}