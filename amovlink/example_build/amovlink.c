/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-09-04 19:31:59
 * @LastEditors: L LC @amov
 * @LastEditTime: 2023-09-18 11:37:13
 * @FilePath: \SFP210-gimbal-powerControl\common\amovLink\example_build\amovlink.c
 */

#include "amovlink.h"

#include "stdio.h"
#include "string.h"
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>

int rcvScoketFd = -1, sendSocketFd = -1;
struct sockaddr_in rcvAddr, sendAddr;

uint16_t getBytes(uint8_t *bytes, void *handler)
{
    int len = sizeof(rcvAddr);
    int lenght = recvfrom(rcvScoketFd, (char *)bytes, 65536, 0,
                          (struct sockaddr *)&rcvAddr, &len);
    // for (uint16_t i = 0; i < lenght; i++)
    // {
    //     printf("%02X,", bytes[i]);
    // }
    // printf("rcv\r\n");
    return (lenght = lenght > 0 ? lenght : 0);
}
uint16_t sendBytes(uint8_t *bytes, uint16_t len, void *handler)
{
    // for (uint8_t i = 0; i < len; i++)
    // {
    //     printf("%02X,", bytes[i]);
    // }
    // printf("send\r\n");
    int lenght = sendto(sendSocketFd, (const char *)bytes, len, 0,
                        (struct sockaddr *)&sendAddr,
                        sizeof(sendAddr));
    return (lenght > 0 ? lenght : 0);
}
int main(int argc, char **argv)
{
    rcvScoketFd = socket(AF_INET, SOCK_DGRAM, 0);
    sendSocketFd = socket(AF_INET, SOCK_DGRAM, 0);
    if (rcvScoketFd == -1 || sendSocketFd == -1)
    {
        close(rcvScoketFd);
        close(sendSocketFd);
        printf("creat\r\n");
        return -1;
    }

    memset(&rcvAddr, 0, sizeof(rcvAddr));
    memset(&sendAddr, 0, sizeof(sendAddr));

    sendAddr.sin_family = AF_INET;
    sendAddr.sin_addr.s_addr = inet_addr(argv[1]);
    sendAddr.sin_port = htons(atoi(argv[2]));

    if (sendAddr.sin_addr.s_addr == INADDR_NONE ||
        sendAddr.sin_addr.s_addr == INADDR_ANY)
    {
        close(sendSocketFd);
        printf("addr\r\n");
        return -1;
    }

    rcvAddr.sin_family = AF_INET;
    rcvAddr.sin_addr.s_addr = INADDR_ANY;
    rcvAddr.sin_port = htons(atoi(argv[3]));

    if (rcvAddr.sin_addr.s_addr == INADDR_NONE)
    {
        close(rcvScoketFd);
        printf("rcv addr\r\n");
        return -1;
    }

    if (bind(rcvScoketFd, (struct sockaddr *)&rcvAddr, sizeof(rcvAddr)) == -1)
    {
        close(rcvScoketFd);
        return -1;
    }

    printf("send: %d\r\n", sendAddr.sin_port);
    printf("rcv: %d\r\n", rcvAddr.sin_port);

    AMOVLINK_IO_T temp;
    amovLinkInit();
    amovLinkIoReg(&temp, getBytes, sendBytes, NULL);
    AMOVLINK_SYS_MESSAGE_T heart = {.cmd = 20, .len = 0, .target = 0XFF};
    AMOVLINK_SYS_MESSAGE_T cmd = {.cmd = 192, .len = 0, .target = 0X02};
    int count = 1000;
    while (1)
    {
        sleep(1);
        amovLinkSendMsg(&heart);
        amovLinkSendMsg(&cmd);
        amovLinkHandle();
    }
}