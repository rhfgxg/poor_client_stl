cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 定义常量
project("poor_client_stl" VERSION 1.0)
set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows")
set(VCPKG_INCLUDE "${CMAKE_PREFIX_PATH}/include")
set(VCPKG_LIB "${CMAKE_PREFIX_PATH}/lib")
set(VCPKG_BIN "${CMAKE_PREFIX_PATH}/bin")
set(VCPKG_DEBUG_LIB "${CMAKE_PREFIX_PATH}/debug/lib")
set(VCPKG_DEBUG_BIN "${CMAKE_PREFIX_PATH}/debug/bin")
# 设置 vcpkg 工具链文件路径
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)    # 防止环境中定义了此变量
    set(CMAKE_TOOLCHAIN_FILE "D:/Opt/ide/vs2022/vs2022/VC/vcpkg/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

# 头文件的路径列表
include_directories(
    ${VCPKG_INCLUDE}     # 第三方库的头文件所在目录
    # proto生成的 grpc服务头文件目录
    ${CMAKE_SOURCE_DIR}/protobuf/src
    # 共享工具类
    ${CMAKE_SOURCE_DIR}/src/common
    # 主模块及功能实现类
    ${CMAKE_SOURCE_DIR}/src/gateway
    ${CMAKE_SOURCE_DIR}/src/user
    ${CMAKE_SOURCE_DIR}/src/file
    # 插件模块
    ${CMAKE_SOURCE_DIR}/src/plugins/file
    ${CMAKE_SOURCE_DIR}/src/plugins/logic
)
# 需要编译的源文件列表
set(SOURCES
    src/main.cpp
    # 主模块功能实现类
    ${CMAKE_SOURCE_DIR}/src/gateway/gateway_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/user/user_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/file/file_manager.cpp
    # 公共模块
    ${CMAKE_SOURCE_DIR}/src/common/logger_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/common/plugin_manager.cpp
    ${CMAKE_SOURCE_DIR}/src/common/connection_pool.cpp
    # proto生成的源文件
    ${CMAKE_SOURCE_DIR}/protobuf/src/common.grpc.pb.cc
    ${CMAKE_SOURCE_DIR}/protobuf/src/common.pb.cc
    ${CMAKE_SOURCE_DIR}/protobuf/src/server_gateway.grpc.pb.cc
    ${CMAKE_SOURCE_DIR}/protobuf/src/server_gateway.pb.cc
    ${CMAKE_SOURCE_DIR}/protobuf/src/server_login.grpc.pb.cc
    ${CMAKE_SOURCE_DIR}/protobuf/src/server_login.pb.cc
    ${CMAKE_SOURCE_DIR}/protobuf/src/server_file.grpc.pb.cc
    ${CMAKE_SOURCE_DIR}/protobuf/src/server_file.pb.cc
)
# 指定编译生成的可执行文件信息：文件名${PROJECT_NAME} ，${SOURCES} 项目所有源文件路径
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4251 /wd4828 /utf-8")   # 忽略警告，使用utf-8编码

# 自动寻找第三方库 lib列表
find_package(absl REQUIRED)
find_package(Boost REQUIRED system)
find_package(Boost REQUIRED COMPONENTS asio)
find_package(c-ares REQUIRED)
find_package(fmt REQUIRED)
find_package(gRPC REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Protobuf REQUIRED)
find_package(re2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(ZLIB REQUIRED)
find_package(upb REQUIRED)
find_package(dlfcn-win32 REQUIRED)
find_package(libzip CONFIG REQUIRED)

# 链接第三方库lib
target_link_libraries(${PROJECT_NAME} PRIVATE
# 注释了一些暂时不需要的库
#    absl::base
#    absl::flags
#    Boost::system
#    c-ares::cares
#    fmt::fmt
#    OpenSSL::SSL
#    OpenSSL::Crypto
#    protobuf::libprotobuf
#    re2::re2
#    upb::upb
#    ZLIB::ZLIB
    Boost::asio
    gRPC::grpc++
    spdlog::spdlog
    Threads::Threads    # 多线程库
    dlfcn-win32::dl
    libzip::zip
)

# 第三方库 dll列表
set(DLL_FILES
    "${VCPKG_DEBUG_BIN}/boost_system-vc143-mt-gd-x64-1_86.dll"
    "${VCPKG_DEBUG_BIN}/dl.dll"
    "${VCPKG_DEBUG_BIN}/libprotobufd.dll"
    "${VCPKG_DEBUG_BIN}/libprotocd.dll"
    "${VCPKG_DEBUG_BIN}/libssl-3-x64.dll"
    "${VCPKG_DEBUG_BIN}/libcrypto-3-x64.dll"
    "${VCPKG_DEBUG_BIN}/abseil_dll.dll"
    "${VCPKG_BIN}/zlib1.dll"
    "${VCPKG_DEBUG_BIN}/zlibd1.dll"
    "${VCPKG_DEBUG_BIN}/re2.dll"
    "${VCPKG_DEBUG_BIN}/cares.dll"
    "${VCPKG_DEBUG_BIN}/fmtd.dll"
    "${VCPKG_DEBUG_BIN}/spdlogd.dll"
    "${VCPKG_DEBUG_BIN}/zip.dll"
    "${VCPKG_DEBUG_BIN}/bz2d.dll"
)
# 复制其他dll文件到可执行文件所在目录
foreach(DLL_FILE ${DLL_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_FILE}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>  # 复制到可执行文件生成目录
    )
endforeach()

# 安装可执行文件到 install/x64-Debug/bin 目录
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
