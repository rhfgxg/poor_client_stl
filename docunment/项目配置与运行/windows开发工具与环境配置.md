# 我更换了一台电脑，刚好来实践一下从零启动项目
电脑：thinkbook 14+ 2025,
系统：win11
全新的一台电脑，更适合观察所需的配置环境

# windwos 开发环境要求与配置
推荐：scoop软件包管理工具，用来安装 mysql，redis，vcpkg等

数据库：mysql8以上
redis：6.2以上
数据库可视化工具：navicat（推荐，但不必须）
软件包管理工具：scoop（推荐，但不必须）
第三方库管理工具：vcpkg
IDE：vs2022
编译器：msvc

## scoop 软件包管理工具
工具优点：
    使用指令安装软件，
    可以对软件进行版本管理（允许回退版本）
```
安装 scoop（需要翻墙）
1. 使用管理员权限打开 powershell
2. 让终端可以执行脚本，执行指令：
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser

3. 添加环境变量，以指定 scoop 的安装目录
$env:SCOOP='D:\Applications\Scoop'
[Environment]::SetEnvironmentVariable('SCOOP', $env:SCOOP, 'User')

4. 安装 scoop
Invoke-Expression (New-Object System.Net.WebClient).DownloadString('https://get.scoop.sh')
或执行：
iwr -useb get.scoop.sh | iex

5. 安装软件
scoop install mysql
scoop install redis

6. 将三个工具添加到系统环境变量

1. 安装 mysql后需要手动将 mysql 注册到系统服务中，执行指令：（需要管理员权限启动终端）
mysqld install
2. 启动 mysql 服务：
net start mysql

redis 安装后，每次都需要手动启动 redis 服务，指令：
redis-server.exe

安装后，软件的安装目录保存在 scoop安装目录/apps/包名/current 下
```

## 安装第三方库（使用vcpkg）
项目使用 vcpkg 作为第三方库管理工具，所以需要先安装 vcpkg
项目根目录下有一个 vcpkg.json 文件，里面列出了项目所需的第三方库

### 安装 vcpkg
不需要安装，vs2022 自带 vcpkg
路径：vs安装目录/VC/vcpkg

### 将 vcpkg 添加到系统环境变量
在 设置-系统-高级系统设置-环境变量-系统变量 中添加
```
添加变量：VCPKG_ROOT，指向 vcpkg 的安装目录
编辑变量：Path，添加 vcpkg 的安装目录
```

### 安装
在项目根目录下打开终端，执行：
```
vcpkg install
```

### 安装第三方库时遇到的错误

#### boost-headers:x64-windows 编译错误
```
Installing 1/79 boost-headers:x64-windows@1.86.0...
Building boost-headers:x64-windows@1.86.0...
-- Using cached boostorg-headers-boost-1.86.0.tar.gz.
-- Cleaning sources at F:/Program Source/vcpkg/buildtrees/boost-headers/src/ost-1.86.0-7dc5ae651a.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source F:/Program Source/vcpkg/downloads/boostorg-headers-boost-1.86.0.tar.gz
-- Using source at F:/Program Source/vcpkg/buildtrees/boost-headers/src/ost-1.86.0-7dc5ae651a.clean
CMake Warning (dev) at scripts/cmake/vcpkg_find_acquire_program.cmake:70 (cmake_parse_arguments):
  An empty string was given as the value after the INTERPRETER keyword.
  Policy CMP0174 is not set, so cmake_parse_arguments() will unset the
  arg_INTERPRETER variable rather than setting it to an empty string.
Call Stack (most recent call first):
  scripts/cmake/vcpkg_find_acquire_program.cmake:143 (z_vcpkg_find_acquire_program_find_internal)
  installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_configure.cmake:116 (vcpkg_find_acquire_program)
  installed/x64-windows/share/vcpkg-boost/boost-install.cmake:55 (vcpkg_cmake_configure)
  ports/boost-headers/portfile.cmake:12 (boost_configure_and_install)
  scripts/ports.cmake:192 (include)
This warning is for project developers.  Use -Wno-dev to suppress it.

CMake Warning (dev) at scripts/cmake/vcpkg_find_acquire_program.cmake:30 (cmake_parse_arguments):
  An empty string was given as the value after the INTERPRETER keyword.
  Policy CMP0174 is not set, so cmake_parse_arguments() will unset the
  arg_INTERPRETER variable rather than setting it to an empty string.
Call Stack (most recent call first):
  scripts/cmake/vcpkg_find_acquire_program.cmake:149 (z_vcpkg_find_acquire_program_find_external)
  installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_configure.cmake:116 (vcpkg_find_acquire_program)
  installed/x64-windows/share/vcpkg-boost/boost-install.cmake:55 (vcpkg_cmake_configure)
  ports/boost-headers/portfile.cmake:12 (boost_configure_and_install)
  scripts/ports.cmake:192 (include)
This warning is for project developers.  Use -Wno-dev to suppress it.

-- Found external ninja('1.11.0').
-- Configuring x64-windows
CMake Error at scripts/cmake/vcpkg_execute_required_process.cmake:127 (message):
    Command failed: "C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja/ninja.exe" -v
    Working Directory: F:/Program Source/vcpkg/buildtrees/boost-headers/x64-windows-rel/vcpkg-parallel-configure
    Error code: 1
    See logs for more information:
      F:\Program Source\vcpkg\buildtrees\boost-headers\config-x64-windows-rel-CMakeCache.txt.log
      F:\Program Source\vcpkg\buildtrees\boost-headers\config-x64-windows-rel-CMakeConfigureLog.yaml.log
      F:\Program Source\vcpkg\buildtrees\boost-headers\config-x64-windows-out.log

Call Stack (most recent call first):
  installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_configure.cmake:269 (vcpkg_execute_required_process)
  installed/x64-windows/share/vcpkg-boost/boost-install.cmake:55 (vcpkg_cmake_configure)
  ports/boost-headers/portfile.cmake:12 (boost_configure_and_install)
  scripts/ports.cmake:192 (include)


error: building boost-headers:x64-windows failed with: BUILD_FAILED
See https://learn.microsoft.com/vcpkg/troubleshoot/build-failures?WT.mc_id=vcpkg_inproduct_cli for more information.
Elapsed time to handle boost-headers:x64-windows: 5.1 s
Please ensure you're using the latest port files with `git pull` and `vcpkg update`.
Then check for known issues at:
  https://github.com/microsoft/vcpkg/issues?q=is%3Aissue+is%3Aopen+in%3Atitle+boost-headers
You can submit a new issue at:
  https://github.com/microsoft/vcpkg/issues/new?title=[boost-headers]+Build+error+on+x64-windows&body=Copy+issue+body+from+F%3A%2FProgram%20Source%2Fvcpkg%2Finstalled%2Fvcpkg%2Fissue_body.md

```

#### 安装编译 python3 错误
```
正在安装 1/71 个 python3[core,extensions]:x64-windows@3.11.8#5...
正在生成 python3[core,extensions]:x64-windows@3.11.8#5...
C:\Users\m1341\AppData\Local\vcpkg\registries\git-trees\cd869d19271e7b865248145bb3e14093faa1b687: info: 从 Git 注册表安装 git+https://github.com/microsoft/vcpkg@cd869d19271e7b865248145bb3e14093faa1b687
-- Using cached python-cpython-v3.11.8.tar.gz
-- Cleaning sources at D:/Opt/scoop/apps/vcpkg/current/buildtrees/python3/src/v3.11.8-23a9422c08.clean. Use --editable to skip cleaning for the packages you specify.
-- Extracting source D:/Opt/scoop/apps/vcpkg/current/downloads/python-cpython-v3.11.8.tar.gz
-- Applying patch 0001-only-build-required-projects.patch
...

CMake Error at scripts/cmake/vcpkg_execute_required_process.cmake:127 (message):
    Command failed: D:/Opt/ide/vs2022/vs2022/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja/ninja.exe -v
    Working Directory: D:/Opt/scoop/apps/vcpkg/current/buildtrees/python3/x64-windows-rel/vcpkg-parallel-configure
    Error code: 1
    See logs for more information:
      D:\Opt\scoop\apps\vcpkg\current\buildtrees\python3\cmake-get-vars-x64-windows-dbg-CMakeCache.txt.log
      D:\Opt\scoop\apps\vcpkg\current\buildtrees\python3\cmake-get-vars-x64-windows-rel-CMakeCache.txt.log
      D:\Opt\scoop\apps\vcpkg\current\buildtrees\python3\cmake-get-vars-x64-windows-dbg-CMakeConfigureLog.yaml.log
      D:\Opt\scoop\apps\vcpkg\current\buildtrees\python3\cmake-get-vars-x64-windows-rel-CMakeConfigureLog.yaml.log
      D:\Opt\scoop\apps\vcpkg\current\buildtrees\python3\cmake-get-vars-x64-windows-out.log

Call Stack (most recent call first):
  D:/Project/cpp/poor/poor_server_stl/vcpkg_installed/x64-windows/share/vcpkg-cmake/vcpkg_cmake_configure.cmake:269 (vcpkg_execute_required_process)
  D:/Project/cpp/poor/poor_server_stl/vcpkg_installed/x64-windows/share/vcpkg-cmake-get-vars/vcpkg_cmake_get_vars.cmake:15 (vcpkg_cmake_configure)
  D:/Project/cpp/poor/poor_server_stl/vcpkg_installed/x64-windows/share/vcpkg-msbuild/z_vcpkg_msbuild_create_props.cmake:56 (vcpkg_cmake_get_vars)
  D:/Project/cpp/poor/poor_server_stl/vcpkg_installed/x64-windows/share/vcpkg-msbuild/vcpkg_msbuild_install.cmake:53 (z_vcpkg_msbuild_create_props)
  C:/Users/m1341/AppData/Local/vcpkg/registries/git-trees/cd869d19271e7b865248145bb3e14093faa1b687/portfile.cmake:190 (vcpkg_msbuild_install)
  scripts/ports.cmake:206 (include)


error: 生成 python3:x64-windows 失败，结果为: BUILD_FAILED
有关详细信息，请参阅 https://learn.microsoft.com/vcpkg/troubleshoot/build-failures?WT.mc_id=vcpkg_inproduct_cli。
处理 python3:x64-windows 所用时间: 3.7 s
请确保使用具有“git pull” 和 “vcpkg update”的最新端口文件。
然后，在以下位置查看已知问题:
  https://github.com/microsoft/vcpkg/issues?q=is%3Aissue+is%3Aopen+in%3Atitle+python3
可在以下位置提交新问题:
  https://github.com/microsoft/vcpkg/issues/new?title=[python3]+Build+error+on+x64-windows&body=Copy%20issue%20body%20from%20D%3A%2FProject%2Fcpp%2Fpoor%2Fpoor_server_stl%2Fvcpkg_installed%2Fvcpkg%2Fissue_body.md

```
两个异常都是因为 windows sdk 10 缺失导致的
可以在 vs install工具中安装 windows 10 sdk，不需要全部安装，只需要安装 sdk 10.0.18362.0 版本
我在安装 vs 时D盘下出现了一个 windows Tiks 文件夹，里面就是 windows sdk
我手动将这个文件夹剪切到 vs 的安装目录下，就出现了异常
将vs安装目录下的 windows Tiks 文件夹复制到 d盘原位置，重启vs后，异常解决，成功安装所有第三方库

## 其他准备
### 编译 protobuf 文件（grpc）
安装第三方库时会自动安装和编译所需的 proto库 和 grpc 库
```
./tools/debug/protoc_make.cmd_
```

### 复制配置文件到生成目录
```
./tools/debug/copy_config.cmd
```

### 修改项目根目录下的 CMakeLists.txt
修改 CMAKE_TOOLCHAIN_FILE 常量
修改vcpkg安装目录为实际的 vcpkg 安装路径
```
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)    # 防止环境中定义了此变量
    set(CMAKE_TOOLCHAIN_FILE "vcpkg安装目录/current/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()
```

### vs2022 配置
#### 安装插件
上方菜单栏 - 扩展 - 管理扩展
搜索并下载插件，然后重启 vs即可

1. force utf-8 （no BOM）
- 作用：在保存文件时，自动将文件保存为 utf-8编码
2. FileEncoding
- 作用：在右下角查看并设置本文件编码