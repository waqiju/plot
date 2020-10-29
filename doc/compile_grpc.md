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
