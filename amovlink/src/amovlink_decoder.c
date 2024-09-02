/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-09-01 11:57:30
 * @LastEditors: L LC @amov
 * @LastEditTime: 2024-05-23 16:29:16
 * @FilePath: \SFP210-gimbal-powerControl\common\amovLink\src\amovlink_decoder.c
 */
#include "amovlink_decoder.h"
#include "string.h"
#include "amovlink_check.h"

/**
 * The function initializes the rxTemp variable in the AMOVLINK_IO_T struct to zero.
 *
 * @param io io is a pointer to an AMOVLINK_IO_T structure.
 */
void amovLinkDecoderInit(AMOVLINK_IO_T *io)
{
    memset(&io->rxTemp, 0, sizeof(AMOVLINK_MESSAGE_T));
    io->state = AMOVLINK_MESSAGE_STATE_IDEL;
}

/**
 * The function `amovLinkDecoder` decodes a message received over a communication link and checks its
 * integrity using a CRC checksum.
 *
 * @param io io is a pointer to a structure of type AMOVLINK_IO_T. This structure contains information
 * about the input/output operations for the AMOVLINK communication protocol.
 * @param msgOut A pointer to an AMOVLINK_SYS_MESSAGE_T structure where the decoded message will be
 * stored.
 *
 * @return a boolean value.
 */
bool amovLinkDecoder(AMOVLINK_IO_T *io, uint8_t byte, AMOVLINK_SYS_MESSAGE_T *msgOut)
{
    bool ret = false;
    switch (io->state)
    {
    case AMOVLINK_MESSAGE_STATE_IDEL:
        if (byte == AMOVLINK_HEAD)
        {
            io->rxTemp.head = byte;
            io->state = AMOVLINK_MESSAGE_STATE_HEAD_RCV;
        }
        break;
    case AMOVLINK_MESSAGE_STATE_HEAD_RCV:
        if (byte == AMOVLINK_VERSION)
        {
            io->rxTemp.version = byte;
            io->state = AMOVLINK_MESSAGE_STATE_VERSION_RCV;
        }
        else
        {
            io->state = AMOVLINK_MESSAGE_STATE_IDEL;
        }
        break;
    case AMOVLINK_MESSAGE_STATE_VERSION_RCV:
        io->rxTemp.target = byte;
        io->state = AMOVLINK_MESSAGE_STATE_TARGET_RCV;
        break;
    case AMOVLINK_MESSAGE_STATE_TARGET_RCV:
        io->rxTemp.source = byte;
        io->state = AMOVLINK_MESSAGE_STATE_SOURCE_RCV;
        break;
    case AMOVLINK_MESSAGE_STATE_SOURCE_RCV:
        io->rxTemp.len = byte;
        io->state = AMOVLINK_MESSAGE_STATE_LENGHT_RCV;
        io->pDataRx = io->rxTemp.data;
        io->dataLenght = byte;
        break;
    case AMOVLINK_MESSAGE_STATE_LENGHT_RCV:
        io->rxTemp.command = byte;
        if (io->dataLenght > 0)
        {
            io->state = AMOVLINK_MESSAGE_STATE_DATA_RCV;
        }
        else
        {
            io->state = AMOVLINK_MESSAGE_STATE_CRC_RCV1;
        }

        break;
    case AMOVLINK_MESSAGE_STATE_DATA_RCV:
        *io->pDataRx = byte;
        io->dataLenght--;
        io->pDataRx++;
        if (io->dataLenght == 0)
        {
            io->state = AMOVLINK_MESSAGE_STATE_CRC_RCV1;
        }
        break;
    case AMOVLINK_MESSAGE_STATE_CRC_RCV1:
        io->rxTemp.crc.f8[0] = byte;
        io->state = AMOVLINK_MESSAGE_STATE_END;
        break;
    case AMOVLINK_MESSAGE_STATE_END:
        io->rxTemp.crc.f8[1] = byte;
        // 校验
        if (io->rxTemp.crc.f16 ==
            amovLinkCheckCrc16((uint8_t *)&io->rxTemp, 6 + io->rxTemp.len))
        {
            memcpy(msgOut->data, io->rxTemp.data, io->rxTemp.len);
            msgOut->cmd = io->rxTemp.command;
            msgOut->len = io->rxTemp.len;
            msgOut->target = io->rxTemp.target;
            msgOut->source = io->rxTemp.source;
            ret = true;
        }
        else
        {
            memset(&io->rxTemp, 0, sizeof(AMOVLINK_MESSAGE_T));
        }
        io->state = AMOVLINK_MESSAGE_STATE_IDEL;
        break;

    default:
        io->state = AMOVLINK_MESSAGE_STATE_IDEL;
        break;
    }

    return ret;
}