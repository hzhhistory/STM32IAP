#include"stm32f10x.h"
// #include"SEGGER_RTT.h"n
#include"LM3644YFFR.h"

int main()
{
    LM3644YFFR_INIT();

    while(1)
    {

        LM3644YFFR_LEDFlash(Infrared_8);
    }


}