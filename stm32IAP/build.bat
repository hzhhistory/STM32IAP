mkdir .output

del .\.build\
cmake -B .build -DTAR_PRO=APP -DLIBTYPE=STD -DCMAKE_TOOLCHAIN_FILE=example_build/cmake/stm32f412re.cmake  -GNinja
cmake --build .build
copy .\.build\stm32iap_example_app.elf .\.output\stm32iap_example_app.elf
copy .\.build\stm32iap_example_app.hex .\.output\stm32iap_example_app.hex
copy .\.build\stm32iap_example_app.map .\.output\stm32iap_example_app.map
copy .\.build\stm32iap_example_app.bin .\.output\stm32iap_example_app.bin

del .\.build\
cmake -B .build -DTAR_PRO=BOOT -DLIBTYPE=STD -DCMAKE_TOOLCHAIN_FILE=example_build/cmake/stm32f412re.cmake  -GNinja
cmake --build .build
copy .\.build\stm32iap_example_boot.elf .\.output\stm32iap_example_boot.elf
copy .\.build\stm32iap_example_boot.hex .\.output\stm32iap_example_boot.hex
copy .\.build\stm32iap_example_boot.map .\.output\stm32iap_example_boot.map
copy .\.build\stm32iap_example_boot.bin .\.output\stm32iap_example_boot.bin

@REM srec_cat.exe .output\app.hex -intel .output\boot.hex -intel -o .output\output.hex -intel