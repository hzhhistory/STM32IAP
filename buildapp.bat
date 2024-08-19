mkdir .output

del .\.build\
cmake -B .build -DTAR_PRO=APP -DLIBTYPE=STD -DCMAKE_TOOLCHAIN_FILE=cmake/stm32f103cbt6.cmake  -GNinja
cmake --build .build
copy .\.build\SU17IAR_app.elf .\.output\SU17IAR_app.elf
copy .\.build\SU17IAR_app.hex .\.output\SU17IAR_app.hex
copy .\.build\SU17IAR_app.map .\.output\SU17IAR_app.map
copy .\.build\SU17IAR_app.bin .\.output\SU17IAR_app.bin

del .\.build\
cmake -B .build -DTAR_PRO=BOOT -DLIBTYPE=STD -DCMAKE_TOOLCHAIN_FILE=cmake/stm32f103cbt6.cmake  -GNinja
cmake --build .build
copy .\.build\SU17IAR_boot.elf .\.output\SU17IAR_boot.elf
copy .\.build\SU17IAR_boot.hex .\.output\SU17IAR_boot.hex
copy .\.build\SU17IAR_boot.map .\.output\SU17IAR_boot.map
copy .\.build\SU17IAR_boot.bin .\.output\SU17IAR_boot.bin
