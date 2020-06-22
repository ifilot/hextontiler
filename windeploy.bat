@ECHO OFF

rmdir /S bin

REM *************
REM Release build
REM *************
@ECHO *****************
@ECHO Deploying BRAVAIS
@ECHO *****************

REM Create directories
mkdir bin

REM Copy files Bravais
copy build-hextontiler-Desktop_Qt_5_10_1_MSVC2017_64bit-Release\release\hextontiler.exe bin
copy assets\icons\hextontiler_logo.ico bin\hextontiler.ico
copy ..\..\Libraries\boost-1.70.0-win-x64\lib\boost_regex-vc141-mt-x64-1_70.dll bin
if exist C:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe (
    C:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe bin\hextontiler.exe --release --force
)
if exist D:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe (
    D:\Qt\5.10.1\msvc2017_64\bin\windeployqt.exe bin\hextontiler.exe --release --force
)