You can find this live at https://webserver.drillengine.net

Camera integration (view webcam, upload to server, download image later) is at https://webserver.drillengine.net/CINSCamera.html

Project structure:

/bootloader/bootloader.asm contains the x86 assembly OS kernel

/Kernel/ contains the userspace website code

/build/ contains OSBuild, which is the project that turns the userspace DLL and kernel image into a final OS image, and several batch files for compiling and testing with QEMU

/TCPTestSuite/ contains testing code for the OS's TCP implementation

Build instructions for testing:

1. Install Visual Studio

2. Install QEMU, make sure qemu-system-x86_64.exe is in PATH

3. Download the NASM assembler, put nasm.exe in /bootloader

4. Open /build/OSBuild/OSBuild.sln, build in debug mode

5. Open /Kernel/Kernel.sln, build in release mode

6. Run /build/compile_and_test_virtio.bat to compile a disk image and run it with QEMU


The build process looks like this:

bootloader.asm -> bootloader binary (using nasm)

Kernel.sln -> Kernel.dll (using visual studio)

Kernel.dll -> Kernel.dxe (OSBuild.exe parses DLL and writes custom executable format)

bootloader binary + kernel.sln -> disk.img (OSBuild writes the programs into their respective locations on the final disk image)