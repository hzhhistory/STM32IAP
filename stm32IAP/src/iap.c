/*
 * @Description:
 * @Author: L LC @amov
 * @Date: 2023-08-31 18:36:35
 * @LastEditors: L LC @amov
 * @LastEditTime: 2024-05-17 19:14:23
 * @FilePath: \F0\common\iap\src\iap.c
 */
#include "iap.h"
#include "iap_check.h"
#include "string.h"

static struct IAP_T
{
    bool (*getMessage)(void *);
    bool (*sendMessage)(void *);
    void (*debug)(const char *, ...);
#ifdef BOOT
    uint32_t (*getSysTickMs)(void);
    uint8_t (*blockWrite)(uint32_t, uint8_t *, uint32_t);
    uint8_t (*blockErase)(uint32_t, uint32_t);
    uint32_t jumpTick;
#endif
} iapTCB;
#pragma pack(1)
typedef struct
{
    uint32_t size;
    uint32_t appStart;
    char version[20];
    char buildInfo[30];
    uint32_t crc;
} FIRMWARE_INFO_T;
#pragma pack(0)

extern const volatile uint32_t __app_start;
extern const volatile int32_t _estack;
typedef void (*pFunction)(void);

// #include "misc.h"

// build info
char *softInfo = __DATE__;
char *hardwareInfo = __TIME__;
extern const volatile uint32_t __size;

// uint8_t __isr_vector_table[48] __attribute__((section(".ARM.__at_0x20000000")));
// CRC calculate by Computer Interface
__attribute__((section(".firmware_info"))) const FIRMWARE_INFO_T firmwareInfo =
    {
        .size = (volatile uint32_t) & __size,
#ifdef APP
        .appStart = (volatile uint32_t) & __app_start - 0X400,
        .version = APP_VERSION,
#endif
#ifdef BOOT
        .appStart = 0X8000000,
        .version = BOOT_VERSION,
#endif
        .buildInfo = __DATE__ "-"__TIME__,
        .crc = 0XCCCCCCCC};

/**
 * The function `IAPJump` checks if the application area is valid and jumps to the application code if
 * it is, otherwise it returns an error state.
 *
 * @return an IAP_STATE_T enum value.
 */
static IAP_STATE_T iapJump(void)
{
#if defined BOOT
    // 校验app区是否合法 核对SP指向地址是否为栈顶
    if (*(const volatile uint32_t *)&__app_start != (const volatile uint32_t) & _estack)
    {
        return IAP_STATE_FAILD;
    }

    // crc校验固件是否损坏 需要结合编译后脚本计算的 数据

    // volatile uint8_t *pCrcInfo = (uint8_t *)&firmwareInfo + 0X400;
    // volatile uint8_t *pCrcStart = (uint8_t *)&__app_start;
    // uint32_t crcValue = ((FIRMWARE_INFO_T *)pCrcInfo)->crc;
    // uint32_t crcLen = ((FIRMWARE_INFO_T *)pCrcInfo)->size;

    // if ((iapCheckCRC32(pCrcStart, ((FIRMWARE_INFO_T *)pCrcInfo)->size) != ((FIRMWARE_INFO_T *)pCrcInfo)->crc) &&
    //     (((FIRMWARE_INFO_T *)pCrcInfo)->crc != 0XCCCCCCCC))
    // {
    //     iapTCB.debug("%X,%X,%X,%X \r\n",pCrcStart,pCrcInfo,*&((FIRMWARE_INFO_T *)pCrcInfo)->size,*&((FIRMWARE_INFO_T *)pCrcInfo)->crc);
    //     return IAP_STATE_FAILD;
    // }

    pFunction app;
    // 起始pc指针存在 1 int 的偏移
    app = (pFunction) * (volatile uint32_t *)(&__app_start + 1);

    //  Set ISR Disable
#if defined(ARM_CM7) || defined(ARM_CM4) || defined(ARM_CM3)
    for (uint8_t i = 0; i < 8; i++)
    {
        *((volatile uint32_t *)(0xE000E000UL + 0x0100UL + 0x0080UL) + i) = 0xFFFFFFFF;
        *((volatile uint32_t *)(0xE000E000UL + 0x0100UL + 0x0180UL) + i) = 0xFFFFFFFF;
    }
    for (uint8_t i = 0; i < 240; i++)
    {
        *((volatile uint32_t *)(0xE000E000UL + 0x0100UL + 0x0300UL) + i) = 0x00000000;
    }
#elif defined(ARM_CM0) || defined(ARM_CM0PLUS)
    *(uint32_t *)(0xE000E000UL + 0x0100UL + 0x0080UL) = 0xFFFFFFFF;
    *(uint32_t *)(0xE000E000UL + 0x0100UL + 0x0180UL) = 0xFFFFFFFF;
    for (uint8_t i = 0; i < 8; i++)
    {
        *((uint32_t *)(0xE000E000UL + 0x0100UL + 0x0300UL) + i) = 0x00000000;
    }
#else
#error "Define according the used Cortex core ARM_CM7, ARM_CM4, ARM_CM3, ARM_CM0PLUS or ARM_CM0"
#endif

    // Reset systick
    *(volatile uint32_t *)(0xE000E000UL + 0x0010UL) = 0x00000000;
    *(volatile uint32_t *)(0xE000E000UL + 0x0010UL + 0X0008UL) = 0x00000000;

    // 设置新的向量表
#if defined(ARM_CM7) || defined(ARM_CM4) || defined(ARM_CM3)
    *(volatile uint32_t *)(0xE000E000UL + 0x0D00UL + 0X0008UL) = __app_start;
#elif defined(ARM_CM0)
    extern const volatile uint32_t __isr_vector_size;
    memcpy((uint8_t *)0x20000000, (uint8_t *)__app_start, __isr_vector_size);
    uint32_t tmpctrl = *(volatile uint32_t *)0x40010000;
    tmpctrl &= (~((uint32_t)0x00000003));
    tmpctrl |= 0x03;
    *(volatile uint32_t *)0x40010000 = tmpctrl;
#elif defined(ARM_CM0PLUS)
#error "Not supported ARM_CM0 & ARM_CM0PLUS yet"
#else
#error "Define according the used Cortex core ARM_CM7, ARM_CM4, ARM_CM3, ARM_CM0PLUS or ARM_CM0"
#endif

    // NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t)(((volatile int32_t *)&__app_start) - 0x8000000));

    // Use MSP
#if defined USED_OS
    __asm volatile("MSR psp, %0\n"
                   :
                   : "r"(*(volatile int32_t *)&__app_start)
                   :);
#endif

    __asm volatile("MSR control, %0"
                   :
                   : "r"(0)
                   : "memory");
    __asm volatile("MSR msp, %0\n"
                   :
                   : "r"(*(volatile int32_t *)&__app_start)
                   :);
    app();
#elif defined APP

    __asm volatile("MSR primask, %0"
                   :
                   : "r"(1)
                   : "memory");

    __asm volatile("dsb 0xF" ::
                       : "memory");
#if defined(ARM_CM7) || defined(ARM_CM4) || defined(ARM_CM3)
    *(volatile uint32_t *)(0xE000E000UL + 0x0D00UL + 0X000CUL) = (uint32_t)((0x5FAUL << 16U) |
                                                                            (*(volatile uint32_t *)(0xE000E000UL + 0x0D00UL + 0X000CUL) & (7UL << 8)) |
                                                                            (1UL << 2));
#elif defined(ARM_CM0) || defined(ARM_CM0PLUS)
    *(volatile uint32_t *)(0xE000E000UL + 0x0D00UL + 0X000CUL) = (uint32_t)((0x5FAUL << 16) |
                                                                            (1UL << 2));
#else
#error "Define according the used Cortex core ARM_CM7, ARM_CM4, ARM_CM3, ARM_CM0PLUS or ARM_CM0"
#endif

    __asm volatile("dsb 0xF" ::
                       : "memory");
    while (1)
    {
        __asm volatile("nop");
    }

#endif

    return IAP_STATE_FAILD;
}

/**
 * The function `iapInit` initializes the IAP (In-Application Programming) module by setting function
 * pointers and sending an initialization message.
 *
 * @param check32 A function pointer that takes a pointer to a uint8_t array and a uint16_t value as
 * arguments and returns a uint32_t value. This function is used to check the integrity of a block of
 * data by calculating a checksum.
 * @param getSysTickMs The `getSysTickMs` parameter is a function pointer that points to a function
 * that returns a `uint32_t` value representing the current system tick in milliseconds. This function
 * is used to get the current system time for timing purposes in the IAP module.
 * @param blockWrite The "blockWrite" parameter is a function pointer that points to a function
 * responsible for writing data to a specific block of memory. It takes three arguments:
 * @param blockErase A function pointer that points to a function responsible for erasing a block of
 * memory. It takes two parameters: the starting address of the block to be erased and the size of the
 * block.
 * @param getMessage The `getMessage` parameter is a function pointer that points to a function that
 * retrieves an IAP_MESSAGE_T structure from a message queue or buffer. This function is responsible
 * for receiving incoming messages from the communication interface.
 * @param sendMessage A function pointer that takes a pointer to an IAP_MESSAGE_T structure as an
 * argument and returns a boolean value indicating whether the message was successfully sent or not.
 * @param debug A function pointer to a function that takes a string and variable arguments and is used
 * for debugging purposes. It is called to print debug messages.
 * @param custom The "custom" parameter is a function pointer that allows you to provide your own
 * custom implementation or behavior within the "iapInit" function. You can define your own function
 * and pass it as an argument to the "iapInit" function. This allows you to extend or modify the
 * functionality of the "
 */
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
)
{
    IAP_MESSAGE_T message;
    iapTCB.debug = debug;
    iapTCB.getMessage = getMessage;
    iapTCB.sendMessage = sendMessage;
#ifdef APP
    // 设置新的向量表
#if defined(ARM_CM7) || defined(ARM_CM4) || defined(ARM_CM3)
    // *(volatile uint32_t *)(0xE000E000UL + 0x0D00UL + 0X0008UL) = __app_start;
#elif defined(ARM_CM0)
    // extern const volatile uint32_t __isr_vector_size;
    // memcpy((uint8_t *)0x20000000, (uint8_t *)__app_start, __isr_vector_size);
    // uint32_t tmpctrl = *(volatile uint32_t *)0x40010000;
    // tmpctrl &= (~((uint32_t)0x00000003));
    // tmpctrl |= 0x03;
    // *(volatile uint32_t *)0x40010000 = tmpctrl;
#elif defined(ARM_CM0PLUS)
#error "Not supported ARM_CM0 & ARM_CM0PLUS yet"
#else
#error "Define according the used Cortex core ARM_CM7, ARM_CM4, ARM_CM3, ARM_CM0PLUS or ARM_CM0"
#endif
#ifdef _DEBUG_
    iapTCB.debug("APP Run!!!\r\n");
#endif
    message.type = IAP_COMMAND_SOFT_INFO;
    message.len = strlen(softInfo);
    memcpy(message.data, softInfo, message.len);
#endif

#ifdef BOOT
#ifdef _DEBUG_
    iapTCB.debug("BOOT Run!!!\r\n");
#endif
    message.type = IAP_COMMAND_HARDWARE_INFO;
    message.len = strlen(hardwareInfo);
    memcpy(message.data, hardwareInfo, message.len);

    iapTCB.getSysTickMs = getSysTickMs;
    iapTCB.blockWrite = blockWrite;
    iapTCB.blockErase = blockErase;
    iapTCB.jumpTick = iapTCB.getSysTickMs() + BOOT_WAIT;
#endif
#ifdef _DEBUG_
    iapTCB.debug("version: %s\r\nbuild :%s\r\n",
                 firmwareInfo.version,
                 firmwareInfo.buildInfo);
#endif

    iapTCB.sendMessage(&message);

    message.type = IAP_COMMAND_JUMP;
    message.len = 1;

#ifdef APP
    message.data[0] = 2;
#endif

#ifdef BOOT
    message.data[0] = 1;
#endif

    iapTCB.sendMessage(&message);

    custom();
}

/**
 * The iapHandle function handles various IAP commands, including jumping to a new firmware, providing
 * software and hardware information, erasing flash memory, and writing firmware blocks.
 */
void iapHandle(void)
{
    static IAP_MESSAGE_T message;

#ifdef BOOT
    static bool iapJumpFlag = true;
    static uint8_t firmwareBlock[2048] __attribute__((aligned(4)));
    static uint32_t blockCRC;
    static uint32_t blockAddr;

    if ((iapTCB.getSysTickMs() > iapTCB.jumpTick) && iapJumpFlag)
    {
        message.data[0] = iapJump();

        // if jump not run here without error
        // or without APP
        message.type = IAP_COMMAND_JUMP;
        message.len = 1;
        iapTCB.sendMessage(&message);
        iapJumpFlag = false;
    }
#endif

    if (iapTCB.getMessage(&message))
    {
        if (message.type == IAP_COMMAND_JUMP)
        {
#ifdef APP
            if (message.data[0] == 1)
            {
#ifdef _DEBUG_
                iapTCB.debug("jump to boot !! \r\n");
#endif
                message.data[0] = iapJump();
            }
            message.data[0] = 2;
#endif
#ifdef BOOT

            if (message.data[0] == 2)
            {
#ifdef _DEBUG_
                iapTCB.debug("jump to app !! \r\n");
#endif
                message.data[0] = iapJump();
            }
            message.data[0] = 1;
#endif

            // not run here without error
            message.type = IAP_COMMAND_JUMP;
            message.len = 1;
        }
        else if (message.type == IAP_COMMAND_SOFT_INFO)
        {
#ifdef _DEBUG_
            iapTCB.debug("%s \r\n", softInfo);
#endif
            message.len = strlen(softInfo);
            memcpy(message.data, softInfo, message.len);
        }
#ifdef BOOT
        else if (message.type == IAP_COMMAND_HARDWARE_INFO)
        {
            message.len = strlen(hardwareInfo);
            memcpy(message.data, hardwareInfo, message.len);
        }
        else if (message.type == IAP_COMMAND_FLASH_ERASE)
        {
            // 擦除一个APP
            message.data[0] = iapTCB.blockErase((uint32_t)&__app_start, firmwareInfo.size);
            message.len = 1;
        }

        else if (message.type == IAP_COMMAND_BOLCK_INFO)
        {
            memcpy(&blockAddr, message.data, sizeof(uint32_t));
            memcpy(&blockCRC, message.data + 4, sizeof(uint32_t));

            message.data[0] = IAP_STATE_SUCCEED;
            message.len = 1;
        }
        else if (message.type >= IAP_COMMAND_BLOCK_START &&
                 message.type <= IAP_COMMAND_BLOCK_END)
        {
            memcpy(firmwareBlock + ((message.type - IAP_COMMAND_BLOCK_START) * sizeof(message.data)), message.data,
                   sizeof(message.data));

            message.data[0] = IAP_STATE_SUCCEED;
            message.len = 1;
        }
        else if (message.type == IAP_COMMAND_BLOCK_WRITE)
        {
            uint32_t blockAddrTemp;
            uint32_t blockCRCTemp;

            memcpy(&blockAddrTemp, message.data, sizeof(uint32_t));

            message.len = 1;

            blockCRCTemp = iapCheckCRC32(firmwareBlock, sizeof(firmwareBlock));
            if (blockCRCTemp == blockCRC)
            {
                if (blockAddrTemp != blockAddr)
                {
                    message.data[0] = IAP_STATE_FAILD;
                }
                else
                {
                    message.data[0] = iapTCB.blockWrite(blockAddr, firmwareBlock, sizeof(firmwareBlock));
                }
            }
            else
            {
                message.data[0] = IAP_STATE_FAILD;
            }
            memset(firmwareBlock, 0, sizeof(firmwareBlock));
            blockCRC = 0;
            blockAddr = 0;
        }
        else if (message.type == IAP_COMMAND_CHECK_ALL)
        {
            message.len = 1;

            // if ((((FIRMWARE_INFO_T *)(uint8_t *)(&firmwareInfo) + 0X400)->crc !=
            //      iapCheckCRC32((uint8_t *)__app_start, ((FIRMWARE_INFO_T *)(uint8_t *)(&firmwareInfo) + 0X400)->size)) ||
            //     ((FIRMWARE_INFO_T *)(((uint8_t *)&firmwareInfo) + 0X400))->crc != 0XCCCCCCCC)// watermark without calculate
            // {
            //     message.data[0] = IAP_STATE_FAILD;
            // }
            // else
            // {
            message.data[0] = IAP_STATE_SUCCEED;
            // }
        }
        // if rcv once iap message , don't jump to app
        iapJumpFlag = false;
#endif
        iapTCB.sendMessage(&message);
    }
}