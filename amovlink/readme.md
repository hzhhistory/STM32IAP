# amov 通信交互定义集合

帧定义格式  
    0XAF 0X02 target(u8) source(u8) len(u8) cmd(u8) data(max64) crc(u16)

特殊节点
    0X01 IAP节点
    0XFF 广播节点

允许转发组网，支持星形拓扑，不允许形成环路


build:

```bash
cmake -B .build -DCMAKE_TOOLCHAIN_FILE=amovlink.cmake  -GNinja
```

获取系统链接文件 链接工具ld
```bash
ld --verbose --> sys.lds
```

