<!--
 * @Description: 
 * @Author: L LC @amov
 * @Date: 2023-08-31 17:47:13
 * @LastEditors: L LC @amov
 * @LastEditTime: 2024-05-22 11:20:52
 * @FilePath: \chiplib\readme.md
-->
# stm32 驱动库

当前支持列表 

    stm32f4 标准库
    stm32f30 标准库
    stm32f1 标准库
    stm32f0 标准库

build test:

```bash
cmake -B .build -DLIBTYPE=STD -DCMAKE_TOOLCHAIN_FILE=example_build/cmake/stm32f412re.cmake  -GNinja
cmake --build .build
```


