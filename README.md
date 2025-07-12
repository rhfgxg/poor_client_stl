# poor_server_stl 项目介绍
本项目是主要使用c++ QT库 实现的一个客户端
对应服务端项目：https://github.com/rhfgxg/poor_server_stl
在 tools 文件夹下，会有一些工具脚本，用来辅助开发 

本客户端只用来做服务器接口测试

## 项目目的：实现并学习下面功能：
1. 功能实现：
1.1 游戏：炉石传说
1.2 网盘：文件上传下载，断点续传
1.3 网盘：图片预览，压缩等

2. 使用多模块设计，在编译时将其他模块作为dll进行链接
    实现功能的插件化，用户可以选择性安装功能模块

# 安装说明
运行环境：windows 10 / 11
## 源码编译
```git clone https://github.com/rhfgxg/poor_client_stl.git```

## 安装包（暂无）


# 参与项目

## 项目规范
[项目开发文档](docunment)

## 开发环境配置
[开发环境配置](docunment/client/配置和运行项目.md)

## 项目文件树，项目关键文件说明
[文件树](tree.txt)

一些关键文件说明：
tree.txt：文件树（包含其他文件的详细介绍）
vcpkg_json：第三方库文件列表
.editorconfig：VS 配置文件，设置文件编码格式为 utf-8（不使用 VS 可以忽略）

config/：系统配置文件
projrct_document/：项目文档
protobuf/：各模块 grpc通信 proto协议文件
src/：各模块源码文件
tools/：工具文件
vpckg_installed/：第三方库文件安装目录

使用 VS: out/build/x64-debug/ ：项目可执行文件根目录

### 参与此项目，你可能需要修改的部分文件
由于开发环境的变化，可能需要修改一些路径或配置文件

注意，这些文件请不要提交到 git 仓库中，只用来作为个人环境适配           
1. config/server_config.lua    # 服务器配置文件，可以根据需要修改地址和端口（代码中读取此文件进行初始化）
2. tools/debug/protoc_make.cmd      # 生成 proto 文件的脚本，根据自己的项目目录修改 PROTOC 和 GRPC_PLUGIN变量，在9，10行
3. 所有的 CMake 文件，常量部分：vcpkg工具链路径：指向 vcpkg安装目录下的 vcpkg.cmake 文件
    vcpkg管理的第三方库路径

## 编译与部署
### 编译
在本地环境下，使用 cmake 编译项目
得到可执行文件，dll文件

### 部署（脚本一键部署）
部署时，/config/server_config.lua文件会被脚本自动分割（每个服务器独立的配置文件）
为了保证代码一致性，开发时，需要使用另一个脚本，将debug模式的服务器配置文件分割并复制到各个服务器目录下