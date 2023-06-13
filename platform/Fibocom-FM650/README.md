# BoAT for Fibocom-FM650整合说明


## 一、前言

本文档说明如何在Fibocom-FM650开发环境中整合BoAT、编译BoAT静态库以及生成demo程序。


## 二、放置文件

约定`<FM650 Root>`是Fibocom-FM650平台SDK的根目录：

1、拷贝 BoAT 代码至Linux中。

2、在`<FM650 Root>/fibo-sdk/umdp/example`下新建文件夹`boat_demo`，在`boat_demo`文件夹下新建`libs`文件夹。

3、拷贝 BoAT-SupportLayer/platform/Fibocom-FM650/ex_libs目录下所有的库文件到`<FM650 Root>/fibo-sdk/umdp/example/boat_demo/libs`下。

4、拷贝 BoAT-SupportLayer/platform/Fibocom-FM650/ex_libs/Makefile到`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下。

5、放置合约c接口文件至boat_demo中，以Venachain为例，将demo合约c接口文件mycontract.c和mycontract.h文件放在`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下。

6、放置合约调用的demo至boat_demo中。


完成上述操作后的目录和文件结构如下：
```
<FM650 Root>
|
|-- build.sh
|-- fibo-kernel
|-- fibo-rootfs
|-- fibo_sign_package
|-- curl
|-- fibo-compiletool
|-- fibo-ota
|-- fibo-uboot
`-- fibo-sdk
    |-- Makefile
    |-- umdp
    |   |-- bin
    |   |-- config
    |   |-- include
    |   |-- lib
    |   |-- example
    |   |   |-- Makefile
    |   |   |-- boat_demo
    |   |   |   |-- Makefile
    |   |   |   |-- demo_venachain_mycontract.c
    |   |   |   |-- mycontract.c
    |   |   |   |-- mycontract.h
    |   |   |   |-- libs
    |   |   |   |   |--libcrypto.so.1.0.2
    |   |   |   |   |--libcurl.so.4.5.0
    |   |   |   |   |--libssl.so.1.0.2
```


## 三、文件修改


### 1、修改demo的Makefile

打开`<FM650 Root>/fibo-sdk/umdp/example/boat_demo/Makefile`文件

在配置头文件目录的地方新添加以下内容,例如：
```
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-Engine/include
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/include
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/include
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/log
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/inet
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/dal
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/src/osal
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/common/storage
CFLAGS += -I/home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/keystore
```

根据本地实际的情况设置所需链接库的位置，例如：
```
LDFLAGS += -L{real_location}/lib -lboatengine
LDFLAGS += -L{real_location}/lib -lboatvendor

LDFLAGS += ./libs/ex_shared_libs/libcurl.so.4.5.0
LDFLAGS += ./libs/ex_shared_libs/libssl.so.1.0.2
LDFLAGS += ./libs/ex_shared_libs/libcrypto.so.1.0.2
```

设置生成的可执行文件的名字，例如：
```
TARGET = demo_boat_call_venachain
```


### 2、配置动态库头文件路径

  打开`BoAT-SupportLayer/platform/Fibocom-FM650/external.env`文件
  
  在`EXTERNAL_INC`后面配置FM650平台curl和openssl的头文件和动态库的实际路径，例如：
```
EXTERNAL_INC := -I/home/alex/tool/curl/curl/include \
                -I/home/alex/works/fibocom/openssl-OpenSSL_1_0_2r/include

EXTERNAL_LIBS := /home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/ex_libs/libcurl.so.4.5.0
EXTERNAL_LIBS += /home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/ex_libs/libssl.so.1.0.2
EXTERNAL_LIBS += /home/alex/works/BoAT-ProjectTemplate/BoAT-SupportLayer/platform/Fibocom-FM650/ex_libs/libcrypto.so.1.0.2
```
  
## 四、编译BoAT静态库

### 1、配置交叉编译器
   
   #### a、在Linux上打开一个终端，进入到fibo-compiletool目录
   ```
    cd <FM650 Root>/fibo-compiletool
   ```

   #### b、使交叉编译指令在该终端上生效
   ```
    source environment-setup-aarch64-unisoc-linux
   ```
   生效成功后，在该终端执行`echo $CC`和`echo $AR`会看到相应的命令内容，后续的BoAT静态库的编译也必须保证在该终端中完成。

### 2、编译BoAT静态库

   #### a、配置主Makefile中的编译信息，修改以下内容（以Venachain为例）：
   ```
   BOAT_PROTOCOL_USE_VENACHAIN    ?= 1

   PLATFORM_TARGET ?= Fibocom-FM650
   ```
   
   #### b、编译BoAT静态库
   ```
   make clean
   make all
   ```
   
   编译成功后，在`lib`下会生成静态库libboatvendor.a、libboatengine.a。
   
## 五、编译并运行调用BoAT的demo

### 1、配置区块链信息（以Venachain为例）

  #### a、将部署好或者已知可用的节点地址配置给`<FM650 Root>/fibo-sdk/umdp/example/boat_demo/demo_venachain_mycontract.c`中的`demoUrl`变量，例如：
  ```
  const BCHAR *demoUrl = "http://121.0.0.1:7545";

  ```
  #### b、将已在链上部署好的智能合约地址配置给`<FM650 Root>/fibo-sdk/umdp/example/boat_demo/demo_venachain_mycontract.c`中的`demoRecipientAddress`变量，例如：
  ```
  const BCHAR *demoRecipientAddress = "0xaac9fb1d70ee0d4b5a857a28b9c3b16114518e45";
  ```


### 2、编译demo

  #### a、运行`<FM650 Root>/build.sh`并配置项目
  ```
  ./build.sh project FG650_CN_OPENCPU_OPEN
  ```

  #### b、编译fibo-sdk
  ```
  ./build.sh sdk
  ```
  执行成功后，会在`<FM650 Root>/fibo-sdk/umdp/example/boat_demo`下生成`Object`文件夹，其中包含最终生成的可执行文件`demo_boat_call_venachain`

### 4、运行demo

  #### a、将FM650板子上电并通过USB线连接PC

  #### b、按照板子的文档说明，打开ADB功能

  #### c、在pc上打开一个终端，通过ADB将可执行文件`demo_boat_call_venachain`push到板子内（以/home/user为例）
  ```
  adb push demo_boat_call_venachain /home/user
  ```

  #### d、在FM650的SIM卡槽中插入一张可用的SIM卡

  #### e、在PC上打开一个终端，并运行FM650中的demo
  ```
  adb shell
  cd /home/user
  chmod +x demo_boat_call_venachain
  ./demo_boat_call_venachain
  ```
