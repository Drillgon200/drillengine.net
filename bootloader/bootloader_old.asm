
%define CR0_PROTECTION_ENABLE (1 << 0)
%define CR0_MONITOR_COPROCESSOR (1 << 1)
%define CR0_EMULATION (1 << 2)
%define CR0_PAGING (1 << 31)
%define CR4_PHYSICAL_ADDRESS_EXTENSION (1 << 5)
%define CR4_PAGE_GLOBAL (1 << 7)
%define CR4_FXSAVE_FXRSTORE (1 << 9)
%define CR4_UNMASKED_SIMD_FLOATING_POINT_EXCEPTIONS (1 << 10)

%define IA32_EFER 0xC0000080
%define IA32_EFER_LONG_MODE_ENABLE (1 << 8)

%define GDT_START 0x1000
%define GDT_SIZE 40 ; null entry (8) + code segment (8) + data segment (8) + task segment (16)
%define PAGE_TABLE_START 0x2000
%define TSS_START 0x1800
%define TSS_SIZE 104

%define CODE_SEGMENT 8
%define DATA_SEGMENT 16
%define TASK_SEGMENT 24

%define PAGE_PRESENT (1 << 0)
%define PAGE_WRITE (1 << 1)
%define PAGE_USER_SUPERVISOR (1 << 2)
%define PAGE_WRITE_THROUGH (1 << 3)
%define PAGE_CACHE_DISABLE (1 << 4)
%define PAGE_ACCESSED (1 << 5)
%define PAGE_DIRTY (1 << 6)
%define PAGE_ATTRIBUTE_TABLE (1 << 7)
%define PAGE_GLOBAL (1 << 8)

%define DESCRIPTOR_TYPE_OFFSET 8
%define SEGMENT_DESC_TYPE_READ_WRITE (0010b << 8)
%define SEGMENT_DESC_TYPE_EXECUTE_READ (1010b << 8)
%define SEGMENT_DESC_CODE_OR_DATA (1 << 12)
%define SEGMENT_DESC_PRIVILEGE_LEVEL_USER (3 << 13)
%define DESCRIPTOR_LIMIT_LOW_FULL 0xFFFF
%define DESCRIPTOR_LIMIT_HIGH_FULL (1111b << 16)
%define SEGMENT_DESC_64_BIT_CODE_SEGMENT (1 << 21)
%define SEGMENT_DESC_DEFAULT_OPERATION_SIZE_32 (1 << 22)
%define DESCRIPTOR_GRANULARITY (1 << 23)
%define IDT_IST1 (1 << 32)
%define IDT_DESC_PRESENT (1 << 47)
%define IDT_DESC_TYPE_64_BIT_INTERRUPT_GATE (1110b << 40)
%define DESCRIPTOR_64_BIT_TSS (1001b << 8)
%define DESCRIPTOR_PRESENT (1 << 15)

%define SCRATCH_PAGES_START (1024 * 1024)
%define SCRATCH_PAGES_SIZE (4096 * 8)

; So I just figured out that nasm has structs. That would have been nice to know
struc MBRPartition
	.bootIndicator:          resb 1
	.startingHead:           resb 1
	.startingSectorCylinder: resw 1 ; First 6 bits are sector, next 10 are cylinder
	.systemID:               resb 1
	.endingHead:             resb 1
	.endingSectorCylinder:   resw 1
	.startingLBA:            resd 1
	.totalSectors:           resd 1
endstruc

%define MBR_PARTITION1_OFFSET 0x01BE
%define MBR_PARTITION2_OFFSET 0x01CE
%define MBR_PARTITION3_OFFSET 0x01DE
%define MBR_PARTITION4_OFFSET 0x01EE
%define MBR_BOOT_INDICATOR_ACTIVE 0x80
%define MBR_BOOT_INDICATOR_NOT_ACTIVE 0

%define PIC0_CMD 0x20
%define PIC0_DATA 0x21
%define PIC1_CMD 0xA0
%define PIC1_DATA 0xA1
%define PIT_CHANNEL_0 0x40
%define PIT_CHANNEL_1 0x41
%define PIT_CHANNEL_2 0x42
%define PIT_COMMAND 0x43
%define PIT_CHANNEL_SELECT_0 0b00000000
%define PIT_ACCESS_MODE_LO_HI_BYTE 0b110000
%define PIT_OPERATING_MODE_RATE_GENERATOR 0b0100
%define PIT_MODE_BINARY 0b0
%define PIT_MODE_BCD 0b1
; Set if ICW4 should be received
%define PIC_ICW1_IC4 (1 << 0)
; Only 1 PIC if set. There are two PICs in x86, so leave clear
%define PIC_ICW1_SNGL (1 << 1)
; Address interval, ignore
%define PIC_ICW1_ADT (1 << 2)
; Level triggered mode
%define PIC_ICW1_LTIM (1 << 3)
%define PIC_INITIALIZATION (1 << 4)
; x86 uses IRQ 2 for the PIC cascade
%define PIC_SLAVE_IR_NUMBER 2
; 1 if 80x86 mode, 0 if MCS-80/86 mode
%define PIC_ICW4_MICROPROCESSOR_MODE (1 << 0)
; Perform EOI automatically on interrput acknowledge pulse
%define PIC_ICW4_AEOI (1 << 1)
; Relevant if BUF is set, selects master buffer if 1, else slave
%define PIC_ICW4_BUF_MS (1 << 2)
; Controller operates in buffered mode
%define PIC_ICW4_BUF (1 << 3)
; Special Fully Nested Mode, for systems with a lot of cascaded controllers
%define PIC_ICW4_SFNM (1 << 4)
; Bits 4 and 3 are 01 to signify OCW3, bit 1 enables bit 0, bit 0 is 1 to read the in service register
%define PIC_OCW3_READ_ISR (0b00001011)
%define PIC_OCW2_EOI (1 << 5)

%define APIC_REGISTER_PAGE (SCRATCH_PAGES_START + SCRATCH_PAGES_SiZE)
%define APIC_ID_OFFSET 0x20
%define APIC_VERSION_OFFSET 0x30
%define APIC_EOI_OFFSET 0xB0
%define APIC_SPURIOUS_VECTOR_OFFSET 0xF0
%define APIC_SPURIOUS_VECTOR_SOFTWARE_ENABLE (1 << 8)
%define APIC_SPURIOUS_VECTOR_FOCUS_PROCESSOR_CHECKING (1 << 9)
%define APIC_SPURIOUS_VECTOR_EOI_BROADCAST_SUPPRESSION (1 << 12)
%define IA32_APIC_BASE 0x1B

%define REMAPPED_IRQ0 0x20
%define APIC_SPURIOUS_INTERRUPT_VECTOR (REMAPPED_IRQ0 + 16)
%define AHCI_INTERRUPT_VECTOR (APIC_SPURIOUS_INTERRUPT_VECTOR + 1)

%define PS2_DATA_PORT 0x60

%define TSS_RSP0 4
%define TSS_RSP1 12
%define TSS_RSP2 20
%define TSS_IST1 36
%define TSS_IST2 44
%define TSS_IST3 52
%define TSS_IST4 60
%define TSS_IST5 68
%define TSS_IST6 76
%define TSS_IST7 84
%define TSS_IO_MAP_BASE 102

%define VGA_ADDRESS 0x3D4
%define VGA_DATA 0x3D5
%define VGA_CURSOR_LOW_BYTE 0x0F
%define VGA_CURSOR_HIGH_BYTE 0x0E
%define TEXT_VIDEO_MEMORY 0xb8000

; struct DISKCMD {
;	u32 port; // Port to run the command on
;	u32 id; // Command id, returned as part of the read/write function, id increments for each command issued
;	u64 memAddressAndRW; // Memory virtual address plus whether the command is read or write.
;	u64 diskAddress; // The page offset into the disk
;	u32 numSectors; // number of sectors to transfer
;	u32 queued; // Number of bytes from the start that have already been queued. In range 0-size
;	u64 usrData; // Arbitrary user data for the user that issued the command
; }
%define DISKCMD_PORT_OFFSET 0
%define DISKCMD_ID_OFFSET 4
%define DISKCMD_MEM_ADDR_RW_OFFSET 8
%define DISKCMD_MEM_ADDR_RW_WRITE_BIT 1
%define DISKCMD_MEM_ADDR_RW_READ_BIT 0
%define DISKCMD_DISK_ADDR_OFFSET 16
%define DISKCMD_SECTORS_OFFSET 24
%define DISKCMD_QUEUED_OFFSET 28
%define DISKCMD_USER_DATA_OFFSET 32
%define DISKCMD_SIZE 40
; struct PORTCMD {
;	u32 cmdIdx;
;	u32 sectorCount;
; }
%define PORTCMD_CMD_IDX_OFFSET 0
%define PORTCMD_SECTOR_COUNT_OFFSET 4
%define PORTCMD_SIZE 8
%define DISK_PORT_CMD_QUEUE_SIZE (32 * 32 * PORTCMD_SIZE) ; 32 possible ports, 32 possible command slots per port, 8 bytes per PORTCMD
%define DISK_PORTS_ACTIVE_SIZE (32 * 4) ; 32 possible ports, one 32 bit integer per port
%define DISK_PORT_QUEUE_MAX_COMMANDS 32
; struct PORTQUEUE {
;	DISKCMD cmdQueue[DISK_PORT_QUEUE_MAX_COMMANDS]; // Read/write commands that haven't completed yet
;	PORTCMD activeCommands[32]; // Port operations that haven't finished their transfer yet
;	u32 queuedDiskCmdsMask; // One bit for each present DISKCMD
;	u32 activeCommandsMask; // One bit for each operation currently running on a port
;	u64 diskSize; // Size of disk in bytes
; }
%define DISK_PORT_QUEUE_CMD_QUEUE_OFFSET 0
%define DISK_PORT_QUEUE_ACTIVE_CMDS_OFFSET (DISK_PORT_QUEUE_CMD_QUEUE_OFFSET + DISK_PORT_QUEUE_MAX_COMMANDS * DISKCMD_SIZE)
%define DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET (DISK_PORT_QUEUE_ACTIVE_CMDS_OFFSET + 32 * PORTCMD_SIZE)
%define DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET (DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET + 4)
%define DISK_PORT_QUEUE_DISK_SIZE_OFFSET (DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET + 4)
%define DISK_PORT_QUEUE_SIZE (DISK_PORT_QUEUE_DISK_SIZE_OFFSET + 8) ; Works out to 1552 currently. Subject to change, don't trust this comment.

%define AHCI_HBA_CAPABILITIES 0x00
%define AHCI_HBA_CAPABILITIES_STAGGERED_SPINUP (1 << 27)
%define AHCI_HBA_CAPABILITIES_NUM_COMMAND_SLOTS_OFFSET 8
%define AHCI_HBA_CAPABILITIES_NUM_COMMAND_SLOTS_MASK 0b11111
%define AHCI_HBA_CAPABILITIES_NUM_PORTS_OFFSET 0
%define AHCI_HBA_CAPABILITIES_NUM_PORTS_MASK 0b11111
%define AHCI_GLOBAL_HBA_CONTROL 0x04
%define AHCI_GLOBAL_HBA_CONTROL_AHCI_ENABLE (1 << 31)
%define AHCI_GLOBAL_HBA_CONTROL_INTERRUPT_ENABLE (1 << 1)
%define AHCI_GLOBAL_HBA_CONTROL_HBA_RESET (1 << 0)
%define AHCI_INTERRUPT_STATUS 0x08
%define AHCI_PORTS_IMPLEMENTED 0x0C
%define AHCI_VERSION 0x10
%define AHCI_CAPABILITIES_EXTENDED 0x24
%define AHCI_CAPABILITIES_EXTENDED_BIOS_OS_HANDOFF (1 << 0)
%define AHCI_BIOS_OS_HANDOFF 0x28
%define AHCI_BIOS_OS_HANDOFF_BIOS_BUSY (1 << 4)
%define AHCI_BIOS_OS_HANDOFF_OS_OWNED_SEMAPHORE (1 << 1)
%define AHCI_BIOS_OS_HANDOFF_BIOS_OWNED_SEMAPHORE (1 << 0)
%define AHCI_PORT0_OFFSET 100h
%define AHCI_PORT_SIZE 80h
%define AHCI_PORT_COMMAND_LIST_BASE 0x00
%define AHCI_PORT_COMMAND_LIST_BASE_UPPER 0x04
%define AHCI_PORT_FIS_BASE 0x08
%define AHCI_PORT_FIS_BASE_UPPER 0x0C
%define AHCI_PORT_INTERRUPT_STATUS 0x10
%define AHCI_PORT_INTERRUPT_STATUS_ALL_CLEAR_BITS 0b11111101100000000000000010101111 ; Every bit in the interrupt status register that's read/write clear
%define AHCI_PORT_INTERRUPT_ENABLE 0x14
%define AHCI_PORT_INTERRUPT_ENABLE_DEVICE_TO_HOST_REGISTER_FIS_ENABLE (1 << 0)
%define AHCI_PORT_INTERRUPT_ENABLE_PIO_SETUP_FIS_ENABLE (1 << 1)
%define AHCI_PORT_INTERRUPT_ENABLE_DMA_SETUP_FIS_ENABLE (1 << 2)
%define AHCI_PORT_INTERRUPT_ENABLE_SET_DEVICE_BITS_FIS_ENABLE (1 << 3)
%define AHCI_PORT_INTERRUPT_ENABLE_DESCRIPTOR_PROCESSED_ENABLE (1 << 5)
%define AHCI_PORT_INTERRUPT_ENABLE_PORT_CHANGE_ENABLE (1 << 6)
%define AHCI_PORT_INTERRUPT_ENABLE_ALL 0b01111101110000000000000001111111
%define AHCI_PORT_COMMAND 0x18
%define AHCI_PORT_COMMAND_COMMAND_LIST_RUNNING (1 << 15)
%define AHCI_PORT_COMMAND_FIS_RECEIVE_RUNNING (1 << 14)
%define AHCI_PORT_COMMAND_FIS_RECEIVE_ENABLED (1 << 4)
%define AHCI_PORT_COMMAND_START (1 << 0)
%define AHCI_PORT_TASK_FILE_DATA 0x20
%define AHCI_PORT_TASK_FILE_DATA_STS_BUSY (1 << 7)
%define AHCI_PORT_TASK_FILE_DATA_STS_DATA_TRANSFER_REQUESTED (1 << 3)
%define AHCI_PORT_TASK_FILE_DATA_STS_DATA_ERROR (1 << 0)
%define AHCI_PORT_SIGNATURE 0x24
%define AHCI_PORT_SATA_STATUS 0x28
%define AHCI_PORT_SATA_STATUS_DEVICE_DETECTION_MASK 0b1111
%define AHCI_PORT_SATA_CONTROL 0x2C
%define AHCI_PORT_SATA_CONTROL_DET_NO_ACTION 0
%define AHCI_PORT_SATA_CONTROL_DET_INITIALIZE 1
%define AHCI_PORT_SATA_CONTROL_DET_DISABLE 4
%define AHCI_PORT_SATA_CONTROL_DET_MASK 0b1111
%define AHCI_PORT_SATA_ERROR 0x30
%define AHCI_PORT_SATA_ACTIVE 0x34
%define AHCI_PORT_COMMAND_ISSUE 0x38
%define AHCI_PORT_SATA_NOTIFICATION 0x3C
%define AHCI_PORT_FIS_BASED_SWITCHING_CONTROL 0x40
%define AHCI_PORT_DEVICE_SLEEP 0x44
%define AHCI_PHYSICAL_REGION_DESCRIPTOR_DATA_BASE_ADDRESS_OFFSET 0
%define AHCI_PHYSICAL_REGION_DESCRIPTOR_RESERVED_OFFSET 8
%define AHCI_PHYSICAL_REGION_DESCRIPTOR_DATA_BYTE_COUNT_OFFSET 12

%define FIS_TYPE_REG_HOST_TO_DEVICE 0x27
%define FIS_TYPE_REG_DEVICE_TO_HOST 0x34
%define FIS_TYPE_DMA_ACTIVATE 0x39
%define FIS_TYPE_DMA_SETUP 0x41
%define FIS_TYPE_DATA 0x46
%define FIS_TYPE_BIST 0x58
%define FIS_TYPE_PIO_SETUP 0x5F
%define FIS_TYPE_DEVICE_BITS 0xA1

%define ATA_CMD_READ_DMA_EXT 0x25
%define ATA_CMD_WRITE_DMA_EXT 0x35
%define ATA_CMD_IDENTIFY_DEVICE 0xEC


%define VIRTIO_DEVICE_TYPE_RESERVED 0
%define VIRTIO_DEVICE_TYPE_NETWORK_CARD 1
%define VIRTIO_DEVICE_TYPE_BLOCK_DEVICE 2
%define VIRTIO_DEVICE_TYPE_CONSOLE 3
%define VIRTIO_DEVICE_TYPE_ENTROPY_SOURCE 4
%define VIRTIO_DEVICE_TYPE_MEMORY_BALOONING_TRADITIONAL 5
%define VIRTIO_DEVICE_TYPE_IO_MEMORY 6
%define VIRTIO_DEVICE_TYPE_RPMSG 7
%define VIRTIO_DEVICE_TYPE_SCSI_HOST 8
%define VIRTIO_DEVICE_TYPE_9P_TRANSPORT 9
%define VIRTIO_DEVICE_TYPE_MAC_80211_WLAN 10
%define VIRTIO_DEVICE_TYPE_RPROC_SERIAL 11
%define VIRTIO_DEVICE_TYPE_VIRTIO_CAIF 12
%define VIRTIO_DEVICE_TYPE_MEMORY_BALOON 13
%define VIRTIO_DEVICE_TYPE_GPU_DEVICE 16
%define VIRTIO_DEVICE_TYPE_TIMER_CLOCK_DEVICE 17
%define VIRTIO_DEVICE_TYPE_INPUT_DEVICE 18
%define VIRTIO_TRANSITIONAL_DEVICE_NETWORK_CARD 0x1000
%define VIRTIO_TRANSITIONAL_DEVICE_BLOCK_DEVICE 0x1001
%define VIRTIO_TRANSITIONAL_DEVICE_MEMORY_BALOONING_TRADITIONAL 0x1002
%define VIRTIO_TRANSITIONAL_DEVICE_CONSOLE 0x1003
%define VIRTIO_TRANSITIONAL_DEVICE_SCSI_HOST 0x1004
%define VIRTIO_TRANSITIONAL_DEVICE_ENTROPY_SOURCE 0x1005
%define VIRTIO_TRANSITIONAL_DEVICE_9P_TRANSPORT 0x1009

%define VIRTIO_DESC_COUNT_MAX 128 ; Largest power of two where my virtq struct can still fit in a 4k page
; The following structs are from the VirtIO 1.0 spec (more or less)
; struct virtq_desc {
;	le64 addr; // guest physical
;	le32 len; // Length of the part of the buffer this descriptor points at
;	le16 flags; // flags includes next (buffer continues via next field), write (buffer is write only, otherwise read only), indirect (buffer has a list of buffer descriptors)
;	le16 next; // link to next desc in the list to chain descriptors
; }
%define VIRTQ_DESC_ADDR_OFFSET 0
%define VIRTQ_DESC_LEN_OFFSET 8
%define VIRTQ_DESC_FLAGS_OFFSET 12
%define VIRTQ_DESC_FLAGS_NEXT (1 << 0)
%define VIRTQ_DESC_FLAGS_WRITE (1 << 1)
%define VIRTQ_DESC_FLAGS_INDIRECT (1 << 2)
%define VIRTQ_DESC_NEXT_OFFSET 14
%define VIRTQ_DESC_SIZE 16
; struct virtq_avail {
;	le16 flags; // flags includes no_interrupt (hint to device to not interrupt when buffer added. The device does not have to follow this, it's just an optimization)
;	le16 idx; // index of the next available buffer
;	le16 ring[VIRTIO_DESC_COUNT_MAX]; // Ring buffer of available buffers. May be treated as smaller than VIRTIO_DESC_COUNT_MAX if the device does not support that much
; }
%define VIRTQ_AVAIL_FLAGS_OFFSET 0
%define VIRTQ_AVAIL_FLAGS_NO_INTERRUPT (1 << 0)
%define VIRTQ_AVAIL_IDX_OFFSET 2
%define VIRTQ_AVAIL_RING_OFFSET 4
%define VIRTQ_AVAIL_SIZE (VIRTQ_AVAIL_RING_OFFSET + VIRTIO_DESC_COUNT_MAX * 2)
; struct virtq_used_elem {
;	le32 id; // Start of used descriptor chain
;	le32 len; // Length of the descriptor chain that was written to
; }
%define VIRTQ_USED_ELEM_ID_OFFSET 0
%define VIRTQ_USED_ELEM_LEN_OFFSET 4
%define VIRTQ_USED_ELEM_SIZE 8
; struct virtq_used {
;	le16 flags; // flags includes no_notify (hint to device to not interrupt when buffer consumed. The device does not have to follow this, it's just an optimization)
;	le16 idx;
;	virtq_used_elem ring[VIRTIO_DESC_COUNT_MAX];
; }
%define VIRTQ_USED_FLAGS_OFFSET 0
%define VIRTQ_USED_FLAGS_NO_NOTIFY (1 << 0)
%define VIRTQ_USED_IDX_OFFSET 2
%define VIRTQ_USED_RING_OFFSET 4
%define VIRTQ_USED_SIZE (VIRTQ_USED_RING_OFFSET + VIRTIO_DESC_COUNT_MAX * VIRTQ_USED_ELEM_SIZE)
; struct virtq {
;	virtq_desc desc[VIRTIO_DESC_COUNT_MAX];
;	virtq_avail avail;
;	virtq_used used;
;	u32 num;
;	u32 descCount; // The real desc count negotiated with the device
; }
%define VIRTQ_DESC_OFFSET 0
%define VIRTQ_AVAIL_OFFSET (VIRTQ_DESC_OFFSET + VIRTIO_DESC_COUNT_MAX * VIRTQ_DESC_SIZE)
%define VIRTQ_USED_OFFSET (VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_SIZE)
%define VIRTQ_NUM_OFFSET (VIRTQ_USED_OFFSET + VIRTQ_USED_SIZE)
%define VIRTQ_DESC_COUNT_OFFSET (VIRTQ_NUM_OFFSET + 4)
%define VIRTQ_SIZE (VIRTQ_DESC_COUNT_OFFSET + 4)

%define VIRTIO_PCI_CAP_COMMON_CFG 1
%define VIRTIO_PCI_CAP_NOTIFY_CFG 2
%define VIRTIO_PCI_CAP_ISR_CFG 3
%define VIRTIO_PCI_CAP_DEVICE_CFG 4
%define VIRTIO_PCI_CAP_PCI_CFG 5
; struct virtio_pci_cap {
;	u8 cap_vndr; // Generic PCI field, PCI_CAP_ID_VNDR (0x09)
;	u8 cap_next; // Generic PCI field, next capability offset
;	u8 cap_len; // Generic PCI field, capability length
;	u8 cfg_type; // Identifies the structure
;	u8 bar; // Where to find it
;	u8 padding[3]; // Allign to dword
;	le32 offset; // Offset within bar
;	le32 length; // Length of the structure, in bytes
; }
%define VIRTIO_PCI_CAP_CAP_VNDR_OFFSET 0
%define VIRTIO_PCI_CAP_CAP_NEXT_OFFSET 1
%define VIRTIO_PCI_CAP_CAP_LEN_OFFSET 2
%define VIRTIO_PCI_CAP_CAP_TYPE_OFFSET 3
%define VIRTIO_PCI_CAP_BAR_OFFSET 4
%define VIRTIO_PCI_CAP_OFFSET_OFFSET 8
%define VIRTIO_PCI_CAP_LENGTH_OFFSET 12
%define VIRTIO_PCI_CAP_SIZE 16
; struct virtio_pci_common_cfg {
;	le32 device_feature_select;
;	le32 device_features;
;	le32 driver_feature_select;
;	le32 driver_feature;
;	le16 msix_config;
;	le16 num_queues;
;	u8 device_status;
;	u8 config_generation;
;
;	le16 queue_select;
;	le16 queue_size;
;	le16 queue_msix_vector;
;	le16 queue_enable;
;	le16 queue_notify_off;
;	le64 queue_desc;
;	le64 queue_avail;
;	le64 queue_used;
; }
%define VIRTIO_PCI_COMMON_DEVICE_FEATURE_SELECT_OFFSET 0
%define VIRTIO_PCI_COMMON_DEVICE_FEATURES_OFFSET 4
%define VIRTIO_PCI_COMMON_DRIVER_FEATURE_SELECT_OFFSET 8
%define VIRTIO_PCI_COMMON_DRIVER_FEATURES_OFFSET 12
%define VIRTIO_PCI_COMMON_MSIX_CONFIG_OFFSET 16
%define VIRTIO_PCI_COMMON_NUM_QUEUES_OFFSET 18
%define VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET 20
%define VIRTIO_PCI_COMMON_CONFIG_GENERATION_OFFSET 21
%define VIRTIO_PCI_COMMON_QUEUE_SELECT_OFFSET 22
%define VIRTIO_PCI_COMMON_QUEUE_SIZE_OFFSET 24
%define VIRTIO_PCI_COMMON_QUEUE_MSIX_VECTOR_OFFSET 26
%define VIRTIO_PCI_COMMON_QUEUE_ENABLE_OFFSET 28
%define VIRTIO_PCI_COMMON_QUEUE_NOTIFY_OFF_OFFSET 30
%define VIRTIO_PCI_COMMON_QUEUE_DESC_OFFSET 32
%define VIRTIO_PCI_COMMON_QUEUE_AVAIL_OFFSET 40
%define VIRTIO_PCI_COMMON_QUEUE_USED_OFFSET 48
%define VIRTIO_PCI_COMMON_SIZE 56
; struct virtio_notify_cap {
;	virtio_pci_cap cap;
;	le32 notify_off_multiplier;
; }
%define VIRTIO_NOTIFY_CAP_CAP_OFFSET 0
%define VIRTIO_NOTIFY_CAP_NOTIFY_OFF_MULTIPLIER_OFFSET VIRTIO_PCI_CAP_SIZE
%define VIRTIO_NOTIFY_CAP_SIZE (VIRTIO_NOTIFY_CAP_NOTIFY_OFF_MULTIPLIER_OFFSET + 4)


%define PCI_CONFIG_ADDRESS 0xCF8
%define PCI_CONFIG_DATA 0xCFC
%define PCI_CONTROL_INTERRUPT_DISABLE (1 << 10)
%define PCI_CONTROL_FAST_BACK_TO_BACK_ENABLE (1 << 9)
%define PCI_CONTROL_SERR_ENABLE (1 << 8)
%define PCI_CONTROL_PARITY_ERROR_RESPONSE (1 << 6)
%define PCI_CONTROL_VGA_PALLETE_SNOOP (1 << 5)
%define PCI_CONTROL_MEMORY_WRITE_INVALIDATE_ENABLE (1 << 4)
%define PCI_CONTROL_SPECIAL_CYCLES (1 << 3)
%define PCI_CONTROL_BUS_MASTER (1 << 2)
%define PCI_CONTROL_MEMORY_SPACE (1 << 1)
%define PCI_CONTROL_IO_SPACE (1 << 0)



; Global variables
%define VARIABLES_START 0xC800
%define DEBUG_TEXT_OFFSET_LOCATION (VARIABLES_START + 0)
%define PCI_AHCI_CONTROLLER_ADDRESS (VARIABLES_START + 8)
%define PCI_AHCI_CONTROLLER_ABAR_ADDRESS (VARIABLES_START + 16)
%define PCI_AHCI_CONTROLLER_ABAR_SIZE (VARIABLES_START + 24)
%define PIT_CLOCK (VARIABLES_START + 32)
%define PIT_CLOCK_MILLISECONDS (VARIABLES_START + 36) ; Clock is two 64 bit integers, with the low 32 bits being fractions of a millisecond. Loading from the middle of the two should give you milliseconds from the low 64 bit int plus the low milliseconds from the second 64 bit int in the high bits, perfect
%define PIT_CLOCK_HIGH (VARIABLES_START + 40)
%define AHCI_DEVICES_PRESENT (VARIABLES_START + 48)
%define AHCI_NUM_COMMAND_SLOTS (VARIABLES_START + 52)
%define DISK_CMD_CURRENT_ID (VARIABLES_START + 56)
%define VIRTIO_NET_TX_QUEUE_NOTIFY_ADDR (VARIABLES_START + 60)
%define VIRTIO_NET_RX_QUEUE_NOTIFY_ADDR (VARIABLES_START + 64)
%define VIRTIO_NET_QUEUE_SIZE (VARIABLES_START + 68)
%define VIRTIO_NET_PCI_ADDRESS (VARIABLES_START + 72)

; Global memory offsets
%define AHCI_PORT_RECEIVED_FIS_OFFSET (1024 * 1024 * 2 - 13 * 4096) ; 256 bytes per received fis, 32 ports, 256 * 32 == 8192, needs 2 pages, 11 pages already used for ABAR mapping and command lists
%define AHCI_PORT_RECEIVED_FIS_SIZE 0x100
%define AHCI_PORT_COMMAND_LIST_OFFSET (1024 * 1024 * 2 - 11 * 4096) ; 1024 bytes per command list with 32 commands, 32 ports, 1024 * 32 == 32768, needs 8 pages, 3 pages already used for ABAR mapping
%define AHCI_PORT_COMMAND_LIST_SIZE 32 * 32 ; 32 bytes per command header, 32 command headers, 32 * 32 == 1024
%define AHCI_MAPPED_OFFSET (1024 * 1024 * 2 - 3 * 4096)
%define AHCI_NUM_COMMAND_TABLE_ENTRIES 8 ; Must be a multiple of 8, since command lists are aligned to 128
%define AHCI_COMMAND_TABLE_CMD0_OFFSET 0x80
%define AHCI_COMMAND_TABLE_CMD_SIZE (4 * 4) ; 4 DWORDS
%define AHCI_COMMAND_TABLE_SIZE (AHCI_COMMAND_TABLE_CMD0_OFFSET + AHCI_NUM_COMMAND_TABLE_ENTRIES * AHCI_COMMAND_TABLE_CMD_SIZE)
%define AHCI_COMMAND_TABLES_REQUIRED_PAGES ((32 * 32 * AHCI_COMMAND_TABLE_SIZE + 4095) / 4096) ; 32 ports, 32 tables per port
%define AHCI_COMMAND_TABLES_OFFSET (AHCI_PORT_RECEIVED_FIS_OFFSET - AHCI_COMMAND_TABLES_REQUIRED_PAGES * 4096)
%define DISK_PORT_QUEUES_REQUIRED_PAGES ((DISK_PORT_QUEUE_SIZE * 32 + 4095) / 4096)
%define DISK_PORT_QUEUES_OFFSET (AHCI_COMMAND_TABLES_OFFSET - DISK_PORT_QUEUES_REQUIRED_PAGES * 4096)
%define VIRTIO_NET_QUEUE_NOTIFY_PAGES (DISK_PORT_QUEUES_OFFSET - 2 * 4096) ; Yes that's right, two whole pages for 4 bytes of data, because apparently that's the worst case.
%define VIRTIO_NET_RX_QUEUE_OFFSET (VIRTIO_NET_QUEUE_NOTIFY_PAGES - 4096)
%define VIRTIO_NET_TX_QUEUE_OFFSET (VIRTIO_NET_RX_OFFSET - 4096)

%define BOOT_STACK 0x800
%define INTERRUPT_STACK 0x400

org 0x7C00
bits 16
	jmp main

; Arguments: si for string
biosPrint:
	push ax
	push bx
printLoop:
	; Load character, test if end of string
	mov al, [si]
	cmp al, 00h
	je endPrintLoop

	mov ah, 0Eh ; Video teletype output
	mov bx, 0x0007 ; Page 0, color grey on black
	int 10h ; Video interrupt

	; Increment string pointer, keep going
	inc si
	jmp printLoop
endPrintLoop:
	pop bx
	pop ax
	ret

main:
	cli ; Clear interrupts
	cld ; Clear direction flag

	; Load the long mode part of the bootloader.
	mov ah, 42h ; Extended read
	; dl contains our drive number when the BIOS calls our code
	mov si, longModeLoaderRead ; read packet
	int 13h

	mov sp, BOOT_STACK; Set stack pointer, 1kb of stack should do

	; Enable A20 with the BIOS so addresses don't wrap at 1 megabyte
	; May not work on all machines, will have to test this
	mov ax, 2401h ; Enable A20
	int 15h ; Invoke system bios service

	mov ah, 00h ; Video set mode
	mov al, 03h ; Mode 
	int 10h ; Video interrupt

	mov ah, 02h ; Video set cursor position
	mov bh, 00h ; Page number
	mov dh, 0 ; Row
	mov dl, 0 ; Column
	int 10h ; Video interrupt

	mov eax, 0x80000001
	cpuid
	and edx, (1 << 29)
	cmp edx, 0
	jne hasLongMode
	mov si, errNoLongMode
	call biosPrint
noLongMode:
	hlt
	jmp noLongMode

hasLongMode:

	; Zero GDT memory
	mov di, GDT_START
	mov ecx, 0x800/4
	xor eax, eax
	rep stosd

	; Code segment
	mov dword [GDT_START + 8], DESCRIPTOR_LIMIT_LOW_FULL
	mov dword [GDT_START + 12], SEGMENT_DESC_TYPE_EXECUTE_READ | SEGMENT_DESC_CODE_OR_DATA | DESCRIPTOR_PRESENT | DESCRIPTOR_LIMIT_HIGH_FULL | SEGMENT_DESC_64_BIT_CODE_SEGMENT | DESCRIPTOR_GRANULARITY
	; Data segment
	mov dword [GDT_START + 16], DESCRIPTOR_LIMIT_LOW_FULL
	mov dword [GDT_START + 20], SEGMENT_DESC_TYPE_READ_WRITE | SEGMENT_DESC_CODE_OR_DATA | DESCRIPTOR_PRESENT | DESCRIPTOR_LIMIT_HIGH_FULL | SEGMENT_DESC_DEFAULT_OPERATION_SIZE_32 | DESCRIPTOR_GRANULARITY
	; Task segment (16 bytes)
	mov dword [GDT_START + 24], TSS_SIZE | (TSS_START << 16)
	mov dword [GDT_START + 28], DESCRIPTOR_64_BIT_TSS | DESCRIPTOR_PRESENT
	mov dword [GDT_START + 32], 0
	mov dword [GDT_START + 36], 0

	; Identity mapping for first few pages before we go to long mode
	; Zero out virtual memory buffers. 4kb per buffer, 4 level addressing
	mov di, PAGE_TABLE_START
	mov ecx, 1000h
	xor eax, eax
	rep stosd

	mov dword [PAGE_TABLE_START], (PAGE_TABLE_START + 1000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [PAGE_TABLE_START + 1000h], (PAGE_TABLE_START + 2000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [PAGE_TABLE_START + 2000h], (PAGE_TABLE_START + 3000h) | PAGE_PRESENT | PAGE_WRITE
	; Map first two megabytes, completely filling last table
	mov di, PAGE_TABLE_START + 3000h
	mov bx, PAGE_TABLE_START + 3000h + 1000h
	mov eax, PAGE_PRESENT | PAGE_WRITE
identityMap:
	mov dword [di], eax
	add di, 8
	add eax, 1000h
	cmp di, bx
	jne identityMap

	; Map APIC registers
	mov dword [PAGE_TABLE_START + 3000h + APIC_REGISTER_PAGE/512], 0xFEE00000 | PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE

	; Disable interrupts
	mov al, 0xFF
	out PIC1_DATA, al
	out PIC0_DATA, al

	; Zero length idt for NMIs. We'll set one up in a bit
	mov dword [esp - 4], 0
	mov dword [esp - 8], 0
	lidt [esp - 8]

	; Go to long mode

	mov eax, cr4
	or eax, CR4_PHYSICAL_ADDRESS_EXTENSION | CR4_PAGE_GLOBAL
	mov cr4, eax

	mov eax, PAGE_TABLE_START
	mov cr3, eax

	mov ecx, IA32_EFER
	rdmsr
	or eax, IA32_EFER_LONG_MODE_ENABLE
	wrmsr

	mov eax, cr0
	or eax, CR0_PROTECTION_ENABLE | CR0_PAGING
	mov cr0, eax

	mov word [esp - 10], GDT_SIZE - 1
	mov dword [esp - 8], GDT_START
	mov dword [esp - 4], 0
	lgdt [esp - 10]

bits 64
	push CODE_SEGMENT
	push longMode
	retfq
longMode:
	mov ax, DATA_SEGMENT
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	jmp longModeLoader

longModeLoaderRead:
db 16 ; size of packet in bytes
db 0 ; reserved, zero
dw (endBootloader - $$ + 512 - 1) / 512 - 1 ; number of sectors to transfer (max 127)
dd 7E00h ; memory dst, right after where the BIOS loaded us
dd 1 ; lower 32 bits of 48 bit LBA address. This is in sectors, so 2 would start reading at byte 1024
dd 0 ; upper 16 bits of 48 bit LBA address

times MBR_PARTITION1_OFFSET - ($-$$) db 0
; Not fully sure if this is correct, we'll see
istruc MBRPartition
	at .bootIndicator, db MBR_BOOT_INDICATOR_ACTIVE
	at .startingHead, db 0
	at .startingSectorCylinder, dw 2
	at .systemID, db 0xDA ; Non-FS Data, we're not using a file system
	at .endingHead, db 0
	at .endingSectorCylinder, dw 3
	at .startingLBA, dd 1
	at .totalSectors, dd 1
iend

; The other 3 MBR physical partitions go here, don't put anything else

times 510 - ($-$$) db 0
dw 0xAA55
bootSectorEnd:

; In case long mode isn't supported, show a message
errNoLongMode db "Not a 64 bit computer, failed to boot!", 0dh, 0ah, 00h

longModeLoader:
	mov dword [DEBUG_TEXT_OFFSET_LOCATION], 80

	; Set up task state segment for interrupt stack switching
	; Zero TSS memory
	mov di, TSS_START
	mov ecx, TSS_SIZE/4
	xor eax, eax
	rep stosd

	mov [TSS_START + TSS_RSP0], rsp
	mov [TSS_START + TSS_RSP1], rsp
	mov [TSS_START + TSS_RSP2], rsp
	mov qword [TSS_START + TSS_IST1], INTERRUPT_STACK
	mov qword [TSS_START + TSS_IST2], INTERRUPT_STACK
	mov qword [TSS_START + TSS_IST3], INTERRUPT_STACK
	mov qword [TSS_START + TSS_IST4], INTERRUPT_STACK
	mov qword [TSS_START + TSS_IST5], INTERRUPT_STACK
	mov qword [TSS_START + TSS_IST6], INTERRUPT_STACK
	mov qword [TSS_START + TSS_IST7], INTERRUPT_STACK
	; Since TSS_SIZE is greater than or equal to the segment limit (also TSS_SIZE), all IO operations will general protection fault in user mode
	mov word [TSS_START + TSS_IO_MAP_BASE], TSS_SIZE
	; Load task register
	mov ax, TASK_SEGMENT
	ltr ax


	; Setup interrupts
%define IDT_START 0x6000
%define IDT_SIZE (256 * 16)
	
	lea eax, [IDT_START]
	lea ebx, [IDT_START + IDT_SIZE]
	mov rdx, (CODE_SEGMENT << 16) | IDT_IST1 | IDT_DESC_TYPE_64_BIT_INTERRUPT_GATE | IDT_DESC_PRESENT
	lea rcx, [interruptGeneric]
.fillIDT:
	or rcx, rdx
	mov qword [rax], rcx
	add eax, 16
	cmp eax, ebx
	jne .fillIDT

	lea eax, [IDT_START + REMAPPED_IRQ0 * 16]
	lea ebx, [IDT_START + (REMAPPED_IRQ0 + 16) * 16]
	lea rcx, [interruptPIC0Generic]
	or rcx, rdx
.fillIRQs:
	; Fill the rest with PIC1 interrupt handler after the PIC0 interrupts are filled
	cmp eax, IDT_START + (REMAPPED_IRQ0 + 8) * 16
	jne .keepFillingIRQs
	lea rcx, [interruptPIC1Generic]
	or rcx, rdx
.keepFillingIRQs:
	mov qword [rax], rcx
	add eax, 16
	cmp eax, ebx
	jne .fillIRQs

	; Add PIT handler
	lea rcx, [interruptPIC0Timer]
	or rcx, rdx
	mov qword [IDT_START + (REMAPPED_IRQ0) * 16], rcx
	; Add keyboard handler
	lea rcx, [interruptPIC0Keyboard]
	or rcx, rdx
	mov qword [IDT_START + (REMAPPED_IRQ0 + 1) * 16], rcx
	; Add APIC spurious handler
	lea rcx, [interruptAPICSpurious]
	or rcx, rdx
	mov qword [IDT_START + APIC_SPURIOUS_INTERRUPT_VECTOR * 16], rcx
	; Add AHCI handler
	lea rcx, [interruptAHCI]
	or rcx, rdx
	mov qword [IDT_START + AHCI_INTERRUPT_VECTOR * 16], rcx


	; Setup PIT
	mov qword [PIT_CLOCK], 0
	mov qword [PIT_CLOCK_HIGH], 0
	mov al, PIT_CHANNEL_SELECT_0 | PIT_ACCESS_MODE_LO_HI_BYTE | PIT_OPERATING_MODE_RATE_GENERATOR | PIT_MODE_BINARY
	out PIT_COMMAND, al
	mov ax, 10000 ; Divider. 10k is only about 10 ms of precision, but qemu can't seem to keep up with a 1k divider
	out PIT_CHANNEL_0, al
	mov al, ah
	out PIT_CHANNEL_0, al

	mov word [rsp - 10], IDT_SIZE
	mov qword [rsp - 8], IDT_START
	lidt [rsp - 10]

	sti ; Enable interrupts

	; Set up the PICs
	; ICW1, initialize PICs
	mov al, PIC_ICW1_IC4 | PIC_INITIALIZATION
	out PIC0_CMD, al
	out PIC1_CMD, al
	; ICW2, remap PIC to 0x20
	mov al, REMAPPED_IRQ0
	out PIC0_DATA, al
	mov al, REMAPPED_IRQ0 + 8
	out PIC1_DATA, al
	; ICW3, master gets the bit the slave is on, slave gets the number the master used directly
	mov al, (1 << PIC_SLAVE_IR_NUMBER)
	out PIC0_DATA, al
	mov al, PIC_SLAVE_IR_NUMBER
	out PIC1_DATA, al
	; ICW4, enable 80x86 mode
	mov al, PIC_ICW4_MICROPROCESSOR_MODE
	out PIC0_DATA, al
	out PIC1_DATA, al
	; Done with init, enable PIC interrupts
	mov al, 0
	out PIC0_DATA, al
	out PIC1_DATA, al

	; Enable APIC (probably done by BIOS, but just in case)
	mov dword [APIC_REGISTER_PAGE + APIC_SPURIOUS_VECTOR_OFFSET], APIC_SPURIOUS_INTERRUPT_VECTOR | APIC_SPURIOUS_VECTOR_SOFTWARE_ENABLE

	; Enable SSE
	mov rax, cr0
	and rax, ~CR0_EMULATION
	or rax, CR0_MONITOR_COPROCESSOR
	mov cr0, rax
	mov rax, cr4
	or rax, CR4_FXSAVE_FXRSTORE | CR4_UNMASKED_SIMD_FLOATING_POINT_EXCEPTIONS
	mov cr4, rax

	mov dword [PCI_AHCI_CONTROLLER_ADDRESS], 0
	mov dword [VIRTIO_NET_PCI_ADDRESS], 0
	call scanPCIBus
	;lea eax, [achiControllerAddress]
	;call debugPrint
	mov eax, [PCI_AHCI_CONTROLLER_ADDRESS]
	test eax, eax
	jnz .hasAHCIController
	lea eax, [noAHCIControllerFound]
	call debugPrint
	jmp .noAHCIController
.hasAHCIController:
	;call debugPrintInteger
	;call debugPrintNewline
	call initializeAHCI
.noAHCIController:

	mov eax, [VIRTIO_NET_PCI_ADDRESS]
	test eax, eax
	jz .noVirtIONet
	call initializeVirtIONet
.noVirtIONet:


	; print("Startup done, took " + timeMillis + " milliseconds.")
	lea rax, [startupDoneTookStr]
	call debugPrint
	mov rax, [PIT_CLOCK_MILLISECONDS]
	call debugPrintInteger
	lea rax, [millisecondsStr]
	call debugPrint
	
	call testAHCIReadWrite


.halt:
	hlt
	jmp .halt


; Does virtual memory mapping manually (I can't believe there isn't a built in instruction for this)
; rax contains virtual address
; rbx gets physical page or'd with page present and user flags
tlbAddressLookup:
	push rcx
	push rdx

	mov ebx, PAGE_PRESENT | PAGE_USER_SUPERVISOR
	mov rdx, rax
	shr rdx, 39
	and rdx, 0b111111111 ; 9 bit lookup
	mov rcx, [PAGE_TABLE_START + rdx * 8] ; Get Page Directory Pointer Table from Page Map Level 4
	test ecx, PAGE_PRESENT
	jz .pageNotPresent
	and ebx, ecx
	and rcx, ~0b111111111111 ; Clear low 12 bits
	mov rdx, rax
	shr rdx, 30
	and rdx, 0b111111111 ; 9 bit lookup
	mov rcx, [rcx + rdx * 8] ; Get Page Directory from Page Directory Pointer Table
	test rcx, PAGE_PRESENT
	jz .pageNotPresent
	and rcx, ~0b111111111111 ; Clear low bits
	mov rdx, rax
	shr rdx, 21
	and rdx, 0b111111111 ; 9 bit lookup
	mov rcx, [rcx + rdx * 8] ; Get value Page Table from Page Directory
	test rcx, PAGE_PRESENT
	jz .pageNotPresent
	and rcx, ~0b111111111111 ; Clear low bits
	mov rdx, rax
	shr rdx, 12
	and rdx, 0b111111111 ; 9 bit lookup
	mov rcx, [rcx + rdx * 8] ; Get value Physical Address from Page Table
	test rcx, PAGE_PRESENT
	jz .pageNotPresent
	and ebx, ecx
	and rcx, ~0b111111111111 ; Clear low bits
	or rbx, rcx
	jmp .lookupDone
.pageNotPresent:
	xor ebx, ebx
.lookupDone:

	pop rdx
	pop rcx
	ret


interruptPIC7:
	push rax
	; Check for spurious interrupt on first PIC
	mov al, PIC_OCW3_READ_ISR
	out PIC0_CMD, al ; Next read will be ISR
	in al, PIC0_CMD ; Get ISR
	test al, al
	jz interrputPIC7Ret

interruptPIC7Handle:
	mov al, PIC_OCW2_EOI
	out PIC0_CMD, al
interrputPIC7Ret:
	pop rax
	iret

interruptPIC15:
	push rax
	; Check for spurious interrupt on second PIC
	mov al, PIC_OCW3_READ_ISR
	out PIC1_CMD, al ; Next read will be ISR
	in al, PIC1_CMD ; Get ISR
	test al, al
	jz interrputPIC15Ret
interrputPIC15Handle:
	mov al, PIC_OCW2_EOI
	out PIC1_CMD, al
interrputPIC15Ret:
	; First PIC must always be reset in this case
	mov al, PIC_OCW2_EOI
	out PIC0_CMD, al
	pop rax
	iret

interruptPIC0Generic:
	push rax
	mov al, PIC_OCW2_EOI
	out PIC0_CMD, al
	pop rax
	iretq

interruptPIC0Timer:
	push rax
	mov rax, 35995910900 ; (2^32) * (1000/(1193182/10000)), where 1193182 is the PIT clock rate in hz and the 10000 is the divider. Should cause an increment of the high dword every millisecond
	add qword [PIT_CLOCK], rax
	adc qword [PIT_CLOCK_HIGH], 0 ; Add carries to another qword to make sure we don't overflow

	mov al, PIC_OCW2_EOI
	out PIC0_CMD, al
	pop rax
	iretq

interruptPIC0Keyboard:
	push rax
	push rbx
	xor eax, eax
	in al, PS2_DATA_PORT
	test al, 0b10000000
	jnz keyReleased
	mov bl, al ; Save scan code
	mov byte al, [keyscancodemap + eax]
	test al, al
	jz keyNotASCII
	call debugPrintChar
keyNotASCII:
keyReleased:

	mov al, PIC_OCW2_EOI
	out PIC0_CMD, al
	pop rbx
	pop rax
	iretq

interruptPIC1Generic:
	push rax
	mov al, PIC_OCW2_EOI
	out PIC1_CMD, al
	out PIC0_CMD, al
	pop rax
	iretq

interruptAPICSpurious:
	; I don't think I have to do anything here?
	iretq

interruptAHCI:
	push rax
	push rbx
	push rcx
	push rdx

	;lea rax, [ahciInterrupt]
	;call debugPrint

	mov eax, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	mov ebx, [eax + AHCI_INTERRUPT_STATUS]
	mov [eax + AHCI_INTERRUPT_STATUS], ebx ; Write back interrupt status to clear interrupts
	test ebx, ebx
	jz .interruptHandlingComplete
.interruptHandlePort:
	tzcnt ecx, ebx ; portIdx
	imul ecx, ecx, AHCI_PORT_SIZE
	mov edx, [eax + AHCI_PORT0_OFFSET + ecx + AHCI_PORT_INTERRUPT_STATUS]
	mov [eax + AHCI_PORT0_OFFSET + ecx + AHCI_PORT_INTERRUPT_STATUS], edx
	test edx, edx
	jz .portNoInterruptFound
	; Any active command with a bit set where command issue isn't set has completed
	mov edx, [eax + AHCI_PORT0_OFFSET + ecx + AHCI_PORT_COMMAND_ISSUE] ; cmdsToCheck
	tzcnt ecx, ebx ; portIdx
	imul ecx, ecx, DISK_PORT_QUEUE_SIZE
	mov eax, [DISK_PORT_QUEUES_OFFSET + ecx + DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET]
	and [DISK_PORT_QUEUES_OFFSET + ecx + DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET], edx
	mov ecx, eax
	mov eax, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	; 'xor' sees which bits have flipped, 'and' ensures that we get only the ones where active commands is on and command issue is now off
	xor edx, ecx ; cmdsToCheck
	and edx, ecx ; cmdsToCheck
	test edx, edx
	jz .portNoInterruptFound
.commandsCompletingLoop:
	tzcnt ecx, edx ; portCmdIdx = tzcnt(cmdsToCheck)
	push rax
	push rbx
	tzcnt ebx, ebx ; portIdx
	imul ebx, ebx, DISK_PORT_QUEUE_SIZE
	add ebx, DISK_PORT_QUEUES_OFFSET ; queue
	lea eax, [ebx + DISK_PORT_QUEUE_ACTIVE_CMDS_OFFSET + ecx * PORTCMD_SIZE] ; portCmd
	imul ecx, ecx, DISKCMD_SIZE
	lea ecx, [ebx + ecx] ; diskCmd
	; ebx = queue
	; eax = portCmd
	; ecx = diskCmd
	push rdx
	mov edx, [eax + PORTCMD_SECTOR_COUNT_OFFSET] ; portCmd.sectorCount
	sub [ecx + DISKCMD_QUEUED_OFFSET], edx ; diskCmd.queued -= portCmd.sectorCount
	sub [ecx + DISKCMD_SECTORS_OFFSET], edx ; diskCmd.queued -= portCmd.sectorCount
	shl edx, 9 ; sectorCount * 512
	add [ecx + DISKCMD_MEM_ADDR_RW_OFFSET], edx ; diskCmd.memAddressAndRW += sectorCount * 512
	; If sectors left to transfer is zero, finish the command
	mov edx, [ecx + DISKCMD_SECTORS_OFFSET]
	test edx, edx
	jnz .commandNotDone
	; Finish command here, write boolean for now
	mov rdx, [ecx + DISKCMD_USER_DATA_OFFSET]
	mov byte [rdx], 1
	mov edx, [eax + PORTCMD_CMD_IDX_OFFSET]
	btr [ebx + DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET], edx
.commandNotDone:
	pop rdx

	pop rbx
	pop rax
	tzcnt ecx, edx ; portCmdIdx = tzcnt(cmdsToCheck)
	btr edx, ecx
	test edx, edx
	jnz .commandsCompletingLoop
	call ahciProcessQueuedCommandsForPort
.portNoInterruptFound:
	tzcnt ecx, ebx ; portIdx
	btr ebx, ecx
	test ebx, ebx
	jnz .interruptHandlePort
.interruptHandlingComplete:

	pop rdx
	pop rcx
	pop rbx
	pop rax
	mov dword [APIC_REGISTER_PAGE + APIC_EOI_OFFSET], 0
	iretq

interruptVirtIONetwork:
	iretq

interruptVirtIOBlock:
	iretq

interruptGeneric:
	; No handling, immediately triple fault
	xor eax, eax
	mov cr3, rax
	iretq

keyscancodemap db 0,0,"1234567890-=",0,09h,"QWERTYUIOP[]",0x0a,0,"ASDFGHJKL;'` \ZXCVBNM,./",0,0,0," ",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0





debugUpdateCursor:
	mov dx, VGA_ADDRESS
	mov al, VGA_CURSOR_LOW_BYTE
	out dx, al
	mov dx, VGA_DATA
	mov al, bl
	out dx, al

	mov dx, VGA_ADDRESS
	mov al, VGA_CURSOR_HIGH_BYTE
	out dx, al
	mov dx, VGA_DATA
	mov al, bh
	out dx, al

	ret

; ax contains character and color
debugPrintChar:
	mov byte [rsp - 2], al
	mov byte [rsp - 1], 0 ; null terminator
	mov [rsp - 16], rax
	lea rax, [rsp - 2]
	sub rsp, 16
	call debugPrint
	add rsp, 16
	mov rax, [rsp - 16]
	ret

; Function to easily print newlines
debugPrintNewline:
	push rax
	mov al, 0x0a ; newline character
	call debugPrintChar
	pop rax
	ret

; rax has pointer to null terminated string
debugPrint:
	push rax
	push rbx
	push rcx
	push rdx

	mov rdx, rax
	mov dword ebx, [DEBUG_TEXT_OFFSET_LOCATION]
	mov ah, 0x7 ; Color grey
	xor ecx, ecx

debugStringPrintLoop:
	mov al, [rdx]
	test al, al
	jz debugEndString

	cmp al, 0x0d ; carriage return
	jne notCarriageReturn
	add rdx, 1
	jmp debugStringPrintLoop

notCarriageReturn:
	cmp al, 0x0a ; newline
	jne notNewline

debugNewlinePrintLoop:
	mov al, 0x20 ; space

	mov word [TEXT_VIDEO_MEMORY + ebx * 2], ax
	add ebx, 1
	cmp ebx, 80 * 25
	cmove ebx, ecx

	push rdx
	mov dx, 0
	mov ax, bx
	mov cx, 80
	div cx
	mov ah, 0x7 ; Color grey
	xor ecx, ecx
	test dx, dx
	pop rdx
	jnz debugNewlinePrintLoop

	add rdx, 1
	jmp debugStringPrintLoop

notNewline:
	mov word [TEXT_VIDEO_MEMORY + ebx * 2], ax
	add ebx, 1
	cmp ebx, 80 * 25
	cmove ebx, ecx

	add rdx, 1
	jmp debugStringPrintLoop
debugEndString:

	call debugUpdateCursor
	mov [DEBUG_TEXT_OFFSET_LOCATION], ebx

	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; rax contains integer
debugPrintInteger:
	push rax
	push rbx
	push rcx
	push rdx
	sub rsp, 32 ; 32 is enough space for the max 64 bit integer in base 10

	mov byte [rsp + 31], 0 ; null terminator
	mov ebx, 31

integerToASCIILoop:
	xor edx, edx
	test rax, rax
	jz integerToASCIIZero
	
	mov ecx, 10
	div rcx

integerToASCIIZero:
	add dl, 0x30 ; add ascii 0
	sub ebx, 1
	mov [rsp + rbx], dl

	test rax, rax
	jnz integerToASCIILoop

	lea rax, [rsp + rbx]
	call debugPrint

	add rsp, 32
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; bl contains bus number
; bh contains device number
; cl contains function number
; ch contains offset in bytes
; eax gets result
getPCIAddressDword:
	push rdx

	movzx eax, bl
	shl eax, 5
	movzx edx, bh
	or eax, edx
	shl eax, 3
	movzx edx, cl
	or eax, edx
	shl eax, 8
	movzx edx, ch
	or eax, edx
	or eax, 0x80000000 ; Set enable bit

	pop rdx
	ret

; eax contains address
; eax gets result
readPCIConfigDword:
	push rdx

	mov dx, PCI_CONFIG_ADDRESS
	out dx, eax
	mov dx, PCI_CONFIG_DATA
	in eax, dx

	pop rdx
	ret

; eax contains pci config address
; rax gets result
readPCIBar:
	push rbx
	push rax ; Save the config address
	call readPCIConfigDword
	test eax, 1
	jnz .ioSpaceBar
	; Memory space bar
	mov ebx, eax
	and ebx, 0b110
	cmp ebx, 0b100 ; If type is two, the bar is 64 bit
	jne .thirtyTwoBitBar
	; 64 bit bar, read the next register and put it in the upper bits
	mov ebx, eax
	mov eax, [rsp] ; Get the bar address back
	add eax, 4 ; Address of the next bar field
	call readPCIConfigDword
	shl rax, 32 ; Next bar register is the upper 32 bits of the 64 bit address
	or rax, rbx
.thirtyTwoBitBar:
	and rax, 0xFFFFFFF0 ; Take off low four bits, memory space bars are aligned to 16
	jmp .end
.ioSpaceBar
	and eax, 0xFFFFFFFC ; Take off low two bits, io space bars are aligned to 4
.end
	add rsp, 8
	pop rbx
	ret

; eax contains address
; edx contains data to write
writePCIConfigDword:
	push rax
	push rdx

	push rdx
	mov dx, PCI_CONFIG_ADDRESS
	out dx, eax
	pop rax
	mov dx, PCI_CONFIG_DATA
	out dx, eax

	pop rdx
	pop rax
	ret

scanPCIBus:
	push rax
	push rbx
	push rcx
	push rdx

	mov bl, 0 ; Bus number
	mov bh, 0 ; Device number
	mov cl, 0 ; Function number
	mov ch, 0 ; Offset
.busPCILoop:
.devicePCILoop:

	mov ch, 0 ; Offset
	call getPCIAddressDword
	call readPCIConfigDword
	cmp ax, 0xFFFF
	je .deviceNotPresentPCI

	lea eax, [headerType]
	call debugPrint
	mov ch, 12 ; Offset
	call getPCIAddressDword
	call readPCIConfigDword
	shr eax, 16
	and eax, 0xFF
	call debugPrintInteger
	call debugPrintNewline

	mov cl, 0 ; Function number
.functionTypePCILoop:
	push rax

	mov ch, 0 ; Offset
	call getPCIAddressDword
	call readPCIConfigDword
	cmp ax, 0xFFFF
	je .foundNoFunctionPCI
	lea rax, [foundStr]
	call debugPrint
	mov ch, 8 ; Offset
	call getPCIAddressDword
	call readPCIConfigDword
	mov edx, eax

	; Check if we've found an AHCI controller
	shr eax, 24
	cmp eax, 1
	jne .notAHCIController
	mov eax, edx
	shr eax, 16
	and eax, 0xFF
	cmp eax, 6
	jne .notAHCIController
	mov eax, edx
	shr eax, 8
	and eax, 0xFF
	cmp eax, 1
	jne .notAHCIController

	; Skip if we've already found an AHCI controller. It might be good to support multiple in the future, but for now, we'll just use the first one
	mov eax, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	test eax, eax
	jnz .notAHCIController

	mov ch, 0 ; Offset
	call getPCIAddressDword
	mov [PCI_AHCI_CONTROLLER_ADDRESS], eax
	lea eax, [ahciControllerFound]
	call debugPrint
	mov ch, 0 ; Offset
	call getPCIAddressDword
	call debugPrintInteger
	call debugPrintNewline
	call getPCIAddressDword
	call readPCIConfigDword
	shr eax, 16
	call debugPrintInteger
	call debugPrintNewline
	mov ch, 0x24 ; Offset ABAR
	call getPCIAddressDword
	call readPCIBar
	mov [PCI_AHCI_CONTROLLER_ABAR_ADDRESS], eax
	call debugPrintInteger
	call debugPrintNewline
	; Get size. Write all ones, then read it back. For bits that are part of it, zeros will be returned, so we can not it to get ones there.
	mov edx, 0xFFFFFFFF
	call getPCIAddressDword
	call writePCIConfigDword
	call getPCIAddressDword
	call readPCIConfigDword
	; Lower 4 bits are prefetcable flag (don't care), valid memory space mapping (32 or 64 bit, also don't care), and the memory space indicator zero bit
	and eax, 0xFFFFFFF0
	not eax
	; Add 1 to get the size (rather than valid bits for address space)
	add eax, 1
	mov [PCI_AHCI_CONTROLLER_ABAR_SIZE], eax
	call debugPrintInteger
	call debugPrintNewline
	mov edx, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	call getPCIAddressDword
	call writePCIConfigDword
	jmp .foundNoFunctionPCI
.notAHCIController:
	
	; Check if we've found a VirtIO network device
	mov ch, 0 ; Offset, device id | vendor id
	call getPCIAddressDword
	call readPCIConfigDword
	cmp ax, 0x1AF4 ; All VirtIO devices have vendor id 1AF4
	jnz .notVirtIONet
	shr eax, 16
	cmp ax, VIRTIO_TRANSITIONAL_DEVICE_NETWORK_CARD
	jz .isVirtIONet
	cmp ax, 0x1040 + VIRTIO_DEVICE_TYPE_NETWORK_CARD
	jz .isVirtIONet
	jmp .notVirtIONet
.isVirtIONet:
	lea eax, [virtIONetFound]
	call debugPrint
	call getPCIAddressDword
	mov [VIRTIO_NET_PCI_ADDRESS], eax
	jmp .foundNoFunctionPCI
.notVirtIONet:
.foundNoFunctionPCI:
	pop rax
	test al, 0x80
	jz .functionTypePCILoopEnd
	add cl, 1
	cmp cl, 8
	jne .functionTypePCILoop
.functionTypePCILoopEnd:

	call debugPrintNewline

.deviceNotPresentPCI:
	add bh, 1
	cmp bh, 32
	jnz .devicePCILoop

	add bl, 1
	test bl, bl
	jnz .busPCILoop

.halt:
	hlt
	jmp .halt

	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

foundStr db "Found",0x0a,0x0
classCode db "ClassCode: ",0x0
subclass db "Subclass: ",0x0
progIF db "ProgIF: ",0x0
headerType db "HeaderType: ",0x0
functionNum db "FunctionNum: ",0x0
ahciControllerFound db "AHCI controller found!",0x0a,0x0
achiControllerAddress db "AHCI controller address: ",0x0
noAHCIControllerFound db "No AHCI Controller Found!",0x0a,0x0
ahciInterrupt db "AHCI Interrupt!",0x0a,0x0
startupDoneTookStr db "Startup done, took ",0x0
millisecondsStr db " milliseconds.",0x0a,0x0
msiPending db "MSI pending",0x0a,0x0
virtIONetFound db "VirtIO net device found!",0x0a,0x0

initializeAHCI:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	mov rbp, rsp
	sub rsp, 8

	mov ecx, [PCI_AHCI_CONTROLLER_ADDRESS] ; PCI config address for the AHCI controller

	; Enable message signaled interrupts
	lea eax, [ecx + 0x34] ; Offset for capabilities pointer
	call readPCIConfigDword
	and eax, 0b11111100 ; 8 bit value, must be dword aligned, so take off the last two bits
	test eax, eax
	jz .msiEnableDone
.msiCapabilitySearch:
	add eax, ecx
	mov edi, eax
	call readPCIConfigDword ; Read Message Control | Next Pointer | Capability Id. See PCI spec, section 6.8.1
	mov edx, eax
	and edx, 0xFF ; Get type
	cmp edx, 0x05 ; 5 is the MSI id
	jne .msiCapabilityNotMSI
	and eax, ~(0b111 << 20) ; Set message control to one message enabled
	or eax, (1 << 16) ; Enable MSI
	test eax, (1 << 23) ; Check if 64 bit capable
	setnz bl
	mov edx, eax
	mov eax, edi
	call writePCIConfigDword
	lea eax, [edi + 4]
	mov edx, 0xFEE00000 ; See intel software developer manual vol 3, section 11.11.1 Message Address Register Format
	call writePCIConfigDword
	test bl, bl
	jz .msiNot64Bit
	add edi, 4
	lea eax, [edi + 4]
	xor edx, edx ; Zero out upper bits
	call writePCIConfigDword
.msiNot64Bit:
	lea eax, [edi + 8]
	mov edx, AHCI_INTERRUPT_VECTOR ; See manual again, we only care about the vector in this case
	call writePCIConfigDword
	jmp .msiEnableDone
.msiCapabilityNotMSI:
	shr eax, 8 ; Get next pointer
	and eax, 0b11111100 ; 8 bit value, must be dword aligned, so take off the last two bits
	test eax, eax ; Zero signifies the end of the list
	jnz .msiCapabilitySearch
.msiEnableDone:

	; Enable PCI interrupts, memory control, and bus mastering
	lea eax, [ecx + 0x04] ; Offset for Status | Command
	call readPCIConfigDword
	or eax, PCI_CONTROL_MEMORY_SPACE | PCI_CONTROL_BUS_MASTER
	and eax, ~PCI_CONTROL_INTERRUPT_DISABLE
	mov edx, eax
	lea eax, [ecx + 0x04] ; Offset for Status | Command
	call writePCIConfigDword

	; Map ABAR memory
	mov eax, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	mov edx, eax
	; Align to page, since I'm pretty sure ABAR is only guaranteed to be aligned to 16, not a full page
	and eax, ~(1000h - 1)
	or eax, PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + AHCI_MAPPED_OFFSET / 512], eax
	add eax, 1000h
	mov [PAGE_TABLE_START + 3000h + (AHCI_MAPPED_OFFSET + 1000h) / 512], eax
	add eax, 1000h
	mov [PAGE_TABLE_START + 3000h + (AHCI_MAPPED_OFFSET + 2000h) / 512], eax
	; Get the low offset
	and edx, 1000h - 1
	; Mapped virtual address of PCI ABAR
	lea eax, [edx + AHCI_MAPPED_OFFSET]
	invlpg [eax]
	invlpg [eax + 1000h]
	invlpg [eax + 2000h]
	mov [PCI_AHCI_CONTROLLER_ABAR_ADDRESS], eax

	; BIOS to OS handoff
	mov edx, [eax + AHCI_CAPABILITIES_EXTENDED]
	test dx, AHCI_CAPABILITIES_EXTENDED_BIOS_OS_HANDOFF
	jz .noHandoffSupported
	mov edx, [eax + AHCI_BIOS_OS_HANDOFF]
	or edx, AHCI_BIOS_OS_HANDOFF_OS_OWNED_SEMAPHORE
	mov [eax + AHCI_BIOS_OS_HANDOFF], edx
.biosBusy:
	mov edx, [eax + AHCI_BIOS_OS_HANDOFF]
	test dx, AHCI_BIOS_OS_HANDOFF_BIOS_OWNED_SEMAPHORE
	jnz .biosBusy
.noHandoffSupported:




	; OSDev wiki and a driver I read resets the HBA here, but the spec doesn't say I have to
	; I'm not sure why you would if you don't have to, I'll have to look more into that
	; I'll just follow the instructions in the spec for now

	; Step 1: Enable AHCI
	mov edx, [eax + AHCI_GLOBAL_HBA_CONTROL]
	or edx, AHCI_GLOBAL_HBA_CONTROL_AHCI_ENABLE
	mov [eax + AHCI_GLOBAL_HBA_CONTROL], edx

	; Step 2: Find ports implemented
	mov ebx, [eax + AHCI_PORTS_IMPLEMENTED]
	mov [rbp - 8], ebx ; Save ports implemented

	; Step 3: Ensure each port is idle, set to idle if not
	push rax
	lea ecx, [eax + AHCI_PORT0_OFFSET]
.portIdleLoop:
	test ebx, 1
	; Skip if not implemented
	jz .portAlreadyIdle
	mov eax, [ecx + AHCI_PORT_COMMAND]
	test eax, AHCI_PORT_COMMAND_COMMAND_LIST_RUNNING | AHCI_PORT_COMMAND_FIS_RECEIVE_RUNNING | AHCI_PORT_COMMAND_FIS_RECEIVE_ENABLED | AHCI_PORT_COMMAND_START
	jz .portAlreadyIdle
	and eax, ~AHCI_PORT_COMMAND_START
	mov [ecx + AHCI_PORT_COMMAND], eax
	; TODO implement timeout
.waitForCommandRunningDone:
	mov eax, [ecx + AHCI_PORT_COMMAND]
	test eax, AHCI_PORT_COMMAND_COMMAND_LIST_RUNNING
	jnz .waitForCommandRunningDone
	test eax, AHCI_PORT_COMMAND_FIS_RECEIVE_ENABLED
	jz .portAlreadyIdle
	and eax, ~AHCI_PORT_COMMAND_FIS_RECEIVE_ENABLED
	mov [ecx + AHCI_PORT_COMMAND], eax
.waitForCommandFISRunningDone:
	mov eax, [ecx + AHCI_PORT_COMMAND]
	test eax, AHCI_PORT_COMMAND_FIS_RECEIVE_RUNNING
	jnz .waitForCommandFISRunningDone
.portAlreadyIdle:
	add ecx, AHCI_PORT_SIZE
	shr ebx, 1
	test ebx, ebx
	jnz .portIdleLoop
	pop rax

	; Step 4: Determine number of command slots supported
	mov edx, [eax + AHCI_HBA_CAPABILITIES]
	shr edx, AHCI_HBA_CAPABILITIES_NUM_COMMAND_SLOTS_OFFSET
	and edx, AHCI_HBA_CAPABILITIES_NUM_COMMAND_SLOTS_MASK
	add edx, 1 ; 0's based value, add 1 to get the number of command slots
	mov [AHCI_NUM_COMMAND_SLOTS], edx

	; Step 5: Allocate memory for each port's command list and FIS
	; Map memory for port received FIS and port command list as uncacheable
	push rax
	mov ebx, AHCI_PORT_RECEIVED_FIS_OFFSET | PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + AHCI_PORT_RECEIVED_FIS_OFFSET / 512], ebx
	add ebx, 1000
	mov [PAGE_TABLE_START + 3000h + (AHCI_PORT_RECEIVED_FIS_OFFSET + 1000h) / 512], ebx
	mov ebx, AHCI_PORT_COMMAND_LIST_OFFSET | PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + AHCI_PORT_COMMAND_LIST_OFFSET / 512], ebx
	invlpg [AHCI_PORT_RECEIVED_FIS_OFFSET]
	invlpg [AHCI_PORT_RECEIVED_FIS_OFFSET + 1000h]
	invlpg [AHCI_PORT_COMMAND_LIST_OFFSET]
	; Zero out command list and received FIS memory
	xor eax, eax
	mov edi, AHCI_PORT_RECEIVED_FIS_OFFSET
	mov ecx, (AHCI_PORT_RECEIVED_FIS_SIZE * 32) / 4
	rep stosd
	mov edi, AHCI_PORT_COMMAND_LIST_OFFSET
	mov ecx, (AHCI_PORT_COMMAND_LIST_SIZE * 32) / 4
	rep stosd
	pop rax
	; Set command list and FIS for each port
	push rax
	lea ecx, [eax + AHCI_PORT0_OFFSET]
	lea esi, [AHCI_PORT_COMMAND_LIST_OFFSET]
	lea edi, [AHCI_PORT_RECEIVED_FIS_OFFSET]
	mov eax, [rbp - 8] ; Get ports implented
.setPortMemory:
	test eax, 1
	jz .setPortMemoryPortNotPresent
	mov [ecx + AHCI_PORT_COMMAND_LIST_BASE], esi
	mov dword [ecx + AHCI_PORT_COMMAND_LIST_BASE_UPPER], 0
	mov [ecx + AHCI_PORT_FIS_BASE], edi
	mov dword [ecx + AHCI_PORT_FIS_BASE_UPPER], 0
	; Set FIS receive enabled for this port
	mov edx, [ecx + AHCI_PORT_COMMAND]
	or edx, AHCI_PORT_COMMAND_FIS_RECEIVE_ENABLED
	mov [ecx + AHCI_PORT_COMMAND], edx
	; Set start for this port
	mov edx, [ecx + AHCI_PORT_COMMAND]
	or edx, AHCI_PORT_COMMAND_START
	mov [ecx + AHCI_PORT_COMMAND], edx
.setPortMemoryPortNotPresent:
	add ecx, AHCI_PORT_SIZE
	add esi, AHCI_PORT_COMMAND_LIST_SIZE
	add edi, AHCI_PORT_RECEIVED_FIS_SIZE
	shr eax, 1
	test eax, eax
	jnz .setPortMemory
	pop rax

	; Step 6: Clear SATA_ERROR and interrupts
	push rax
	mov ebx, [rbp - 8] ; Get ports implented
	lea ecx, [eax + AHCI_PORT0_OFFSET]
.clearPortInterrupts:
	test ebx, 1
	jz .clearPortInterruptsPortNotPresent
	mov dword [ecx + AHCI_PORT_SATA_ERROR], 0xFFFFFFFF ; Clear errors
	mov dword [ecx + AHCI_PORT_INTERRUPT_STATUS], AHCI_PORT_INTERRUPT_STATUS_ALL_CLEAR_BITS ; Clear interrupts, must be done before clearing global interrupt flags
.clearPortInterruptsPortNotPresent:
	shr ebx, 1
	add ecx, AHCI_PORT_SIZE
	test ebx, ebx
	jnz .clearPortInterrupts
	pop rax
	mov dword [eax + AHCI_INTERRUPT_STATUS], 0xFFFFFFFF ; Clear HBA interrupts

	; Step 7: Program the interrupt enable bits
	push rax
	mov ebx, [rbp - 8] ; Get ports implented
	lea ecx, [eax + AHCI_PORT0_OFFSET]
.setPortInterrupts:
	test ebx, 1
	jz .setPortInterruptsPortNotPresent
	;mov dword [ecx + AHCI_PORT_INTERRUPT_ENABLE], AHCI_PORT_INTERRUPT_ENABLE_DEVICE_TO_HOST_REGISTER_FIS_ENABLE | AHCI_PORT_INTERRUPT_ENABLE_PIO_SETUP_FIS_ENABLE | AHCI_PORT_INTERRUPT_ENABLE_DMA_SETUP_FIS_ENABLE | AHCI_PORT_INTERRUPT_ENABLE_SET_DEVICE_BITS_FIS_ENABLE | AHCI_PORT_INTERRUPT_ENABLE_DESCRIPTOR_PROCESSED_ENABLE
	mov dword [ecx + AHCI_PORT_INTERRUPT_ENABLE], AHCI_PORT_INTERRUPT_ENABLE_ALL
.setPortInterruptsPortNotPresent:
	shr ebx, 1
	add ecx, AHCI_PORT_SIZE
	test ebx, ebx
	jnz .setPortInterrupts
	pop rax
	; Enable global interrupts
	mov edx, [eax + AHCI_GLOBAL_HBA_CONTROL]
	or edx, AHCI_GLOBAL_HBA_CONTROL_INTERRUPT_ENABLE
	mov [eax + AHCI_GLOBAL_HBA_CONTROL], edx

	; Zero out important queue structures
	push rax
	xor eax, eax
	; Zero out DISKQUEUE structures
	mov edi, DISK_PORT_QUEUES_OFFSET
	mov ecx, DISK_PORT_QUEUES_REQUIRED_PAGES * 1024
	rep stosd
	; Zero out command table structures
	mov edi, AHCI_COMMAND_TABLES_OFFSET
	mov ecx, AHCI_COMMAND_TABLES_REQUIRED_PAGES * 1024
	rep stosd
	pop rax

	; Start disks and ask for their information
	push rax
	lea ecx, [eax + AHCI_PORT0_OFFSET]
	mov eax, [rbp - 8] ; Get ports implented
	; esi will contain the final mask of implemented drives
	xor esi, esi
	mov edi, 1
.queryDrives:
	test eax, 1
	jz .queryDrivesDriveNotPresent
	mov edx, [ecx + AHCI_PORT_TASK_FILE_DATA]
	mov ebx, AHCI_PORT_TASK_FILE_DATA_STS_BUSY | AHCI_PORT_TASK_FILE_DATA_STS_DATA_TRANSFER_REQUESTED
	test edx, ebx ; PxTFD.STS.BSY and PxTFD.STS.DRQ must be 0 for this to be a functional device
	jnz .queryDrivesDriveNotPresent
	mov edx, [ecx + AHCI_PORT_SATA_STATUS]
	and edx, AHCI_PORT_SATA_STATUS_DEVICE_DETECTION_MASK
	cmp edx, 3 ; PxSSTS.DET must be 3 for this to be a functional device
	jne .queryDrivesDriveNotPresent
	; This is a functional device
	or esi, edi ; add the current bit to the functional device mask
	; Send ATA identify
	push rax
	push rcx
	; Setup identify FIS
	mov byte [AHCI_COMMAND_TABLES_OFFSET], FIS_TYPE_REG_HOST_TO_DEVICE
	mov byte [AHCI_COMMAND_TABLES_OFFSET + 1], 0x80 ; Set command bit
	mov byte [AHCI_COMMAND_TABLES_OFFSET + 2], ATA_CMD_IDENTIFY_DEVICE
	; Configure command table
	mov dword [AHCI_PORT_COMMAND_LIST_OFFSET], (1 << 16) | 5 ; 1 table length, FIS length is 5 dwords
	mov dword [AHCI_PORT_COMMAND_LIST_OFFSET + 4], 0 ; PRD byte count
	mov dword [AHCI_PORT_COMMAND_LIST_OFFSET + 8], AHCI_COMMAND_TABLES_OFFSET ; Command table base address
	mov dword [AHCI_PORT_COMMAND_LIST_OFFSET + 12], 0 ; Command table base address upper
	; Configure descriptor table entry
	mov dword [AHCI_COMMAND_TABLES_OFFSET + AHCI_COMMAND_TABLE_CMD0_OFFSET], AHCI_COMMAND_TABLES_OFFSET + 1024 ; Data base address
	mov dword [AHCI_COMMAND_TABLES_OFFSET + AHCI_COMMAND_TABLE_CMD0_OFFSET + 4], 0 ; Data base address upper
	mov dword [AHCI_COMMAND_TABLES_OFFSET + AHCI_COMMAND_TABLE_CMD0_OFFSET + 8], 0 ; Reserved
	mov dword [AHCI_COMMAND_TABLES_OFFSET + AHCI_COMMAND_TABLE_CMD0_OFFSET + 12], 512 - 1 ; Data byte count. Minus one because an even number of bytes must be transferd, and the low bit is set to 1, rounding up
	; Send command
	mov dword [ecx + AHCI_PORT_COMMAND_ISSUE], 1
.identifyRunning:
	mov eax, [ecx + AHCI_PORT_COMMAND_ISSUE]
	test eax, 1
	jnz .identifyRunning
	mov rax, [AHCI_COMMAND_TABLES_OFFSET + 1024 + 200]
	shl rax, 9 ; Sectors to bytes (multiply by 512)
	call debugPrintInteger
	call debugPrintNewline
	; Store hard disk size in DISKQUEUE
	tzcnt ecx, edi
	imul ecx, DISK_PORT_QUEUE_SIZE
	mov [DISK_PORT_QUEUES_OFFSET + ecx + DISK_PORT_QUEUE_DISK_SIZE_OFFSET], rax
	pop rcx
	pop rax
.queryDrivesDriveNotPresent:
	add ecx, AHCI_PORT_SIZE
	shl edi, 1
	shr eax, 1
	test eax, eax
	jnz .queryDrives
	pop rax
	mov [AHCI_DEVICES_PRESENT], esi

	lea rax, [ahciInitDone]
	call debugPrint

	add rsp, 8
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; eax contains port
ahciProcessQueuedCommandsForPort:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	mov rbp, rsp
	sub rsp, 40
	; Local variable table
	; [rbp - 4]  : portIdx
	; [rbp - 8]  : totalTransferSize
	; [rbp - 12] : cmdSizeBytes
	; [rbp - 16] : portCmdIdx
	; [rbp - 20] : descriptor
	; [rbp - 24] : transferSize
	; [rbp - 32] : start
	; [rbp - 36] ; cmdQueuedBytes

	; Check to make sure the port is present first
	and eax, 31
	bt [AHCI_DEVICES_PRESENT], eax
	jnc .end

	mov [ebp - 4], eax ; port
	; eax gets pointer to PORTQUEUE struct
	imul eax, eax, DISK_PORT_QUEUE_SIZE ; queue
	add eax, DISK_PORT_QUEUES_OFFSET
	mov edx, [eax + DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET]
	not edx
	test edx, edx
	jz .end ; Already full of active commands, can't queue anything new

	mov ebx, [eax + DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET] ; notFullyQueuedCmds
	jmp .checkAllCommandsContinue
.checkAllCommands:
	mov ecx, ebx ; queuedCmds
	jmp .checkCommandsContinueNoReset
.checkCommands:
	tzcnt edx, ecx ; cmdIdx
	imul esi, edx, DISKCMD_SIZE
	lea esi, [eax + DISK_PORT_QUEUE_CMD_QUEUE_OFFSET + esi] ; cmd
	imul edi, [esi + DISKCMD_SECTORS_OFFSET], 512 ; cmdSizeBytes
	mov [ebp - 12], edi ; cmdSizeBytes
	imul edx, [esi + DISKCMD_QUEUED_OFFSET], 512 ; cmdQueuedBytes
	mov [ebp - 36], edx ; cmdQueuedBytes
	tzcnt edx, ecx ; cmdIdx
	test edi, edi
	jnz .cmdSizeNotZero
	btr [eax + DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET], edx
	btr ebx, edx ; btr notFullyQueuedCmds, cmdIdx
	; Finish command here
	; For now, write a boolean true to the user data as a pointer, figure out something better later
	mov rsi, [rsi + DISKCMD_USER_DATA_OFFSET]
	mov byte [rsi], 1
	jmp .checkCommandsContinue
.cmdSizeNotZero:
	cmp [ebp - 36], edi ; cmdQueuedBytes == cmdSizeBytes
	jne .cmdHasMoreToQueue
	btr ebx, edx ; btr notFullyQueuedCmds, cmdIdx
	jmp .checkCommandsContinue
.cmdHasMoreToQueue:
	; By here, we know we have to actually queue command data
	mov edx, [eax + DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET]
	not edx
	tzcnt edx, edx ; portCmdIdx
	mov [ebp - 16], edx ; portCmdIdx

	push rax
	push rbx
	push rcx
	mov dword [ebp - 20], 0 ; descriptor
	mov dword [ebp - 8], 0 ; totalTransferSize
	jmp .descriptorLoopContinue
.descriptorLoop:
	mov rax, [esi + DISKCMD_MEM_ADDR_RW_OFFSET] ; memoryOffset
	and rax, ~DISKCMD_MEM_ADDR_RW_WRITE_BIT ; 32 bit immediate sign extended to 64 bits
	mov ebx, [rbp - 36] ; cmdQueuedBytes
	add rax, rbx ; memoryOffset
	call tlbAddressLookup
	test ebx, PAGE_PRESENT
	jnz .pageWasPresent
	; Found a page without a physical mapping, end transfer here
	mov edi, [ebp - 36] ; cmdSizeBytes = cmdQueuedBytes
	mov [ebp - 12], edi ; cmdSizeBytes (stack) = cmdSizeBytes (register)
	shr edi, 9 ; cmdSizeBytes / 512
	mov [esi + DISKCMD_SECTORS_OFFSET], edi ; cmd.numSectors = cmdSizeBytes / 512
	jmp .descriptorLoopEnd
.pageWasPresent:
	and rbx, ~0xFFF
	mov edx, eax
	and edx, 0xFFF
	mov [ebp - 32], rbx ; start
	add [ebp - 32], rdx ; start
	; pageTransfer (edx) = min(4096 - (memoryOffset & 0xFFF), cmdSize - cmdQueued)
	mov ecx, 4096
	sub ecx, edx
	mov edx, edi ; cmdSizeBytes
	sub edx, [ebp - 36] ; cmdQueuedBytes
	cmp ecx, edx
	cmovl edx, ecx
	add rax, rdx ; memoryOffset += pageTransfer
	mov [ebp - 24], edx ; transferSize
	jmp .commandEntryLoopContinue
.commandEntryLoop:
	mov rcx, rbx ; prevPhysicalPage
	call tlbAddressLookup
	test rbx, PAGE_PRESENT
	jnz .pageWasPresent2
	; Page with no physical mapping, end transfer here
	mov edi, [ebp - 36] ; cmdSizeBytes = cmdQueuedBytes
	add edi, [ebp - 24] ; cmdSizeBytes += transferSize
	mov [ebp - 12], edi ; cmdSizeBytes (stack) = cmdSizeBytes (register)
	shr edi, 9 ; cmdSizeBytes / 512
	mov [esi + DISKCMD_SECTORS_OFFSET], edi ; cmd.numSectors = cmdSizeBytes / 512
	jmp .commandEntryLoopEnd
.pageWasPresent2:
	and rbx, ~0xFFF
	sub rcx, rbx
	cmp rcx, -4096 ; Distance between pages should be 4096, one page
	jne .commandEntryLoopEnd
	; pageTransfer (edx) = min(1024 * 1024 * 4 - transferSize, min(4096, cmdSizeBytes - (cmdQueuedBytes + transferSize)));
	mov ecx, [ebp - 24] ; transferSize
	add ecx, [ebp - 36] ; cmdQueuedBytes
	mov edx, edi ; cmdSizeBytes
	sub edx, ecx ; pageTransfer = cmdSizeBytes - (cmdQueuedBytes + transferSize)
	mov ecx, 4096
	cmp ecx, edx
	cmovl edx, ecx ; pageTransfer = min(4096, pageTransfer)
	mov ecx, 1024 * 1024 * 4
	sub ecx, [ebp - 24] ; transferSize
	cmp ecx, edx
	cmovl edx, ecx ; pageTransfer = min(1024 * 1024 * 4 - transferSize, pageTransfer)
	add [ebp - 24], edx ; transferSize += pageTransfer
	add rax, rdx ; src += pageTransfer
.commandEntryLoopContinue:
	cmp dword [ebp - 24], 1024 * 1024 * 4 ; transferSize < 4 megabyte limit for entries
	jge .commandEntryLoopEnd
	mov edx, [ebp - 36] ; cmdQueuedBytes
	add edx, [ebp - 24] ; transferSize
	cmp edx, edi ; cmdQueuedBytes + transferSize < cmdSizeBytes
	jl .commandEntryLoop
.commandEntryLoopEnd:
	mov ecx, [ebp - 4] ; portIdx
	shl ecx, 5 ; multiply by 32 tables per port
	add ecx, [ebp - 16] ; portCmdIdx
	imul ecx, ecx, AHCI_COMMAND_TABLE_SIZE
	imul edx, [ebp - 20], AHCI_COMMAND_TABLE_CMD_SIZE ; descriptorIdx
	lea ecx, [AHCI_COMMAND_TABLES_OFFSET + ecx + AHCI_COMMAND_TABLE_CMD0_OFFSET + edx]
	mov rdx, [ebp - 32] ; start
	mov [ecx + AHCI_PHYSICAL_REGION_DESCRIPTOR_DATA_BASE_ADDRESS_OFFSET], rdx
	mov dword [ecx + AHCI_PHYSICAL_REGION_DESCRIPTOR_RESERVED_OFFSET], 0
	mov edx, [ebp - 24] ; transferSize
	sub edx, 1 ; byte counts are aligned to 2, with the low bit set to 1, rounding up
	or edx, 1
	mov [ecx + AHCI_PHYSICAL_REGION_DESCRIPTOR_DATA_BYTE_COUNT_OFFSET], edx
	mov edx, [ebp - 24] ; transferSize
	add [ebp - 8], edx ; totalTransferSize += transferSize
	add [ebp - 36], edx ; cmdQueuedBytes += transferSize
	add dword [ebp - 20], 1 ; descriptor++
.descriptorLoopContinue:
	cmp dword [ebp - 20], AHCI_NUM_COMMAND_TABLE_ENTRIES ; descriptor < AHCI_NUM_COMMAND_TABLE_ENTRIES
	jge .descriptorLoopEnd
	cmp [ebp - 36], edi ; cmdQueuedBytes < cmdSizeBytes
	jl .descriptorLoop
.descriptorLoopEnd:
	pop rcx
	pop rbx
	pop rax

	tzcnt edx, ecx ; cmdIdx
	cmp [ebp - 36], edi ; cmdQueuedBytes == cmdSizeBytes
	jne .cmdHasMoreToQueue2
	btr ebx, edx ; notFullyQueuedCmds = btr(notFullyQueuedCmds, cmdIdx);
	jmp .cmdSizeCorrectionNotNecessary
.cmdHasMoreToQueue2:
	push rbx
	push rcx
	; Correct the transfer amount so it's always a multiple of 512 (we can only transfer full sectors)
	; correction = (512 - ((cmd.memAddressAndRW & ~DISKCMD_MEM_ADDR_RW_WRITE_BIT) & 511)) & 511
	mov ecx, [esi + DISKCMD_MEM_ADDR_RW_OFFSET]
	and ecx, (~DISKCMD_MEM_ADDR_RW_WRITE_BIT) & 511
	mov ebx, 512
	sub ebx, ecx
	and ebx, 511
	sub [ebp - 8], ebx ; totalTransferSize -= correction
	sub [ebp - 36], ebx ; cmdQueuedBytes -= correction
	; commandTables[port][portCmdIdx].descriptors[descriptorIdx - 1].dataByteCount -= correction
	mov ecx, [ebp - 4] ; portIdx
	shl ecx, 5 ; multiply by 32 tables per port
	add ecx, [ebp - 16] ; portCmdIdx
	imul ecx, ecx, AHCI_COMMAND_TABLE_SIZE
	mov edx, [ebp - 20] ; descriptorIdx
	sub edx, 1
	imul edx, edx, AHCI_COMMAND_TABLE_CMD_SIZE
	sub [AHCI_COMMAND_TABLES_OFFSET + ecx + AHCI_COMMAND_TABLE_CMD0_OFFSET + edx + AHCI_PHYSICAL_REGION_DESCRIPTOR_DATA_BYTE_COUNT_OFFSET], ebx
	pop rcx
	pop rbx
.cmdSizeCorrectionNotNecessary:

	test edi, edi ; cmdSizeBytes
	jnz .cmdStillRunning
	; Finish Command here
	; Set boolean for now
	mov rsi, [rsi + DISKCMD_USER_DATA_OFFSET]
	mov byte [rsi], 1
	jmp .checkCommandsContinue
.cmdStillRunning:
	tzcnt edx, ecx ; cmdIdx
	push rcx
	push rdx
	; cmd.queued = cmdQueuedBytes / 512
	mov edi, [ebp - 36] ; cmdQueuedBytes
	shr edi, 9 ; cmdQueuedBytes / 512
	mov [esi + DISKCMD_QUEUED_OFFSET], edi
	; queue.activeCommands[portCmdIdx] = PORTCMD{ cmdIdx, totalTransferSize / 512 };
	mov ecx, [ebp - 16] ; portCmdIdx
	mov [eax + DISK_PORT_QUEUE_ACTIVE_CMDS_OFFSET + ecx * PORTCMD_SIZE + PORTCMD_CMD_IDX_OFFSET], edx
	mov edi, [ebp - 8] ; totalTransferSize
	shr edi, 9 ; totalTransferSize / 512
	mov [eax + DISK_PORT_QUEUE_ACTIVE_CMDS_OFFSET + ecx * PORTCMD_SIZE + PORTCMD_SECTOR_COUNT_OFFSET], edi
	; queue.activeCommands |= 1 << portCmdIdx
	bts [eax + DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET], ecx
	; Setup FIS
	imul ecx, [ebp - 4], AHCI_COMMAND_TABLE_SIZE ; portIdx
	add ecx, AHCI_COMMAND_TABLES_OFFSET
	test dword [esi + DISKCMD_MEM_ADDR_RW_OFFSET], DISKCMD_MEM_ADDR_RW_WRITE_BIT
	jz .isReadCmd
	mov dword [ecx + 0], FIS_TYPE_REG_HOST_TO_DEVICE | (0x80 << 8) | (ATA_CMD_WRITE_DMA_EXT << 16) ; 0x80 is the command bit (as opposed to a control FIS)
	jmp .isWriteCmd
.isReadCmd:
	mov dword [ecx + 0], FIS_TYPE_REG_HOST_TO_DEVICE | (0x80 << 8) | (ATA_CMD_READ_DMA_EXT << 16) ; 0x80 is the command bit (as opposed to a control FIS)
.isWriteCmd:
	mov rdx, [rsi + DISKCMD_DISK_ADDR_OFFSET]
	and rdx, 0xFFFFFF
	or edx, 0b01000000 << 24 ; I guess this is the Drive/head register from IDE? This was in the osdev wiki example, and that's my best guess for what "1 << 6" means in the "device" field
	mov [ecx + 4], edx
	mov rdx, [rsi + DISKCMD_DISK_ADDR_OFFSET]
	shr rdx, 24
	and rdx, 0xFFFFFF
	mov [ecx + 8], edx
	and edi, 0xFFFF ; totalTransferSectors
	mov [ecx + 12], edi
	mov dword [ecx + 16], 0
	; Configure command table
	mov ecx, [ebp - 4] ; portIdx
	shl ecx, 5 ; multiply by 32 tables per port
	add ecx, [ebp - 16] ; portCmdIdx
	shl ecx, 5 ; multiply by 32 bytes per command header to get header offset from AHCI_PORT_COMMAND_LIST_OFFSET
	mov edx, [rbp - 20] ; descriptor count
	shl edx, 16
	or edx, 5 ; FIS length is 5 dwords
	mov edi, [esi + DISKCMD_MEM_ADDR_RW_OFFSET]
	and edi, DISKCMD_MEM_ADDR_RW_WRITE_BIT
	shr edi, 5
	or edx, edi ; Add in the write bit
	mov [AHCI_PORT_COMMAND_LIST_OFFSET + ecx], edx
	mov dword [AHCI_PORT_COMMAND_LIST_OFFSET + ecx + 4], 0 ; PRD byte count (zero initialize, updated by hardware)
	mov edx, [ebp - 4] ; portIdx
	shl edx, 5 ; multiply by 32 tables per port
	add edx, [ebp - 16] ; portCmdIdx
	imul edx, edx, AHCI_COMMAND_TABLE_SIZE
	add edx, AHCI_COMMAND_TABLES_OFFSET
	mov dword [AHCI_PORT_COMMAND_LIST_OFFSET + ecx + 8], edx ; Command table base address
	mov dword [AHCI_PORT_COMMAND_LIST_OFFSET + ecx + 12], 0 ; Command table base address upper
	; Issue command
	mov edi, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	imul esi, [ebp - 4], AHCI_PORT_SIZE
	xor edx, edx
	mov ecx, [ebp - 16]
	bts edx, ecx
	mov [edi + AHCI_PORT0_OFFSET + esi + AHCI_PORT_COMMAND_ISSUE], edx
	pop rdx
	pop rcx
.checkCommandsContinue:
	tzcnt edx, ecx ; cmdIdx
	btr ecx, edx
.checkCommandsContinueNoReset:
	; If no commands to process, break
	test ecx, ecx ; queuedCommands
	jz .checkCommandsEnd
	; If all command slots are full, break
	mov edx, [eax + DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET]
	not edx
	test edx, edx
	jnz .checkCommands
.checkCommandsEnd:
.checkAllCommandsContinue:
	; If no commands to process, break
	test ebx, ebx ; notFullyQueuedCmds
	jz .checkAllCommandsEnd
	; If all command slots are full, break
	mov edx, [eax + DISK_PORT_QUEUE_ACTIVE_COMMANDS_MASK_OFFSET]
	not edx
	test edx, edx
	jnz .checkAllCommands
.checkAllCommandsEnd:
	
.end:
	mov rsp, rbp
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

testAHCIReadWrite:
	push rax
	sub rsp, 8

	;mov eax, 512
;.loop:
	;sub eax, 1
	;mov [1024 * 1024 + 512 + eax], al
	;test eax, eax
	;jnz .loop

	lea rax, [1024 * 1024 + 1024]
	mov dword [rax + DISKCMD_PORT_OFFSET], 0
	mov dword [rax + DISKCMD_ID_OFFSET], 0
	mov qword [rax + DISKCMD_MEM_ADDR_RW_OFFSET], 1024 * 1024 + 512 | DISKCMD_MEM_ADDR_RW_READ_BIT
	mov qword [rax + DISKCMD_DISK_ADDR_OFFSET], 2
	mov dword [rax + DISKCMD_SECTORS_OFFSET], 1
	mov dword [rax + DISKCMD_QUEUED_OFFSET], 0
	mov qword [rax + DISKCMD_USER_DATA_OFFSET], rsp
	mov byte [rsp], 0
	call ahciQueueDiskCmd

.notDone:
	cmp byte [rsp], 1
	je .done
	hlt
	jmp .notDone
.done:

	lea rax, [ahciReadComplete]
	call debugPrint
	
	lea rax, [1024 * 1024 + 1024]
	mov dword [rax + DISKCMD_PORT_OFFSET], 0
	mov dword [rax + DISKCMD_ID_OFFSET], 0
	mov qword [rax + DISKCMD_MEM_ADDR_RW_OFFSET], 1024 * 1024 + 512 | DISKCMD_MEM_ADDR_RW_WRITE_BIT
	mov qword [rax + DISKCMD_DISK_ADDR_OFFSET], 15
	mov dword [rax + DISKCMD_SECTORS_OFFSET], 1
	mov dword [rax + DISKCMD_QUEUED_OFFSET], 0
	mov qword [rax + DISKCMD_USER_DATA_OFFSET], rsp
	mov byte [rsp], 0
	call ahciQueueDiskCmd

.notDone2:
	cmp byte [rsp], 1
	je .done2
	hlt
	jmp .notDone2
.done2:

	lea rax, [ahciWriteComplete]
	call debugPrint

	add rsp, 8
	pop rax
	ret

; rax has pointer to DISKCMD structure
ahciQueueDiskCmd:
	push rax
	push rbx
	push rcx
	push rdx

	mov ebx, [rax + DISKCMD_PORT_OFFSET]
	and ebx, 31
	; Check to make sure the port is present first
	bt [AHCI_DEVICES_PRESENT], ebx
	jnc .end
	imul ebx, ebx, DISK_PORT_QUEUE_SIZE

	; Ensure that the disk offset is valid
	mov rdx, [DISK_PORT_QUEUES_OFFSET + rbx + DISK_PORT_QUEUE_DISK_SIZE_OFFSET]
	mov rcx, [rax + DISKCMD_DISK_ADDR_OFFSET]
	shl rcx, 9 ; sectors to bytes, multiply by 512
	cmp ecx, edx
	jl .diskStartValid
	; Trying to write entirely outside the disk size, just zero it out and let a later function complete the operation
	mov qword [rax + DISKCMD_DISK_ADDR_OFFSET], 0
	mov dword [DISKCMD_SECTORS_OFFSET], 0
	jmp .diskEndValid
.diskStartValid:
	mov ecx, [rax + DISKCMD_SECTORS_OFFSET]
	add rcx, [rax + DISKCMD_DISK_ADDR_OFFSET]
	mov rdx, [DISK_PORT_QUEUES_OFFSET + rbx + DISK_PORT_QUEUE_DISK_SIZE_OFFSET]
	shr rdx, 9 ; Bytes to sectors, divide by 512
	cmp rcx, rdx ; writeEnd <= diskSize
	jle .diskEndValid
	; cmd.sectors = disk.maxSectors - cmd.start
	sub rdx, [rax + DISKCMD_DISK_ADDR_OFFSET]
	mov [rax + DISKCMD_SECTORS_OFFSET], edx
.diskEndValid:

	; Block until a queue slot is available
.testQueueAvailable:
	mov ecx, [DISK_PORT_QUEUES_OFFSET + ebx + DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET]
	not ecx ; If it's all ones, wait for it to have a zero
	test ecx, ecx
	jnz .queueAvailable
	hlt
	jmp .testQueueAvailable
.queueAvailable:

	; Give it the next available id
	mov ecx, [DISK_CMD_CURRENT_ID]
	add dword [DISK_CMD_CURRENT_ID], 1
	mov [rax + DISKCMD_ID_OFFSET], ecx
	; Queue it up
	mov edx, [DISK_PORT_QUEUES_OFFSET + ebx + DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET]
	not edx
	tzcnt ecx, edx
	; Copy command to array
	imul edx, ecx, DISKCMD_SIZE
	movups xmm0, [rax]
	movups xmm1, [rax + 16]
	mov rax, [rax + 32]
	movups [DISK_PORT_QUEUES_OFFSET + ebx + DISK_PORT_QUEUE_CMD_QUEUE_OFFSET + edx], xmm0
	movups [DISK_PORT_QUEUES_OFFSET + ebx + DISK_PORT_QUEUE_CMD_QUEUE_OFFSET + edx + 16], xmm1
	mov [DISK_PORT_QUEUES_OFFSET + ebx + DISK_PORT_QUEUE_CMD_QUEUE_OFFSET + edx + 32], rax
	; Set command queued bit
	bts [DISK_PORT_QUEUES_OFFSET + ebx + DISK_PORT_QUEUE_QUEUED_DISK_CMDS_MASK_OFFSET], ecx

	cli ; Ensure we don't race with an AHCI interrupt here. I think this is the correct thing to do, but not fully sure
	mov eax, ecx
	call ahciProcessQueuedCommandsForPort
	sti

.end:
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

	


initializeVirtIONet:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi

	mov ecx, [VIRTIO_NET_PCI_ADDRESS] ; PCI config address for the VirtIO network interface

	; Scan capabilities for virtio common cfg, virtio notify cfg, and enable MSI-X
	xor esi, esi ; Bit flags for completed search. Common cfg is 0b1, notify is 0b10, msi x is 0b100
	lea eax, [ecx + 0x34] ; Offset for capabilities pointer
	call readPCIConfigDword
	and eax, 0b11111100 ; 8 bit value, must be dword aligned, so take off the last two bits
	test eax, eax
	jz .capabilitySearchDone
.capabilitySearch:
	add eax, ecx
	mov edi, eax
	call readPCIConfigDword ; Read Message Control | Next Pointer | Capability Id. See PCI spec, section 6.8.1
	mov edx, eax
	and edx, 0xFF ; Get type

	; Check for MSI-X TODO
	cmp edx, 0x11 ; 0x11 is the MSI-X id
	jne .capabilityNotMSIX
	test esi, 0b100
	jnz .capabilityContinue ; We already found an MSIX
	and eax, ~(0b111 << 20) ; Set message control to one message enabled
	or eax, (1 << 16) ; Enable MSI
	test eax, (1 << 23) ; Check if 64 bit capable
	setnz bl
	mov edx, eax
	mov eax, edi
	call writePCIConfigDword
	lea eax, [edi + 4]
	mov edx, 0xFEE00000 ; See intel software developer manual vol 3, section 11.11.1 Message Address Register Format
	call writePCIConfigDword
	test bl, bl
	jz .msiNot64Bit
	add edi, 4
	lea eax, [edi + 4]
	xor edx, edx ; Zero out upper bits
	call writePCIConfigDword
.msiNot64Bit:
	lea eax, [edi + 8]
	mov edx, AHCI_INTERRUPT_VECTOR ; See manual again, we only care about the vector in this case
	call writePCIConfigDword
	or esi, 0b100 ; Mark msi done
	jmp .capabilityContinue
.capabilityNotMSIX:
	cmp edx, 0x09 ; PCI_CAP_ID_VNDR
	jne .capabilityNotVendorSpecific
	mov edx, eax
	shr edx, 24 ; Extract cfg_type, see virtio spec section 4.1.4


.capabilityNotVendorSpecific:
.capabilityContinue:
	cmp esi, 0b111
	jz .capabilitySearchDone ; We've found all 3 necessary components, we're done searching through capabilities now
	mov eax, edi
	call readPCIConfigDword
	shr eax, 8 ; Get next pointer
	and eax, 0b11111100 ; 8 bit value, must be dword aligned, so take off the last two bits
	test eax, eax ; Zero signifies the end of the list
	jnz .capabilitySearch
.capabilitySearchDone:

	; Enable PCI interrupts, memory control, and bus mastering
	lea eax, [ecx + 0x04] ; Offset for Status | Command
	call readPCIConfigDword
	or eax, PCI_CONTROL_MEMORY_SPACE | PCI_CONTROL_BUS_MASTER
	and eax, ~PCI_CONTROL_INTERRUPT_DISABLE
	mov edx, eax
	lea eax, [ecx + 0x04] ; Offset for Status | Command
	call writePCIConfigDword

	; Map ??? memory
	mov eax, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	mov edx, eax
	; Align to page, since I'm pretty sure ABAR is only guaranteed to be aligned to 16, not a full page
	and eax, ~(1000h - 1)
	or eax, PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + AHCI_MAPPED_OFFSET / 512], eax
	add eax, 1000h
	mov [PAGE_TABLE_START + 3000h + (AHCI_MAPPED_OFFSET + 1000h) / 512], eax
	add eax, 1000h
	mov [PAGE_TABLE_START + 3000h + (AHCI_MAPPED_OFFSET + 2000h) / 512], eax
	; Get the low offset
	and edx, 1000h - 1
	; Mapped virtual address of PCI ABAR
	lea eax, [edx + AHCI_MAPPED_OFFSET]
	invlpg [eax]
	invlpg [eax + 1000h]
	invlpg [eax + 2000h]
	mov [PCI_AHCI_CONTROLLER_ABAR_ADDRESS], eax
	
.end:
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

ahciInitDone db "AHCI Init Done",0x0a,0x00
ahciWriteComplete db "AHCI Write Complete",0x0a,0x00
ahciReadComplete db "AHCI Read Complete",0x0a,0x00
debugGotHere db "GotHere",0x0a,0x00

times 512 - ($-$$) % 512 db 0
endBootloader: