## What is the BoAT Support Layer?
What is the ***BoAT Support Layer***? It is necessary to understand what the ***BoAT Infra Arch*** is.

## What is the BoAT Infra Arch?
The ***BoAT Infra Arch***, or BoAT Infrastructure Architecture, is a cross-platform application framework that provides a unified cross-platform application abstraction interface by abstracting different platform application interfaces. Applications can quickly port between different platforms by calling these abstract interfaces.

The ***BoAT Infra Arch*** consists of four layers:

1. The platform layer, which provides basic system support and platform-specific APIs to the upper layers.

2. The ***BoAT Support Layer***, which is the core layer of the ***BoAT Infra Arch*** architecture. The ***BoAT Support Layer*** provides a unified cross-platform application abstraction interface to the upper layer, and implements different platform-specific cross-platform application abstraction interface instances based on the system APIs provided by different platforms.

3. The Composable BoAT Core, which is the core component of the BoAT's features. The Composable BoAT Core uses the cross-platform application abstraction interface provided by the ***BoAT Support Layer*** to implement BoAT's core functions, including support for different blockchain upload and inquiry operations.

4. The application layer, which can directly implement application functions through the cross-platform application abstraction interface provided by the ***BoAT Support Layer***, or access various blockchains by calling the blockchain operation interface provided by the Composable BoAT Core.

## What Functions does the BoAT Support Layer Implement?
The ***BoAT Support Layer*** is the core pillar layer of the ***BoAT Infra Arch*** architecture. All applications implemented based on the ***BoAT Infra Arch*** architecture are based on the ***BoAT Support Layer*** layer. The ***BoAT Support Layer*** provides unified cross-platform application abstraction interfaces to the application layer and the Composable BoAT Core layer. The layer below it provides the concrete instances of the abstract interfaces based on the system APIs provided by different platforms. Applications call the unified cross-platform application abstraction interface to indirectly call the system APIs that are adapted to the platform to implement the application functions.

## BoAT Support Layer Components
***OSAL:***

The Operating System Abstraction Layer defines cross-platform application abstraction interfaces for platform-specific operational system-related abstract objects. The interface is implemented according to different platform operating system abstract object interfaces. Generally, it includes system functional abstraction objects such as semaphores, tasks, timers, and queues.

***DAL:***

The Driver Abstraction Layer defines cross-platform application abstraction interfaces for platform-specific driver abstract objects. The interface is implemented according to the interface definition of different platform operating system abstract object interfaces. Generally, it includes system functional abstraction objects such as UART, I2C, virtual AT, and storage.

***BoAT Common Components:***

The BoAT Common Components include cross-platform application common interfaces related to blockchain applications. They are not only used by blockchain applications, but can also be called by other applications. They include keypair, keystore, third-party, and other common components.

## Directory Description
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
## Usage Instructions
Refer to the README.md of BoAT-ProjectTemplate for the compilation and application of BoAT-SupportLayer.

https://github.com/aitos-io/BoAT-ProjectTemplate/blob/dev/README.md
