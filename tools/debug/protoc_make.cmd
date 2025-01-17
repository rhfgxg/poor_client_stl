@echo off
rem 根据 proto文件，生成对应源码


rem 设置路径常量
set SCRIPT_DIR=%~dp0
set PROJECT_DIR=%SCRIPT_DIR%..\..
set PROTO_DIR=%PROJECT_DIR%\protobuf
set PROTOC=%PROJECT_DIR%\vcpkg_installed\x64-windows\tools\protobuf\protoc.exe
set GRPC_PLUGIN=%PROJECT_DIR%\vcpkg_installed\x64-windows\tools\grpc\grpc_cpp_plugin.exe


rem 生成 file 模块：文件服务器模块
set FILE_PROTO_DIR=%PROTO_DIR%\file
%PROTOC% --proto_path=%FILE_PROTO_DIR% --cpp_out=%FILE_PROTO_DIR% %FILE_PROTO_DIR%\server_file.proto
%PROTOC% --proto_path=%FILE_PROTO_DIR% --grpc_out=%FILE_PROTO_DIR% --plugin=protoc-gen-grpc=%GRPC_PLUGIN% %FILE_PROTO_DIR%\server_file.proto

rem 生成 gateway 模块：网关模块
set GATEWAY_PROTO_DIR=%PROTO_DIR%\gateway
%PROTOC% --proto_path=%GATEWAY_PROTO_DIR% --cpp_out=%GATEWAY_PROTO_DIR% %GATEWAY_PROTO_DIR%\server_gateway.proto
%PROTOC% --proto_path=%GATEWAY_PROTO_DIR% --grpc_out=%GATEWAY_PROTO_DIR% --plugin=protoc-gen-grpc=%GRPC_PLUGIN% %GATEWAY_PROTO_DIR%\server_gateway.proto

rem 生成 logic 模块：逻辑服务模块
set LOGIC_PROTO_DIR=%PROTO_DIR%\logic
%PROTOC% --proto_path=%LOGIC_PROTO_DIR% --cpp_out=%LOGIC_PROTO_DIR% %LOGIC_PROTO_DIR%\server_logic.proto
%PROTOC% --proto_path=%LOGIC_PROTO_DIR% --grpc_out=%LOGIC_PROTO_DIR% --plugin=protoc-gen-grpc=%GRPC_PLUGIN% %LOGIC_PROTO_DIR%\server_logic.proto

rem 生成 login 模块：登录模块
set LOGIN_PROTO_DIR=%PROTO_DIR%\login
%PROTOC% --proto_path=%LOGIN_PROTO_DIR% --cpp_out=%LOGIN_PROTO_DIR% %LOGIN_PROTO_DIR%\server_login.proto
%PROTOC% --proto_path=%LOGIN_PROTO_DIR% --grpc_out=%LOGIN_PROTO_DIR% --plugin=protoc-gen-grpc=%GRPC_PLUGIN% %LOGIN_PROTO_DIR%\server_login.proto


echo 脚本执行完毕
pause