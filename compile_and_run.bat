@echo off
del /q main.exe
cls
cmd.exe /k "compiler.exe main.c -lwsock32&main.exe&rm main.exe"
