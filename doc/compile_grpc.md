# compile_grpc


## Guide

https://github.com/grpc/grpc/blob/v1.33.1/BUILDING.md

https://github.com/grpc/grpc/blob/v1.33.1/src/cpp/README.md

https://www.cnblogs.com/MakeView660/p/11511136.html
博客作者的步骤教程，日期比较旧了，建议还是以官方为准


## Step

1. 安装choco

https://chocolatey.org/install

按照网站指引安装

2. 安装依赖

choco install nasm
choco install ninja


3. 获取源码

git clone -b v1.33.0 https://github.com/grpc/grpc


4. 编译源码

```
cmake .. -G "Visual Studio 14 2015"
=>
cmake .. -G "Visual Studio 16 2019" -DBUILD_SHARED_LIBS=ON
```

## 本地拉取 github 仓库代码速度过慢

整个 grpc + submodule 大小有 200 M

使用香港的服务器拉取了代码，压缩为 tar.gz 传回本地。本地解压使用 7zip。

注意，仓库里有使用软链接。windows 需要管理员权限，从开始菜单找到 7zip，右键管理员权限，接着找到目录解压等。

TODO 自行编译 grpc ...


## 使用 vcpkg 安装 grpc

这是最简便的方法了，除了需要网络梯子外。

```
# install vcpkg package manager on your system using the official instructions
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg integrate install

# install gRPC using vcpkg package manager
vcpkg install grpc
```

问题，vcpkg 下载 strawberry-perl-5.30.0.1-32bit.zip 会一直失败。
在浏览器中下载好，然后放置到 vcpkg/downloads 目录下。


## visual studio proejct 配置 grpc

1. 头文件

C++ -> 常规 -> 附加包含目录

D:\1_Workspace\vcpkg\installed\x86-windows\include

2. lib

链接器 -> 常规 -> 附加库目录

D:\1_Workspace\vcpkg\installed\x86-windows\debug\lib


链接器 -> 输入 -> 附加库依赖项

grpc.lib
grpc++.lib
gpr.lib
libprotobufd.lib
Ws2_32.lib
zlibd.lib
cares.lib
address_sorting.lib


## visual studio proejct 配置 grpc, 通过 grpc install grpc

链接器 -> 输入 -> 附加库依赖项
Ws2_32.lib

vcpkg 管理的包，在 visual studio 中使用，直接在代码里 include <头文件> 即可。
** 不用额外配置头文件包含目录、链接包含目录和链接依赖项 **

这里需要额外配置下 Ws2_32.lib, 原因未知。
