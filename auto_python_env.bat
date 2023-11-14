@echo off
chcp 65001 > nul

echo 以下是自动检测Python环境路径, 如果是conda用户, 或需要手动设置
where python
echo.
set /p folderPath=请输入Python环境路径: 
echo 你输入的路径是：%folderPath%

REM 检查路径是否存在
if not exist "%folderPath%" (
    echo 该文件夹路径不存在，请重新输入。
    pause
    exit /b 1
)

REM 复制动态库到可执行目录
for %%i in ("%folderPath%\Python3*.dll") do (
    copy "%%i" ".\msvc-build\"
)

REM 自动生成python_thread.pri文件
set "FileName=.\video\python\python_thread.pri"
REM 拼接静态库路径
set "libsPath=%folderPath%\libs"
set "incPath=%folderPath%\include"
set "numpyPath=%folderPath%\Lib\site-packages\numpy\core\include\numpy"
REM 将反斜杠替换为斜杠
set "libsPath=%libsPath:\=/%"
set "incPath=%incPath:\=/%"
set "numpyPath=%numpyPath:\=/%"


echo.>%FileName%
echo.>>%FileName%

echo #filename:     python_thread.pri >> %FileName%
REM 获取当前日期和时间
for /f "delims=" %%a in ('wmic OS Get localdatetime ^| find "."') do set datetime=%%a
REM 将日期时间格式化为 YYYYMMDD-HHMMSS
set formattedDateTime=%datetime:~0,8%-%datetime:~8,6%
echo #time:         %formattedDateTime% >> %FileName%
echo #author:       yunke120 >> %FileName%

echo.>>%FileName%
echo HEADERS += $$PWD/pythonthread.h >> %FileName%
echo SOURCES += $$PWD/pythonthread.cpp >> %FileName%
echo.>>%FileName%
echo INCLUDEPATH += %incPath% >> %FileName%
echo INCLUDEPATH += %numpyPath% >> %FileName%
echo.>>%FileName%
echo LIBS += -L%libsPath% \ >> %FileName%
for %%i in ("%folderPath%\libs\Python3*.lib") do (
    echo            -l%%~ni \ >> %FileName%
)

echo 自动生成文件 python_thread.pri 成功!
echo ---------------------------------------------------------
type %FileName%
echo ---------------------------------------------------------
pause
