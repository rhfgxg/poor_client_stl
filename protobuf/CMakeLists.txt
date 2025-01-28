# 定义 protobuf_generate_cpp 函数
function(protobuf_generate_cpp)
    if(NOT ARGN)
        message(SEND_ERROR "Error: protobuf_generate_cpp() called without any proto files")
        return()
    endif()

    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        list(APPEND PROTO_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
        list(APPEND PROTO_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc"
                   "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h"
            COMMAND ${Protobuf_PROTOC_EXECUTABLE}
            ARGS --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
                 --proto_path "${CMAKE_CURRENT_SOURCE_DIR}"
                 "${ABS_FIL}"
            DEPENDS "${ABS_FIL}"
            COMMENT "Running C++ protocol buffer compiler on ${FIL}"
            VERBATIM )
    endforeach()

    set(${PROTO_SRCS} ${PROTO_SRCS} PARENT_SCOPE)
    set(${PROTO_HDRS} ${PROTO_HDRS} PARENT_SCOPE)
endfunction()

# 定义 grpc_generate_cpp 函数
function(grpc_generate_cpp)
    if(NOT ARGN)
        message(SEND_ERROR "Error: grpc_generate_cpp() called without any proto files")
        return()
    endif()

    foreach(FIL ${ARGN})
        get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
        get_filename_component(FIL_WE ${FIL} NAME_WE)
        list(APPEND GRPC_SRCS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc")
        list(APPEND GRPC_HDRS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h")
        add_custom_command(
            OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.cc"
                   "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.grpc.pb.h"
            COMMAND ${Protobuf_PROTOC_EXECUTABLE}
            ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
                 --plugin=protoc-gen-grpc="${gRPC_CPP_PLUGIN_EXECUTABLE}"
                 --proto_path "${CMAKE_CURRENT_SOURCE_DIR}"
                 "${ABS_FIL}"
            DEPENDS "${ABS_FIL}"
            COMMENT "Running gRPC C++ protocol buffer compiler on ${FIL}"
            VERBATIM )
    endforeach()

    set(${GRPC_SRCS} ${GRPC_SRCS} PARENT_SCOPE)
    set(${GRPC_HDRS} ${GRPC_HDRS} PARENT_SCOPE)
endfunction()

# 定义常量
set(PROTOBUF_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# 查找 Protocol Buffers 和 gRPC
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# 需要编译的 proto 文件列表
set(PROTO_FILES
    ${PROTOBUF_DIR}/common.proto
    ${PROTOBUF_DIR}/server_gateway.proto
    ${PROTOBUF_DIR}/server_logic.proto
    ${PROTOBUF_DIR}/server_login.proto
    ${PROTOBUF_DIR}/server_file.proto
)

# 生成 Protocol Buffers 和 gRPC 源文件
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})
grpc_generate_cpp(GRPC_SRCS GRPC_HDRS ${PROTO_FILES})

# 添加库
add_library(protobuf_files ${PROTO_SRCS} ${GRPC_SRCS})

# 包含自定义头文件的目录
target_include_directories(protobuf_files PUBLIC ${PROTOBUF_DIR} ${CMAKE_BINARY_DIR})

# 设置编译选项
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4828 /utf-8")   # 忽略警告，使用utf-8编码

# 向其他模块提供接口，将一些公共头文件指定为 PUBLIC，其他模块可以访问
target_include_directories(protobuf_files PUBLIC ${PROTOBUF_DIR})

