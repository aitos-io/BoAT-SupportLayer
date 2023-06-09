# BoAT-X-Framework for 中移物联ML307A-DCLN整合说明


## 一、前言

本文档说明如何在ML307A-DCLN开发环境中整合BoAT、编译BoAT静态库以及生成demo程序。ML307A-DCLN的SDK版本号为Confidential_ML307A_OpenCPU_Standard_1.3.1.2023032918_alpha。

## 二、代码克隆和配置

约定`<ML307A Root>`是ML307A-DCLN平台SDK的根目录：

以下以Venachain为例。

1、通过git clone命令，在`<ML307A Root>\custom`目录下下载BoAT-ProjectTemplate库，命令参考如下:

  ```
    git clone git@github.com:aitos-io/BoAT-ProjectTemplate.git .
  ```

2、参照BoAT-ProjectTemplate的使用文档进行下载代码和本地配置。

3、在`<ML307A Root>\custom\custom_main\src`下创建VenaChain_demo目录，将demo合约文件和调用合约的demo文件放在该目录下。


完成上述操作后的目录和文件结构如下：
```
<ML307A Root>
|
`-- custom
    `--BoAT-ProjectTemplate
        |-- BoAT-Engine
        |-- BoAT-SupportLayer
        |-- BoATLibs.conf
        |-- config.py
        |-- Makefile
        |-- README_cn.md
        |-- README.md
    `--custom_main
        |-- inc
        `-- src
            |-- VenaChain_demo
                `--contract
                |-- demo_venachain_mycontract.c
            |-- custom_main.c
        |-- custom_main.mk
    `--custom.mk
|-- docs
|-- examples
|-- include
|-- out
|-- prebuilt
|-- src
|-- third-party
|-- tools
|-- ML307A_build.bat
|-- README.md
```


## 三、文件修改

### 1、修改SupportLayer中的平台external.env

打开`<ML307A Root>\custom\BoAT-ProjectTemplate\BoAT-SupportLayer\platform\ChinaMobile-ML307A-DCLN\external.env`文件
  
  配置本地实际的交叉编译器，例如：
  ```
    CC := $(CURDIR)/../../tools/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-gcc
    AR := $(CURDIR)/../../tools/toolchain/gcc-arm-none-eabi/bin/arm-none-eabi-ar
  ```

  配置本地的Cygwin环境，例如：
  ```
    CYGWIN_BASE := D:\tools\Cygwin64\bin
    BOAT_RM := $(CYGWIN_BASE)\rm -rf
    BOAT_MKDIR := $(CYGWIN_BASE)\mkdir
    BOAT_FIND := $(CYGWIN_BASE)\find
    BOAT_COPY := $(CYGWIN_BASE)\cp
  ```

### 2、关闭SDK中的自带的cJSON选项
  
  打开`<ML307A Root>\third-party\third-party.mk`文件，
  
  将cJSON的引用注释掉，详见如下：
  ```
  #include $(THIRD-PARTY_DIR)/cJSONFiles/cJSON.mk
  ```

### 3、添加demo的源文件路径、头文件路径以及BoAT库的链接路径

  打开`<ML307A Root>\custom\custom.mk`文件，

  添加demo源文件路径和头文件路径，例如：
  ```
  INC += -I'$(CUSTOM_DIR)/custom_main/src/VenaChain_demo/contract' \

  OC_FILES += $(CUSTOM_DIR)/custom_main/src/VenaChain_demo/demo_venachain_mycontract.c \
			$(CUSTOM_DIR)/custom_main/src/VenaChain_demo/contract/mycontract.c \
  ```

  添加BoAT相关静态库的链接路径：
  ```
  OC_LIBS += $(CUSTOM_DIR)/BoAT-ProjectTemplate/lib/libboatengine.a
  OC_LIBS += $(CUSTOM_DIR)/BoAT-ProjectTemplate/lib/libboatvendor.a
  ```

## 四、BoAT静态库的编译和使用

### 1、在Windows下编译BoAT静态库.a文件
   
   
   打开cmd并进入BoAT-ProjectTemplate目录编译BoAT静态库：
   ```
   cd <ML307A Root>\custom\BoAT-ProjectTemplate
   make clean
   make all
   ```
   
   编译成功后，在`<ML307A Root>\custom\BoAT-ProjectTemplate`下会生成静态库libboatvendor.a、libboatengine.a。
   

### 2、编译ML307A演示demo程序，生成下载文件
   
   打开cmd并进入`<ML307A Root>`
   ```
   cd <ML307A Root>
   ML307A_build.bat CLEAN
   ML307A_build.bat DCLN
   ```
   编译成功会在`<ML307A Root>\out\images\ML307A\DCLN`下生成ML307A_DCLN_APP.zip文件。
