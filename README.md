Project structure:
bootloader/bootloader.asm contains the x86 assembly OS kernel
Kernel/ contains the userspace website code
build/ contains OSBuild, which is the project that turns the userspace DLL and kernel image into a final OS image, and several batch files for compiling and testing with QEMU
TCPTestSuite/ contains testing code for the OS's TCP implementation