## BoAT Support Layer

***BoAT Support Layer***，是实现***BoAT EDGE***产品的[***BoAT Infra Arch***](https://github.com/phengao/hello-world/blob/master/docs/BoAT_Overall_Design_cn.md)基础架构的核心组件，在不同***platform***平台和***BoAT EDGE***产品应用组件之间构建跨平台应用接口静态库。  
基于***BoAT Infra Arch***架构的所有应用程序的实现都以***BoAT Support Layer***层为基础。***BoAT Support Layer***向上为应用层和 ***Composable BoAT Core***层提供统一的跨平台应用抽象接口，向下按照不同平台提供的系统API接口实现抽象接口的具体实例，应用程序通过调用统一的跨平台应用抽象接口，间接调用适配平台的系统API接口实现应用功能。

## BoAT Support Layer组件
***OSAL（Operating System Abstraction Layer）：***

操作系统抽象层，定义底层平台操作系统相关抽象对象的跨平台应用抽象接口，按照接口定义实现不同平台操作系统抽象对象接口的实例，通常包含 ***semaphore***、***task***、***timer***、***queue***等系统功能抽象对象
	
***DAL（Driver Abstraction Layer）：***

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

## BoAT Support Layer代码下载
[代码下载](https://github.com/aitos-io/BoAT-SupportLayer/releases/tag/BoAT-SupportLayer-v3.1.0)
## BoAT Support Layer使用说明  

BoAT-SupportLayer 的编译和应用参考 BoAT-ProjectTemplate的[使用说明](https://github.com/aitos-io/BoAT-ProjectTemplate/blob/dev/README_cn.md)
