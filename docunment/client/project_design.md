# 项目设计文档
[项目设计文档](project_docunment/aaa.md)

项目使用多模块，插件化设计，方便扩展和维护
用户可以选择性安装部分功能模块
主模块打包成为一个可执行文件，其他模块打包成为 dll插件，放在指定目录下，主模块自动加载插件

模块之间通过grpc通信

## 各模块功能
- [x] 主模块 central
包含用户管理，网关，插件管理，配置文件管理等核心功能
其他模块的所有功能都需要通过主模块提供的接口与服务器通信
日志模块，负责记录所有模块的日志

- [ ] 文件模块（插件）
负责客户端所有资源文件的管理，包括下载，更新，删除等
负责游戏资源文件的管理
负责网盘功能

- [ ] 游戏模块（插件）


## 项目结构
### 主模块
只通过类和文件夹来区分模块，不进行单独编译

### 子模块
编译为 dll文件或其他适合作为插件的文件格式
不同模块占用独立文件夹，不同模块之间不允许有相互引用
模块之间的共享资源使用 commont文件夹管理（插件管理属于主模块部分）


# 插件化设计
## 主模块
插件管理类：plugin_manager，负责加载和卸载插件
主模块创建 插件管理对象，管理插件

## 插件
插件接口类（抽象类）：plugin_interface，所有插件都需要继承该类，提供插件的生命周期函数
插件类：plugin，继承plugin_interface，实现插件的具体功能
插件功能实现类：*_service，继承grpc，实现插件的具体功能
