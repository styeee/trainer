@echo off
del /q program.exe
if [%1]==[] goto start
start util\remote.exe
:start
cmd.exe /k "compiler.exe main.c -lwsock32 -luser32 -o program.exe&program.exe"