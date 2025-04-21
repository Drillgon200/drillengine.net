#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <socketapi.h>
#include <ws2tcpip.h>
#include <string>
#include <unordered_map>
#pragma comment(lib, "Ws2_32.lib")

namespace Build {

	typedef signed __int8 i8;
	typedef unsigned __int8 u8;
	typedef signed __int16 i16;
	typedef unsigned __int16 u16;
	typedef signed __int32 i32;
	typedef unsigned __int32 u32;
	typedef signed __int64 i64;
	typedef unsigned __int64 u64;
	typedef u64 uptr;
	typedef u8 byte;
	typedef float f32;
	typedef double f64;
	typedef u32 b32;
	typedef u32 flags32;

#define ALIGN_HIGH(num, alignment) (((num) + (static_cast<decltype(num)>(alignment) - 1)) & ~(static_cast<decltype(num)>(alignment) - 1))
#define OFFSET_OF(type, member) __builtin_offsetof(type, member)

	struct String {
		char* str;
		u64 length;
	};

	bool operator==(const String& a, const String& b) {
		bool result = false;
		if (a.length == b.length) {
			result = true;
			for (uint32_t i = 0; i < a.length; i++) {
				if (a.str[i] != b.str[i]) {
					result = false;
					break;
				}
			}
		}
		return result;
	}

	struct ByteBuf {
		byte* bytes;
		u32 offset;
		u32 capacity;
		b32 failed;

		void wrap(void* buffer, u32 size) {
			bytes = reinterpret_cast<byte*>(buffer);
			offset = 0;
			capacity = size;
			failed = false;
		}

		bool has_data_left(u32 size) {
			return i64(capacity) - i64(offset) >= i64(size);
		}

		u8 read_u8() {
			u8 result;
			if (capacity - offset < sizeof(u8)) {
				result = 0;
			} else {
				result = bytes[offset];
				offset += sizeof(u8);
			}
			return result;
		}

		u16 read_u16() {
			u16 result;
			if (capacity - offset < sizeof(u16)) {
				result = 0;
				failed = true;
			} else {
				result =
					(u32(bytes[offset + 0]) << 0) |
					(u32(bytes[offset + 1]) << 8);
				offset += sizeof(u16);
			}
			return result;
		}

		u32 read_u32() {
			u32 result;
			if (capacity - offset < sizeof(u32)) {
				result = 0;
				failed = true;
			} else {
				result =
					(u32(bytes[offset + 0]) << 0) |
					(u32(bytes[offset + 1]) << 8) |
					(u32(bytes[offset + 2]) << 16) |
					(u32(bytes[offset + 3]) << 24);
				offset += sizeof(u32);
			}
			return result;
		}

		f32 read_f32() {
			f32 result;
			if (capacity - offset < sizeof(f32)) {
				result = 0.0F;
				failed = true;
			} else {
				memcpy(&result, bytes + offset, sizeof(f32));
				offset += sizeof(f32);
			}
			return result;
		}

		template<typename T>
		void read_struct(T* outStruct) {
			if (!has_data_left(sizeof(T))) {
				failed = true;
			} else {
				memcpy(outStruct, bytes + offset, sizeof(T));
				offset += sizeof(T);
			}
		}

		void seek(u32 newPos) {
			if (newPos > capacity) {
				failed = true;
			}
			offset = newPos;
		}
	};

	// Second bit set in PML4
	const u64 USER_PROGRAM_MEMORY_OFFSET = 0x10000000000;

	struct ProgramSection {
		u32 fileOffset;
		u32 dataSize;
		// Memory offset relative to USER_PROGRAM_MEMORY_OFFSET
		// Should be aligned to page boundaries
		u32 memoryOffset;
		u32 virtualSize;
		static const u32 FLAG_WRITE = (1 << 0);
		static const u32 FLAG_EXECUTE = (1 << 1);
		static const u32 FLAG_CACHE_DISABLE = (1 << 2);
		u32 flags;
		u32 reserved;
	};

	struct Program {
		u32 fileSize;
		u32 numSections;
		u32 entrypointAddress;
		u32 reserved;
		// Array is actually numSections long
		ProgramSection sections[1];
		//u8[] programData;
	};

	byte* read_full_file(u32* count, const char* fileName) {
		byte* result = nullptr;
		HANDLE file = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (file != INVALID_HANDLE_VALUE) {
			u32 size = GetFileSize(file, NULL);
			result = reinterpret_cast<byte*>(malloc(size));
			if (!result) {
				std::cout << "File alloc failed\n";
				CloseHandle(file);
			} else {
				DWORD numBytesRead{};
				if (!ReadFile(file, result, size, &numBytesRead, NULL)) {
					free(result);
					result = nullptr;
					DWORD fileReadError = GetLastError();
					std::cout << "Failed to read file, code: " << fileReadError << '\n';
				}
				CloseHandle(file);
				*count = numBytesRead;
			}
		} else {
			std::cout << "Failed to create file, code: " << GetLastError() << '\n';
		}
		return result;
	}

	void write_buffer_to_file(const char* fileName, void* buffer, u32 count) {
		HANDLE file = CreateFileA(fileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
		if (file != INVALID_HANDLE_VALUE) {
			DWORD bytesWritten;
			if (!WriteFile(file, buffer, count, &bytesWritten, NULL)) {
				std::cout << "Failed file write to " << fileName << '\n';
				std::cout << "Error: " << GetLastError() << '\n';
			} else if (bytesWritten != count) {
				std::cout << "Failed to write all data to " << fileName << '\n';
				std::cout << "Error: " << GetLastError() << '\n';
			}
			CloseHandle(file);
		} else {
			std::cout << "Failed to create file, error: " << GetLastError() << '\n';
		}
	}

	// Return -1 on failure
	u32 output_program_offset_from_virtual_address(u32 virtualAddress, Program* program, IMAGE_SECTION_HEADER* sectionHeaders, u32 numSections) {
		u32 result = -1;
		for (u32 i = 0; i < numSections; i++) {
			if (virtualAddress >= sectionHeaders[i].VirtualAddress && virtualAddress < sectionHeaders[i].VirtualAddress + sectionHeaders[i].Misc.VirtualSize) {
				result = program->sections[i].fileOffset + (virtualAddress - sectionHeaders[i].VirtualAddress);
			}
		}
		return result;
	}

void build_disk_image(const char* outputImageName, const char* outputProgramName, const char* kernelName, const char* programDllName) {
	u32 size;
	byte* file = read_full_file(&size, programDllName);
	if (!file) {
		std::cout << "Failed to read program dll\n";
		exit(EXIT_FAILURE);
	}

	ByteBuf programDLL{};
	programDLL.wrap(file, size);

	IMAGE_DOS_HEADER dosHeader;
	programDLL.read_struct(&dosHeader);
	if (dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
		std::cout << "Program DLL magic bytes did not match IMAGE_DOS_SIGNATURE\n";
		exit(EXIT_FAILURE);
	}

	programDLL.seek(dosHeader.e_lfanew);

	IMAGE_NT_HEADERS64 ntHeader;
	programDLL.read_struct<IMAGE_NT_HEADERS64>(&ntHeader);
		
	if (ntHeader.Signature != IMAGE_NT_SIGNATURE) {
		std::cout << "Program DLL signature bytes were not IMAGE_NT_SIGNATURE\n";
		exit(EXIT_FAILURE);
	}
	if (ntHeader.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
		std::cout << "Program DLL optional header magic not IMAGE_NT_OPTIONAL_HDR64_MAGIC\n";
		exit(EXIT_FAILURE);
	}

	if (programDLL.failed) {
		std::cout << "Program DLL parse failed\n";
		exit(EXIT_FAILURE);
	}
		
	if (ntHeader.FileHeader.NumberOfSections > 64) {
		std::cout << "Too many sections (corrupted file likely)\n";
		exit(EXIT_FAILURE);
	}

	u32 outputProgramSize = ALIGN_HIGH(OFFSET_OF(Program, sections[ntHeader.FileHeader.NumberOfSections]), 512);
	Program* outputProgram = reinterpret_cast<Program*>(malloc(outputProgramSize));
	if (!outputProgram) {
		std::cout << "Allocate output program failed\n";
		exit(EXIT_FAILURE);
	}
	outputProgram->fileSize = outputProgramSize;
	outputProgram->numSections = ntHeader.FileHeader.NumberOfSections;
	outputProgram->entrypointAddress = 0;
	outputProgram->reserved = 0;

	IMAGE_SECTION_HEADER* sectionHeaders = reinterpret_cast<IMAGE_SECTION_HEADER*>(alloca(ntHeader.FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)));
	u32 relocationOffset = 0;
	for (u32 i = 0; i < ntHeader.FileHeader.NumberOfSections; i++) {
		programDLL.read_struct(&sectionHeaders[i]);
		ProgramSection& section = outputProgram->sections[i];
		section.fileOffset = outputProgramSize;
		section.dataSize = sectionHeaders[i].SizeOfRawData;
		section.memoryOffset = sectionHeaders[i].VirtualAddress;
		section.virtualSize = sectionHeaders[i].Misc.VirtualSize;
		section.flags = 0;
		section.reserved = 0;
		if (sectionHeaders[i].Characteristics & IMAGE_SCN_MEM_WRITE) {
			section.flags |= ProgramSection::FLAG_WRITE;
		}
		if (sectionHeaders[i].Characteristics & IMAGE_SCN_MEM_EXECUTE) {
			section.flags |= ProgramSection::FLAG_EXECUTE;
		}
		if (sectionHeaders[i].Characteristics & IMAGE_SCN_MEM_NOT_CACHED) {
			section.flags |= ProgramSection::FLAG_CACHE_DISABLE;
		}
		outputProgramSize += ALIGN_HIGH(sectionHeaders[i].SizeOfRawData, 512);
	}
	outputProgram = reinterpret_cast<Program*>(realloc(outputProgram, outputProgramSize));
	if (!outputProgram) {
		std::cout << "Reallocate output program failed\n";
		exit(EXIT_FAILURE);
	}
	outputProgram->fileSize = outputProgramSize;

	u32 relocationsOffset = 0xFFFFFFFF;
	u32 relocationsSize = 0;
	for (u32 i = 0; i < ntHeader.FileHeader.NumberOfSections; i++) {
		if (memcmp(sectionHeaders[i].Name, ".reloc\0\0", IMAGE_SIZEOF_SHORT_NAME) == 0) {
			relocationsOffset = sectionHeaders[i].PointerToRawData;
			relocationsSize = sectionHeaders[i].SizeOfRawData;
		}
		programDLL.seek(sectionHeaders[i].PointerToRawData);
		if (!programDLL.has_data_left(sectionHeaders[i].SizeOfRawData)) {
			programDLL.failed = true;
		}
		if (!programDLL.failed) {
			memcpy(reinterpret_cast<byte*>(outputProgram) + outputProgram->sections[i].fileOffset, programDLL.bytes + programDLL.offset, sectionHeaders[i].SizeOfRawData);
		}
	}

	if (relocationsOffset != 0xFFFFFFFF) {
		// Relocate any pointers
		programDLL.seek(relocationsOffset);
		while (!programDLL.failed && programDLL.offset < relocationsOffset + relocationsSize) {
			u32 virtualAddress = programDLL.read_u32();
			u32 sizeOfBlock = programDLL.read_u32();
			if (sizeOfBlock == 0) {
				break;
			}
			if (sizeOfBlock < 8 || (sizeOfBlock & 1) != 0) {
				std::cout << "Relocation block size was wrong\n";
				exit(EXIT_FAILURE);
			}
			u32 numRelocations = (sizeOfBlock - 2 * sizeof(u32)) / 2;
			while (numRelocations--) {
				u16 baseRelocationBlock = programDLL.read_u16();
				u16 relocationType = baseRelocationBlock >> 12;
				u16 relocationOffset = baseRelocationBlock & 0xFFF;
				if (relocationType == IMAGE_REL_BASED_ABSOLUTE) {
					continue;
				}
				if (relocationType != IMAGE_REL_BASED_DIR64) {
					std::cout << "Unknown relocation type\n";
					exit(EXIT_FAILURE);
				}
				u32 outputProgramFileAddress = output_program_offset_from_virtual_address(virtualAddress + relocationOffset, outputProgram, sectionHeaders, ntHeader.FileHeader.NumberOfSections);
				if (outputProgramFileAddress == u32(-1)) {
					std::cout << "Could not look up file address from virtual address\n";
					exit(EXIT_FAILURE);
				}
				// UB, but MSVC will be ok with it and it's easy
				*reinterpret_cast<u64*>(reinterpret_cast<byte*>(outputProgram) + outputProgramFileAddress) += USER_PROGRAM_MEMORY_OFFSET - ntHeader.OptionalHeader.ImageBase;
			}
		}
	}
	

	// Look through export directory until we find a possible entrypoint
	u32 exportDirectoryOffset = ntHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	exportDirectoryOffset = output_program_offset_from_virtual_address(exportDirectoryOffset, outputProgram, sectionHeaders, ntHeader.FileHeader.NumberOfSections);
	if (exportDirectoryOffset == u32(-1)) {
		std::cout << "Could not look up file address from virtual address\n";
		exit(EXIT_FAILURE);
	}
	u32 exportDirectorySize = ntHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	// More UB, whatever
	IMAGE_EXPORT_DIRECTORY* exportDirectory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(reinterpret_cast<byte*>(outputProgram) + exportDirectoryOffset);
	u32* functionTable = reinterpret_cast<u32*>(reinterpret_cast<byte*>(outputProgram) + output_program_offset_from_virtual_address(exportDirectory->AddressOfFunctions, outputProgram, sectionHeaders, ntHeader.FileHeader.NumberOfSections));
	u32* nameTable = reinterpret_cast<u32*>(reinterpret_cast<byte*>(outputProgram) + output_program_offset_from_virtual_address(exportDirectory->AddressOfNames, outputProgram, sectionHeaders, ntHeader.FileHeader.NumberOfSections));
	u16* nameOrdinalTable = reinterpret_cast<u16*>(reinterpret_cast<byte*>(outputProgram) + output_program_offset_from_virtual_address(exportDirectory->AddressOfNameOrdinals, outputProgram, sectionHeaders, ntHeader.FileHeader.NumberOfSections));
	u32 entrypointAddress = -1;
	for (u32 i = 0; i < exportDirectory->NumberOfFunctions; i++) {
		const char* funcName = reinterpret_cast<char*>(outputProgram) + output_program_offset_from_virtual_address(nameTable[i], outputProgram, sectionHeaders, ntHeader.FileHeader.NumberOfSections);
		if (strstr(funcName, "server_main")) {
			entrypointAddress = functionTable[nameOrdinalTable[i]];
		}
	}
	if (entrypointAddress == u32(-1)) {
		std::cout << "Failed to find entrypoint\n";
		exit(EXIT_FAILURE);
	}

	outputProgram->entrypointAddress = entrypointAddress;

	if (programDLL.failed) {
		std::cout << "Program DLL parse failed.\n";
		exit(EXIT_FAILURE);
	}

	write_buffer_to_file(outputProgramName, outputProgram, outputProgram->fileSize);

	std::cout << "Output program written to " << outputProgramName << '\n';

	u32 kernelSize;
	byte* kernel = read_full_file(&kernelSize, kernelName);
	if (!kernel) {
		std::cout << "Failed to read kernel file\n";
		exit(EXIT_FAILURE);
	}

	
	const u32 userProgramDiskOffset = 512 * 1024;
	u32 outputImageSize = ALIGN_HIGH(userProgramDiskOffset + outputProgram->fileSize, 4096);
	// Should be at least the size of a floppy, otherwise some programs won't accept it as a real image
	const u32 floppySize = 1474560;
	if (outputImageSize < floppySize) {
		outputImageSize = floppySize;
	}
	byte* diskImage = reinterpret_cast<byte*>(malloc(outputImageSize));
	if (!diskImage) {
		std::cout << "Allocate disk image failed\n";
		exit(EXIT_FAILURE);
	}

	if (kernelSize > userProgramDiskOffset) {
		std::cout << "Warning, kernel size greater than specified user program disk offset";
	}
	memcpy(diskImage, kernel, kernelSize);
	memset(diskImage + kernelSize, 0, userProgramDiskOffset - kernelSize);

	memcpy(diskImage + userProgramDiskOffset, outputProgram, outputProgram->fileSize);
	memset(diskImage + userProgramDiskOffset + outputProgram->fileSize, 0, outputImageSize - (userProgramDiskOffset + outputProgram->fileSize));

	write_buffer_to_file(outputImageName, diskImage, outputImageSize);
	std::cout << "Output image written to " << outputImageName << '\n';
}

}