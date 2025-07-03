@echo off
chcp 65001 >nul
rem 用于debug时，同步生成文件文件夹的配置文件。修改配置文件后，需要执行此脚本
rem 配置文件复制到 可执行文件同级的 /config 中

rem 项目文件根目录
set SCRIPT_DIR=%~dp0
set PROJECT_DIR=%SCRIPT_DIR%..\..
rem 配置文件目录
set CONFIG_DIR=%PROJECT_DIR%\config
rem 生成文件目录
set DEBUG_OUT_DIR=%PROJECT_DIR%\out\build\x64-debug

rem 子模块配置文件目录
set OUT_CONFIG_DIR=%DEBUG_OUT_DIR%\src\config

rem 创建文件夹
if not exist %OUT_CONFIG_DIR% mkdir %OUT_CONFIG_DIR%

rem 复制配置文件
copy /y %CONFIG_DIR%\config.ini	%OUT_CONFIG_DIR%

echo 脚本执行完毕
pause