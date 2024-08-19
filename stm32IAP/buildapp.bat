del .\.build\
cmake -B .build -DTAR_PRO=APP -DLIBTYPE=STD -DCMAKE_TOOLCHAIN_FILE=example_build/cmake/stm32f412re.cmake  -GNinja
cmake --build .build
copy .\.build\stm32iap_example_app.elf .\.output\stm32iap_example_app.elf
copy .\.build\stm32iap_example_app.hex .\.output\stm32iap_example_app.hex
copy .\.build\stm32iap_example_app.map .\.output\stm32iap_example_app.map
copy .\.build\stm32iap_example_app.bin .\.output\stm32iap_example_app.bin