cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 定义常量
project("poor_client_stl" VERSION 1.0)
set(VCPKG_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows")
set(VCPKG_INCLUDE "${VCPKG_DIR}/include")
set(VCPKG_LIB "${VCPKG_DIR}/lib")
set(VCPKG_BIN "${VCPKG_DIR}/bin")
set(VCPKG_DEBUG_LIB "${VCPKG_DIR}/debug/lib")
set(VCPKG_DEBUG_BIN "${VCPKG_DIR}/debug/bin")

# 包含头文件的目录，在子模块中生效
include_directories(
    ${VCPKG_INCLUDE}     # 第三方库的头文件所在目录
    # 共享工具类
    ${CMAKE_SOURCE_DIR}/src/common
    # 各服务器实现类
    ${CMAKE_SOURCE_DIR}/src/file
    ${CMAKE_SOURCE_DIR}/src/gateway
    ${CMAKE_SOURCE_DIR}/src/logic
    ${CMAKE_SOURCE_DIR}/src/login
    # proto生成的 grpc服务头文件目录
    ${CMAKE_SOURCE_DIR}/protobuf/file
    ${CMAKE_SOURCE_DIR}/protobuf/gateway
    ${CMAKE_SOURCE_DIR}/protobuf/logic
    ${CMAKE_SOURCE_DIR}/protobuf/login
)

# 需要编译的源文件列表
set(SOURCES
    src/main.cpp
)

# 指定编译生成的可执行文件信息：文件名${PROJECT_NAME} ，${SOURCES} 项目所有源文件路径
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /wd4251 /wd4828 /utf-8")   # 忽略警告，使用utf-8编码
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /D_ITERATOR_DEBUG_LEVEL=2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /D_ITERATOR_DEBUG_LEVEL=0")

# 第三方库 lib列表
set(THIRD_PARTY_LIBS
)

# 第三方库 dll列表
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DLL_FILES
    )
else()
    set(DLL_FILES
    )
endif()

# 添加每个模块，确保可以将所有变量传入子模块
add_subdirectory(protobuf)
add_subdirectory(./src/file)
add_subdirectory(./src/gateway)
add_subdirectory(./src/logic)
add_subdirectory(./src/login)

# 链接第三方库
target_link_libraries(${PROJECT_NAME} PRIVATE ${THIRD_PARTY_LIBS})

# 安装可执行文件到 install/x64-Debug/bin 目录
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# 遍历 dll列表，将所有 .dll 文件，逐一复制到可执行文件生成目录
foreach(DLL_FILE ${DLL_FILES})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DLL_FILE}"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>  # 复制到可执行文件生成目录
    )
endforeach()