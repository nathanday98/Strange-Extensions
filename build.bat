@echo off

IF NOT EXIST build mkdir build

pushd build

cl -nologo -FC -Z7 ..\code\win32_strangeextensions.cpp /link Wininet.lib

popd