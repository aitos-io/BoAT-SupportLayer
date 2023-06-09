# BoAT-SupportLayer for Fibocom-FG150整合说明


## 一、前言

本文档说明如何在Fibocom-FG150开发环境中整合BoAT-SupportLayer、编译BoAT静态库以及生成demo程序。


## 二、配置交叉编译环境

在Linux环境中：

1、解压`Fibocom_Application_V1.0.0.tar.gz`文件：
  ```
  tar –zxf Fibocom_Application_V1.0.0.tar.gz
  ```

2、进入交叉编译工具目录：
  ```
  cd Fibocom_Application/toolchain
  ```

3、安装交叉编译环境：
  ```
  ./oecore-x86_64-armv7at2hf-neon-toolchain-nodistro.0.sh

  /home/toolchain

  Y
  ```

4、配置交叉编译环境：
  ```
  source /home/toolchain/environment-setup-armv7at2hf-neon-oe-linux-gnueabi
  ```
  通过`$CC -v`可以查看到交叉编译器的具体信息，
  后续所有的操作都需要在当前命令行端口执行，如果更换新的端口，需要重新执行交叉编译环境的配置。


## 三、文件修改


### 1、根据实际情况修改external.env
  例如：
  ```
  EXTERNAL_LIBS := /home/alex/works/fibocom/FG150/sdx55-1-2-sdk/SDX55_apps/apps_proc/fibo-sdk/lib/libcurl.so.4.5.0
  EXTERNAL_LIBS += /home/alex/works/fibocom/openssl-OpenSSL_1_0_2r/out/lib/libssl.so.1.0.0
  EXTERNAL_LIBS += /home/alex/works/fibocom/openssl-OpenSSL_1_0_2r/out/lib/libcrypto.so.1.0.0
  ```
  
## 四、编译BoAT静态库

### 1、编译BoAT静态库

   #### a、配置主Makefile中的编译信息，修改以下内容（以PlatONE为例）：
   ```
   BOAT_PROTOCOL_USE_PLATONE    ?= 1

   PLATFORM_TARGET ?= Fibocom-FG150
   ```
   
   #### b、编译BoAT静态库
   ```
   make clean
   make all
   ```
   
   编译成功后，在`lib`下会生成静态库libboatvendor.a、libboatengine.a。
   
## 五、编译并运行调用BoAT的demo

### 1、配置区块链信息（以PlatONE为例）

  #### a、将部署好或者已知可用的节点地址配置给`demo_platone_mycontract.c`中的`demoUrl`变量，例如：
  ```
  const BCHAR *demoUrl = "http://121.0.0.1:7545";

  ```
  #### b、将已在链上部署好的智能合约地址配置给`demo_platone_mycontract.c`中的`demoRecipientAddress`变量，例如：
  ```
  const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";
  ```

### 2、引用智能合约

  #### a、按照BoAT用户手册将智能合约ABI文件编译成c接口文件

  #### b、在`demo_platone_mycontract.c`中引用智能合约c接口文件并调用合约的c接口，（以智能合约my_contract为例）例如：
  ```
  #include "my_contract.cpp.abi.h"
  ```
  ```
  result_str = my_contract_cpp_abi_setName(&tx_ctx, "HelloWorld");
  ```

### 3、编译demo

  在终端中执行如下命令：
  ```
  cd boat_demo
  $(CC) $(BUILD_FLAGS) -I. -I../BoAT-X-Framework/include -I../BoAT-X-Framework/vendor/platform/include my_contract.cpp.abi.c demo_platone_mycontract.c ../BoAT-X-Framework/lib/libboatwallet.a ../BoAT-X-Framework/lib/libboatvendor.a libcurl.so.4.5.0 -o sample_platone.bin
  ```
  执行成功后，会在`boat_demo`下生成可执行文件`sample_platone.bin`

### 4、运行demo

  #### a、将FG150板子上电并通过USB线连接PC

  #### b、按照板子的文档说明，打开ADB功能

  #### c、在pc上打开一个终端，通过ADB将可执行文件`sample_platone.bin`push到板子内（以/home/user为例）
  ```
  adb push sample_platone.bin /home/user
  ```

  #### d、在FG150的SIM卡槽中插入一张可用的SIM卡

  #### e、在PC上打开一个终端，并运行FG150中的demo
  ```
  adb shell
  cd /home/user
  chmod +x sample_platone.bin
  ./sample_platone.bin
  ```
