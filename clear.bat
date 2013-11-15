@echo off

rmdir /S /Q debug
rmdir /S /Q release
rmdir /S /Q obj
rmdir /S /Q GeneratedFiles
rmdir /S /Q Win32

del /A:H *.suo
del /F /Q *.user
