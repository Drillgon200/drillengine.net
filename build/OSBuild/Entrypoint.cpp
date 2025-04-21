#include "Build.h"

int main(int argc, char* argv[]) {
	if (argc < 3) {
		Build::build_disk_image("../disk.img", "../user_program.dxe", "../../bootloader/bootloader", "../../kernel/Kernel/x64/Release/Kernel.dll");
	} else {
		Build::build_disk_image(argv[1], argv[2], argv[3], argv[4]);
	}
	return EXIT_SUCCESS;
}