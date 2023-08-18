# Linux-default 平台下BoAT Infra Arch 基础架构区块应用适配说明


## 一、前言

本文档说明如何在`linux-default`开发环境中，使用 `BoAT Infra Arch` 基础架构开发模板`BoAT-ProjectTemplate`构建不同区块链`demo`程序的过程。
### 编译环境
操作系统：
```
ubuntu 16.04
```
软件依赖：
```
gcc
   ubuntu 16.04已安装
make
   ubuntu 16.04已安装
git
   sudo apt install git -y
python3
   sudo apt install python3 -y
curl
   sudo apt install curl -y
```
通常通过虚拟机安装`ubuntu 16.04`操作系统，在其中进行编译。

## 二、构建编译目录

约定`<Linux Root>`是`linux-default`平台编译的根目录：

1、下载BoAT-ProjectTemplate编译模板

在`ubuntu 16.04`操作系统中打开终端，进入`<linux Root>`目录，通过`git clone`命令，在`<Linux Root>`目录下下载`BoAT-ProjectTemplate`库，命令参考如下:
```
git clone -b dev git@github.com:aitos-io/BoAT-ProjectTemplate.git
```
或
```
git clone -b dev https://github.com/aitos-io/BoAT-ProjectTemplate.git
```
  
2、进入 `<Linux Root>/BoAT-ProjectTemplate/` 目录，修改 `BoATLibs.conf` 文件

将 `BoATLibs.conf` 文件替换为以下内容：
```
BoAT-SupportLayer
BoAT-Engine
```
保存并退出。

3、在 `<Linux Root>/BoAT-ProjectTemplate/` 目录下执行配置脚本
```
python3 config.py
```
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
选择使用的区块链，可以选择一个或多个区块链，本例选择三个区块链。

输入“985”回车，选择VENACHAIN、CHAINMAKER_V2、HLFABRIC区块链。
```
input:985
Blockchain selected:
 [5] HLFABRIC
 [8] CHAINMAKER_V2
 [9] VENACHAIN

Select the platform list as below:
[1] linux-default             : Default linux platform
[2] Fibocom-L610              : Fibocom's LTE Cat.1 module
[3] create a new platform
```
输入‘1’回车，选择“linux-default”平台
```
1
 
platform is : linux-default

include BoAT-SupportLayer.conf

include BoAT-Engine.conf


./BoAT-SupportLayer/demo/ False
./BoAT-Engine/demo/ True
Configuration completed
```
配置完成。

完成上述操作后部分目录和文件结构如下：
```
<Linux Root>
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

通过指令查询目录下的文件:
```
u1604@ubuntu:~/linuxRoot$ ls
BoAT-Engine  BoATLibs.conf  BoAT-SupportLayer  config.py  Makefile  README.md  README_en.md

```

## 四、编译

可分别编译静态库和区块链`demo`应用程序。

### 1、编译静态库

```
make clean
make all
```
编译成功后，在`<Linux Root>/lib`目录下会生成静态库`libboatvendor.a`、`libboatengine.a`，在区块链`demo`应用程序链接时，需要引用这两个库。
   
### 2、编译VENACHAIN、HLFABRIC、CHAINMAKER区块链`demo`
```
make demo VENACHAIN_DEMO_IP="x.x.x.x" FABRIC_DEMO_IP="y.y.y.y" CHAINMAKER_DEMO_IP="z.z.z.z" 
```
其中`"x.x.x.x"`、`"y.y.y.y"`、`"z.z.z.z"`分别为`Venachain`、`HLFABRIC`、`CHAINMAKER`各个区块链节点ip地址。

编译成功后，会在`<Linux Root>/build/BoAT-Engine/demo`目录下生成三个区块链的`demo`目录：
```
demo_chainmaker
demo_fabric
demo_venachain
```

目录中存放各个区块链的`demo`可执行文件。运行这些可执行文件可以演示区块链的应用过程和执行结果。

各目录中的可执行文件如下：
```
ls build/BoAT-Engine/demo/demo_chainmaker/
demo_chainmaker_load   demo_chainmaker_persist   demo_chainmaker_onetime

ls build/BoAT-Engine/demo/demo_fabric/
demo_fabric_abac_onetime  demo_fabric_abac_persist

ls build/BoAT-Engine/demo/demo_venachain/
demo_venachain_mycontract_create_internalGen
demo_venachain_mycontract_create_native
demo_venachain_mycontract_create_pkcs
demo_venachain_mycontract_onetime_internalGen
demo_venachain_mycontract_onetime_native
demo_venachain_mycontract_onetime_pkcs
```


### 3、编译常见问题
编译过程中可能会遇到以下错误，按照给出的处理方法可解决相应问题。

#### 常见问题1：
```
curlport.c:33:23: fatal error: curl/curl.h: No such file or directory
```
安装`libcurl4-gnutls-dev`依赖库：
```
sudo apt install libcurl4-gnutls-dev
```
#### 常见问题2：
```
boatssl.c:26:25: fatal error: openssl/evp.h: No such file or directory
```
安装`libssl-dev`依赖库
```
sudo apt install libssl-dev
```

## 五、运行demo
在`<Linux Root>`目录下运行demo，以运行`chainmaker demo`为例。

执行指令：
```
./build/BoAT-Engine/demo/demo_chainmaker/demo_chainmaker_onetime
```
执行结果如下：
```
LOG_NORMAL: boat_keystore_intf.c:505, BoatPort_keyCreate(): prikey_genMode = 1 .
LOG_NORMAL: boat_keystore_intf.c:513, BoatPort_keyCreate(): prikey_format = 1 .
LOG_NORMAL: boat_keystore_intf.c:517, BoatPort_keyCreate(): keypair private key[pkcs] set...
LOG_NORMAL: boatutility.c:1102, UtilityPKCS2Native():  UtilityPKCS2Native TLV_Level_3.len = 8 
LOG_NORMAL: boatutility.c:1113, UtilityPKCS2Native():  UtilityPKCS2Native oid_secp256r1 .
LOG_NORMAL: boatkeypair.c:1024, BoatKeypairCreate(): BoatPort_keyCreate success , index = 0 
LOG_NORMAL: boatkeypair.c:846, BoATKeypair_DataCtx_Store(): check keypair num fail
LOG_NORMAL: boatkeypair.c:913, BoATKeypair_DataCtx_Store():  keypair num = 1 ,result = 0 
LOG_NORMAL: boat_keystore_soft.c:397, BoAT_Keystore_store_prikey_soft(): read prikey num  = 0 
LOG_NORMAL: network_chainmaker.c:724, BoATChainmakerNetworkCreate(): network create ontime
LOG_NORMAL: network_chainmaker.c:578, BoATChainmaker_NetworkDataCtx_Store(): read networkNum = 0
LOG_NORMAL: network_chainmaker.c:636, BoATChainmaker_NetworkDataCtx_Store(): store chainmaker network, networkLength = 3147
LOG_NORMAL: network_chainmaker.c:748, BoATChainmakerNetworkCreate(): network creat success networkIndex = 0
LOG_CRITICAL: demo_chainmaker.c:264, main(): wallet init keypair_index =0, network_index = 0
LOG_NORMAL: network_chainmaker.c:1005, BoATChainmaker_GetNetworkByIndex(): network num = 1 
LOG_NORMAL: network_chainmaker.c:1023, BoATChainmaker_GetNetworkByIndex(): networkindex = 0 , index = 0 
LOG_NORMAL: network_chainmaker.c:1024, BoATChainmaker_GetNetworkByIndex(): protocol type = 7 
LOG_NORMAL: network_chainmaker.c:1030, BoATChainmaker_GetNetworkByIndex(): begin to read the network that index = 0 
LOG_NORMAL: boat_keystore_soft.c:260, BoAT_GetPrikeyByIndex_soft(): prikey num = 1
LOG_NORMAL: boat_keystore_soft.c:290, BoAT_GetPrikeyByIndex_soft(): prikeyindex = 0 , index = 0
LOG_VERBOSE: boatchainmaker.c:150, hlchainmakerTransactionPacked(): Transaction Message tx_id = 31106e41dc4b42fe8b7582f86e4d7750f5b5156bd0cef517ef53a1968270685c
LOG_VERBOSE: boatchainmaker.c:151, hlchainmakerTransactionPacked(): Transaction Message time = 1688096006
LOG_VERBOSE: boatchainmaker.c:155, hlchainmakerTransactionPacked(): Transaction Message key = time, value = 1669200547
LOG_VERBOSE: boatchainmaker.c:155, hlchainmakerTransactionPacked(): Transaction Message key = file_hash, value = a3f485ea14d946168d2a448adea8d47123
LOG_VERBOSE: boatchainmaker.c:155, hlchainmakerTransactionPacked(): Transaction Message key = file_name, value = file_name
LOG_VERBOSE: boatssl.c:157, BoatConnect(): localIP: 192.168.47.130:42232.
LOG_VERBOSE: boatssl.c:160, BoatConnect(): z.z.z.z:12301[3] connected!
LOG_VERBOSE: http2intf.c:112, on_frame_recv_callback(): [http2] NGHTTP2_SETTINGS received.
LOG_VERBOSE: http2intf.c:112, on_frame_recv_callback(): [http2] NGHTTP2_SETTINGS received.
LOG_VERBOSE: http2intf.c:106, on_frame_recv_callback(): [http2] NGHTTP2_HEADERS received. 
LOG_VERBOSE: http2intf.c:109, on_frame_recv_callback(): [http2] NGHTTP2_DATA received.
LOG_VERBOSE: http2intf.c:106, on_frame_recv_callback(): [http2] NGHTTP2_HEADERS received. 
LOG_CRITICAL: boatssl.c:295, BoatRecv(): tlsContext  can't be NULL.
LOG_NORMAL: api_chainmaker.c:236, BoatChainmakerTxRequest(): [http2] respond received.
LOG_NORMAL: boat_keystore_soft.c:260, BoAT_GetPrikeyByIndex_soft(): prikey num = 1
LOG_NORMAL: boat_keystore_soft.c:290, BoAT_GetPrikeyByIndex_soft(): prikeyindex = 0 , index = 0
LOG_VERBOSE: boatchainmaker.c:150, hlchainmakerTransactionPacked(): Transaction Message tx_id = a32b664fa61ced3ccd8b199ac8ba73e4871f43303a579682517d7c497c9698ce
LOG_VERBOSE: boatchainmaker.c:151, hlchainmakerTransactionPacked(): Transaction Message time = 1688096006
LOG_VERBOSE: boatchainmaker.c:155, hlchainmakerTransactionPacked(): Transaction Message key = txId, value = 31106e41dc4b42fe8b7582f86e4d7750f5b5156bd0cef517ef53a1968270685c
LOG_VERBOSE: boatssl.c:157, BoatConnect(): localIP: 192.168.47.130:42234.
LOG_VERBOSE: boatssl.c:160, BoatConnect(): z.z.z.z:12301[3] connected!
LOG_VERBOSE: http2intf.c:112, on_frame_recv_callback(): [http2] NGHTTP2_SETTINGS received.
LOG_VERBOSE: http2intf.c:112, on_frame_recv_callback(): [http2] NGHTTP2_SETTINGS received.
LOG_VERBOSE: http2intf.c:106, on_frame_recv_callback(): [http2] NGHTTP2_HEADERS received. 
LOG_VERBOSE: http2intf.c:109, on_frame_recv_callback(): [http2] NGHTTP2_DATA received.
LOG_VERBOSE: http2intf.c:106, on_frame_recv_callback(): [http2] NGHTTP2_HEADERS received. 
LOG_CRITICAL: boatssl.c:295, BoatRecv(): tlsContext  can't be NULL.
LOG_NORMAL: api_chainmaker.c:236, BoatChainmakerTxRequest(): [http2] respond received.
LOG_CRITICAL: demo_chainmaker.c:297, main(): response code = 0, message = SUCCESS
LOG_NORMAL: api_chainmaker.c:577, BoatChainmakerContractQuery(): Submit will execute...
LOG_NORMAL: boat_keystore_soft.c:260, BoAT_GetPrikeyByIndex_soft(): prikey num = 1
LOG_NORMAL: boat_keystore_soft.c:290, BoAT_GetPrikeyByIndex_soft(): prikeyindex = 0 , index = 0
LOG_VERBOSE: boatchainmaker.c:150, hlchainmakerTransactionPacked(): Transaction Message tx_id = a5b247b7c0c35d8e1f64f0f0f2628f07a965c569f472772fc301217746bcda9b
LOG_VERBOSE: boatchainmaker.c:151, hlchainmakerTransactionPacked(): Transaction Message time = 1688096008
LOG_VERBOSE: boatchainmaker.c:155, hlchainmakerTransactionPacked(): Transaction Message key = file_hash, value = a3f485ea14d946168d2a448adea8d47123
LOG_VERBOSE: boatssl.c:157, BoatConnect(): localIP: 192.168.47.130:42236.
LOG_VERBOSE: boatssl.c:160, BoatConnect(): z.z.z.z:12301[3] connected!
LOG_VERBOSE: http2intf.c:112, on_frame_recv_callback(): [http2] NGHTTP2_SETTINGS received.
LOG_VERBOSE: http2intf.c:112, on_frame_recv_callback(): [http2] NGHTTP2_SETTINGS received.
LOG_VERBOSE: http2intf.c:106, on_frame_recv_callback(): [http2] NGHTTP2_HEADERS received. 
LOG_VERBOSE: http2intf.c:109, on_frame_recv_callback(): [http2] NGHTTP2_DATA received.
LOG_VERBOSE: http2intf.c:106, on_frame_recv_callback(): [http2] NGHTTP2_HEADERS received. 
LOG_CRITICAL: boatssl.c:295, BoatRecv(): tlsContext  can't be NULL.
LOG_NORMAL: api_chainmaker.c:236, BoatChainmakerTxRequest(): [http2] respond received.
LOG_CRITICAL: demo_chainmaker.c:318, main(): response code = 0,  message = SUCCESS,  contract_result = {"file_hash":"a3f485ea14d946168d2a448adea8d47123","file_name":"file_name","time":1669200547}, tx_id = a5b247b7c0c35d8e1f64f0f0f2628f07a965c569f472772fc301217746bcda9b
```

LOG信息显示`chainmaker` 区块链 `demo` 执行详细过程，可参照 `chainmaker_demo.c` 程序源码了解详细信息。
