# file 文件服务器模块，管理文件上传下载，游戏资源分发等
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 定义常量
project("file_plugin" VERSION 1.0)
set(MAIN_PROJECT_SOURCE_DIR "../../../")  # 手动定义指向主模块的路径：主模块单独编译，无法使用主模块变量
set(CMAKE_PREFIX_PATH "${MAIN_PROJECT_SOURCE_DIR}/vcpkg_installed/x64-windows")
set(VCPKG_INCLUDE "${MAIN_PROJECT_SOURCE_DIR}/include")
set(VCPKG_LIB "${MAIN_PROJECT_SOURCE_DIR}/lib")
set(VCPKG_BIN "${MAIN_PROJECT_SOURCE_DIR}/bin")
set(VCPKG_DEBUG_LIB "${MAIN_PROJECT_SOURCE_DIR}/debug/lib")
set(VCPKG_DEBUG_BIN "${MAIN_PROJECT_SOURCE_DIR}/debug/bin")
# 设置 vcpkg 工具链文件路径
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)    # 防止环境中定义了此变量
    set(CMAKE_TOOLCHAIN_FILE "D:/opt/tools/cpp/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()


# 包含头文件的目录
include_directories(
    ${VCPKG_INCLUDE}     # 第三方库的头文件所在目录
    # 共享工具类
    ${MAIN_PROJECT_SOURCE_DIR}/src/common
    # 插件模块
    ${MAIN_PROJECT_SOURCE_DIR}/src/plugins/file
    ${MAIN_PROJECT_SOURCE_DIR}/src/plugins/logic
    ${MAIN_PROJECT_SOURCE_DIR}/src/plugins/common
    # proto生成的 grpc服务头文件目录
    ${MAIN_PROJECT_SOURCE_DIR}/protobuf/src
)

# 需要编译的源文件列表
set(SOURCES
    ${MAIN_PROJECT_SOURCE_DIR}/src/plugins/file/file_server.cpp
    ${MAIN_PROJECT_SOURCE_DIR}/src/plugins/file/file_plugin.cpp
    # 共享工具类
    ${MAIN_PROJECT_SOURCE_DIR}/src/common/logger_manager.cpp
    # proto生成的源文件
    ${MAIN_PROJECT_SOURCE_DIR}/protobuf/src/server_file.pb.cc
    ${MAIN_PROJECT_SOURCE_DIR}/protobuf/src/server_file.grpc.pb.cc
)
# 指定编译生成的可执行文件信息：文件名${MODULE_NAME} ，${SOURCES} 项目所有源文件路径
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# 自动寻找第三方库 lib列表
find_package(absl REQUIRED)
find_package(Boost REQUIRED system)
find_package(c-ares REQUIRED)
find_package(fmt REQUIRED)
find_package(gRPC REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Protobuf REQUIRED)
find_package(re2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(ZLIB REQUIRED)
find_package(upb REQUIRED)

# 下面的库无法使用 find_package 寻找，需要手动指定
find_library(ADDRESS_SORTING_LIB address_sorting PATHS "${VCPKG_LIB}")  # address_sorting.lib, 用于 grpc
# 下面文件需要直接在子模块中使用路径引入

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
#    ZLIB::ZLIB
#    upb::upb
    gRPC::grpc++
    spdlog::spdlog
    ${ADDRESS_SORTING_LIB}  # address_sorting 库：用于gRPC的地址排序
)

# 第三方库 dll列表
set(DLL_FILES
    "${VCPKG_DEBUG_BIN}/boost_system-vc143-mt-gd-x64-1_86.dll"
    "${VCPKG_DEBUG_BIN}/libprotobufd.dll"
    "${VCPKG_DEBUG_BIN}/libprotocd.dll"
    "${VCPKG_DEBUG_BIN}/libssl-3-x64.dll"
    "${VCPKG_DEBUG_BIN}/libcrypto-3-x64.dll"
    "${VCPKG_DEBUG_BIN}/abseil_dll.dll"
    "${VCPKG_DEBUG_BIN}/zlibd1.dll"
    "${VCPKG_DEBUG_BIN}/re2.dll"
    "${VCPKG_DEBUG_BIN}/cares.dll"
    "${VCPKG_DEBUG_BIN}/fmtd.dll"
    "${VCPKG_DEBUG_BIN}/spdlogd.dll"
)
# 复制其他dll文件到可执行文件所在目录
foreach(DLL_FILE ${DLL_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_FILE}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>  # 复制到可执行文件生成目录
    )
endforeach()

# 安装可执行文件到 plugin 目录
install(TARGETS ${PROJECT_NAME} DESTINATION plugin)
