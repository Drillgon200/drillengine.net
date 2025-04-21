..\bootloader\nasm -f bin ..\bootloader\bootloader.asm -o ..\bootloader\bootloader
.\OSBuild\x64\Debug\OSBuild.exe .\disk.img .\user_program.dxe ..\bootloader\bootloader ..\kernel\Kernel\x64\Release\Kernel.dll
pause