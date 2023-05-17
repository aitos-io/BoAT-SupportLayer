# BoAT-X-Framework for 紫光展锐Unisoc-8850整合说明


## 一、前言

本文档说明如何在Unisoc-8850开发环境中整合BoAT、编译BoAT静态库以及生成demo程序。Unisoc-8850的SDK版本号为ivykit_rtos_rls_8850_volte_W23.01.6_P3__20230314_2227。

## 二、代码克隆和配置

约定`<8850 Root>`是Unisoc-8850平台SDK的根目录：

以下以Venachain为例。

1、通过git clone命令，在`<8850 Root>/idh/idh.code/`目录下下载BoAT-ProjectTemplate库，命令参考如下:

  ```
    git clone git@github.com:aitos-io/BoAT-ProjectTemplate.git .
  ```

2、参照BoAT-ProjectTemplate的使用文档进行下载代码和本地配置。

3、在`<8850 Root>/idh/idh.code/components/appstart/src`下创建venachain demo源文件。


完成上述操作后的目录和文件结构如下：
```
<8850 Root>
|
`-- idh
      |-- CUS_Release_Notes
      |-- version.txt
      `-- idh.code
            |-- BoAT-ProjectTemplate
            |-- cmake
            |-- components
            |-- examples
            |-- prebuilts
            |-- target
            |-- tools
            |-- CMakeLists.txt
            |-- Kconfig

```


## 三、文件修改

### 1、修改SupportLayer中的平台external.env

打开`<8850 Root>idh/idh.code/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Unisoc-8850/external.env`文件
  
  配置本地实际的交叉编译器，例如：
  ```
    CC := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/gcc-arm-none-eabi-10.2.1/bin/arm-none-eabi-gcc
    AR := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/gcc-arm-none-eabi-10.2.1/bin/arm-none-eabi-ar
  ```

### 2、添加demo的源文件路径、头文件路径以及BoAT库的链接路径

  打开`<8850 Root>/idh/idh.code/components/appstart/CMakeLists.txt`文件，

  在`if(CONFIG_SOC_8850)`的条件下面添加demo源文件路径和头文件路径，例如：
  ```
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-Engine/include)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/include)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Unisoc-8850/src/log)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Unisoc-8850/src/inet)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Unisoc-8850/src/dal)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Unisoc-8850/src/osal)
    include_directories(${PROJECT_SOURCE_DIR}/BoAT-ProjectTemplate/BoAT-SupportLayer/keystore)

    add_library(${target} STATIC src/app_start_8850.c src/boat_venachain_demo.c src/mycontract.c)

    target_include_targets_if(CONFIG_MBEDTLS_SSL_TLS THEN ${target} PRIVATE mbedtls)
    target_include_targets_if(CONFIG_MBEDTLS_USE_CE_CRYPTO THEN ${target} PRIVATE crypto)

  ```
  打开`<8850 Root>/idh/idh.code/cmake/toolchain-gcc.cmake`文件，

  在`if(CONFIG_CPU_ARM_CA5)`的条件下面添加BoAT相关静态库的链接路径：
  ```
  set(libboat_engine_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatengine.a)
    set(libboat_vendor_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatvendor.a)
  ```

  打开`<8850 Root>/idh/idh.code/CMakeLists.txt`文件，
  在`all_libs`的目标下面添加BoAT相关静态库：
  ```
  list(APPEND app_libraries ${libboat_engine_name} ${libboat_vendor_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name})
  ```

  打开`<8850 Root>/idh/idh.code/components/appstart/src/app_start_8850.c`文件，
  在函数`prvPowerOn`的末尾，添加BoAT demo的入口执行函数的调用，例如：
  ```
  extern int boat_entry_function();

  boat_entry_function();
  ```

## 四、BoAT静态库的编译和使用

### 1、在Linux下编译BoAT静态库.a文件
   
   
   打开终端并进入BoAT-ProjectTemplate目录编译BoAT静态库：
   ```
   cd <8850 Root>/idh/idh.code/BoAT-ProjectTemplate
   make clean
   make all
   ```
   
   编译成功后，在`<8850 Root>/idh/idh.code/BoAT-ProjectTemplate/lib`下会生成静态库libboatvendor.a、libboatengine.a。
   

### 2、编译8850固件
   
   打开终端并进入`<8850 Root>`
   ```
   cd <8850 Root>/idh/idh.code
   . tools/launch.sh
   cout
   cmake ../.. -G Ninja
   ninja
   ```
  编译成功后会在`<8850 Root>/idh/idh.code/out/8850DG_cat1bis_volte_debug/hex`下生成8850DG_cat1bis_volte-8850XG_cat1bis_volte_gnss_ab-debug.pac文件。
