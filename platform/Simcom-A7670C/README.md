# BoAT for 芯讯通A7670C整合说明


## 一、前言

本文档说明如何在A7670C开发环境中整合BoAT，编译BoAT静态库，以及生成demo程序。


## 二、下载BoAT和编写demo

约定`<A7670C Root>`是A7670C平台SDK的根目录：

1、根据github上的操作说明下载BoAT-ProjectTemplate项目至`<A7670C Root>`下。

拷贝后的目录和文件结构如下：
```
<A7670C Root>
|-- BoAT-ProjectTemplate
    |-- BoAT-Engine
    |-- BoAT-SupportLayer
|-- config
|-- examples
|-- kernel
|-- sc_lib	
|-- script	
|-- tools
|-- app_build_doc.md
|-- CMakeLists.txt
|-- makeDepend.mak
|-- Makefile
|-- sc_application.c	
`-- sc_demo
    |-- inc
    |-- src
    |-- CMakeLists.txt
```

2、在`<A7670C Root>/sc_demo/src`下创建demo文件夹，以venachain的demo为例，可以创建venachain_demo文件夹，并将demo的c文件`venachain_demo_entry.c`、合约的c接口文件`mycontract.c`和合约c接口的声明文件`mycontract.h`放在该文件夹中。

## 三、文件修改

### 1、添加BoAT生成的静态库.a文件到A7670C平台

  打开`<A7670C Root>/CMakeLists.txt`文件。
  
  首先找到
  ```
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
    "${CMAKE_SOURCE_DIR}/sc_lib/${SCMODULE}/lib/libsc_lib.a"
  )
  ```
  然后在下方添加以下内容：
  ```
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
            "${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatengine.a"
        )
  set_property(TARGET third_libs APPEND PROPERTY IMPORTED_OBJECTS
            "${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatvendor.a"
        )
  ```

### 2、添加BoAT有关头文件路径到A7670C平台

  打开`<A7670C Root>/sc_demo/CMakeLists.txt`文件。
  
  首先找到 `${CMAKE_SOURCE_DIR}/sc_demo/inc`，然后在下方添加以下内容：
  ```
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-Engine/include
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/include
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/keystore
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/keystore/soft
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Simcom-A7670C/src/dal
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Simcom-A7670C/src/inet
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Simcom-A7670C/src/log
    ${CMAKE_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Simcom-A7670C/src/osal
  ```

  再找到 `list(APPEND sc_demo_src ./src/demo_helloworld.c)`，然后在下方添加以下内容：
  ```
  list(APPEND sc_demo_src ./src/venachain_demo/venachain_demo_entry.c)
  list(APPEND sc_demo_src ./src/venachain_demo/mycontract.c)
  ```


## 四、编译BoAT静态库

### 1、在Windows中编译BoAT静态库.a文件

   #### a、修改BoAT-ProjectTemplate/Makefile中的platform target
   ```
   PLATFORM_TARGET ?= Simcom-A7670C
   ```
   
   #### b、修改BoAT-ProjectTemplate/Makefile中的chain target
   ```
   BOAT_PROTOCOL_USE_VENACHAIN          ?= 1
   ```

   #### b、打开windows PowerShell终端进入BoAT-ProjectTemplate目录编译BoAT静态库
   ```
   cd <A7670C Root>/BoAT-ProjectTemplate
   make clean
   make all
   ```
   
   编译成功后，在BoAT-ProjectTemplate/lib下会生成静态库`libboatvendor.a`、`libboatengine.a`。
   

### 2、编译A7670C演示demo程序，生成.zip下载文件

   参照模组的demo，将自定义demo的入口函数加入到模组启动函数中

   打开windows PowerShell终端进入`<A7670C Root>`
   ```
   cd <A7670C Root>
   make clean
   make A7670C_LANS_1606_V701
   ```

   编译成功会在`<A7670C Root>/out`下生成`A7670C_LANS_1606_V701.zip`下载文件。