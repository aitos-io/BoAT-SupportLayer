# BoAT for 移柯L503整合说明


## 一、前言

本文档说明如何在L503开发环境中整合BoAT，编译BoAT静态库，以及生成demo程序。


## 二、配置BoAT和Demo

约定`<L503 Root>`是L503平台SDK的根目录：

1、下载BoAT相关代码库至`<L503 Root>/app/demo`下，至少包含BoAT-Engine和BoAT-SupportLayer。

2、以Venachain的Demo为例，在`<L503 Root>/app/demo`下创建boat_venachain_demo文件夹，并放置合约接口文件和访问合约的代码文件。

完成后的目录和文件结构如下：
```
<L503 Root>
|
|-- bootloader
|-- kernel
`-- app
    |-- build
    |-- config
    `-- demo
        `-- boat_venachain_demo
            `-- include
                |-- mycontract.h
            |-- boat_venachain_demo_entry.c
            |-- mycontract.c
            |-- Makefile
        `-- BoAT-ProjectTemplate
            |-- BoAT-Engine
            |-- BoAT-SupportLayer
            |-- BoATLibs.conf
            |-- config.py
            |-- Makefile
            |-- README_cn.md
            |-- README.md
    |-- release
    |-- system
    |-- tools
    |--build_app.bat
    |-- Makefile

```

## 三、文件修改

### 1、添加BoAT生成的静态库.a文件到L503平台

  打开`<L503 Root>/app/build/makerules.mk`文件。
  
  首先找到 `DEPENDENCY_INCLUDE_LIB := ${foreach lib, ${LOCAL_EXPORT_LIB}, ${SOFT_WORKDIR}/${lib}}`，然后在下方添加以下内容：
  ```
  DEPENDENCY_INCLUDE_LIB += $(SOFT_WORKDIR)/demo/BoAT-ProjectTemplate/lib/libboatengine.a
  DEPENDENCY_INCLUDE_LIB += $(SOFT_WORKDIR)/demo/BoAT-ProjectTemplate/lib/libboatvendor.a
  ```

### 2、添加BoAT有关头文件路径到L503平台

  打开`<L503 Root>/app/build/buildvar.mk`文件。
  
  首先找到 `cust_lc/inc`，在后方添加` \`，然后在最后添加以下BoAT相关头文件路径：
  ```
  BoAT-ProjectTemplate/BoAT-Engine/include \
  BoAT-ProjectTemplate/BoAT-SupportLayer/include \
  BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage \
  BoAT-ProjectTemplate/BoAT-SupportLayer/keystore/soft \
  BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include \
  BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Mobiletek-L503C-6S/src/dal \
  BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Mobiletek-L503C-6S/src/inet \
  BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Mobiletek-L503C-6S/src/log \
  BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Mobiletek-L503C-6S/src/osal 
  ```

### 3、添加日志打印自定义映射接口

  打开`<L503 Root>/kernel/mbtk/cust_api/customer_api.c`文件。
  
  找到`extern int AudioHAL_AifSetPhoneStatus(unsigned int status);`,在下方添加：
  ```
  extern void RTI_LOG(const char* fmt, ...);
  ```
  
  找到结构体类型定义 `mbtk_user_api`，在该结构体中添加函数映射指针：
  ```
  int (*user_uart_printf)(const char *fmt, ...);
  ```

  找到定义映射表结构体 `g_user_api`，在该结构体中添加需要映射函数的初始化：
  ```
  .user_uart_printf = RTI_LOG,
  ```  
  
  打开`<L503 Root>/app/system/inc/mbtk_customer_api.h`文件。
  
  找到结构体 `mbtk_user_api`，在该结构体中添加函数映射指针：
  ```
  int (*user_uart_printf)(const char *fmt, ...);
  ``` 
  
  打开`<L503 Root>/app/system/src/api_customer_init.c`文件。
  
  找到定义 `int (*ol_path_ctrl)(UINT32 path);`在下方添加定义：
  ```
  int (*user_app_printf)(const char *fmt, ...);
  ```
  
  找到函数 `mbtk_api_init_customer`，在函数中`if(api_table)`下方的大括号内添加：
  ```
  user_app_printf = api_table->user_uart_printf;
  ```
  

## 四、编译BoAT静态库和打包下载程序

### 1、在Windows环境下编译BoAT静态库.a文件

   #### a、修改BoAT-ProjectTemplate/Makefile中的target
   ```
   PLATFORM_TARGET ?= Mobiletek-L503C-6S
   ```
   
   #### b、以venachain为例，在BoAT-ProjectTemplate/Makefile中设置目标链
   ```
   BOAT_PROTOCOL_USE_VENACHAIN          ?= 1
   ```

   #### c、打开PowerShell终端进入BoAT-ProjectTemplate目录编译BoAT静态库
   ```
   cd <L503 Root>/app/demo/BoAT-ProjectTemplate
   ../tools/gnumake.exe clean
   ../tools/gnumake.exe all
   ```
   
   编译成功后，在BoAT-ProjectTemplate/lib下会生成静态库`libboatvendor.a`、`libboatengine.a`。
   

### 2、配置并编译L503演示demo程序，生成.zip下载文件（以venachain为例）

   #### a、打开 `<L503 Root>/app/demo/Makefile`，将Demo加入编译
   ```
    LOCAL_MODULE_DEPENDS += demo/boat_venachain_demo
   ```

   #### b、打开 `<L503 Root>/app/demo/boat_venachain_demo/Makefile`，配置BoAT相关头文件路径

   ```
    LOCAL_API_DEPENDS :=  ${API_PLATFORM_DEPENDS} \
                        demo/boat_venachain_demo/include \
                        demo/BoAT-ProjectTemplate/BoAT-Engine/include \
                        demo/BoAT-ProjectTemplate/BoAT-SupportLayer/include \
                        demo/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include \
                        demo/BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage \
                        demo/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Mobiletek-L503C-6S/src/osal \
                        demo/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Mobiletek-L503C-6S/src/dal \
                        demo/BoAT-ProjectTemplate/BoAT-SupportLayer/keystore \
                        demo/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Mobiletek-L503C-6S/src/log
   ```
   
   #### c、打开`<L503 Root>/app/demo/main.c`，将Demo的入口函数配置到模组的任务线程中

   在app_open_task函数的上方extern Demo的入口函数，例如：
   ```
    extern void boat_venachain_demo_entry(void);
   ```

   在app_open_task函数中添加Demo入口函数的调用，例如：
   ```
    boat_venachain_demo_entry();
   ```

   #### d、编译app模块
   
   打开windows PowerShell终端进入`<L503 Root>/app`
   ```
   cd <L503 Root>/app
   build_app.bat -8
   ```

   编译成功会在`<L503 Root>/app/release`下生成`user_app.bin`文件。

   #### e、编译Kernel模块

   将`user_app.bin`文件拷贝并覆盖到`<L503 Root>/kernel/mbtk/releasepack/L503C_6S/images`下。

   打开windows PowerShell终端进入`<L503 Root>/kernel`
   ```
   cd <L503 Root>/kernel
   make_clean.bat
   L503C_6S_build_open.bat
   ```

   编译成功会在`<L503 Root>/kernel/OUTPUT_L503C_6S`下生成`ASR_CRANEL_EVB_CRANEL_A0_08MB.zip`下载文件。