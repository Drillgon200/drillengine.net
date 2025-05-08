..\bootloader\nasm -f bin ..\bootloader\bootloader.asm -o ..\bootloader\bootloader
.\OSBuild\x64\Debug\OSBuild.exe .\disk.img .\user_program.dxe ..\bootloader\bootloader ..\Kernel\x64\Release\Kernel.dll
test_virtio.bat