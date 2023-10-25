# BoAT for 芯翼XinYi-XY4100整合说明


## 一、前言

本文档说明如何在XinYi-XY4100开发环境中整合BoAT、编译BoAT静态库以及生成demo程序。

## 二、代码克隆和配置

约定`<XY4100 Root>`是XinYi-XY4100平台SDK的根目录：

以下以Venachain为例。

1、通过git clone命令，在`<XY4100 Root>/`目录下下载BoAT-ProjectTemplate库，命令参考如下:

  ```
    git clone git@github.com:aitos-io/BoAT-ProjectTemplate.git .
  ```

2、参照BoAT-ProjectTemplate的使用文档进行下载代码和本地配置。

3、在`<XY4100 Root>/application`下创建放置venachain demo源文件的文件夹`boat_venachain_demo`,并将demo源文件和合约接口文件放在该文件夹中。


完成上述操作后的目录和文件结构如下：
```
<XY4100 Root>
|
`-- application
      |-- boat_venachain_demo
            |-- CMakeLists.txt
            |-- venachain_demo_entry.c
            |-- mycontract.c
            |-- mycontract.h
|-- BoAT-ProjectTemplate
      |-- BoAT-Engine
      |-- BoAT-SupportLayer
      |-- BoATLibs.conf
      |-- config.py
      |-- Makefile
      |-- README_cn.md
      |-- README.md
|-- boot
|-- buildtools
|-- kernel
|-- tools
|-- CMakeLists.txt
|-- copy_prebuilts.bat
|-- README.txt
|-- set_prebuilts_environment.bat
|-- xybuild.bat
|-- xybuild.py
|-- xybuild.sh

```


## 三、文件修改

### 1、修改SupportLayer中的平台external.env

打开`<XY4100 Root>/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/XinYi-XY4100/external.env`文件
  
  配置本地实际的交叉编译器，例如：
  ```
    CC := $(BOAT_BASE_DIR)/../buildtools/prebuilts/win64/riscv64-elf-tools/bin/riscv64-unknown-elf-gcc.exe
    AR := $(BOAT_BASE_DIR)/../buildtools/prebuilts/win64/riscv64-elf-tools/bin/riscv64-unknown-elf-gcc-ar.exe
  ```

### 2、添加demo的源文件路径、头文件路径以及BoAT库的链接路径

  打开`<XY4100 Root>/application/boat_venachain_demo/CMakeLists.txt`文件，

  添加需要引用的BoAT的头文件和demo的源文件，例如：
  ```
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-Engine/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/keystore
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/keystore/soft
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/platform/XinYi-XY4100/src/dal
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/platform/XinYi-XY4100/src/inet
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/platform/XinYi-XY4100/src/log
                    ${CMAKE_CURRENT_SOURCE_DIR}/../BoAT-ProjectTemplate/BoAT-SupportLayer/platform/XinYi-XY4100/src/osal)

    file(GLOB_RECURSE BOAT_VENACHAIN_DEMO_SOURCES 
        "${CMAKE_CURRENT_SOURCE_DIR}/boat_venachain_demo/*.c"
    )

    shorten_src_file_macro(${BOAT_VENACHAIN_DEMO_SOURCES})

    target_sources(application
        PRIVATE 
            ${BOAT_VENACHAIN_DEMO_SOURCES}
    )

    target_include_directories(application
        PUBLIC
            ${INCLUDE_BOAT_VENACHAIN_DEMO_DIRS}
    )

    relocate_code(CODE_LOCATION FLASH CODE_TARGET application SOURCES_LIST ${BOAT_VENACHAIN_DEMO_SOURCES})

  ```

  打开`<XY4100 Root>/application/CMakeLists.txt`文件，添加demo的component：
  ```
  add_component(boat_venachain_demo)
  ```

  打开`<XY4100 Root>/CMakeLists.txt`文件，添加BoAT静态库的链接：

  ```
  target_link_libraries(${TARGET_NAME}
    -Wl,--start-group
    ${LIBRARIES}
    ${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatengine.a
    ${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatvendor.a
    -Wl,--end-group)
  ```

  demo入口函数的编写请参照其他application的示例。

## 四、BoAT静态库的编译和使用

### 1、在Windows下编译BoAT静态库.a文件
   
   
   打开终端并进入BoAT-ProjectTemplate目录编译BoAT静态库：
   ```
   cd <XY4100 Root>/BoAT-ProjectTemplate
   make clean
   make all
   ```
   
   编译成功后，在`<XY4100 Root>/BoAT-ProjectTemplate/lib`下会生成静态库libboatvendor.a、libboatengine.a。
   

### 2、编译XY4100的AP固件
   
   打开终端并进入`<XY4100 Root>`
   ```
   cd <XY4100 Root>
   xybuild.bat clean
   xybuild.bat all
   ```
  编译成功后会在`<XY4100 Root>/build/bin`下生成ap.img文件。
