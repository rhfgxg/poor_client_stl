@echo off
rem 将生成的dll插件复制到主模块的执行文件目录下的 pligins 文件夹
rem 插件重新生成时，应该执行该脚本，及时更新插件


rem 设置路径常量
set SCRIPT_DIR=%~dp0
set PROJECT_DIR=%SCRIPT_DIR%..\..
set OUT_DEBUG_DIR=\out\build\x64-Debug
set PLUGIN_DIR=%PROJECT_DIR%\src\plugins

rem 定义插件名列表
set PLUGINS=file logic

rem 创建目标文件夹
set TARGET_DIR=%PROJECT_DIR%\%OUT_DEBUG_DIR%\plugins
if not exist "%TARGET_DIR%" (
    mkdir "%TARGET_DIR%"
)


rem 遍历插件名列表并复制对应 dll 插件
for %%p in (%PLUGINS%) do (
    echo 复制 %%p 插件...
    copy "%PLUGIN_DIR%\%%p\%OUT_DEBUG_DIR%\%%p_plugin.dll" "%TARGET_DIR%"
)

echo 所有插件已复制完成
pause