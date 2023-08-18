# Fibocom-L610模组基于BoAT Infra Arc基础架构区块链应用适配说明


## 一、前言

本文档说明如何在 `Fibocom L610` 开发环境中使用 `BoAT Infra Arch` 基础架构编译相关BoAT静态库以及生成区块链DEMO应用程序。

本例中使用到 `BoAT Infra Arch` 基础架构的静态库包括：

`BoAT-SupportLayer` ： 为基于 BoAT Infra Arch 基础架构的应用程序和特定功能中间库提供平台操作系统调用接口抽象和驱动抽象应用接口。

`BoAT-Engine`：为基于 BoAT Infra Arch 基础架构的应用程序提供区块链访问应用接口。

`Fibocom L610` 的SDK版本号为 `core_sdk_16000.1000.00.96.64.04`。

## 二、代码克隆和配置

约定`<L610 Root>`是 `Fibocom L610` 平台SDK的根目录：

以下以`Venachain`为例。

1、通过`git clone`命令，在`<L610 Root>`目录下下载`BoAT-ProjectTemplate`库，命令参考如下:

  ```
  git clone -b dev git@github.com:aitos-io/BoAT-ProjectTemplate.git
  ```
  或
  ```
  git clone -b dev https://github.com/aitos-io/BoAT-ProjectTemplate.git
  ```
  
2、进入 `<L610 Root>/BoAT-ProjectTemplate/` 目录，修改 `BoATLibs.conf` 文件

将 BoATLibs.conf 文件替换为以下内容：
```
BoAT-SupportLayer
BoAT-Engine
```
保存并退出。

3、在 `<L610 Root>/BoAT-ProjectTemplate/` 目录下执行配置脚本
```
python3 config.py
```
依次根据提示选择不同依赖库的配置如下：

根据提示输入选择，配置过程如下：
```  
We will clone the BoAT-SupportLayer repository, which may take several minutes

Input the branch name or null:
``` 
输入‘dev’回车，选择BoAT-SupporLayer仓库dev分支。
``` 
Input the branch name or null:dev
branch name is [ -b dev]

git clone -b dev git@github.com:aitos-io/BoAT-SupportLayer.git

Cloning into 'BoAT-SupportLayer'...
remote: Enumerating objects: 2930, done.
remote: Counting objects: 100% (704/704), done.
remote: Compressing objects: 100% (327/327), done.
remote: Total 2930 (delta 441), reused 589 (delta 362), pack-reused 2226
Receiving objects: 100% (2930/2930), 3.40 MiB | 21.00 KiB/s, done.
Resolving deltas: 100% (1826/1826), done.
git cmd succ


We will clone the BoAT-Engine repository, which may take several minutes

Input the branch name or null:
```
输入‘dev’回车，选择BoAT-Engine仓库dev分支。
```
Input the branch name or null:dev 
branch name is [ -b dev]

git clone -b dev git@github.com:aitos-io/BoAT-Engine.git

Cloning into 'BoAT-Engine'...
remote: Enumerating objects: 900, done.
remote: Counting objects: 100% (39/39), done.
remote: Compressing objects: 100% (27/27), done.
remote: Total 900 (delta 18), reused 22 (delta 12), pack-reused 861
Receiving objects: 100% (900/900), 527.23 KiB | 37.00 KiB/s, done.
Resolving deltas: 100% (567/567), done.
git cmd succ


overwrite the Makefile?(Y/n):
```
输入回车，选择重写Makefile
```
Yes

 Select blockchain list as below:
 [1] ETHEREUM          : 
 [2] PLATON            : 
 [3] PLATONE           : 
 [4] FISCOBCOS         : 
 [5] HLFABRIC          : 
 [6] HWBCS             : 
 [7] CHAINMAKER_V1     : 
 [8] CHAINMAKER_V2     : 
 [9] VENACHAIN         : 
 [a] QUORUM            : 
 [b] CITA              : 
 [0] All block chains
 Example:
  Select blockchain list as below:
  input:1a
  Blockchain selected:
   [1] ETHEREUM
   [a] QUORUM

input:
```
输入“9”回车，选择VENACHAIN区块链。
```
input:9
Blockchain selected:
 [9] VENACHAIN

Select the platform list as below:
[1] linux-default             : Default linux platform
[2] Fibocom-L610              : Fibocom's LTE Cat.1 module
[3] create a new platform
```
输入‘2’回车，选择“Fibocom-L610”平台
```
2
 
platform is : Fibocom-L610

include BoAT-SupportLayer.conf

include BoAT-Engine.conf


./BoAT-SupportLayer/demo/ False
./BoAT-Engine/demo/ True
Configuration completed
```
配置完成。

完成上述操作后部分目录和文件结构如下：
```
<L610 Root>
|
`-- BoAT-ProjectTemplate
      |-- BoAT-SupportLayer
      |-- BoAT-Engine
      |-- BoATLibs.conf
      |-- config.py
      |-- Makfile
      |-- README.md
      |-- README_en.md
```

4、在`<L610 Root>`目录下创建 `demo.c` 源文件。
区块链应用程序在demo.c中实现。
demo.c作为Fibocom L610模组的opencpu应用，需参考Fibocom L610模组应用入口设计程序。
区块链应用具体实现方法参考'<L610 Root>/BoAT-ProjectTemplate/BoAT-Engine/demo/demo_venachain'目录相关代码。
完成上述操作后部分目录和文件结构如下：
```
<L610 Root>
|
`-- BoAT-ProjectTemplate
      |-- BoAT-SupportLayer
      |-- BoAT-Engine
      |-- BoATLibs.conf
      |-- config.py
      |-- Makfile
      |-- README.md
      |-- README_en.md
`-- cmake
      |-- toolchain-gcc.cmake
|-- CMakeLists.txt
|-- demo.c
```

## 三、文件修改

### 1、修改SupportLayer中的平台external.env

打开`<L610 Root>/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/external.env`文件
  
  配置本地实际的交叉编译器，例如：
  ```
  CC := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
  AR := $(CURDIR)/../prebuilts/linux/gcc-arm-none-eabi/bin/arm-none-eabi-ar
  ```

### 2、需改 httpclient.c 禁能 SSL
打开`<L610 Root>/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/dal/http/httpclient.c`文件

屏蔽或删除SSL使能宏定义：
```
//#define BOAT_HTTPCLIENT_SSL_ENABLE
```
保存并退出
	
### 3、添加demo的源文件路径、头文件路径以及BoAT库的链接路径

  3.1、打开`<L610 Root>/CMakeLists.txt`文件，

  在`include_directories`中添加demo源文件路径和头文件路径，例如：
  ```
include_directories(components/include 
                    components/newlib/include
                    BoAT-ProjectTemplate/BoAT-SupportLayer/include        
                    BoAT-ProjectTemplate/BoAT-SupportLayer/tests
                    BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/log
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/osal
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-L610/src/dal
                    BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include
                    BoAT-ProjectTemplate/BoAT-SupportLayer/third-party/cJSON
                    BoAT-ProjectTemplate/BoAT-SupportLayer/keystore
                    BoAT-ProjectTemplate/BoAT-Engine/include
                    BoAT-ProjectTemplate/
                   )

  ```
在`if(CONFIG_APPIMG_LOAD_FLASH)`条件编译中添加 demo 需要编译的相关源文件路径和连接所需外部库名称，例如：
  ```
      add_appimg(${target} ${flash_ldscript}
          demo.c)
      target_link_libraries(${target} PRIVATE ${libboatengine_file_name} ${libboatvendor_file_name} ${libc_file_name} ${libm_file_name} ${libgcc_file_name} )
  ```
  注意各个库链接的顺序，libboatengine要放在libboatvendor之前，否则会造成链接错误。
  如上 PRIVATE 后包含的连接库名称如下：
  ```  
  ${libboatengine_file_name} 
  ${libboatvendor_file_name} 
  ${libc_file_name} 
  ${libm_file_name} 
  ${libgcc_file_name}
  ```
  以上各个链接库名称在 `<L610 Root>/cmake/toolchain-gcc.cmake` 文件中定义。
  
  `<L610 Root>/CMakeLists.txt`文件配置结束。
  
  3.2、打开`<L610 Root>//cmake/toolchain-gcc.cmake`文件，

  在`if(CONFIG_CPU_ARM_CA5)`的条件下面添加BoAT相关静态库的路径：
  ```
  if(CONFIG_CPU_ARM_CA5)
      set(abi_options -mcpu=cortex-a5 -mtune=generic-armv7-a -mthumb -mfpu=neon-vfpv4
          -mfloat-abi=hard -mno-unaligned-access)
      set(partial_link_options)
      if(L610_CN_02_90_ALIEVK)
      message(STATUS "BUILD L610_CN_02_90_ALIEVK")
      set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/alievk/libc.a)
      else()
      set(libc_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/libc.a)
      endif()
      set(libm_file_name ${CMAKE_CURRENT_SOURCE_DIR}/components/newlib/armca5/libm.a)
      set(libboatvendor_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatvendor.a)
      set(libboatengine_file_name ${CMAKE_CURRENT_SOURCE_DIR}/BoAT-ProjectTemplate/lib/libboatengine.a)
  endif()
  ```
`<L610 Root>//cmake/toolchain-gcc.cmake` 文件配置结束。

## 四、BoAT静态库的编译和使用

### 1、在Linux下编译BoAT静态库.a文件
   
   
   打开终端并进入BoAT-ProjectTemplate目录编译BoAT静态库：
   ```
   cd <L610 Root>/BoAT-ProjectTemplate
   make clean
   make all
   ```
   
   编译成功后，在`<L610 Root>/BoAT-ProjectTemplate/lib`下会生成两个静态库：
   ```
   libboatvendor.a
   libboatengine.a
   ```

### 2、编译 L610 固件
   
   打开终端并进入`<L610 Root>/`目录，执行以下指令完成固件编译
   ```
   . tools/core_launch.sh
   cout
   cmake ../.. -G Ninja
   ninja
   ```
  编译成功后会在`<L610 Root>/out/appimage_debug/hex`下生成`hello_flash.pac`文件。
  
## 五 `demo` 执行

  下载`hello_flash.pac`文件到Fibocom L610模组，复位模组可运行demo，可通过模组调试工具查看demo执行信息，模组代码下载和调试工具使用参考L610平台操作说明。

