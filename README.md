## ***BoAT Support Layer 是什么***
***BoAT Support Layer***是什么？必须了解***BoAT Infra Arch***是什么
    
## ***BoAT Infra Arch 是什么***
***BoAT Infra Arch*** 即BoAT基础架构,是一个跨平台应用框架，通过抽象不同平台的应用接口，为应用提供统一的跨平台应用抽象接口，应用程序可以通过调用这些抽象接口快速实现在不同平台之间移植。

***BoAT Infra Arch***包含四层结构
    
1.平台层，提供基础的系统支持，向上层提供各个平台的系统应用接口。
    
2.***BoAT Support Layer***层，是***BoAT Infra Arch***架构的核心层，***BoAT Support Layer***向上层提供统一的跨平台应用抽象接口，并按照不同平台提供的系统API接口实现不同平台的跨平台应用抽象接口的实例，即跨平台应用抽象接口在不同平台的具体实现。

3.***Composable BoAT Core*** 可组合BoAT核心层，熟悉BoAT的朋友会知道这就是曾经BoAT功能的核心组件，***Composable BoAT Core***使用***BoAT Support Layer***提供的跨平台应用抽象接口实现BoAT的核心功能，包括对不同区块链上链、查询等操作的支持。

4.应用层，可以直接使用***BoAT Support Layer***提供的跨平台应用抽象接口实现应用功能，也可以调用***Composable BoAT Core***提供的区块链操作接口实现对各种区块链的访问。

## ***BoAT Support Layer*** 实现什么功能
***BoAT Support Layer***是***BoAT Infra Arch***架构的核心支柱层，基于***BoAT Infra Arch***架构的所有应用程序的实现都是以***BoAT Support Layer***层为基础。***BoAT Support Layer***向上为应用层和 ***Composable BoAT Core***层提供统一的跨平台应用抽象接口，向下按照不同平台提供的系统API接口实现抽象接口的具体实例，应用程序通过调用统一的跨平台应用抽象接口，间接调用适配平台的系统API接口实现应用功能。

## ***BoAT Support Layer***组件
***OSAL：***

操作系统抽象层，定义底层平台操作系统相关抽象对象的跨平台应用抽象接口，按照接口定义实现不同平台操作系统抽象对象接口的实例，通常包含 ***semaphore***、***task***、***timer***、***queue***等系统功能抽象对象
	
***DAL：***

驱动抽象层，定义平台相关驱动的抽象对象的跨平台应用抽象接口，按照接口定义实现不同平台操作系统抽象对象接口的实例，通常包含uart、i2c、虚拟AT、storage等
	
***BoAT Common Components：***

***BoAT***通用组件，包含区块链应用相关的跨平台应用通用接口，不仅仅提供给区块链应用使用，也可以为其他应用调用，包含***keypair***、***keystore***、***third-party***以及其他common组件

## 目录说明
### BoAT-SupportLayer
```
<BoAT-SupportLayer>
|
+---common            | Universal soft algorithms implementation
|   \---http2intf     |     Universal soft algorithms implementation
|   \---rpc           |     rpc interface
|   \---storage       |     interface for storing data or files
|   \---utilities     |     Utility APIs
+---include           | Header files for application to include
+---keypair           | Interface for generating and managing key pairs
+---keystore          | Store a series of Secret Keys, Key Pairs, or Certificates
|   +---SE            |     Keystore Interface Implemented by Security Chip
|   \---soft          |     Keystore Interface Implemented by Software
+---platform          | Implementation of Abstract Interfaces on Different Operating Platforms
|   +---Chin...DCLN   |     Implementation of Abstract Interface in ChinaMobile-ML307A-DCLN
|   +---Fibocom-L610  |     Implementation of Abstract Interface in Fibocom-L610
|   +---include       |     Header files for platform internal use
|   +---linux-default |     Implementation of Abstract Interface in ChinaMobile-ML307A-DCLN
+---tests             | Test cases
+---third-party       |     Third party libraries
|   \---cJSON         |     JSON parsing interface for C language 
|   \---crypto        |     Including various software encryption algorithms
|   \---nghttp2       |     HTTP/2 library implemented by C language
|   \---protobuf-c    |     protobuf interface
|   \---protos        |     protos interface
|   +---rlp           |     RLP encoder

```
## 使用说明
BoAT-SupportLayer 的编译和应用参考 BoAT-ProjectTemplate的README_cn.md

https://github.com/aitos-io/BoAT-ProjectTemplate/blob/dev/README_cn.md
