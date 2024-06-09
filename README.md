## [muse-simulator](#)
**介绍**: 分布式算法仿真软件，该程序的目的是构建模拟的分布式网络环境，使得用户可以轻松进行分布式算法的测试。

-----

### 使用、安装
前提本地环境已经安装好 cmake、git 等工具， 直接使用，在当前目录下继续开发，可以参考如下命令，运行：
```cpp
git clone git@github.com:sorise/muse-simulator.git
cd dependency
//windows 系统中手动解压 bls.zip 
//linux 系统中解压 bls.zip：  unzip bls.zip
cmake -S . -B build
cmake --build build
```
**安装**：需要有管理员权限， windows安装.... 暂时还没摸索出来
```cpp
cd build
make install 
```

### 说明
该分布式算法仿真软件较为简单，模拟了网络环境、主机网卡、CPU、分布式定时器。
* 支持用户自定义网络时延。
* 缺点：**对主机的CPU模拟采用高度抽象的方式，只模拟了主机CPU的调度，用户必须自己指定某个函数的CPU处理时间。**
* 缺点：该软件测试结果与实际环境只具有相似性，不具备完备的模拟能力，无法对CPU进行指令执行级模拟
* 假设主机具有无线的内存和硬盘能力


#### 外部依赖
* [Google benchmark](#) 基准测试工具
* [Google Gtest](#) 基础测试工具
* [fmt](#) 格式化输出工具
* [secp256k1-zkp](#) 提高了schnorr签名和 MutSig2 聚合签名
* [OpenSSL](#)  开源的加密库，提供了安全通信的基础设施。
* [bls-signatures](#) BLS signatures in C++, using the blst library for BLS12-381
