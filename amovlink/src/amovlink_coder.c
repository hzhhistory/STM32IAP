/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-09-04 09:18:06
 * @LastEditors: L LC @amov
 * @LastEditTime: 2024-05-23 16:28:57
 * @FilePath: \SFP210-gimbal-powerControl\common\amovLink\src\amovlink_coder.c
 */
#include "amovlink_coder.h"
#include "string.h"
#include "amovlink_check.h"

/**
 * The function `amovLinkCoder` encodes a system message into a custom protocol and sends it over a
 * communication interface.
 *
 * @param io io is a pointer to an AMOVLINK_IO_T structure, which likely contains function pointers for
 * sending and receiving bytes.
 * @param source The source parameter is a uint8_t variable that represents the source of the message.
 * @param msgOut The parameter `msgOut` is a pointer to a structure of type `AMOVLINK_SYS_MESSAGE_T`.
 */
uint16_t amovLinkCoder(AMOVLINK_IO_T *io, uint8_t source, AMOVLINK_SYS_MESSAGE_T *msgOut)
{
    if (io == NULL)
    {
        return false;
    }
    AMOVLINK_MESSAGE_T txTemp;

    txTemp.head = AMOVLINK_HEAD;
    txTemp.version = AMOVLINK_VERSION;
    txTemp.source = source;
    txTemp.command = msgOut->cmd;
    txTemp.target = msgOut->target;
    txTemp.len = msgOut->len;
    memcpy(txTemp.data, msgOut->data, txTemp.len);
    *(uint16_t *)(txTemp.data + txTemp.len) = amovLinkCheckCrc16((uint8_t *)&txTemp, txTemp.len + 6);

    return io->sendBytes((uint8_t *)&txTemp, txTemp.len + 8,io->userHandle);
}
