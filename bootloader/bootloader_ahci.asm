
%define CR0_PROTECTION_ENABLE (1 << 0)
%define CR0_MONITOR_COPROCESSOR (1 << 1)
%define CR0_EMULATION (1 << 2)
%define CR0_PAGING (1 << 31)
%define CR4_PHYSICAL_ADDRESS_EXTENSION (1 << 5)
%define CR4_PAGE_GLOBAL (1 << 7)
%define CR4_FXSAVE_FXRSTORE (1 << 9)
%define CR4_UNMASKED_SIMD_FLOATING_POINT_EXCEPTIONS (1 << 10)

%define EFLAGS_INTERRUPT_FLAG (1 << 9)

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
%define PAGE_SIZE (1 << 7)
%define PAGE_GLOBAL (1 << 8)

%define PML_SELF_REFERENCE 511
%define PML4_BIT_OFFSET 39
%define PML3_BIT_OFFSET 30
%define PML2_BIT_OFFSET 21
%define PML1_BIT_OFFSET 12
%define FOUR_LEVEL_PAGING_HIGH_BITS 0xFFFF000000000000

%define MEMORY_ALLOCATION_BITMAP_SUPERVISOR_ADDRESS 0xFFFF800000000000 ; physical memory allocation bitmap goes here
%define MEMORY_ALLOCATION_BITMAP_USER_ADDRESS 0xFFFFC00000000000 ; Tracks user allocations, gets reset to supervisor one on user application restart
%define BYTES_IN_MEMORY_ALLOCATION_BITMAP_BYTE (0x1000 * 8) ; One page per bit, 8 bits per byte
%define BYTES_IN_MEMORY_ALLOCATION_BITMAP_PAGE (0x1000 * 8 * 0x1000) ; One bit per page, 8 bits per byte, 0x1000 bytes per page

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

%define APIC_REGISTER_PAGE (SCRATCH_PAGES_START + SCRATCH_PAGES_SIZE)
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
%define AHCI_VIRTIO_NET_VECTOR (AHCI_INTERRUPT_VECTOR + 1)

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

; struct MEMENTRY {
;	u64 baseAddress;
;	u64 lengthBytes;
;	u32 type;
;	u32 padding;
; }
%define MEMENTRY_BASE_ADDRESS_OFFSET 0
%define MEMENTRY_LENGTH_OFFSET 8
%define MEMENTRY_END_OFFSET 8 ; Field changes to end after processing
%define MEMENTRY_TYPE_OFFSET 16
%define MEMENTRY_TYPE_UNAVAILABLE 0 ; This is not standard, I'm just using it to represent anything that isn't available.
%define MEMENTRY_TYPE_AVAILABLE 1
%define MEMENTRY_TYPE_RESERVED 2
%define MEMENTRY_TYPE_ACPI_RECLAIMABLE 3
%define MEMENTRY_TYPE_ACPI_NVS 4
%define MEMENTRY_SIZE 24

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
%define VIRTIO_MSI_NO_VECTOR 0xFFFF
%define VIRTIO_DEVICE_STATUS_ACKNOWLEDGE 1
%define VIRTIO_DEVICE_STATUS_DRIVER 2
%define VIRTIO_DEVICE_STATUS_FAILED 128
%define VIRTIO_DEVICE_STATUS_FEATURES_OK 8
%define VIRTIO_DEVICE_STATUS_DRIVER_OK 4
%define VIRTIO_DEVICE_STATUS_DEVICE_NEEDS_RESET 64
%define VIRTIO_F_VERSION_1 (1 << 32)
%define VIRTIO_NET_F_CSUM (1 << 0)
%define VIRTIO_NET_F_MAC (1 << 5)

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
;	le16 flags; // flags includes no_interrupt (hint to device to not interrupt when buffer consumed. The device does not have to follow this, it's just an optimization)
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
;	le16 flags; // flags includes no_notify (hint to driver to not kick the device when buffer added. The driver does not have to follow this, it's just an optimization)
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
;	u16 numFreeDescriptors; // If zero, can't allocate any more descriptors
;	u16 freeDescriptorChainHead; // Free descriptors are a linked list of descriptors
;	u16 descCount; // The real desc count negotiated with the device
;	u16 lastSeenUsed; // The last used descriptor index we haven't yet processed. Used to figure out how many more used indices to process when the device gives us back buffers
; }
%define VIRTQ_DESC_OFFSET 0
%define VIRTQ_AVAIL_OFFSET (VIRTQ_DESC_OFFSET + VIRTIO_DESC_COUNT_MAX * VIRTQ_DESC_SIZE)
%define VIRTQ_USED_OFFSET (VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_SIZE)
%define VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET (VIRTQ_USED_OFFSET + VIRTQ_USED_SIZE)
%define VIRTQ_FREE_DESCRIPTOR_CHAIN_HEAD_OFFSET (VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET + 2)
%define VIRTQ_DESC_COUNT_OFFSET (VIRTQ_FREE_DESCRIPTOR_CHAIN_HEAD_OFFSET + 2)
%define VIRTQ_LAST_SEEN_USED_OFFSET (VIRTQ_DESC_COUNT_OFFSET + 2)
%define VIRTQ_SIZE (VIRTQ_LAST_SEEN_USED_OFFSET + 2)

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
; struct virtio_net_hdr {
;	u8 flags;
;	u8 gso_type;
;	le16 hdr_len;
;	le16 gso_size;
;	le16 csum_start;
;	le16 csum_offset;
;	le16 num_buffers;
; }
%define VIRTIO_NET_HDR_FLAG_NEEDS_CSUM 1
%define VIRTIO_NET_HDR_FLAGS_OFFSET 0
%define VIRTIO_NET_HDR_GSO_NONE 0
%define VIRTIO_NET_HDR_GSO_TCPV4 1
%define VIRTIO_NET_HDR_GSO_UDP 3
%define VIRTIO_NET_HDR_GSO_TCPV6 4
%define VIRTIO_NET_HDR_GSO_ECN 0x80
%define VIRTIO_NET_HDR_GSO_TYPE_OFFSET 1
%define VIRTIO_NET_HDR_HDR_LEN_OFFSET 2
%define VIRTIO_NET_HDR_GSO_SIZE_OFFSET 4
%define VIRTIO_NET_HDR_CSUM_START_OFFSET 6
%define VIRTIO_NET_HDR_CSUM_OFFSET_OFFSET 8
%define VIRTIO_NET_HDR_NUM_BUFFERS_OFFSET 10
%define VIRTIO_NET_HDR_SIZE 12

%define VIRTIO_NET_PACKET_BUFFER_SIZE 1526 ; packet size 1500 + 14 byte ethernet header + 12 byte virtio_net_hdr
%define VIRTIO_NET_PACKET_BUFFER_PACKET_SIZE 1514 ; packet size 1500 + 14 byte ethernet header


%define HTON16(x) ((((x) << 8) | ((x) >> 8)) & 0xFFFF) ; Host to network byte order, a bswap since we're little endian
%define HTON32(x) ((((x) & 0xFF) << 24) | (((x) & 0xFF00) << 8) | (((x) & 0xFF0000) >> 8) | (((x) & 0xFF000000) >> 24))
%define ETHERTYPE_IPV4 0x0800
%define ETHERTYPE_ARP 0x0806
%define ETHERTYPE_IPV6 0x86DD
%define ETHERNET_HEADER_TYPE_OFFSET 12
%define ETHERNET_HEADER_SIZE 14
%define IPV4_HEADER_VERSION_INTERNET_HEADER_LENGTH_OFFSET 0
%define IPV4_HEADER_PROTOCOL_OFFSET 9
%define IPV4_HEADER_MIN_SIZE 20
%define ARP_OP_REQUEST 1
%define ARP_OP_REPLY 2
%define ARP_HARDWARE_TYPE_ETHERNET 1
; struct ARPHeader {
;	u16 hardwareType;
;	u16 protocolType;
;	u8 hardwareAddressSize; // Should be 6 for ethernet
;	u8 protocolAddressSize; // Should be 4 for ipv4
;	u16 opcode;
;	u8[6] senderMacAddress;
;	u8[4] senderIPAddress;
;	u8[6] targetMacAddress;
;	u8[4] targetIPAddress;
; }
%define ARP_HEADER_HARDWARE_TYPE_OFFSET 0
%define ARP_HEADER_PROTOCOL_TYPE_OFFSET 2
%define ARP_HEADER_HARDWARE_ADDRESS_SIZE_OFFSET 4
%define ARP_HEADER_PROTOCOL_ADDRESS_SIZE_OFFSET 5
%define ARP_HEADER_OPCODE_OFFSET 6
%define ARP_HEADER_SENDER_MAC_ADDRESS_OFFSET 8
%define ARP_HEADER_SENDER_IP_ADDRESS_OFFSET 14
%define ARP_HEADER_TARGET_MAC_ADDRESS_OFFSET 18
%define ARP_HEADER_TARGET_IP_ADDRESS_OFFSET 24
%define ARP_HEADER_SIZE 28
; ARP protocol type is the same as ethertype
%define ARP_PACKET_SIZE 28
; See RFC 790
%define INTERNET_PROTOCOL_TCP 6
%define INTERNET_PROTOCOL_UDP 17

; struct DHCPMessage {
;	u8 op; // BOOTREQUEST or BOOTREPLY
;	u8 htype; // hardware type from assigned number rfc, arp section. 1 is ethernet
;	u8 hlen; // hardware address len, always 6 in our case
;	u8 hops; // set to zero by us
;	u32 xid; // transaction id, random number chosen by client
;	u16 secs; // Seconds since beginning of DHCP process
;	u16 flags; // First bit is broadcast flag, rest are zero
;	u32 ciaddr; // Client IP
;	u32 yiaddr; // Your IP
;	u32 siaddr; // Next server to use in bootstrap IP
;	u32 giaddr; // Gateway IP
;	byte chaddr[16]; // Client hardware address
;	char sname[64]; // Optional server hostname
;	char file[128]; // Boot file name
;	byte options[0]; // Options
; }
%define DHCP_OP_OFFSET 0
%define DHCP_OP_BOOTREQUEST 1
%define DHCP_OP_BOOTREPLY 2
%define DHCP_HTYPE_OFFSET 1
%define DHCP_HLEN_OFFSET 2
%define DHCP_HOPS_OFFSET 3
%define DHCP_XID_OFFSET 4
%define DHCP_SECS_OFFSET 8
%define DHCP_FLAGS_OFFSET 10
%define DHCP_FLAG_BROADCAST (1 << 15)
%define DHCP_CIADDR_OFFSET 12
%define DHCP_YIADDR_OFFSET 16
%define DHCP_SIADDR_OFFSET 20
%define DHCP_GIADDR_OFFSET 24
%define DHCP_CHADDR_OFFSET 28
%define DHCP_SNAME_OFFSET 44
%define DHCP_FILE_OFFSET 108
%define DHCP_OPTIONS_OFFSET 236
%define DHCP_SIZE_MIN 236 ; Variable length, options could make it larger
%define DHCP_OPTION_MAGIC_COOKIE ((99 << 0) | (130 << 8) | (83 << 16) | (99 << 24))
%define DHCP_OPTION_PAD 0
%define DHCP_OPTION_END 255
%define DHCP_OPTION_SUBNET_MASK_OP 1
%define DHCP_OPTION_SUBNET_MASK_LEN 4
%define DHCP_OPTION_ROUTER_OP 3
%define DHCP_OPTION_ROUTER_LEN 4
%define DHCP_OPTION_REQUESTED_IP_ADDRESS_OP 50
%define DHCP_OPTION_REQUESTED_IP_ADDRESS_LEN 4
%define DHCP_OPTION_IP_ADDRESS_LEASE_TIME_OP 51
%define DHCP_OPTION_IP_ADDRESS_LEASE_TIME_LEN 4
%define DHCP_OPTION_OPTION_OVERLOAD_OP 52
%define DHCP_OPTION_OPTION_OVERLOAD_LEN 1
%define DHCP_OPTION_DHCP_MESSAGE_TYPE_OP 53
%define DHCP_OPTION_DHCP_MESSAGE_TYPE_LEN 1
%define DHCP_OPTION_SERVER_IDENTIFIER_OP 54
%define DHCP_OPTION_SERVER_IDENTIFIER_LEN 4
%define DHCPDISCOVER 1
%define DHCPOFFER 2
%define DHCPREQUEST 3
%define DHCPDECLINE 4
%define DHCPACK 5
%define DHCPNAK 6
%define DHCPRELEASE 7
%define DHCP_SERVER_PORT 67
%define DHCP_CLIENT_PORT 68
%define DHCP_CLIENT_STATE_START 0
%define DHCP_CLIENT_STATE_WAIT_FOR_OFFER 1
%define DHCP_CLIENT_STATE_OFFER_RECEIVED 2
%define DHCP_CLIENT_STATE_PENDING_ACK 3
%define DHCP_CLIENT_STATE_DENIED 4
%define DHCP_CLIENT_STATE_CONFIGURED 5

; struct DNSResourceRecord {
;	u8 name[0];
;	u16 type;
;	u16 class;
;	u32 ttl;
;	u16 rdlength;
;	u8 rdata[0];
; }
%define DNS_RESOURCE_RECORD_TYPE_OFFSET 0
%define DNS_RESOURCE_RECORD_CLASS_OFFSET 2
%define DNS_RESOURCE_RECORD_TTL_OFFSET 4
%define DNS_RESOURCE_RECORD_RDATA_LENGTH_OFFSET 8
%define DNS_RESOURCE_RECORD_RDATA_OFFSET 10
%define DNS_TYPE_A 1 ; host address
%define DNS_TYPE_NS 2 ; authoritative name sever
%define DNS_TYPE_MD 3 ; mail destination (obsolete, use MX)
%define DNS_TYPE_MF 4 ; mail forwarder (obsolete, use MX)
%define DNS_TYPE_CNAME 5 ; canonical name for an alias
%define DNS_TYPE_SOA 6 ; start of a zone authority
%define DNS_TYPE_MB 7 ; mailbox domain name
%define DNS_TYPE_MG 8 ; mail group member
%define DNS_TYPE_MR 9 ; mail rename domain name
%define DNS_TYPE_NULL 10 ; null resource record
%define DNS_TYPE_WKS 11 ; well known service description
%define DNS_TYPE_PTR 12 ; domain name pointer
%define DNS_TYPE_HINFO 13 ; host information
%define DNS_TYPE_MINFO 14 ; mailbox or mail list information
%define DNS_TYPE_MX 15 ; mail exchange
%define DNS_TYPE_TXT 16 ; text strings
%define DNS_QTYPE_AXFR 252 ; zone transfer request
%define DNS_QTYPE_MAILB 253 ; request for mailbox related records (MB, MG, or MR)
%define DNS_QTYPE_MAILA 254 ; request for mail agent resource records (obsolete for MX)
%define DNS_QTYPE_WILDCARD 255 ; request for all records
%define DNS_CLASS_IN 1 ; the internet, the only one we care about
%define DNS_CLASS_CS 2 ; the CSNET class, obsolete
%define DNS_CLASS_CH 3 ; the CHAOS class
%define DNS_CLASS_HS 4 ; Hesiod
%define DNS_QCLASS_WILDCARD 255 ; any class
%define DNS_MESSAGE_OP_STANDARD_QUERY 0
%define DNS_MESSAGE_OP_INVERSE_QUERY 1
%define DNS_MESSAGE_OP_SERVER_STATUS 2
%define DNS_MESSAGE_RESPONSE_CODE_SUCCESS 0
%define DNS_MESSAGE_RESPONSE_CODE_FORMAT_ERROR 1
%define DNS_MESSAGE_RESPONSE_CODE_SERVER_FAILURE 2
%define DNS_MESSAGE_RESPONSE_CODE_NAME_ERROR 3
%define DNS_MESSAGE_RESPONSE_CODE_NOT_IMPLEMENTED 4
%define DNS_MESSAGE_RESPONSE_CODE_REFUSED 5
%define DNS_LOOKUP_RESULT_NO_LOOKUP 0
%define DNS_LOOKUP_RESULT_LOOKUP_PENDING 1
%define DNS_LOOKUP_RESULT_SUCCESS 2
%define DNS_LOOKUP_RESULT_FAILURE 3
; struct DNSMessageHeader {
;	u16 id; // Identifier used to match a query with a response
;	u16 responseCode : 4; // set as part of a query response
;	u16 z : 3; // Reserved for future use, must be 0
;	u16 recursionAvailable : 1; // set in response, recursive query is supported on this server
;	u16 recursionDesired : 1; // asks the nameserver to pursue this query recursively
;	u16 truncation : 1; // this message was truncated due to length too long for transmission channel
;	u16 authoritativeAnswer : 1; // responding nameserver is an authority for the domain name
;	u16 opcode : 4; // what kind of query this is, set by request, copied into response
;	u16 queryOrResponse : 1; // query (0) or response (1)
;	u16 questionEntryCount;
;	u16 answerCount;
;	u16 nameserverCount;
;	u16 additionalRecordCount;
; }
%define DNS_MESSAGE_HEADER_ID_OFFSET 0
%define DNS_MESSAGE_HEADER_FLAGS_OFFSET 2
%define DNS_MESSAGE_HEADER_FLAG_RESPONSE_CODE_SHIFT 0
%define DNS_MESSAGE_HEADER_FLAG_RESPONSE_CODE_MASK 0b1111
%define DNS_MESSAGE_HEADER_FLAG_RECURSION_AVAILABLE (1 << 7)
%define DNS_MESSAGE_HEADER_FLAG_RECURSION_DESIRED (1 << 8)
%define DNS_MESSAGE_HEADER_FLAG_TRUNCATION (1 << 9)
%define DNS_MESSAGE_HEADER_FLAG_AUTHORITATIVE_ANSWER (1 << 10)
%define DNS_MESSAGE_HEADER_FLAG_OPCODE_SHIFT 11
%define DNS_MESSAGE_HEADER_FLAG_OPCODE_MASK 0b1111
%define DNS_MESSAGE_HEADER_FLAG_RESPONSE (1 << 15)
%define DNS_MESSAGE_HEADER_QUESTION_COUNT_OFFSET 4
%define DNS_MESSAGE_HEADER_ANSWER_COUNT_OFFSET 6
%define DNS_MESSAGE_HEADER_NAMESERVER_COUNT_OFFSET 8
%define DNS_MESSAGE_HEADER_ADDITIONAL_RECORD_COUNT_OFFSET 10
%define DNS_MESSAGE_HEADER_SIZE 12

; struct DNSMessageQuestionSection {
;	u8 qname[0]; //
;	u16 qtype;
;	u16 qclass;
; }
; struct DNSMessage

%define DNS_SERVER_PORT 53
%define DNS_CLIENT_PORT 60661 ; Some number I got from a DNS packet, probably some random ephemeral port, but it works for me

%define ARP_LOOKUP_STATE_NO_LOOKUP 0
%define ARP_LOOKUP_STATE_PENDING 1
%define ARP_LOOKUP_STATE_SUCCESS 2
%define ARP_LOOKUP_STATE_FAILURE 3

; struct TCPHeader {
;	u16 srcPort;
;	u16 dstPort;
;	u32 sequenceNum;
;	u32 acknowledgeNum;
;	u8 reserved : 4;
;	u8 dataOffset : 4; // Number of dwords in header, always at least 5
;	u8 flags;
;	u16 window;
;	u16 checksum;
;	u16 urgentPtr;
;	u32 options[0];
; }
%define TCP_HEADER_SRC_PORT_OFFSET 0
%define TCP_HEADER_DST_PORT_OFFSET 2
%define TCP_HEADER_SEQUENCE_NUM_OFFSET 4
%define TCP_HEADER_ACKNOWLEDGE_NUM_OFFSET 8
%define TCP_HEADER_DATA_OFFSET_OFFSET 12
%define TCP_HEADER_FLAGS_OFFSET 13
%define TCP_HEADER_FLAG_CONGESTED_WINDOW_REDUCED (1 << 7)
%define TCP_HEADER_FLAG_ECN_ECHO (1 << 6)
%define TCP_HEADER_FLAG_URGENT (1 << 5)
%define TCP_HEADER_FLAG_ACKNOWLEDGE (1 << 4)
%define TCP_HEADER_FLAG_PUSH (1 << 3)
%define TCP_HEADER_FLAG_RESET (1 << 2)
%define TCP_HEADER_FLAG_SYNCHRONIZE (1 << 1)
%define TCP_HEADER_FLAG_FINALIZE (1 << 0)
%define TCP_HEADER_WINDOW_OFFSET 14
%define TCP_HEADER_CHECKSUM_OFFSET 16
%define TCP_HEADER_URGENT_PTR_OFFSET 18
%define TCP_HEADER_OPTIONS_OFFSET 20
%define TCP_HEADER_MIN_SIZE 20
%define TCP_OPTION_END_OP 0
%define TCP_OPTION_NOP_OP 1
%define TCP_OPTION_MAXIMUM_SEGMENT_SIZE_OP 2
%define TCP_OPTION_MAXIMUM_SEGMENT_SIZE_LEN 4
%define TCP_OPTION_WINDOW_SCALE_OP 3
%define TCP_OPTION_WINDOW_SCALE_LEN 3
%define TCP_OPTION_TIMESTAMP_OP 8
%define TCP_OPTION_TIMESTAMP_LEN 10

%define TCP_STATE_LISTEN 0
%define TCP_STATE_SYN_SENT 1
%define TCP_STATE_SYN_RECEIVED 2
%define TCP_STATE_ESTABLISHED 3
%define TCP_STATE_FIN_WAIT_1 4
%define TCP_STATE_FIN_WAIT_2 5
%define TCP_STATE_CLOSE_WAIT 6
%define TCP_STATE_CLOSING 7
%define TCP_STATE_LAST_ACK 8
%define TCP_STATE_TIME_WAIT 9
%define TCP_STATE_CLOSED 10

%define TCP_SEGMENT_SIZE 1400

; struct ConnectionInfo {
; 	u32 srcIP;
; 	u32 dstIP;
; 	u16 srcPort;
; 	u16 dstPort;
; }

; struct TCPReceivedRange {
;	u32 start;
;	u32 end;
; }
%define TCP_RECEIVED_RANGE_START_OFFSET 0
%define TCP_RECEIVED_RANGE_END_OFFSET 4
%define TCP_RECEIVED_RANGE_SIZE 8
%define TCP_TIME_WAIT_TIMEOUT 5000 ; I know this timeout is supposed to be much longer, but I'd have to change a lot of stuff to make that work

; The transmission control block, one for each connection
; struct TCP_TCB {
; 	u32 freeAndNotifyQueueNextPtr = -1;
; 	u32 sendQueueNextPtr = -1;
; 	
; 	byte* sendBuffer;
; 	u32 sendBufferCapacity;
; 	u32 sendDataPos;
; 	u32 sendDataSize
; 	
; 	u32 sendUnacknowledged;
; 	u32 sendNext; // The latest sequence number we have sent
; 	u32 resendNext; // The sequence number used to send new data (will be lower than sendNext if retransmission occurs)
; 	u32 sendWindow;
; 	// No urgent pointer, this is not relevant to us
; 	u32 sendSequenceNumberForLastWindowUpdate;
; 	u32 sendAcknowledgeNumberForLastWindowUpdate;
; 	u32 sendInitialSequenceNumber;
; 	
; 	byte* receiveBuffer;
; 	u32 receiveBufferCapacity;
; 	u32 receiveDataPos;
; 	u32 receiveDataSize;
; 	
; 	u32 receiveNext;
; 	u32 receiveWindow;
; 	u32 congestionWindow;
; 	u32 slowStartThreshold;
; 	u32 receiveInitialSequenceNumber;
; 	
; 	u32 retransmitTimeoutMilliseconds = 1000;
; 	u32 nextRetransmitTime;
;	u32 roundTripTimer;
;	u32 roundTripTimerSequenceNumber;
; 	u32 smoothedRoundTripTimeMilliseconds = -1;
; 	u32 roundTripTimeVariationMilliseconds;
%define TCP_CLOCK_GRANULARITY_MILLISECONDS 200
%define TCP_INVALID_TIMEOUT_VALUE 0
; 	u32 sendMaximumSegmentSize = 536;
; 	u32 duplicateAckCount;
; 	
%define TCP_TCB_SEGMENT_PROCESS_BUFFER_CAP 8
;	u32 segmentProcessBufferSize;
; 	TCPReceivedRange segmentProcessBuffer[TCP_TCB_SEGMENT_PROCESS_BUFFER_CAP];
; 	
; 	u32 deletionTimeoutTime;
; 	u32 nextCheckTime;
;	u32 timerQueueIdx;
; 	
; 	u32 remoteIP;
; 	u32 localIP;
; 	u16 remotePort
; 	u16 localPort;
; 	u32 state;
%define TCP_TCB_CLOSE_REQUESTED_REQUEST_BIT (1 << 0)
%define TCP_TCB_CLOSE_REQUESTED_FIN_SENT_BIT (1 << 1)
; 	b32 closeRequested;
%define TCP_NOTIFY_HALF_CLOSED 0b1
%define TCP_NOTIFY_TIMED_OUT 0b10
%define TCP_NOTIFY_OPENED 0b100
%define TCP_NOTIFY_DATA 0b1000
%define TCP_NOTIFY_CLOSED 0b10000
%define TCP_NOTIFY_RESET 0b100000
; 	u32 notifyFlags;
;	u32 blockIdx;
; }
%define TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET 0
%define TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET (TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET + 4)

%define TCP_TCB_SEND_BUFFER_OFFSET (TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET + 4)
%define TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET (TCP_TCB_SEND_BUFFER_OFFSET + 8)
%define TCP_TCB_SEND_DATA_POS_OFFSET (TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET + 4)
%define TCP_TCB_SEND_DATA_SIZE_OFFSET (TCP_TCB_SEND_DATA_POS_OFFSET + 4)

%define TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET (TCP_TCB_SEND_DATA_SIZE_OFFSET + 4)
%define TCP_TCB_SEND_NEXT_OFFSET (TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET + 4)
%define TCP_TCB_RESEND_NEXT_OFFSET (TCP_TCB_SEND_NEXT_OFFSET + 4)
%define TCP_TCB_SEND_WINDOW_OFFSET (TCP_TCB_RESEND_NEXT_OFFSET + 4)
%define TCP_TCB_SEND_SEQUENCE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET (TCP_TCB_SEND_WINDOW_OFFSET + 4)
%define TCP_TCB_SEND_ACKNOWLEDGE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET (TCP_TCB_SEND_SEQUENCE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET + 4)
%define TCP_TCB_SEND_INITIAL_SEQUENCE_NUMBER_OFFSET (TCP_TCB_SEND_ACKNOWLEDGE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET + 4)

%define TCP_TCB_RECEIVE_BUFFER_OFFSET (TCP_TCB_SEND_INITIAL_SEQUENCE_NUMBER_OFFSET + 4)
%define TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET (TCP_TCB_RECEIVE_BUFFER_OFFSET + 8)
%define TCP_TCB_RECEIVE_DATA_POS_OFFSET (TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET + 4)
%define TCP_TCB_RECEIVE_DATA_SIZE_OFFSET (TCP_TCB_RECEIVE_DATA_POS_OFFSET + 4)

%define TCP_TCB_RECEIVE_NEXT_OFFSET (TCP_TCB_RECEIVE_DATA_SIZE_OFFSET + 4)
%define TCP_TCB_RECEIVE_WINDOW_OFFSET (TCP_TCB_RECEIVE_NEXT_OFFSET + 4)
%define TCP_TCB_CONGESTION_WINDOW_OFFSET (TCP_TCB_RECEIVE_WINDOW_OFFSET + 4)
%define TCP_TCB_SLOW_START_THRESHOLD_OFFSET (TCP_TCB_CONGESTION_WINDOW_OFFSET + 4)
%define TCP_TCB_RECEIVE_INITIAL_SEQUENCE_NUMBER_OFFSET (TCP_TCB_SLOW_START_THRESHOLD_OFFSET + 4)

%define TCP_TCB_RETRANSMIT_TIMEOUT_MILLISECONDS_OFFSET (TCP_TCB_RECEIVE_INITIAL_SEQUENCE_NUMBER_OFFSET + 4)
%define TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET (TCP_TCB_RETRANSMIT_TIMEOUT_MILLISECONDS_OFFSET + 4)
%define TCP_TCB_ROUND_TRIP_TIMER_OFFSET (TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET + 4)
%define TCP_TCB_ROUND_TRIP_TIMER_SEQUENCE_NUMBER_OFFSET (TCP_TCB_ROUND_TRIP_TIMER_OFFSET + 4)
%define TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET (TCP_TCB_ROUND_TRIP_TIMER_SEQUENCE_NUMBER_OFFSET + 4)
%define TCP_TCB_ROUND_TRIP_TIME_VARIATION_MILLISECONDS_OFFSET (TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET + 4)
%define TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET (TCP_TCB_ROUND_TRIP_TIME_VARIATION_MILLISECONDS_OFFSET + 4)
%define TCP_TCB_DUPLICATE_ACK_COUNT_OFFSET (TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET + 4)

%define TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET (TCP_TCB_DUPLICATE_ACK_COUNT_OFFSET + 4)
%define TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET (TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET + 4)

%define TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET (TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + TCP_RECEIVED_RANGE_SIZE * TCP_TCB_SEGMENT_PROCESS_BUFFER_CAP)
%define TCP_TCB_NEXT_CHECK_TIME_OFFSET (TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET + 4)
%define TCP_TCB_TIMER_QUEUE_IDX_OFFSET (TCP_TCB_NEXT_CHECK_TIME_OFFSET + 4)

%define TCP_TCB_REMOTE_IP_OFFSET (TCP_TCB_TIMER_QUEUE_IDX_OFFSET + 4)
%define TCP_TCB_LOCAL_IP_OFFSET (TCP_TCB_REMOTE_IP_OFFSET + 4)
%define TCP_TCB_REMOTE_PORT_OFFSET (TCP_TCB_LOCAL_IP_OFFSET + 4)
%define TCP_TCB_LOCAL_PORT_OFFSET (TCP_TCB_REMOTE_PORT_OFFSET + 2)
%define TCP_TCB_CONNECTION_INFO_IPS_OFFSET TCP_TCB_REMOTE_IP_OFFSET
%define TCP_TCB_CONNECTION_INFO_PORTS_OFFSET TCP_TCB_REMOTE_PORT_OFFSET
%define TCP_TCB_STATE_OFFSET (TCP_TCB_LOCAL_PORT_OFFSET + 2)
%define TCP_TCB_CLOSE_REQUESTED_OFFSET (TCP_TCB_STATE_OFFSET + 4)
%define TCP_TCB_NOTIFY_FLAGS_OFFSET (TCP_TCB_CLOSE_REQUESTED_OFFSET + 4)
%define TCP_TCB_BLOCK_IDX_OFFSET (TCP_TCB_NOTIFY_FLAGS_OFFSET + 4)
%define TCP_TCB_SIZE (TCP_TCB_BLOCK_IDX_OFFSET + 4) ; Currently 216 (subject to change)


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
%define PCI_BARS_OFFSET 0x10



; Global variables
%define VARIABLES_START 0xE800
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
%define VIRTIO_NET_RX_QUEUE_NOTIFY_ADDR (VARIABLES_START + 60)
%define VIRTIO_NET_TX_QUEUE_NOTIFY_ADDR (VARIABLES_START + 64)
%define VIRTIO_NET_QUEUE_SIZE (VARIABLES_START + 68)
%define VIRTIO_NET_PCI_ADDRESS (VARIABLES_START + 72)
%define VIRTIO_NET_CONFIG_ADDRESS (VARIABLES_START + 80)
%define VIRTIO_NET_MAC (VARIABLES_START + 88)
%define VIRTIO_RECEIVED_PACKET_COUNT (VARIABLES_START + 96)
%define VIRTIO_SENT_PACKET_COUNT (VARIABLES_START + 104)
%define IPV4_NEXT_IDENTIFICATION (VARIABLES_START + 112)
%define VIRTIO_NET_IP_ADDRESS (VARIABLES_START + 116)
%define DHCP_CURRENT_STATE (VARIABLES_START + 120)
%define DHCP_CURRENT_XID (VARIABLES_START + 124)
%define DHCP_LEASE_START (VARIABLES_START + 128)
%define DHCP_LEASE_TIME (VARIABLES_START + 136)
%define DHCP_SERVER_IDENTIFIER (VARIABLES_START + 144)
%define DHCP_OFFERED_IP (VARIABLES_START + 148)
%define DHCP_GATEWAY_MAC (VARIABLES_START + 152)
%define BIOS_MEMORY_MAP_ENTRY_COUNT (VARIABLES_START + 160)
%define PHYSICAL_MEMORY_ALLOC_POSITION (VARIABLES_START + 164)
%define PHYSICAL_MEMORY_ALLOC_SIZE (VARIABLES_START + 168)
%define DNS_LOOKUP_NAME_PTR (VARIABLES_START + 172)
%define DNS_LOOKUP_MESSAGE_ID (VARIABLES_START + 180)
%define DNS_LOOKUP_RESULT_CODE (VARIABLES_START + 184)
%define DNS_LOOKUP_RESULT_IP (VARIABLES_START + 188)
%define DNS_LOOKUP_RESULT_TTL_MILLISECONDS (VARIABLES_START + 192)
%define TCP_FREE_LIST_HEAD_IDX (VARIABLES_START + 200);
%define TCP_SEND_QUEUE_HEAD (VARIABLES_START + 204)
%define TCP_SEND_QUEUE_TAIL (VARIABLES_START + 208)
%define TCP_NOTIFY_QUEUE_HEAD (VARIABLES_START + 212)
%define TCP_LAST_COARSE_CLOCK_TIME (VARIABLES_START + 216)
%define TCP_IS_INITIALIZED (VARIABLES_START + 220)
%define VIRTIO_NET_NEGOTIATED_FEATURES (VARIABLES_START + 224)
%define DHCP_GATEWAY_IP (VARIABLES_START + 228)
%define ARP_LOOKUP_CURRENT_STATE (VARIABLES_START + 232)
%define ARP_LOOKUP_TARGET_IP (VARIABLES_START + 236)
%define ARP_LOOKUP_RESULT_MAC (VARIABLES_START + 240)
%define GLOBAL_VARIABLES_END (TCP_IS_INITIALIZED + 248)

; Global memory offsets
%define BIOS_MEMORY_MAP_OFFSET 0xF000 ; Stores the memory map obtained by the BIOS, ranges from 0xF000 to 0xFFFF
%define BIOS_MEMORY_MAP_MAX_SIZE 0x1000
%define FIRST_MEMORY_ALLOCATION_BITMAP_TABLES 0x10000 ; Stores PML1-3 for the first bitmap
%define FIRST_MEMORY_ALLOCATION_BITMAP_PAGE 0x16000 ; Two pages for the first part of the bitmap (one for user, one for supervisor) so we can start allocating
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
%define VIRTIO_NET_CONFIG_OFFSET (DISK_PORT_QUEUES_OFFSET - 2 * 4096) ; Need two pages, in case the configuration space is on a page boundary
%define VIRTIO_NET_QUEUE_NOTIFY_PAGES (VIRTIO_NET_CONFIG_OFFSET - 2 * 4096) ; Yes that's right, two whole pages for 4 bytes of data, because that's the worst case.
%define VIRTIO_NET_RX_QUEUE_OFFSET (VIRTIO_NET_QUEUE_NOTIFY_PAGES - 4096)
%define VIRTIO_NET_TX_QUEUE_OFFSET (VIRTIO_NET_RX_QUEUE_OFFSET - 4096)
%define VIRTIO_NET_PACKET_BUFFERS_REQUIRED_PAGES ((VIRTIO_NET_PACKET_BUFFER_SIZE * (VIRTIO_DESC_COUNT_MAX / 2) + 4095) / 4096)
%define VIRTIO_NET_RX_PACKETS_OFFSET (VIRTIO_NET_TX_QUEUE_OFFSET - VIRTIO_NET_PACKET_BUFFERS_REQUIRED_PAGES * 4096)
%define VIRTIO_NET_TX_PACKETS_OFFSET (VIRTIO_NET_RX_PACKETS_OFFSET - VIRTIO_NET_PACKET_BUFFERS_REQUIRED_PAGES * 4096)
%define TCP_MAX_CONNECTIONS 128
%define TCP_BLOCK_SIZE (1024 * 512) ; Half a megabyte, the first part is the TCB, the rest makes up the send/receive buffers
%define TCP_CONNECTIONS_OFFSET 0x8000000000 ; First bit set in the PML4
%define TCP_CONNECTION_HASH_TABLE_BYTE_COUNT 0x1000
%define TCP_CONNECTION_HASH_TABLE_OFFSET (TCP_CONNECTIONS_OFFSET + (((TCP_MAX_CONNECTIONS * TCP_BLOCK_SIZE) + 0xFFF) & ~0xFFF)) ; Contains 2048 u16 TCP block indices
%define TCP_CONNECTION_HASH_TABLE_MASK 0x7FF
%define TCP_CONNECTION_HASH_EMPTY 0xFFFF
%define TCP_CONNECTION_HASH_INVALID_IDX 0xFFFF
%define TCP_CONNECTION_BLOCK_INVALID_IDX 0xFFFF
%define TCP_NOTIFY_QUEUE_INVALID_NEXT_PTR (TCP_CONNECTION_BLOCK_INVALID_IDX - 1)
%define TCP_SEND_QUEUE_INVALID_NEXT_PTR (TCP_CONNECTION_BLOCK_INVALID_IDX - 1)
%define TCP_TIMEOUT_QUEUE_BYTE_SIZE (TCP_MAX_CONNECTIONS * 2)
%define TCP_TIMEOUT_QUEUE_OFFSET (TCP_CONNECTION_HASH_TABLE_OFFSET + TCP_CONNECTION_HASH_TABLE_BYTE_COUNT)

%define BOOT_STACK 0x800
%define INTERRUPT_STACK 0x400

%macro DEBUG_PRINT_NUM 1
	push rax
	mov eax, %1
	call debugPrintlnInteger
	pop rax
%endmacro

%macro DEBUG_PRINT_STR 1
	push rax
	lea rax, [%1]
	call debugPrint
	pop rax
%endmacro

%macro DEBUG_SEND_NUM 1
	push rax
	mov eax, %1
	call sendUDPInteger
	pop rax
%endmacro

%macro DEBUG_SEND_NUM64 1
	push rax
	mov rax, %1
	call sendUDPInteger
	pop rax
%endmacro

%macro DEBUG_SEND_STR 1
	push rax
	mov eax, %1
	call sendUDPStr
	pop rax
%endmacro

org 0x7C00
bits 16
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
noLongMode:
initFailed:
	hlt
	jmp initFailed

hasLongMode:

	; Zero global variables
	mov di, VARIABLES_START
	mov ecx, (GLOBAL_VARIABLES_END - VARIABLES_START + 3) / 4
	xor eax, eax
	rep stosd

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

	call getBIOSMemoryMap

	; Identity mapping for first few pages before we go to long mode
	; Zero out virtual memory buffers. 4kb per buffer, 4 level addressing
	mov di, PAGE_TABLE_START
	mov ecx, 0x4000/4
	xor eax, eax
	rep stosd

	; The top level entry has a self reference so I can access tables without needing them mapped somewhere
	mov dword [PAGE_TABLE_START + PML_SELF_REFERENCE * 8], PAGE_TABLE_START | PAGE_PRESENT | PAGE_WRITE
	; Map first two megabytes, completely filling last table
	mov dword [PAGE_TABLE_START], (PAGE_TABLE_START + 1000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [PAGE_TABLE_START + 1000h], (PAGE_TABLE_START + 2000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [PAGE_TABLE_START + 2000h], (PAGE_TABLE_START + 3000h) | PAGE_PRESENT | PAGE_WRITE
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

bits 16
; This function was too large for the boot sector, so I moved it out here. Pretend it's in main.
getBIOSMemoryMap:
	; Get memory map from BIOS
	mov di, BIOS_MEMORY_MAP_OFFSET ; Offset to store memory ranges
	xor ebx, ebx ; Must be preserved accross each BIOS call
	mov es, bx ; Make sure extended segment is zero
	mov edx, 0x534D4150 ; "SMAP"
	mov eax, 0xE820 ; BIOS function call for memory detection
	mov ecx, 24 ; Size of result buffer
	int 0x15
	jc initFailed ; First call returned failed? Can't boot.
	mov dword [BIOS_MEMORY_MAP_ENTRY_COUNT], 0
	cmp ecx, 20 ; Make sure we got at least 20 bytes
	jb .biosMemMapLoop
	mov eax, [di + MEMENTRY_LENGTH_OFFSET]
	or eax, [di + MEMENTRY_LENGTH_OFFSET + 4]
	test eax, eax
	jz .biosMemMapLoop ; length was zero, ignore entry
	add di, 24
	add dword [BIOS_MEMORY_MAP_ENTRY_COUNT], 1
.biosMemMapLoop:
	mov edx, 0x534D4150 ; "SMAP"
	mov eax, 0xE820 ; BIOS function call for memory detection
	mov ecx, 24 ; Size of result buffer
	int 0x15
	jc .biosMemMapDone ; End of list
	cmp ecx, 20 ; Make sure we got at least 20 bytes
	jb .biosMemMapEntryInvalid
	mov eax, [di + MEMENTRY_LENGTH_OFFSET]
	or eax, [di + MEMENTRY_LENGTH_OFFSET + 4]
	test eax, eax
	jz .biosMemMapEntryInvalid ; length was zero, ignore entry
	add di, 24
	add dword [BIOS_MEMORY_MAP_ENTRY_COUNT], 1
.biosMemMapEntryInvalid:
	test ebx, ebx
	jz .biosMemMapDone ; Got all entries
	cmp di, BIOS_MEMORY_MAP_OFFSET + BIOS_MEMORY_MAP_MAX_SIZE - 24
	jbe .biosMemMapLoop
.biosMemMapDone:
	cmp dword [BIOS_MEMORY_MAP_ENTRY_COUNT], 0
	je initFailed
	ret
bits 64

longModeLoader:
	mov dword [DEBUG_TEXT_OFFSET_LOCATION], 80

	; Set up task state segment for interrupt stack switching
	; Zero TSS memory
	mov edi, TSS_START
	mov ecx, TSS_SIZE / 4
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
	; Add VirtIO network handler
	lea rcx, [interruptVirtIONetwork]
	or rcx, rdx
	mov qword [IDT_START + AHCI_VIRTIO_NET_VECTOR * 16], rcx

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
	
	; DO got here

	call setupMemory
	jc .startupFailed

	mov dword [PCI_AHCI_CONTROLLER_ADDRESS], 0
	mov dword [VIRTIO_NET_PCI_ADDRESS], 0
	lea rax, [scanPCIStartStr]
	call debugPrint
	call scanPCIBus
	lea rax, [scanPCIDoneStr]
	call debugPrint
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
	; DO got here
	call initializeVirtIONet
	jc .startupFailed
.noVirtIONet:

	; DO got here

	call allocTCPBlocks
	jc .startupFailed
	
	; DO got here

	; print("Startup done, took " + timeMillis + " milliseconds.")
	lea rax, [startupDoneTookStr]
	call debugPrint
	mov rax, [PIT_CLOCK_MILLISECONDS]
	call debugPrintInteger
	lea rax, [millisecondsStr]
	call debugPrint

	

	;call testAHCIReadWrite

	;call sendTestARPPacket
	;call sendTestARPPacket
	;call sendTestARPPacket
	;call sendTestARPPacket
	;call sendTestARPPacket
	;call sendTestARPPacket
	;call sendTestARPPacket
	;call sendTestARPPacket
	; DO got here
	call startDHCP

	;call sendDNSTestPacket
	lea rax, [googleDotCom]
	call dnsLookupBlocking
	
	DEBUG_SEND_STR serverStartupStr
	DEBUG_SEND_NUM [DHCP_LEASE_TIME]

	call tcpServerLoop

	jmp .halt
.startupFailed:
	lea rax, [startupFailedStr]
	call debugPrint
.halt:
	hlt
	jmp .halt

systemIdle:
.halt:
	; DO got here
	hlt
	jmp .halt
	
doWork:
	.halt:
	jmp .halt

systemRestartOrShutdown:
	xor eax, eax
	mov cr3, rax
	ret ; Should immediately cause a triple fault

; carry flag is 1 if failed, 0 if success
setupMemory:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi

	mov edx, [BIOS_MEMORY_MAP_ENTRY_COUNT]
	imul edx, edx, MEMENTRY_SIZE

	cmp edx, MEMENTRY_SIZE
	je .insertionSortEnd ; Skip sort if only one entry (should never be true in practice)

	; Sort entries by base address. Just use insertion sort, no need to get fancy here.
	mov ebx, MEMENTRY_SIZE
.insertionSort:
	mov ecx, ebx
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ebx + MEMENTRY_BASE_ADDRESS_OFFSET]
.insertionSortFindInsertionIndex:
	cmp rax, [BIOS_MEMORY_MAP_OFFSET + ecx - MEMENTRY_SIZE + MEMENTRY_BASE_ADDRESS_OFFSET]
	jge .insertionSortInnerLoopBreak
	sub ecx, MEMENTRY_SIZE
	test ecx, ecx
	jnz .insertionSortFindInsertionIndex
.insertionSortInnerLoopBreak:
	; Swap entries at ebx and ecx
	movups xmm0, [BIOS_MEMORY_MAP_OFFSET + ebx]
	movups xmm1, [BIOS_MEMORY_MAP_OFFSET + ebx + 8]
	movups xmm2, [BIOS_MEMORY_MAP_OFFSET + ecx]
	movups xmm3, [BIOS_MEMORY_MAP_OFFSET + ecx + 8]
	movups [BIOS_MEMORY_MAP_OFFSET + ebx], xmm2
	movups [BIOS_MEMORY_MAP_OFFSET + ebx + 8], xmm3
	movups [BIOS_MEMORY_MAP_OFFSET + ecx], xmm0
	movups [BIOS_MEMORY_MAP_OFFSET + ecx + 8], xmm1
	add ebx, MEMENTRY_SIZE
	cmp ebx, edx
	jne .insertionSort
.insertionSortEnd:

	; Make each entry either available or not and combine overlapping. Also transition from a start/size format to start/end format
	sub edx, MEMENTRY_SIZE ; Loop up to count - 1
	; Make sure the last entry is still processed
	mov eax, MEMENTRY_TYPE_AVAILABLE
	mov ebx, MEMENTRY_TYPE_UNAVAILABLE
	cmp dword [BIOS_MEMORY_MAP_OFFSET + edx + MEMENTRY_TYPE_OFFSET], MEMENTRY_TYPE_AVAILABLE
	cmovne eax, ebx
	mov [BIOS_MEMORY_MAP_OFFSET + edx + MEMENTRY_TYPE_OFFSET], eax
	mov rax, [BIOS_MEMORY_MAP_OFFSET + edx + MEMENTRY_BASE_ADDRESS_OFFSET]
	add [BIOS_MEMORY_MAP_OFFSET + edx + MEMENTRY_LENGTH_OFFSET], rax
	test edx, edx
	jz .memoryInfoCleanEnd ; Skip if only one entry
	xor ecx, ecx
.memoryInfoCleanLoop:
	; Set to either available or not
	mov eax, MEMENTRY_TYPE_AVAILABLE
	mov ebx, MEMENTRY_TYPE_UNAVAILABLE
	cmp dword [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_TYPE_OFFSET], MEMENTRY_TYPE_AVAILABLE
	cmovne eax, ebx
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_TYPE_OFFSET], eax
	; Make length field end
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_BASE_ADDRESS_OFFSET]
	add [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_LENGTH_OFFSET], rax
	; Combine overlapping
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET]
	mov rbx, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_BASE_ADDRESS_OFFSET]
	cmp rax, rbx
	jle .notOverlapping
	mov ebx, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_TYPE_OFFSET]
	cmp ebx, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_TYPE_OFFSET]
	jne .memoryTypesDifferent
	; Memory types same
	; b.end = max(a.end, b.end)
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET]
	mov rbx, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_END_OFFSET]
	cmp rbx, rax
	cmovg rax, rbx
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_END_OFFSET], rax
	; a.end = b.start
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_BASE_ADDRESS_OFFSET]
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET], rax
	jmp .notOverlapping
.memoryTypesDifferent:
	cmp dword [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_TYPE_OFFSET], MEMENTRY_TYPE_AVAILABLE
	jne .firstTypeNotAvailable
	; a.end = b.start
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_BASE_ADDRESS_OFFSET]
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET], rax
	jmp .notOverlapping
.firstTypeNotAvailable:
	; Not great, but it's conservative and doesn't require making new sub ranges. This is a very unlikely case anyway.
	; b.end = a.end
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET]
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_END_OFFSET], rax
	; b.type = not available
	mov dword [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_TYPE_OFFSET], MEMENTRY_TYPE_UNAVAILABLE
	; a.end = b.start
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_SIZE + MEMENTRY_BASE_ADDRESS_OFFSET]
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET], rax
.notOverlapping:
	add ecx, MEMENTRY_SIZE
	cmp ecx, edx
	jne .memoryInfoCleanLoop
.memoryInfoCleanEnd:
	add edx, MEMENTRY_SIZE ; Get back the actual size instead of size - 1

	; Clip each range to page boundaries
	xor ecx, ecx
.pageClipLoop:
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_BASE_ADDRESS_OFFSET]
	add rax, 0xFFF
	and rax, ~0xFFF
	mov rbx, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET]
	and rbx, ~0xFFF
	; If base is greater than end now, this entry has a size of 0
	xor edi, edi
	cmp rax, rbx
	cmovae rax, rdi
	cmovae rbx, rdi
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_BASE_ADDRESS_OFFSET], rax
	mov [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET], rbx
	add ecx, MEMENTRY_SIZE
	cmp ecx, edx
	jne .pageClipLoop

	; Zero out first bitmap page mapping
	mov edi, FIRST_MEMORY_ALLOCATION_BITMAP_TABLES
	mov ecx, 8000h/4 ; 3 pages for PML1-3, 1 page for the bitmap itself, times two for user/supervisor
	xor eax, eax
	rep stosd
	; Map in supervisor bitmap first page
	mov dword [PAGE_TABLE_START + ((MEMORY_ALLOCATION_BITMAP_SUPERVISOR_ADDRESS >> PML4_BIT_OFFSET) & 0b111111111) * 8], (FIRST_MEMORY_ALLOCATION_BITMAP_TABLES) | PAGE_PRESENT | PAGE_WRITE
	mov dword [FIRST_MEMORY_ALLOCATION_BITMAP_TABLES], (FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 1000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 1000h], (FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 2000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 2000h], (FIRST_MEMORY_ALLOCATION_BITMAP_PAGE) | PAGE_PRESENT | PAGE_WRITE
	mov rax, MEMORY_ALLOCATION_BITMAP_SUPERVISOR_ADDRESS
	invlpg [rax]
	; Map in user bitmap first page
	mov dword [PAGE_TABLE_START + ((MEMORY_ALLOCATION_BITMAP_USER_ADDRESS >> PML4_BIT_OFFSET) & 0b111111111) * 8], (FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 3000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 3000h], (FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 4000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 4000h], (FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 5000h) | PAGE_PRESENT | PAGE_WRITE
	mov dword [FIRST_MEMORY_ALLOCATION_BITMAP_TABLES + 5000h], (FIRST_MEMORY_ALLOCATION_BITMAP_PAGE + 1000h) | PAGE_PRESENT | PAGE_WRITE
	mov rax, MEMORY_ALLOCATION_BITMAP_USER_ADDRESS
	invlpg [rax]
	mov dword [PHYSICAL_MEMORY_ALLOC_SIZE], 1000h

	; Fill in bitmap from BIOS memory mapping
	; At this point the entries are guaranteed to be in order, non overlapping, and page aligned
	xor ecx, ecx
.buildMemoryBitmap:
	cmp dword [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_TYPE_OFFSET], MEMENTRY_TYPE_UNAVAILABLE
	je .buildMemoryBitmapContinue ; Unavailable memory
	mov rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_BASE_ADDRESS_OFFSET]
	cmp rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET]
	je .buildMemoryBitmapContinue ; Zero length entry
.mapBitsLoop:
	mov ebx, [PHYSICAL_MEMORY_ALLOC_SIZE]
	imul rbx, rbx, BYTES_IN_MEMORY_ALLOCATION_BITMAP_BYTE
	cmp rax, rbx
	jb .hasSpaceForBitmap
.ensureSpaceForBitmapLoop:
	xor esi, esi
	mov ebx, [PHYSICAL_MEMORY_ALLOC_SIZE]
	push rax
	mov rax, MEMORY_ALLOCATION_BITMAP_USER_ADDRESS
	add rax, rbx
	call mapPhysicalToVirtual
	pop rax
	jc .outOfMemory
	push rax
	mov rax, MEMORY_ALLOCATION_BITMAP_SUPERVISOR_ADDRESS
	add rax, rbx
	call mapPhysicalToVirtual
	pop rax
	jc .outOfMemory
	add ebx, 0x1000
	mov [PHYSICAL_MEMORY_ALLOC_SIZE], ebx
	imul rbx, rbx, BYTES_IN_MEMORY_ALLOCATION_BITMAP_BYTE
	cmp rax, rbx
	jae .ensureSpaceForBitmapLoop
.hasSpaceForBitmap:
	cmp rax, 1024 * 1024 * 2
	jb .mapBitsContinue ; First two megabytes are reserved for static allocations
	mov rsi, rax
	shr rsi, 12 + 6 ; Bytes to pages, pages to collections of 64 pages
	mov rbx, rax
	shr rbx, 12 ; Bytes to pages
	and ebx, 0b111111
	mov rdi, MEMORY_ALLOCATION_BITMAP_USER_ADDRESS
	bts [rdi + rsi * 8], rbx
	mov rdi, MEMORY_ALLOCATION_BITMAP_SUPERVISOR_ADDRESS
	bts [rdi + rsi * 8], rbx
.mapBitsContinue:
	add rax, 0x1000
	cmp rax, [BIOS_MEMORY_MAP_OFFSET + ecx + MEMENTRY_END_OFFSET]
	jne .mapBitsLoop
.buildMemoryBitmapContinue:
	add ecx, MEMENTRY_SIZE
	cmp ecx, edx
	jne .buildMemoryBitmap

	clc
	jmp .end
.outOfMemory:
	lea rax, [memoryAllocForPageMapFailed]
	call debugPrint
	stc
.end:
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

memoryAllocForPageMapFailed db "Memory Alloc For Page Map Failed!",0x0a,0x00

; rsi contains 0 if supervisor allocation, anything else if user allocation
; rax gets physical address, 0 if out of memory
allocPhysicalPage:
	push rbx
	push rcx
	push rdx

	mov ecx, [PHYSICAL_MEMORY_ALLOC_SIZE]
	mov ebx, [PHYSICAL_MEMORY_ALLOC_POSITION]
	mov rax, MEMORY_ALLOCATION_BITMAP_USER_ADDRESS
	; Read until we have a valid physical address or we hit the end
.findPageLoop:
	cmp ebx, ecx
	je .outOfMemory
	mov rdx, [rax + rbx]
	test rdx, rdx
	jnz .pageFound
	add ebx, 8
	jmp .findPageLoop
.pageFound:
	mov [PHYSICAL_MEMORY_ALLOC_POSITION], ebx
	tzcnt rcx, rdx
	btr rdx, rcx ; Mark as used
	mov [rax + rbx], rdx
	test rsi, rsi
	jnz .notSupervisorAllocation
	mov rax, MEMORY_ALLOCATION_BITMAP_SUPERVISOR_ADDRESS
	btr [rax + rbx], rcx ; Mark as allocated in supervisor bitmap as well
.notSupervisorAllocation:
	lea rax, [rbx * 8 + rcx] ; 8 bits per byte in rbx index, plus the bit index of the page we allocated
	shl rax, 12 ; Page index to byte address
	jmp .end

.outOfMemory:
	xor eax, eax
.end:
	pop rdx
	pop rcx
	pop rbx
	ret

; rsi contains 0 if supervisor mapping, anything else if user mapping
; rax contains desired virtual address to have physical memory at
; carry flag cleared if success, set if fail
mapPhysicalToVirtual:
	push rax
	push rbx
	push rcx
	push rdx
	push rdi

	mov rbx, rax
	and rbx, ~0xFFF ; Align to page

	mov rdi, PAGE_PRESENT | PAGE_WRITE
	test rsi, rsi
	jz .supervisorMapping
	or rdi, PAGE_USER_SUPERVISOR
.supervisorMapping:

	; Check if the PML3 is present, otherwise make a new one
	mov rdx, rbx
	shr rdx, PML4_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) | (PML_SELF_REFERENCE << PML3_BIT_OFFSET) | (PML_SELF_REFERENCE << PML2_BIT_OFFSET) | (PML_SELF_REFERENCE << PML1_BIT_OFFSET) ; Self reference 4 times to get PML4
	or rdx, rcx
	mov rcx, [rdx] ; Get PML3 entry from PML4
	test ecx, PAGE_PRESENT
	jnz .pml3Present
	call allocPhysicalPage
	test rax, rax
	jz .outOfMemory
	mov rcx, rax
.pml3Present:
	test rsi, rsi ; If we're the supervisor, ignore the privilege check
	jz .pml3UserModeCheckDone
	; If the page was already present and does not have the user bit set, that's an error
	test rcx, PAGE_PRESENT
	jz .pml3UserModeCheckDone
	test rcx, PAGE_USER_SUPERVISOR
	jz .userTriedToMapSupervisorAddressSpace
.pml3UserModeCheckDone:
	or rcx, rdi
	mov [rdx], rcx

	; Check if the PML2 is present, otherwise make a new one
	mov rdx, rbx
	shr rdx, PML3_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) | (PML_SELF_REFERENCE << PML3_BIT_OFFSET) | (PML_SELF_REFERENCE << PML2_BIT_OFFSET) ; Self reference 3 times to get PML3
	or rdx, rcx
	mov rcx, [rdx] ; Get PML2 entry from PML3
	test ecx, PAGE_PRESENT
	jnz .pml2Present
	call allocPhysicalPage
	test rax, rax
	jz .outOfMemory
	mov rcx, rax
.pml2Present:
	test rsi, rsi ; If we're the supervisor, ignore the privilege check
	jz .pml2UserModeCheckDone
	; If the page was already present and does not have the user bit set, that's an error
	test rcx, PAGE_PRESENT
	jz .pml2UserModeCheckDone
	test rcx, PAGE_USER_SUPERVISOR
	jz .userTriedToMapSupervisorAddressSpace
.pml2UserModeCheckDone:
	or rcx, rdi
	mov [rdx], rcx

	; Check if the PML1 is present, otherwise make a new one
	mov rdx, rbx
	shr rdx, PML2_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) | (PML_SELF_REFERENCE << PML3_BIT_OFFSET) ; Self reference 2 times to get PML2
	or rdx, rcx
	mov rcx, [rdx] ; Get PML1 entry from PML2
	test ecx, PAGE_PRESENT
	jnz .pml1Present
	call allocPhysicalPage
	test rax, rax
	jz .outOfMemory
	mov rcx, rax
.pml1Present:
	test rsi, rsi ; If we're the supervisor, ignore the privilege check
	jz .pml1UserModeCheckDone
	; If the page was already present and does not have the user bit set, that's an error
	test rcx, PAGE_PRESENT
	jz .pml1UserModeCheckDone
	test rcx, PAGE_USER_SUPERVISOR
	jz .userTriedToMapSupervisorAddressSpace
.pml1UserModeCheckDone:
	or rcx, rdi
	mov [rdx], rcx

	; Check if the page is present, otherwise make a new one
	mov rdx, rbx
	shr rdx, PML1_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) ; Self reference 1 times to get PML1
	or rdx, rcx
	mov rcx, [rdx] ; Get page entry from PML1
	test ecx, PAGE_PRESENT
	jnz .pagePresent
	call allocPhysicalPage
	test rax, rax
	jz .outOfMemory
	mov rcx, rax
.pagePresent:
	test rsi, rsi ; If we're the supervisor, ignore the privilege check
	jz .pageUserModeCheckDone
	; If the page was already present and does not have the user bit set, that's an error
	test rcx, PAGE_PRESENT
	jz .pageUserModeCheckDone
	test rcx, PAGE_USER_SUPERVISOR
	jz .userTriedToMapSupervisorAddressSpace
.pageUserModeCheckDone:
	or rcx, rdi
	mov [rdx], rcx

	invlpg [rbx]

	; Zero out page memory
	mov rdi, rbx
	mov ecx, 0x1000 / 4
	xor eax, eax
	rep stosd

	clc
	jmp .end
.outOfMemory:
.userTriedToMapSupervisorAddressSpace:
	stc
.end:
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; Does virtual to physical memory lookup manually (I can't believe there isn't a built in instruction for this)
; rax contains virtual address
; rbx gets physical page or'd with page present and user flags
tlbAddressLookup:
	push rcx
	push rdx

	mov ebx, PAGE_PRESENT | PAGE_USER_SUPERVISOR

	mov rdx, rax
	shr rdx, PML4_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) | (PML_SELF_REFERENCE << PML3_BIT_OFFSET) | (PML_SELF_REFERENCE << PML2_BIT_OFFSET) | (PML_SELF_REFERENCE << PML1_BIT_OFFSET) ; Self reference 4 times to get PML4
	or rdx, rcx
	mov rcx, [rdx] ; Get PML3 entry from PML4
	test ecx, PAGE_PRESENT
	jz .pageNotPresent
	and ebx, ecx

	mov rdx, rax
	shr rdx, PML3_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) | (PML_SELF_REFERENCE << PML3_BIT_OFFSET) | (PML_SELF_REFERENCE << PML2_BIT_OFFSET) ; Self reference 3 times to get PML3
	or rdx, rcx
	mov rcx, [rdx] ; Get PML2 entry from PML3
	test ecx, PAGE_PRESENT
	jz .pageNotPresent
	and ebx, ecx

	mov rdx, rax
	shr rdx, PML2_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) | (PML_SELF_REFERENCE << PML3_BIT_OFFSET) ; Self reference 2 times to get PML2
	or rdx, rcx
	mov rcx, [rdx] ; Get PML1 entry from PML2
	test ecx, PAGE_PRESENT
	jz .pageNotPresent
	and ebx, ecx

	mov rdx, rax
	shr rdx, PML1_BIT_OFFSET - 3 ; Minus 3 because we want to use 9 bits to index qwords
	and rdx, ~0b111 ; 9 bit lookup
	mov rcx, FOUR_LEVEL_PAGING_HIGH_BITS | (PML_SELF_REFERENCE << PML4_BIT_OFFSET) ; Self reference 1 time to get PML1
	or rdx, rcx
	mov rcx, [rdx] ; Get page entry from PML1
	test ecx, PAGE_PRESENT
	jz .pageNotPresent
	and ebx, ecx

	and rcx, ~0xFFF
	or rbx, rcx

	jmp .lookupDone
.pageNotPresent:
	xor ebx, ebx
.lookupDone:

	pop rdx
	pop rcx
	ret

; Ensures there is length bytes of user accessible memory mapped to address
; rcx contains address
; edx contains length
; carry clear if success, set if failure
tlbVerifyUserBuffer:
	push rax
	push rbx

	cmp edx, 0
	jle .success

	mov rax, rcx
.verifyLoop:
	call tlbAddressLookup
	and ebx, PAGE_PRESENT | PAGE_USER_SUPERVISOR
	cmp ebx, PAGE_PRESENT | PAGE_USER_SUPERVISOR
	jne .fail
	add rax, 0x1000
	sub edx, 0x1000
	cmp edx, 0
	jnle .verifyLoop

.success:
	clc
	jmp .end
.fail:
	stc
.end:
	pop rbx
	pop rax
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

	cmp dword [TCP_IS_INITIALIZED], 0
	je .notTimeToDoTCPCoarseClock
	mov eax, [PIT_CLOCK_MILLISECONDS]
	sub eax, [TCP_LAST_COARSE_CLOCK_TIME]
	sub eax, TCP_CLOCK_GRANULARITY_MILLISECONDS
	cmp eax, 0
	jng .notTimeToDoTCPCoarseClock
	mov eax, [PIT_CLOCK_MILLISECONDS]
	mov [TCP_LAST_COARSE_CLOCK_TIME], eax
	call tcpCoarseClock
.notTimeToDoTCPCoarseClock:


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
	cmp bl, 1
	jne .keyNotEsc
	call systemRestartOrShutdown
.keyNotEsc:
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

	lea rax, [ahciInterrupt]
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

virtIOServiceTX:
	push rax
	push rbx
	push rcx
	push rdx

	; See if transmit queue needs servicing
	movzx eax, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_USED_OFFSET + VIRTQ_USED_IDX_OFFSET]
	movzx ebx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_LAST_SEEN_USED_OFFSET]
	cmp ax, bx
	je .noTxServiceNeeded
.keepServicingTx:
	mov ecx, ebx
	movzx edx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_COUNT_OFFSET]
	sub edx, 1 ; Queue size mask
	and ecx, edx
	; Get id of descriptor head
	mov ecx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_USED_OFFSET + VIRTQ_USED_RING_OFFSET + ecx * VIRTQ_USED_ELEM_SIZE + VIRTQ_USED_ELEM_ID_OFFSET]
	; Add this pair of descriptors to the free list
	mov dx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_FREE_DESCRIPTOR_CHAIN_HEAD_OFFSET] ; Save old free list head
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_FREE_DESCRIPTOR_CHAIN_HEAD_OFFSET], cx ; This is the new head
	; Get its second part
	imul ecx, ecx, VIRTQ_DESC_SIZE
	movzx ecx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_NEXT_OFFSET]
	imul ecx, ecx, VIRTQ_DESC_SIZE
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_NEXT_OFFSET], dx ; Chain the old free list head onto the end of this
	add word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET], 2 ; Two more free
	; See if there are more to free
	add bx, 1
	cmp ax, bx
	jne .keepServicingTx
.noTxServiceNeeded:
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_LAST_SEEN_USED_OFFSET], ax

	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

interruptVirtIONetwork:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi

	call virtIOServiceTX

	; See if receive queue needs servicing
	movzx eax, word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_USED_OFFSET + VIRTQ_USED_IDX_OFFSET]
	movzx ebx, word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_LAST_SEEN_USED_OFFSET]
	cmp ax, bx
	je .noRxServiceNeeded
	mov si, [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_IDX_OFFSET]
.keepServicingRx:
	mov ecx, ebx
	movzx edx, word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_COUNT_OFFSET]
	sub edx, 1 ; Queue size mask
	and ecx, edx
	; Get id of descriptor head
	mov ecx, [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_USED_OFFSET + VIRTQ_USED_RING_OFFSET + ecx * VIRTQ_USED_ELEM_SIZE + VIRTQ_USED_ELEM_ID_OFFSET]
	push rax
	imul eax, ecx, VIRTQ_DESC_SIZE
	movzx eax, word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_NEXT_OFFSET]
	imul eax, eax, VIRTQ_DESC_SIZE
	; The pointer for the packet data
	mov rax, [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_ADDR_OFFSET]
	call processPacket
	; After processing, put packet right back on the receive queue
	movzx eax, si
	and eax, edx
	mov [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_RING_OFFSET + eax * 2], cx
	add si, 1
	pop rax
	; See if there are more to process
	add bx, 1
	cmp ax, bx
	jne .keepServicingRx
	; Update avail index
	mov [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_IDX_OFFSET], si
	; See if we need to notify
	mov cx, [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_USED_OFFSET + VIRTQ_USED_FLAGS_OFFSET] ; Check flags to see if we should notify
	test cx, VIRTQ_USED_FLAGS_NO_NOTIFY
	jnz .noNotify
	mov ecx, [VIRTIO_NET_RX_QUEUE_NOTIFY_ADDR]
	mov word [ecx], 0 ; Write receive queue index to the notify address
.noNotify:
.noRxServiceNeeded:
	mov [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_LAST_SEEN_USED_OFFSET], ax

	cmp dword [TCP_IS_INITIALIZED], 0
	je .tcpDontTryToSendData
	cmp word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET], 4
	jnge .tcpDontTryToSendData
	cmp dword [TCP_SEND_QUEUE_HEAD], TCP_CONNECTION_BLOCK_INVALID_IDX
	je .tcpDontTryToSendData
	call tcpTrySendAllData
.tcpDontTryToSendData:

	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	mov dword [APIC_REGISTER_PAGE + APIC_EOI_OFFSET], 0
	iretq

interruptVirtIOBlock:
	mov dword [APIC_REGISTER_PAGE + APIC_EOI_OFFSET], 0
	iretq

interruptGeneric:
	lea rax, [faultOccured]
	call debugPrint
	mov rax, rsp
	mov rcx, rax
	call debugPrintlnInteger
	pop rax
	mov rdx, rax
	call debugPrintlnInteger
	pop rax
	mov rsi, rax
	call debugPrintlnInteger
	DEBUG_SEND_STR faultOccured
	DEBUG_SEND_NUM64 rcx
	DEBUG_SEND_NUM64 rdx
	DEBUG_SEND_NUM64 rsi
	
	; DO got here (possible stack corruption)
	jmp systemIdle
	; No handling, immediately triple fault
	xor eax, eax
	mov cr3, rax
	iretq

keyscancodemap db 0,0,"1234567890-=",0,0x09,"QWERTYUIOP[]",0x0a,0,"ASDFGHJKL;'` \ZXCVBNM,./",0,0,0," ",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0





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
	pushf
	cli

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

	pop rbx
	test ebx, EFLAGS_INTERRUPT_FLAG
	jz .end
	sti
.end:
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret
	
; rax contains integer
; rdi contains result end pointer
; rdi gets result pointer
integerToString:
	push rax
	push rbx
	push rcx
	push rdx
	
.integerToASCIILoop:
	xor edx, edx
	test rax, rax
	jz .integerToASCIIZero
	
	mov ecx, 10
	div rcx

.integerToASCIIZero:
	add dl, 0x30 ; add ascii 0
	sub rdi, 1
	mov [rdi], dl

	test rax, rax
	jnz .integerToASCIILoop
	
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
	lea rdi, [rsp + 31]
	call integerToString
	mov rax, rdi
	call debugPrint

	add rsp, 32
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

debugPrintlnInteger:
	push rbx
	pushf
	cli

	call debugPrintInteger
	call debugPrintNewline

	pop rbx
	test ebx, EFLAGS_INTERRUPT_FLAG
	jz .end
	sti
.end:
	pop rbx
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
	and rax, ~0b1111 ; Take off low four bits, memory space bars are aligned to 16
	jmp .end
.ioSpaceBar:
	and eax, ~0b11 ; Take off low two bits, io space bars are aligned to 4
.end:
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

foundStr db "Found",0x0a,0x0
classCode db "ClassCode: ",0x0
subclass db "Subclass: ",0x0
progIF db "ProgIF: ",0x0
headerType db "HeaderType: ",0x0
functionNum db "FunctionNum: ",0x0

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
	mov ch, 0x24 ; Offset ABAR
	call getPCIAddressDword
	call readPCIBar
	mov [PCI_AHCI_CONTROLLER_ABAR_ADDRESS], eax
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
	mov edx, [PCI_AHCI_CONTROLLER_ABAR_ADDRESS]
	call getPCIAddressDword
	call writePCIConfigDword
	jmp .foundNoFunctionPCI
.notAHCIController:
	
	cmp dword [VIRTIO_NET_PCI_ADDRESS], 0
	jne .alreadyHasVirtIONet
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
.alreadyHasVirtIONet:
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

	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

ahciControllerFound db "AHCI controller found!",0x0a,0x0
achiControllerAddress db "AHCI controller address: ",0x0
noAHCIControllerFound db "No AHCI Controller Found!",0x0a,0x0
ahciInterrupt db "AHCI Interrupt!",0x0a,0x0
ahciInitStart db "AHCI Init Start",0x0a,0x00
ahciInitDone db "AHCI Init Done",0x0a,0x00
testingAHCI db "Testing AHCI",0x0a,0x00
ahciWriteComplete db "AHCI Write Complete",0x0a,0x00
ahciReadComplete db "AHCI Read Complete",0x0a,0x00
scanPCIStartStr db "Scan PCI start",0x0a,0x00
scanPCIDoneStr db "Scan PCI done",0x0a,0x00
startupDoneTookStr db "Startup done, took ",0x0
startupFailedStr db "Startup failed!",0x0a,0x0
millisecondsStr db " milliseconds.",0x0a,0x0
msiPending db "MSI pending",0x0a,0x0

initializeAHCI:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	mov rbp, rsp
	sub rsp, 8

	; DEBUG
	push rax
	lea rax, [ahciInitStart]
	call debugPrint
	pop rax

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
	mov esi, eax
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
	mov eax, esi
	call readPCIConfigDword
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
	leave
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

	lea rax, [testingAHCI]
	call debugPrint

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
	mov qword [rax + DISKCMD_DISK_ADDR_OFFSET], 36
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

	lea rax, [1024 * 1024 + 1024]
	mov dword [rax + DISKCMD_PORT_OFFSET], 0
	mov dword [rax + DISKCMD_ID_OFFSET], 0
	mov qword [rax + DISKCMD_MEM_ADDR_RW_OFFSET], BIOS_MEMORY_MAP_OFFSET | DISKCMD_MEM_ADDR_RW_WRITE_BIT
	mov qword [rax + DISKCMD_DISK_ADDR_OFFSET], 32
	mov dword [rax + DISKCMD_SECTORS_OFFSET], 8
	mov dword [rax + DISKCMD_QUEUED_OFFSET], 0
	mov qword [rax + DISKCMD_USER_DATA_OFFSET], rsp
	mov byte [rsp], 0
	call ahciQueueDiskCmd

.notDone3:
	cmp byte [rsp], 1
	je .done3
	hlt
	jmp .notDone3
.done3:

	lea rax, [1024 * 1024 + 1024]
	mov dword [rax + DISKCMD_PORT_OFFSET], 0
	mov dword [rax + DISKCMD_ID_OFFSET], 0
	mov rbx, MEMORY_ALLOCATION_BITMAP_SUPERVISOR_ADDRESS | DISKCMD_MEM_ADDR_RW_WRITE_BIT
	mov qword [rax + DISKCMD_MEM_ADDR_RW_OFFSET], rbx
	mov qword [rax + DISKCMD_DISK_ADDR_OFFSET], 40
	mov ebx, [PHYSICAL_MEMORY_ALLOC_SIZE]
	shr ebx, 9
	mov dword [rax + DISKCMD_SECTORS_OFFSET], ebx
	mov dword [rax + DISKCMD_QUEUED_OFFSET], 0
	mov qword [rax + DISKCMD_USER_DATA_OFFSET], rsp
	mov byte [rsp], 0
	call ahciQueueDiskCmd

.notDone4:
	cmp byte [rsp], 1
	je .done4
	hlt
	jmp .notDone4
.done4:

	lea rax, [ahciWriteComplete]
	call debugPrint

	mov eax, [PHYSICAL_MEMORY_ALLOC_SIZE]
	call debugPrintlnInteger

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

	

virtIONetFound db "VirtIO net device found!",0x0a,0x0
virtIONetInterrupt db "VirtIO net interrupt!",0x0a,0x0
virtIONetInitError db "VirtIO net init error!",0x0a,0x0
virtIONetInitDone db "VirtIO net init done!",0x0a,0x0
virtIONetInitStart db "VirtIO net init start!",0x0a,0x0
virtIONetCapabilityFailure db "VirtIO net cap fail!",0x0a,0x0
virtIONetLegacyDevice db "VirtIO net is legacy device!",0x0a,0x0
virtioChecksumOffload db "VirtIO supports checksum offload",0x0a,0x0
virtioNoChecksumOffload db "VirtIO does not support checksum offload",0x0a,0x0
debugTabStr db "    ",0x0

; Carry flag is 0 on success, 1 on failure
initializeVirtIONet:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	mov rbp, rsp
	sub rsp, 16
	; Local variable table
	; [rbp - 8]  : notify address
	; [rbp - 12] : notify offset multipler

	; Zero local variables
	pxor xmm0, xmm0
	movups [rbp - 16], xmm0

	lea rax, [virtIONetInitStart]
	call debugPrint

	mov ecx, [VIRTIO_NET_PCI_ADDRESS] ; PCI config address for the VirtIO network interface

	; Scan capabilities for virtio common cfg, virtio notify cfg, and enable MSI-X
	xor esi, esi ; Bit flags for completed search. Common cfg is 0b1, notify is 0b10, msi x is 0b100, device cfg is 0b1000
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

	; Check for MSI-X
	cmp edx, 0x11 ; 0x11 is the MSI-X id
	jne .capabilityNotMSIX
	test esi, 0b100
	jnz .capabilityContinue ; We already found an MSIX
	or eax, (1 << 31) ; Enable MSI-X
	and eax, ~(1 << 30) ; Make sure function mask is unmasked
	mov edx, eax
	mov eax, edi
	call writePCIConfigDword
	lea eax, [edi + 4] ; table offset and BAR index
	call readPCIConfigDword
	mov ebx, eax
	and eax, 0b111 ; BAR index
	and ebx, ~0b111 ; table offset
	lea eax, [ecx + PCI_BARS_OFFSET + eax * 4] ; Bar pci address
	call readPCIBar
	add rax, rbx ; Physical table address, need to map it to virtual memory
	mov rdx, rax
	; Align to page
	and rax, ~(1000h - 1)
	or rax, PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + SCRATCH_PAGES_START / 512], rax
	add rax, 1000h
	mov [PAGE_TABLE_START + 3000h + (SCRATCH_PAGES_START + 1000h) / 512], rax
	invlpg [SCRATCH_PAGES_START]
	invlpg [SCRATCH_PAGES_START + 1000h]
	; Get the low offset
	and rdx, 1000h - 1
	lea rax, [SCRATCH_PAGES_START + rdx]
	; Configure first vector
	mov dword [rax], 0xFEE00000 ; Message address, see intel software developer manual vol 3, section 11.11.1 Message Address Register Format
	mov dword [rax + 4], 0 ; Message address upper
	mov dword [rax + 8], AHCI_VIRTIO_NET_VECTOR ; Message data
	mov edx, [rax + 12] ; Vector control
	and edx, ~0b1 ; Clear mask bit, preserve reserved bits
	mov [rax + 12], edx ; Vector control
	; Remap pages back to normal identity mapped memory
	mov qword [PAGE_TABLE_START + 3000h + SCRATCH_PAGES_START / 512], SCRATCH_PAGES_START | PAGE_PRESENT | PAGE_WRITE
	mov qword [PAGE_TABLE_START + 3000h + (SCRATCH_PAGES_START + 1000h) / 512], (SCRATCH_PAGES_START + 1000h) | PAGE_PRESENT | PAGE_WRITE
	invlpg [SCRATCH_PAGES_START]
	invlpg [SCRATCH_PAGES_START + 1000h]
	or esi, 0b100 ; Mark msi done
	jmp .capabilityContinue

.capabilityNotMSIX:
	cmp edx, 0x09 ; PCI_CAP_ID_VNDR
	jne .capabilityNotVendorSpecific
	mov edx, eax
	shr edx, 24 ; Extract cfg_type, see virtio spec section 4.1.4
	cmp edx, VIRTIO_PCI_CAP_COMMON_CFG
	jne .notCommonCfg
	test esi, 0b1
	jnz .capabilityContinue ; Already found one of these
	lea eax, [edi + VIRTIO_PCI_CAP_OFFSET_OFFSET] ; Capability base + offset offset
	call readPCIConfigDword
	mov [VIRTIO_NET_CONFIG_ADDRESS], rax ; common config address
	lea eax, [edi + VIRTIO_PCI_CAP_BAR_OFFSET] ; Capability base + bar offset
	call readPCIConfigDword
	and eax, 0xFF ; Only low byte matters here, the rest is padding
	lea eax, [ecx + PCI_BARS_OFFSET + eax * 4] ; Bar pci address
	call readPCIBar
	add [VIRTIO_NET_CONFIG_ADDRESS], rax ; Add the bar address to the offset to get common config address
	or esi, 0b1 ; Set common found bit
	jmp .capabilityContinue

.notCommonCfg:
	cmp edx, VIRTIO_PCI_CAP_NOTIFY_CFG
	jne .notNotifyCfg
	test esi, 0b10
	jnz .capabilityContinue ; Already found one of these
	lea eax, [edi + VIRTIO_PCI_CAP_OFFSET_OFFSET] ; Capability base + offset offset
	call readPCIConfigDword
	mov [rbp - 8], eax ; notify offset, we'll calculate the real value with it after this loop
	lea eax, [edi + VIRTIO_PCI_CAP_BAR_OFFSET] ; Capability base + bar offset
	call readPCIConfigDword
	and eax, 0xFF ; Only low byte matters here, the rest is padding
	lea eax, [ecx + PCI_BARS_OFFSET + eax * 4] ; Bar pci address
	call readPCIBar
	add [rbp - 8], rax ; Add the bar address to the offset
	lea eax, [edi + VIRTIO_NOTIFY_CAP_NOTIFY_OFF_MULTIPLIER_OFFSET] ; Capability base + notify offset multipler offset
	call readPCIConfigDword
	mov [rbp - 12], eax ; notify offset multipler, we'll calculate the real value with it after this loop
	or esi, 0b10 ; Set notify found bit
	jmp .capabilityContinue

.notNotifyCfg:
	cmp edx, VIRTIO_PCI_CAP_DEVICE_CFG
	jne .notDeviceCfg
	test esi, 0b1000
	jne .capabilityContinue ; Already found one of these
	lea eax, [edi + VIRTIO_PCI_CAP_OFFSET_OFFSET] ; Capability base + offset offset
	call readPCIConfigDword
	mov edx, eax
	lea eax, [edi + VIRTIO_PCI_CAP_BAR_OFFSET] ; Capability base + bar offset
	call readPCIConfigDword
	and eax, 0xFF ; Cut off any padding
	lea eax, [ecx + PCI_BARS_OFFSET + eax * 4]
	call readPCIBar
	add rax, rdx ; Physical address of cfg, need to map it to virtual memory
	mov rdx, rax
	; Align to page
	and rax, ~(1000h - 1)
	or rax, PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + SCRATCH_PAGES_START / 512], rax
	add rax, 1000h
	mov [PAGE_TABLE_START + 3000h + (SCRATCH_PAGES_START + 1000h) / 512], rax
	invlpg [SCRATCH_PAGES_START]
	invlpg [SCRATCH_PAGES_START + 1000h]
	; Get the low offset
	and rdx, 1000h - 1
	lea rax, [SCRATCH_PAGES_START + rdx]
	; Grab our mac address
	mov edx, [rax]
	mov [VIRTIO_NET_MAC], edx
	mov dx, [rax + 4]
	mov [VIRTIO_NET_MAC + 4], dx
	mov word [VIRTIO_NET_MAC + 6], 0 ; Only 6 bytes
	; Remap pages back to normal identity mapped memory
	mov qword [PAGE_TABLE_START + 3000h + SCRATCH_PAGES_START / 512], SCRATCH_PAGES_START | PAGE_PRESENT | PAGE_WRITE
	mov qword [PAGE_TABLE_START + 3000h + (SCRATCH_PAGES_START + 1000h) / 512], (SCRATCH_PAGES_START + 1000h) | PAGE_PRESENT | PAGE_WRITE
	invlpg [SCRATCH_PAGES_START]
	invlpg [SCRATCH_PAGES_START + 1000h]
	or esi, 0b1000 ; Set device cfg found bit
	jmp .capabilityContinue
.notDeviceCfg:

.capabilityNotVendorSpecific:
.capabilityContinue:
	cmp esi, 0b1111
	jz .capabilitySearchDone ; We've found all 4 necessary components, we're done searching through capabilities now
	mov eax, edi
	call readPCIConfigDword
	shr eax, 8 ; Get next pointer
	and eax, 0b11111100 ; 8 bit value, must be dword aligned, so take off the last two bits
	test eax, eax ; Zero signifies the end of the list
	jnz .capabilitySearch
.capabilitySearchDone:

	cmp esi, 0b1111
	je .notError1 ; We weren't able to find all necessary capabilities
	; DEBUG
	push rax
	lea rax, [virtIONetCapabilityFailure]
	call debugPrint
	pop rax
	jmp .error
.notError1:


	; Good, now we can start the initialization process

	; Enable PCI interrupts, memory control, and bus mastering
	lea eax, [ecx + 0x04] ; Offset for Status | Command
	call readPCIConfigDword
	or eax, PCI_CONTROL_MEMORY_SPACE | PCI_CONTROL_BUS_MASTER
	and eax, ~PCI_CONTROL_INTERRUPT_DISABLE
	mov edx, eax
	lea eax, [ecx + 0x04] ; Offset for Status | Command
	call writePCIConfigDword

	; Map configuration space
	mov rax, [VIRTIO_NET_CONFIG_ADDRESS] ; Common config address
	mov rdx, rax
	; Align to page
	and rax, ~(1000h - 1)
	or rax, PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + VIRTIO_NET_CONFIG_OFFSET / 512], rax
	add rax, 1000h
	mov [PAGE_TABLE_START + 3000h + (VIRTIO_NET_CONFIG_OFFSET + 1000h) / 512], rax
	; Get the low offset
	and rdx, 1000h - 1
	; Mapped virtual address of PCI ABAR
	lea rax, [rdx + VIRTIO_NET_CONFIG_OFFSET]
	invlpg [VIRTIO_NET_CONFIG_OFFSET]
	invlpg [VIRTIO_NET_CONFIG_OFFSET + 1000h]
	mov [VIRTIO_NET_CONFIG_ADDRESS], rax

	; Begin VirtIO device initialization
	mov byte [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET], 0 ; Reset device
	mov cl, [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET]
	or cl, VIRTIO_DEVICE_STATUS_ACKNOWLEDGE | VIRTIO_DEVICE_STATUS_DRIVER ; Ack and tell it we can drive it
	mov [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET], cl
	mov word [rax + VIRTIO_PCI_COMMON_MSIX_CONFIG_OFFSET], 0 ; vector
	mov dword [rax + VIRTIO_PCI_COMMON_DEVICE_FEATURE_SELECT_OFFSET], 0 ; First 32 bits
	mov ebx, [rax + VIRTIO_PCI_COMMON_DEVICE_FEATURES_OFFSET] ; Read device features
	and ebx, VIRTIO_NET_F_MAC ;| VIRTIO_NET_F_CSUM ; Select NET_F_MAC
	; VIRTIO_NET_F_CSUM is enabled on DO
	mov [VIRTIO_NET_NEGOTIATED_FEATURES], ebx
	mov dword [rax + VIRTIO_PCI_COMMON_DRIVER_FEATURE_SELECT_OFFSET], 0 ; First 32 bits
	mov [rax + VIRTIO_PCI_COMMON_DRIVER_FEATURES_OFFSET], ebx ; Write selected features
	mov dword [rax + VIRTIO_PCI_COMMON_DEVICE_FEATURE_SELECT_OFFSET], 1 ; Second 32 bits
	mov ebx, [rax + VIRTIO_PCI_COMMON_DEVICE_FEATURES_OFFSET] ; Read device features
	test ebx, VIRTIO_F_VERSION_1 >> 32 ; Make sure this isn't a legacy device
	jnz .notLegacyDevice
	mov cl, [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET]
	or cl, VIRTIO_DEVICE_STATUS_FAILED ; Set error
	mov [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET], cl
	; DEBUG
	push rax
	lea rax, [virtIONetLegacyDevice]
	call debugPrint
	pop rax
	jmp .error
.notLegacyDevice:
	and ebx, VIRTIO_F_VERSION_1 >> 32 ; Say we're not a legacy driver
	mov dword [rax + VIRTIO_PCI_COMMON_DRIVER_FEATURE_SELECT_OFFSET], 1 ; Second 32 bits
	mov [rax + VIRTIO_PCI_COMMON_DRIVER_FEATURES_OFFSET], ebx ; Write selected features
	mov cl, [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET]
	or cl, VIRTIO_DEVICE_STATUS_FEATURES_OK ; Features selected
	mov [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET], cl
	mov bl, [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET] ; Read device status to see if features ok is still set
	test bl, VIRTIO_DEVICE_STATUS_FEATURES_OK
	jz .error ; Features not ok, device not supported

	; Zero virtqs
	xor eax, eax
	mov ecx, (4096 * 2) / 4
	mov edi, VIRTIO_NET_TX_QUEUE_OFFSET
	rep stosd
	mov rax, [VIRTIO_NET_CONFIG_ADDRESS]

	; Initialize receive queue
	mov word [rax + VIRTIO_PCI_COMMON_QUEUE_SELECT_OFFSET], 0
	; Calculate notify address
	movzx ebx, word [rax + VIRTIO_PCI_COMMON_QUEUE_NOTIFY_OFF_OFFSET]
	mov ecx, [rbp - 12] ; notify offset multipler
	imul ebx, ecx
	add rbx, [rbp - 8] ; notify address
	; Map notify address
	mov rcx, rbx
	; Align to page
	and rbx, ~(1000h - 1)
	or rbx, PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + VIRTIO_NET_QUEUE_NOTIFY_PAGES / 512], rbx
	invlpg [VIRTIO_NET_QUEUE_NOTIFY_PAGES]
	; Get the low offset
	and rcx, 1000h - 1
	lea rbx, [VIRTIO_NET_QUEUE_NOTIFY_PAGES + rcx]
	mov [VIRTIO_NET_RX_QUEUE_NOTIFY_ADDR], ebx ; Save queue notify virtual address
	; Ensure size is at most VIRTIO_DESC_COUNT_MAX
	movzx ebx, word [rax + VIRTIO_PCI_COMMON_QUEUE_SIZE_OFFSET]
	mov cx, VIRTIO_DESC_COUNT_MAX
	cmp bx, VIRTIO_DESC_COUNT_MAX
	cmovge bx, cx
	mov [rax + VIRTIO_PCI_COMMON_QUEUE_SIZE_OFFSET], bx
	mov [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_COUNT_OFFSET], bx
	mov word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_LAST_SEEN_USED_OFFSET], 0
	mov word [rax + VIRTIO_PCI_COMMON_QUEUE_MSIX_VECTOR_OFFSET], 0 ; MSI-X vector 0
	mov qword [rax + VIRTIO_PCI_COMMON_QUEUE_DESC_OFFSET], VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET
	mov qword [rax + VIRTIO_PCI_COMMON_QUEUE_AVAIL_OFFSET], VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET
	mov qword [rax + VIRTIO_PCI_COMMON_QUEUE_USED_OFFSET], VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_USED_OFFSET
	; Setup receive buffers
	xor edi, edi ; receive buffer idx
.offerReceiveBuffers:
	sub ebx, 2 ; Two descriptors needed for receive, since header is device read only and the data is device write only
	; Add to available buffer
	mov word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_RING_OFFSET + edi * 2], bx
	add edi, 1
	; Setup packet VirtIO header
	imul ecx, ebx, VIRTIO_NET_PACKET_BUFFER_SIZE / 2 ; Over two because two descriptors needed for each packet buffer
	add ecx, VIRTIO_NET_RX_PACKETS_OFFSET
	; I don't think this is actually needed, since the write only flag is set?
	;mov byte [ecx + VIRTIO_NET_HDR_FLAGS_OFFSET], 0 ; Does not need csum
	;mov byte [ecx + VIRTIO_NET_HDR_GSO_TYPE_OFFSET], VIRTIO_NET_HDR_GSO_NONE
	;mov qword [ecx + VIRTIO_NET_HDR_HDR_LEN_OFFSET], 0 ; Next 4 fields are irrelevant to us
	;mov word [ecx + VIRTIO_NET_HDR_NUM_BUFFERS_OFFSET], 1 ; Should always be 1 since VIRTIO_NET_F_MRG_RXBUF was not negotiated
	imul edx, ebx, VIRTQ_DESC_SIZE
	mov [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_ADDR_OFFSET], rcx
	mov dword [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_LEN_OFFSET], VIRTIO_NET_HDR_SIZE
	mov word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_FLAGS_OFFSET], VIRTQ_DESC_FLAGS_WRITE | VIRTQ_DESC_FLAGS_NEXT
	; Chain in next descriptor
	add ebx, 1
	mov [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_NEXT_OFFSET], bx
	add ecx, VIRTIO_NET_HDR_SIZE
	imul edx, ebx, VIRTQ_DESC_SIZE
	mov [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_ADDR_OFFSET], rcx
	mov dword [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_LEN_OFFSET], VIRTIO_NET_PACKET_BUFFER_SIZE - VIRTIO_NET_HDR_SIZE
	mov word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_FLAGS_OFFSET], VIRTQ_DESC_FLAGS_WRITE
	mov word [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_NEXT_OFFSET], 0 ; Zero next pointer anyway
	sub ebx, 1
	test ebx, ebx
	jnz .offerReceiveBuffers
	add [VIRTIO_NET_RX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_IDX_OFFSET], di ; Set index, now the buffers are available for reception
	mov word [rax + VIRTIO_PCI_COMMON_QUEUE_ENABLE_OFFSET], 1 ; Enable queue

	; Initialize transmit queue
	mov word [rax + VIRTIO_PCI_COMMON_QUEUE_SELECT_OFFSET], 1
	; Calculate notify address
	movzx ebx, word [rax + VIRTIO_PCI_COMMON_QUEUE_NOTIFY_OFF_OFFSET]
	mov ecx, [rbp - 12] ; notify offset multipler
	imul ebx, ecx
	add rbx, [rbp - 8] ; notify address
	; Map notify address
	mov rcx, rbx
	; Align to page
	and rbx, ~(1000h - 1)
	or rbx, PAGE_PRESENT | PAGE_WRITE | PAGE_CACHE_DISABLE
	mov [PAGE_TABLE_START + 3000h + (VIRTIO_NET_QUEUE_NOTIFY_PAGES + 1000h) / 512], rbx
	invlpg [VIRTIO_NET_QUEUE_NOTIFY_PAGES + 1000h]
	; Get the low offset
	and rcx, 1000h - 1
	lea rbx, [VIRTIO_NET_QUEUE_NOTIFY_PAGES + 1000h + rcx]
	mov [VIRTIO_NET_TX_QUEUE_NOTIFY_ADDR], ebx ; Save queue notify virtual address
	; Ensure size is at most VIRTIO_DESC_COUNT_MAX
	movzx ebx, word [rax + VIRTIO_PCI_COMMON_QUEUE_SIZE_OFFSET]
	mov cx, VIRTIO_DESC_COUNT_MAX
	cmp bx, VIRTIO_DESC_COUNT_MAX
	cmovge bx, cx
	mov [rax + VIRTIO_PCI_COMMON_QUEUE_SIZE_OFFSET], bx
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_COUNT_OFFSET], bx
	mov word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_LAST_SEEN_USED_OFFSET], 0
	mov word [rax + VIRTIO_PCI_COMMON_QUEUE_MSIX_VECTOR_OFFSET], 0 ; MSI-X vector 0
	mov qword [rax + VIRTIO_PCI_COMMON_QUEUE_DESC_OFFSET], VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET
	mov qword [rax + VIRTIO_PCI_COMMON_QUEUE_AVAIL_OFFSET], VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET
	mov qword [rax + VIRTIO_PCI_COMMON_QUEUE_USED_OFFSET], VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_USED_OFFSET
	; Setup send free descriptors linked list
	mov word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET], bx ; All descriptors are free
	mov word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_FREE_DESCRIPTOR_CHAIN_HEAD_OFFSET], 0
	xor ecx, ecx
.setupFreeSendDescriptors:
	imul edx, ecx, VIRTQ_DESC_SIZE
	mov edi, ecx
	shr edi, 1
	imul edi, edi, VIRTIO_NET_PACKET_BUFFER_SIZE
	add edi, VIRTIO_NET_TX_PACKETS_OFFSET
	test cx, 1
	jnz .bufferDescriptor
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_ADDR_OFFSET], rdi
	mov dword [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_LEN_OFFSET], VIRTIO_NET_HDR_SIZE
	mov word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_FLAGS_OFFSET], VIRTQ_DESC_FLAGS_NEXT
	jmp .headerDescriptor
.bufferDescriptor:
	add edi, VIRTIO_NET_HDR_SIZE
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_ADDR_OFFSET], rdi
	mov dword [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_LEN_OFFSET], 0
	mov word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_FLAGS_OFFSET], 0
.headerDescriptor:
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_NEXT_OFFSET], cx
	add ecx, 1
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + edx + VIRTQ_DESC_NEXT_OFFSET], cx
	cmp ecx, ebx
	jne .setupFreeSendDescriptors
	mov word [rax + VIRTIO_PCI_COMMON_QUEUE_ENABLE_OFFSET], 1 ; Enable queue

	mov bl, [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET]
	test bl, VIRTIO_DEVICE_STATUS_DEVICE_NEEDS_RESET
	jnz .error

	; Init done
	mov cl, [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET]
	or cl, VIRTIO_DEVICE_STATUS_DRIVER_OK ; Set driver ok, device is live
	mov [rax + VIRTIO_PCI_COMMON_DEVICE_STATUS_OFFSET], cl

	; Notify receive queue
	mov ecx, [VIRTIO_NET_RX_QUEUE_NOTIFY_ADDR]
	mov word [ecx], 0 ; Write receive queue index to the notify address

	mov qword [VIRTIO_RECEIVED_PACKET_COUNT], 0 ; Reset received packet count
	mov qword [VIRTIO_SENT_PACKET_COUNT], 0 ; Reset sent packet count

	lea rax, [virtIONetInitDone]
	call debugPrint
	clc ; Clear carry to indicate success
	jmp .end
.error:
	lea rax, [virtIONetInitError]
	call debugPrint
	stc ; Set carry to indicate failure
.end:
	leave
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

waitForTxPacket:
	push rbx

	mov bx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET]
	test bx, bx
	jnz .end

	pushf
	cli
	
.noPacketsAvailable:
	call virtIOServiceTX
	mov bx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET]
	test bx, bx
	jz .noPacketsAvailable
.packetAvailable:

	pop rbx
	test ebx, EFLAGS_INTERRUPT_FLAG
	jz .end
	sti
.end:
	pop rbx
	ret


; eax gets descriptor head number. Descriptors are in chains of two.
acquireTxPacket:
	push rbx
	push rcx
	pushf
	cli

	mov bx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET]
	test bx, bx
	jnz .packetAvailable
.noPacketsAvailable:
	call virtIOServiceTX
	mov bx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET]
	test bx, bx
	jz .noPacketsAvailable
.packetAvailable:

.tryPopPacket:
	movzx eax, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_FREE_DESCRIPTOR_CHAIN_HEAD_OFFSET]
	imul ebx, eax, VIRTQ_DESC_SIZE

	; Setup virtio header
	mov dword [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ebx + VIRTQ_DESC_LEN_OFFSET], VIRTIO_NET_HDR_SIZE
	mov rcx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ebx + VIRTQ_DESC_ADDR_OFFSET]
	mov byte [rcx + VIRTIO_NET_HDR_FLAGS_OFFSET], 0 ; No flags
	mov byte [rcx + VIRTIO_NET_HDR_GSO_TYPE_OFFSET], VIRTIO_NET_HDR_GSO_NONE
	mov word [rcx + VIRTIO_NET_HDR_HDR_LEN_OFFSET], ETHERNET_HEADER_SIZE ; I don't think this is necessary without negotiating TSO or UFO bits
	mov qword [rcx + VIRTIO_NET_HDR_GSO_SIZE_OFFSET], 0 ; Next 4 fields don't matter

	movzx ebx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ebx + VIRTQ_DESC_NEXT_OFFSET]
	imul ebx, ebx, VIRTQ_DESC_SIZE
	mov dword [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ebx + VIRTQ_DESC_LEN_OFFSET], 0 ; Zero out data length
	mov bx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ebx + VIRTQ_DESC_NEXT_OFFSET]
	; Need a cmpxchg here in case an interrupt happened that modified head
	cmpxchg [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_FREE_DESCRIPTOR_CHAIN_HEAD_OFFSET], bx ; if ax still equals free chain head, set it to the next free one, otherwise continue the loop
	jnz .tryPopPacket
	sub word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET], 2

	pop rcx
	test ecx, EFLAGS_INTERRUPT_FLAG
	jz .end
	sti
.end:
	pop rcx
	pop rbx
	ret

; eax contains descriptor head number
; ebx contains packet length
queueTxPacket:
	push rcx
	push rbx
	push rdx
	push rsi

	; Patch in packet length
	imul ecx, eax, VIRTQ_DESC_SIZE
	mov rdx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_ADDR_OFFSET]
	movzx ecx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_NEXT_OFFSET]
	imul ecx, ecx, VIRTQ_DESC_SIZE
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_LEN_OFFSET], ebx
	mov rsi, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_ADDR_OFFSET]
	
	test dword [VIRTIO_NET_NEGOTIATED_FEATURES], VIRTIO_NET_F_CSUM
	jz .noChecksumOffload
	cmp ebx, ETHERNET_HEADER_SIZE + IPV4_HEADER_MIN_SIZE + TCP_HEADER_MIN_SIZE
	jl .noChecksumOffload
	cmp word [rsi + ETHERNET_HEADER_TYPE_OFFSET], HTON16(ETHERTYPE_IPV4)
	jne .noChecksumOffload
	cmp byte [rsi + ETHERNET_HEADER_SIZE + IPV4_HEADER_PROTOCOL_OFFSET], INTERNET_PROTOCOL_TCP
	jne .noChecksumOffload
	movzx ecx, byte [rsi + ETHERNET_HEADER_SIZE + IPV4_HEADER_VERSION_INTERNET_HEADER_LENGTH_OFFSET]
	and ecx, 0x0F
	shl ecx, 2
	add ecx, ETHERNET_HEADER_SIZE
	; Setup virtio net header
	mov byte [rdx + VIRTIO_NET_HDR_FLAGS_OFFSET], VIRTIO_NET_HDR_FLAG_NEEDS_CSUM
	mov [rdx + VIRTIO_NET_HDR_CSUM_START_OFFSET], cx
	mov word [rdx + VIRTIO_NET_HDR_CSUM_OFFSET_OFFSET], TCP_HEADER_CHECKSUM_OFFSET
.noChecksumOffload:

	movzx ecx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_IDX_OFFSET]
	movzx ebx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_COUNT_OFFSET]
	sub ebx, 1 ; descMask
	and ecx, ebx
	mov [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_RING_OFFSET + ecx * 2], ax ; Add to ring
	add word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_AVAIL_OFFSET + VIRTQ_AVAIL_IDX_OFFSET], 1
	mov cx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_USED_OFFSET + VIRTQ_USED_FLAGS_OFFSET] ; Check flags to see if we should notify
	test cx, VIRTQ_USED_FLAGS_NO_NOTIFY
	jnz .noNotify
	mov ecx, [VIRTIO_NET_TX_QUEUE_NOTIFY_ADDR]
	mov word [ecx], 1 ; Write transmit queue index to the notify address
.noNotify:
	pop rsi
	pop rdx
	pop rbx
	pop rcx
	ret

; eax contains current packet length
; rbx contains packet buffer address
; rcx contains dstMac | (ethertype << 48)
; eax gets updated current packet length
encodeEthernetPacket:
	push rcx
	mov [rbx + rax], ecx ; dst mac bytes 0-3
	shr rcx, 32
	mov [rbx + rax + 4], cx ; dst mac bytes 4-5
	shr rcx, 16
	ror cx, 8 ; bswap to network byte order
	mov [rbx + rax + 12], cx ; ethertype
	mov rcx, [VIRTIO_NET_MAC]
	mov [rbx + rax + 6], ecx ; src mac bytes 0-3
	shr rcx, 32
	mov [rbx + rax + 10], cx ; src mac bytes 4-5
	pop rcx
	add eax, 14 ; ethernet header length
	ret

; eax contains current packet length
; rbx contains packet buffer address
; ecx contains 8 bit protocol number (RFC 790)
; edx contains dst ip address
; eax gets updated current packet length
encodeIPv4Packet:
	push rcx
	; See RFC 791
	mov dword [rbx + rax], (4 << 4) | 5 | (0 << 8) | (0 << 16) ; version (IPv4), header length (5 dwords), type of service (routine), total length (will be patched later)
	push rcx
	movzx ecx, word [IPV4_NEXT_IDENTIFICATION]
	ror cx, 8 ; swap to network order
	mov [rbx + rax + 4], ecx ; identification, flags (0 in this case), and fragment offset (we zero it)
	ror cx, 8 ; swap back to little endian
	add ecx, 1
	mov [IPV4_NEXT_IDENTIFICATION], cx ; Upate next identification to something new
	pop rcx
	shl ecx, 8 ; put protocol field in correct place
	or ecx, 128 ; TTL of 128 (no particular reason, I just saw that number in some wireshark packets), header checksum of 0 (we'll patch it later)
	mov [rbx + rax + 8], ecx
	mov ecx, [VIRTIO_NET_IP_ADDRESS] ; src address
	mov [rbx + rax + 12], ecx ; src ip
	mov [rbx + rax + 16], edx ; dst ip
	; no options
	pop rcx
	add eax, 5 * 4 ; 5 dword header
	ret

; Patches length and computes the header checksum
; eax contains IPv4 packet length (including header)
; rbx contains IPv4 header address
patchIPv4Packet:
	push rcx
	push rdx
	ror ax, 8 ; Swap to network byte order
	mov [rbx + 2], ax ; total length
	rol ax, 8 ; Restore original byte order
	; Calculate header checksum
	; the one's complement of the one's complement sum of the 16 bit words in the header
	mov ecx, [rbx]
	add ecx, [rbx + 4]
	adc ecx, [rbx + 8]
	adc ecx, [rbx + 12]
	adc ecx, [rbx + 16]
	adc ecx, 0
	mov edx, ecx
	shr edx, 16
	add cx, dx
	adc cx, 0
	not cx
	mov [rbx + 10], cx
	pop rdx
	pop rcx
	ret

; eax contains current packet length
; rbx contains packet buffer address
; ecx contains srcPort | (dstPort << 16)
; eax gets updated current packet length
encodeUDPPacket:
	push rcx
	ror cx, 8 ; swap src port to network byte order
	mov [rbx + rax], cx
	shr ecx, 16 ; Get dst port
	ror cx, 8 ; swap to network byte order
	mov [rbx + rax + 2], cx
	mov dword [rbx + rax + 4], 0 ; length and checksum, will be patched later
	pop rcx
	add eax, 8
	ret

dstPortWas0Str db "dst port was 0",0x0a,0x00
srcPortWas0Str db "src port was 0",0x0a,0x00

; eax contains current packet length
; rbx contains packet buffer address
; rcx contains srcPort | (dstPort << 16) | (window << 32)
; edx contains flags
; esi contains acknowledge number
; edi contains sequence number
encodeTCPPacket:
	push rcx

	; I just remembered x86 has an instruction that swaps bytes on a mov
	; This makes things nicer
	movbe [rbx + rax + TCP_HEADER_SRC_PORT_OFFSET], cx
	; debug
	test cx, cx
	jnz .srcPortNot0
	DEBUG_PRINT_STR srcPortWas0Str
.srcPortNot0:
	shr rcx, 16 ; get dst port in bottom bits
	movbe [rbx + rax + TCP_HEADER_DST_PORT_OFFSET], cx
	; debug
	test cx, cx
	jnz .dstPortNot0
	DEBUG_PRINT_STR dstPortWas0Str
.dstPortNot0:
	movbe [rbx + rax + TCP_HEADER_SEQUENCE_NUM_OFFSET], edi
	movbe [rbx + rax + TCP_HEADER_ACKNOWLEDGE_NUM_OFFSET], esi
	mov byte [rbx + rax + TCP_HEADER_DATA_OFFSET_OFFSET], 5 << 4
	mov [rbx + rax + TCP_HEADER_FLAGS_OFFSET], dl
	shr rcx, 16
	movbe [rbx + rax + TCP_HEADER_WINDOW_OFFSET], cx
	mov word [rbx + rax + TCP_HEADER_CHECKSUM_OFFSET], 0
	mov word [rbx + rax + TCP_HEADER_URGENT_PTR_OFFSET], 0

	pop rcx
	add eax, TCP_HEADER_MIN_SIZE
	ret

; rbx contains base address of either a UDP or TCP packet
; eax contains packet length
; rcx contains current checksum
; rcx gets finished checksum (16 bits, high bits are 0)
addIPChecksum:
	push rdx

	mov edx, eax
	and edx, ~7 ; align to 8, packet is always at least 8 bytes for both UDP and IP headers so we don't have to check that
	shr edx, 3 ; units in qwords instead of bytes
	dec edx
	clc
	; Sum up with 64 bits for speed
.checksumDataQwords:
	adc rcx, [rbx + rdx * 8]
	dec edx ; dec is important here! It does not affect the carry flag
	jns .checksumDataQwords ; If the sign gets set, edx has wrapped, exit the loop
	adc rcx, 0 ; Add back carry now, easier than preserving the carry flag through all the stuff ahead
	test eax, 0b111 ; Already aligned? Skip the byte adding
	jz .dataIsAlignedTo8
	mov edx, eax
	and edx, ~7 ; Align again, but this time loop up and get the last few bytes
	push rsi
	sub eax, 1 ; eax = size - 1
	cmp edx, eax ; i >= size - 1 ?
	jge .sumOneByte
	; Sum up leftover 16 bit parts
.checksumDataWords:
	movzx rsi, word [rbx + rdx]
	add rcx, rsi
	adc rcx, 0
	add edx, 2
	cmp edx, eax
	jl .checksumDataWords
	; Add last odd byte if necessary
	test eax, 1 ; eax is size - 1 at this point
	jnz .dataSizeIsEven
.sumOneByte:
	movzx rsi, byte [rbx + rax]
	adc rcx, rsi
.dataSizeIsEven:
	add eax, 1 ; eax = size
	pop rsi
.dataIsAlignedTo8:
	adc rcx, 0
	; Checksum finished in 64 bits, move down to 16 bits
	mov rdx, rcx
	shr rdx, 32
	add ecx, edx
	adc ecx, 0
	mov edx, ecx
	shr edx, 16
	add cx, dx
	adc cx, 0
	movzx rcx, cx ; Clear upper bits

	pop rdx
	ret

; Patches length and computes the header checksum
; eax contains UDP packet length (including header)
; rbx contains UDP header address
; rdx contains dst ip address
patchUDPPacket:
	push rcx
	push rdx
	; Compute pseudoheader checksum
	; This code is very easy to break! Be careful with the carry flag for adc.
	mov ecx, [VIRTIO_NET_IP_ADDRESS] ; src ip address
	add rcx, rdx ; dst ip address
	mov edx, eax
	ror dx, 8 ; bswap to network byte order
	mov [rbx + 4], dx ; Add in UDP length while we have it bswapped
	shl edx, 16
	or edx, (INTERNET_PROTOCOL_UDP << 8)
	add rcx, rdx ; zero | protocol | UDP length
	; Add in data checksum
	call addIPChecksum
	not cx
	mov dx, 0xFFFF
	test cx, cx
	cmovz cx, dx ; If the generated checksum is zero, transmit as all 1s (equivalent in one's complement). Zero means no checksum computed.
	mov [rbx + 6], cx ; Add in checksum
	pop rdx
	pop rcx
	ret

; Patches in the checksum
; eax contains TCP packet length (including header)
; rbx contains TCP header address
; rdx contains dst ip address
patchTCPPacket:
	push rcx
	push rdx
	; Compute pseudoheader checksum
	; This code is very easy to break! Be careful with the carry flag for adc.
	mov ecx, [VIRTIO_NET_IP_ADDRESS] ; src ip address
	add rcx, rdx ; dst ip address
	mov edx, eax
	ror dx, 8 ; bswap to network byte order
	shl edx, 16
	or edx, (INTERNET_PROTOCOL_TCP << 8)
	add rcx, rdx ; zero | protocol | TCP length
	test dword [VIRTIO_NET_NEGOTIATED_FEATURES], VIRTIO_NET_F_CSUM
	jnz .deviceComputesChecksum
	; Add in data checksum
	call addIPChecksum
	not cx
	mov dx, 0xFFFF
	test cx, cx
	cmovz cx, dx ; If the generated checksum is zero, transmit as all 1s (equivalent in one's complement). Zero means no checksum computed.
	jmp .driverComputesChecksum
.deviceComputesChecksum:
	; Checksum finished in 64 bits, move down to 16 bits
	mov rdx, rcx
	shr rdx, 32
	add ecx, edx
	adc ecx, 0
	mov edx, ecx
	shr edx, 16
	add cx, dx
	adc cx, 0
.driverComputesChecksum:
	mov [rbx + TCP_HEADER_CHECKSUM_OFFSET], cx ; Add in checksum
	pop rdx
	pop rcx
	ret
	

; rax contains descriptor head
; edx contains dst ip address
; ecx contains UDP srcPort | (dstPort << 16)
; eax gets length, 0
; rbx gets packet base address
; IPv4 header offset is pushed to stack
; UDP header offset is pushed to stack
encodeUDPIPv4Ethernet:
	sub rsp, 16
	push rdi
	mov rdi, [rsp + 24] ; Get the return address
	mov [rsp + 8], rdi ; Put it where it needs to be after the offsets are pushed
	mov edi, ecx ; save UDP ports

	imul eax, eax, VIRTQ_DESC_SIZE
	movzx eax, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_NEXT_OFFSET]
	imul eax, eax, VIRTQ_DESC_SIZE
	mov rbx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_ADDR_OFFSET] ; Address
	xor eax, eax ; Length
	mov ecx, ETHERTYPE_IPV4
	shl rcx, 48
	or rcx, [DHCP_GATEWAY_MAC]
	call encodeEthernetPacket
	mov [rsp + 24], rax ; Save IPv4 header offset
	mov ecx, INTERNET_PROTOCOL_UDP
	call encodeIPv4Packet
	mov [rsp + 16], rax ; Save UDP header offset
	mov ecx, edi ; get ports
	call encodeUDPPacket

	pop rdi
	ret

; eax contains packet length
; edx contains dst ip address
; rbx contains packet base address
; ebx gets full length of packet
; UDP header offset is next to pop on stack
; IPv4 header offset is next after that on stack
patchUDPIPv4Ethernet:
	push rcx

	; Patch lengths and checksum
	mov rcx, [rsp + 16] ; Retrieve UDP header offset
	add rbx, rcx ; UDP header address
	sub eax, ecx ; UDP packet length
	; edx has ip address
	call patchUDPPacket
	sub rbx, rcx ; Restore base address
	add eax, ecx ; Restore full length
	mov rcx, [rsp + 24] ; Retrieve IPv4 header location
	add rbx, rcx ; IPv4 header address
	sub eax, ecx ; IPv4 length
	call patchIPv4Packet
	lea ebx, [eax + ecx] ; Restore full length

	mov rcx, [rsp + 8] ; Get current return address
	mov [rsp + 24], rcx ; Put it where it needs to be, after the header offsets are popped
	pop rcx
	add rsp, 16
	ret

; rax contains descriptor head
; edx contains dst ip address
; rcx contains TCP srcPort | (dstPort << 16) | (window << 32)
; ebx contains flags
; esi contains acknowledge number
; edi contains sequence number
; eax gets length
; rbx gets packet base address
; IPv4 header offset is pushed to stack
; TCP header offset is pushed to stack
encodeTCPIPv4Ethernet:
	sub rsp, 16
	push r8
	mov r8, rbx ; save flags
	push rsi ; save ack
	push rdi ; save seq
	mov rdi, [rsp + 40] ; Get the return address
	mov [rsp + 24], rdi ; Put it where it needs to be after the offsets are pushed
	mov rdi, rcx ; save TCP ports and window

	imul eax, eax, VIRTQ_DESC_SIZE
	movzx eax, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_NEXT_OFFSET]
	imul eax, eax, VIRTQ_DESC_SIZE
	mov rbx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_ADDR_OFFSET] ; Address
	xor eax, eax ; Length

	mov ecx, ETHERTYPE_IPV4
	shl rcx, 48
	or rcx, [DHCP_GATEWAY_MAC]
	call encodeEthernetPacket

	mov [rsp + 40], rax ; Save IPv4 header offset
	mov ecx, INTERNET_PROTOCOL_TCP
	call encodeIPv4Packet

	mov [rsp + 32], rax ; Save TCP header offset
	mov rcx, rdi ; get ports and window
	pop rdi ; retrieve seq
	pop rsi ; retrieve ack
	xchg r8d, edx ; retrieve flags
	call encodeTCPPacket
	mov edx, r8d
	pop r8
	ret

; eax contains packet length
; edx contains dst ip address
; rbx contains packet base address
; ebx gets full length of packet
; TCP header offset is next to pop on stack
; IPv4 header offset is next after that on stack
patchTCPIPv4Ethernet:
	push rcx

	; Patch lengths and checksum
	mov rcx, [rsp + 16] ; Retrieve TCP header offset
	add rbx, rcx ; TCP header address
	sub eax, ecx ; TCP packet length
	; edx has ip address
	call patchTCPPacket
	sub rbx, rcx ; Restore base address
	add eax, ecx ; Restore full length
	mov rcx, [rsp + 24] ; Retrieve IPv4 header location
	add rbx, rcx ; IPv4 header address
	sub eax, ecx ; IPv4 length
	call patchIPv4Packet
	lea ebx, [eax + ecx] ; Restore full length

	mov rcx, [rsp + 8] ; Get current return address
	mov [rsp + 24], rcx ; Put it where it needs to be, after the header offsets are popped
	pop rcx
	add rsp, 16
	ret



webtestDrillengineNet db 7,"webtest",11,"drillengine",3,"net",0
; Update src/dst mac, ipv4 id, header checksum, 
checksumTestPacket db 0,0,0,0,0,0,0,0,0,0,0,0,8,0,0x45,0,0,0x38,0,0,0,0,0x80,0x11,0x1e,0x95,0x0a,0,2,0x0f,8,8,8,8,0xec,0xf5,0,0x35,0,0x24,0xca,0xea,0x17,0xb9,1,0,0,1,0,0,0,0,0,0,6,"google",3,"com",0,0,1,0,1
%define CHECKSUM_TEST_LEN 70

; dx contains hash
dnsLookupChecksumTest:
	push rax
	push rbx
	push rcx
	push rsi
	push rdi
	
	call acquireTxPacket
	push rax
	imul eax, eax, VIRTQ_DESC_SIZE
	movzx eax, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_NEXT_OFFSET]
	imul eax, eax, VIRTQ_DESC_SIZE
	mov rbx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + eax + VIRTQ_DESC_ADDR_OFFSET] ; Address
	mov rdi, rbx
	lea rsi, [checksumTestPacket]
	mov ecx, CHECKSUM_TEST_LEN
	rep movsb
	mov eax, [DHCP_GATEWAY_MAC]
	mov [rbx], eax
	mov ax, [DHCP_GATEWAY_MAC + 4]
	mov [rbx + 4], ax
	mov eax, [VIRTIO_NET_MAC]
	mov [rbx + 6], eax
	mov ax, [VIRTIO_NET_MAC + 4]
	mov [rbx + 10], ax
	movbe cx, [IPV4_NEXT_IDENTIFICATION]
	mov [rbx + 18], cx
	add word [IPV4_NEXT_IDENTIFICATION], 1
	mov eax, [VIRTIO_NET_IP_ADDRESS]
	mov [rbx + 26], eax
	movbe [rbx + 40], dx
	mov eax, 56
	add rbx, 14
	mov word [rbx + 10], 0
	call patchIPv4Packet
	sub rbx, 14
	
	
	
	pop rax
	mov ebx, CHECKSUM_TEST_LEN
	call queueTxPacket
	
	pop rdi
	pop rsi
	pop rcx
	pop rbx
	pop rax
	ret

; rax contains string
sendUDPStr:
	push rax
	push rbx
	push rcx
	push rdx
	push rdi
	
	mov rdi, rax
	
	call acquireTxPacket
	push rax
	mov edx, 0x2144cb9f
	mov ecx, 25565 | (25565 << 16)
	call encodeUDPIPv4Ethernet
	
.encodeStringLoop:
	mov cl, [rdi]
	add rdi, 1
	mov [rbx + rax], cl
	add eax, 1
	test cl, cl
	jnz .encodeStringLoop
	
	call patchUDPIPv4Ethernet
	pop rax
	call queueTxPacket
	
	pop rdi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret
	
; rax contains integer
sendUDPInteger:
	push rax
	push rdi
	sub rsp, 32
	
	mov byte [rsp + 31], 0 ; null terminator
	lea rdi, [rsp + 31]
	call integerToString
	mov rax, rdi
	call sendUDPStr
	
	add rsp, 32
	pop rdi
	pop rax
	ret

; rax contains pointer to null terminated host string
dnsLookupBlocking:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi

	mov rsi, rax
	mov [DNS_LOOKUP_NAME_PTR], rax

	call acquireTxPacket
	push rax ; push descriptor head
	mov edx, 0x08080808 ; google dns server
	mov ecx, DNS_CLIENT_PORT | (DNS_SERVER_PORT << 16) ; srcPort | (dstPort << 16)
	call encodeUDPIPv4Ethernet

	; Add payload
	rdrand cx ; Get a random message id
	mov [DNS_LOOKUP_MESSAGE_ID], cx
	mov [rbx + rax + DNS_MESSAGE_HEADER_ID_OFFSET], cx
	mov word [rbx + rax + DNS_MESSAGE_HEADER_FLAGS_OFFSET], HTON16(DNS_MESSAGE_HEADER_FLAG_RECURSION_DESIRED | (DNS_MESSAGE_OP_STANDARD_QUERY << DNS_MESSAGE_HEADER_FLAG_OPCODE_SHIFT))
	mov word [rbx + rax + DNS_MESSAGE_HEADER_QUESTION_COUNT_OFFSET], HTON16(1)
	mov word [rbx + rax + DNS_MESSAGE_HEADER_ANSWER_COUNT_OFFSET], 0
	mov word [rbx + rax + DNS_MESSAGE_HEADER_NAMESERVER_COUNT_OFFSET], 0
	mov word [rbx + rax + DNS_MESSAGE_HEADER_ADDITIONAL_RECORD_COUNT_OFFSET], 0
	add rax, DNS_MESSAGE_HEADER_SIZE
	; We assume the domain name passed in here is well formed, since we control that
.encodeHostNameLoop:
	mov rdx, rax
	add rax, 1 ; Save start of segment and leave room for the length byte
.encodeSubHostNameLoop:
	mov cl, [rsi]
	cmp cl, '.'
	je .encodeSubHostNameBreak
	test cl, cl
	jz .encodeSubHostNameBreak
	mov [rbx + rax], cl
	add rax, 1
	add rsi, 1
	jmp .encodeSubHostNameLoop
.encodeSubHostNameBreak:
	lea rcx, [rax - 1]
	sub rcx, rdx
	mov byte [rbx + rdx], cl ; Patch the sub host name length
	cmp byte [rsi], 0
	je .encodeHostNameBreak
	add rsi, 1 ; Skip the '.'
	jmp .encodeHostNameLoop
.encodeHostNameBreak:
	mov byte [rbx + rax], 0 ; Null terminate it
	mov word [rbx + rax + 1], HTON16(DNS_TYPE_A)
	mov word [rbx + rax + 3], HTON16(DNS_CLASS_IN)
	add rax, 5

	mov edx, 0x08080808 ; google dns server
	call patchUDPIPv4Ethernet
	pop rax ; pop descriptor head
	mov dword [DNS_LOOKUP_RESULT_CODE], DNS_LOOKUP_RESULT_LOOKUP_PENDING
	call queueTxPacket

	mov rcx, [PIT_CLOCK_MILLISECONDS]
.dnsWaitLoop:
	cmp dword [DNS_LOOKUP_RESULT_CODE], DNS_LOOKUP_RESULT_LOOKUP_PENDING
	jne .dnsWaitComplete
	hlt
	mov rdx, [PIT_CLOCK_MILLISECONDS]
	sub rdx, rcx
	cmp rdx, 5000 ; 5 second timeout
	; DO times out here
.dnsWaitComplete:
	jl .dnsWaitLoop
	mov ecx, [DNS_LOOKUP_RESULT_CODE]
	mov edx, DNS_LOOKUP_RESULT_FAILURE
	cmp ecx, DNS_LOOKUP_RESULT_SUCCESS
	cmovne ecx, edx ; anything other than success is failure
	mov [DNS_LOOKUP_RESULT_CODE], ecx
	
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

sendDNSTestPacket:
	push rax
	push rbx
	push rcx
	push rdx

	call acquireTxPacket
	push rax ; push descriptor head
	mov edx, 0x08080808 ; google dns server
	mov ecx, 25565 | (DNS_SERVER_PORT << 16) ; srcPort | (dstPort << 16)
	call encodeUDPIPv4Ethernet

	; Add payload
	mov word [rbx + rax + DNS_MESSAGE_HEADER_ID_OFFSET], HTON16(32133)
	mov word [rbx + rax + DNS_MESSAGE_HEADER_FLAGS_OFFSET], HTON16(DNS_MESSAGE_HEADER_FLAG_RECURSION_DESIRED | (DNS_MESSAGE_OP_STANDARD_QUERY << DNS_MESSAGE_HEADER_FLAG_OPCODE_SHIFT))
	mov word [rbx + rax + DNS_MESSAGE_HEADER_QUESTION_COUNT_OFFSET], HTON16(1)
	mov word [rbx + rax + DNS_MESSAGE_HEADER_ANSWER_COUNT_OFFSET], 0
	mov word [rbx + rax + DNS_MESSAGE_HEADER_NAMESERVER_COUNT_OFFSET], 0
	mov word [rbx + rax + DNS_MESSAGE_HEADER_ADDITIONAL_RECORD_COUNT_OFFSET], 0
	mov rcx, [webtestDrillengineNet]
	mov [rax + rbx + DNS_MESSAGE_HEADER_SIZE], rcx
	mov rcx, [webtestDrillengineNet + 8]
	mov [rax + rbx + 20], rcx
	mov rcx, [webtestDrillengineNet + 16]
	mov [rax + rbx + 28], rcx
	mov cl, [webtestDrillengineNet + 24]
	mov [rax + rbx + 36], cl
	mov word [rax + rbx + 37], HTON16(DNS_TYPE_A)
	mov word [rax + rbx + 39], HTON16(DNS_CLASS_IN)
	add eax, 41

	call patchUDPIPv4Ethernet
	pop rax ; pop descriptor head
	call queueTxPacket

	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; eax contains current packet length
; rbx contains packet buffer address
; rcx contains bootreqest/bootreply | (xid << 8)
; dx contains flags
; eax gets updated current packet length
encodeDHCPPacket:
	push rcx
	mov [rbx + rax + DHCP_OP_OFFSET], cl
	shr rcx, 8 ; ecx now has xid
	mov byte [rbx + rax + DHCP_HTYPE_OFFSET], ARP_HARDWARE_TYPE_ETHERNET
	mov byte [rbx + rax + DHCP_HLEN_OFFSET], 6
	mov byte [rbx + rax + DHCP_HOPS_OFFSET], 0
	mov [rbx + rax + DHCP_XID_OFFSET], ecx
	mov word [rbx + rax + DHCP_SECS_OFFSET], 0
	ror dx, 8
	mov [rbx + rax + DHCP_FLAGS_OFFSET], dx
	rol dx, 8
	mov ecx, [VIRTIO_NET_IP_ADDRESS]
	mov [rbx + rax + DHCP_CIADDR_OFFSET], ecx
	mov [rbx + rax + DHCP_YIADDR_OFFSET], ecx
	mov dword [rbx + rax + DHCP_SIADDR_OFFSET], 0
	mov dword [rbx + rax + DHCP_GIADDR_OFFSET], 0
	mov rcx, [VIRTIO_NET_MAC]
	mov [rbx + rax + DHCP_CHADDR_OFFSET], rcx
	xor ecx, ecx
	mov [rbx + rax + DHCP_CHADDR_OFFSET + 8], rcx
	pxor xmm0, xmm0
	; Zero out sname
	movups [rbx + rax + DHCP_SNAME_OFFSET], xmm0
	movups [rbx + rax + DHCP_SNAME_OFFSET + 16], xmm0
	movups [rbx + rax + DHCP_SNAME_OFFSET + 32], xmm0
	movups [rbx + rax + DHCP_SNAME_OFFSET + 48], xmm0
	; Zero out file
	movups [rbx + rax + DHCP_FILE_OFFSET], xmm0
	movups [rbx + rax + DHCP_FILE_OFFSET + 16], xmm0
	movups [rbx + rax + DHCP_FILE_OFFSET + 32], xmm0
	movups [rbx + rax + DHCP_FILE_OFFSET + 48], xmm0
	movups [rbx + rax + DHCP_FILE_OFFSET + 64], xmm0
	movups [rbx + rax + DHCP_FILE_OFFSET + 80], xmm0
	movups [rbx + rax + DHCP_FILE_OFFSET + 96], xmm0
	movups [rbx + rax + DHCP_FILE_OFFSET + 112], xmm0
	; Options
	mov dword [rbx + rax + DHCP_SIZE_MIN], DHCP_OPTION_MAGIC_COOKIE
	pop rcx
	add eax, DHCP_SIZE_MIN + 4
	ret

; rcx contains random transaction id
; eax gets descriptor head number
; ebx gets packet length
getDHCPDiscoverPacket:
	push rcx
	push rdx
	push rdi

	mov rdi, rcx ; save transaction id
	call acquireTxPacket
	push rax ; push descriptor head
	mov edx, 0xFFFFFFFF ; broadcast ip
	mov ecx, DHCP_CLIENT_PORT | (DHCP_SERVER_PORT << 16) ; srcPort | (dstPort << 16)
	call encodeUDPIPv4Ethernet

	; Add payload
	mov rcx, rdi ; retrieve transaction id
	shl rcx, 8
	or rcx, DHCP_OP_BOOTREQUEST
	mov dx, DHCP_FLAG_BROADCAST
	call encodeDHCPPacket
	; Discover options
	mov word [rbx + rax], DHCP_OPTION_IP_ADDRESS_LEASE_TIME_OP | (DHCP_OPTION_IP_ADDRESS_LEASE_TIME_LEN << 8)
	mov dword [rbx + rax + 2], 0xFFFFFFFF ; Infinite lease time?
	mov word [rbx + rax + 6], DHCP_OPTION_DHCP_MESSAGE_TYPE_OP | (DHCP_OPTION_DHCP_MESSAGE_TYPE_LEN << 8)
	mov byte [rbx + rax + 8], DHCPDISCOVER
	mov byte [rbx + rax + 9], DHCP_OPTION_END
	add eax, 10

	mov edx, 0xFFFFFFFF ; broadcast ip
	call patchUDPIPv4Ethernet
	pop rax

	pop rdi
	pop rdx
	pop rcx
	ret

; rcx contains random transaction id
; eax gets descriptor head number
; ebx gets packet length
getDHCPRequestPacket:
	push rcx
	push rdx
	push rdi

	mov rdi, rcx ; save transaction id
	call acquireTxPacket
	push rax ; push descriptor head
	mov edx, 0xFFFFFFFF ; broadcast ip
	mov ecx, DHCP_CLIENT_PORT | (DHCP_SERVER_PORT << 16) ; srcPort | (dstPort << 16)
	call encodeUDPIPv4Ethernet

	; Add payload
	mov rcx, rdi ; retrieve transaction id
	shl rcx, 8
	or rcx, DHCP_OP_BOOTREQUEST
	mov dx, DHCP_FLAG_BROADCAST
	call encodeDHCPPacket
	; Request options
	mov word [rbx + rax], DHCP_OPTION_SERVER_IDENTIFIER_OP | (DHCP_OPTION_SERVER_IDENTIFIER_LEN << 8)
	mov ecx, [DHCP_SERVER_IDENTIFIER]
	mov [rbx + rax + 2], ecx
	mov word [rbx + rax + 6], DHCP_OPTION_REQUESTED_IP_ADDRESS_OP | (DHCP_OPTION_REQUESTED_IP_ADDRESS_LEN << 8)
	mov ecx, [DHCP_OFFERED_IP]
	mov [rbx + rax + 8], ecx
	mov word [rbx + rax + 12], DHCP_OPTION_DHCP_MESSAGE_TYPE_OP | (DHCP_OPTION_DHCP_MESSAGE_TYPE_LEN << 8)
	mov byte [rbx + rax + 14], DHCPREQUEST
	mov byte [rbx + rax + 15], DHCP_OPTION_END
	add eax, 16

	mov edx, 0xFFFFFFFF ; broadcast ip
	call patchUDPIPv4Ethernet
	pop rax

	pop rdi
	pop rdx
	pop rcx
	ret

; carry clear if success, set if failure
startDHCP:
	push rax
	push rbx
	push rcx

	mov dword [DHCP_GATEWAY_IP], 0
	mov dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_START
	xor ecx, ecx
	not rcx
	shr rcx, 16
	mov [DHCP_GATEWAY_MAC], rcx ; Store broadcast mac
	; mov ecx, [PIT_CLOCK] ; Best "random" number I've got right now
.getRandom:
	rdrand ecx
	jnc .getRandom ; Carry flag will be 1 if success
	; DO got here
	mov [DHCP_CURRENT_XID], ecx
	call getDHCPDiscoverPacket
	; DO got here
	mov dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_WAIT_FOR_OFFER
	call queueTxPacket
	; DO did not get here
	mov eax, [PIT_CLOCK_MILLISECONDS]
.waitForOffer:
	hlt
	; DO did not get here
	mov ebx, [PIT_CLOCK_MILLISECONDS]
	sub ebx, eax
	cmp ebx, 2000
	jge .timeout
	cmp dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_OFFER_RECEIVED
	jne .waitForOffer

	; DO got here

	; We have an offer now, see if we can accept it
	call getDHCPRequestPacket
	mov dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_PENDING_ACK
	call queueTxPacket
	mov eax, [PIT_CLOCK_MILLISECONDS]
.waitForAck:
	hlt
	mov ebx, [PIT_CLOCK_MILLISECONDS]
	sub ebx, eax
	cmp ebx, 2000
	jge .timeout
	cmp dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_DENIED
	je .timeout ; Received a nak, should try again here
	cmp dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_CONFIGURED
	jne .waitForAck
	
	mov ecx, [DHCP_OFFERED_IP]
	mov [VIRTIO_NET_IP_ADDRESS], ecx
	
	mov eax, [DHCP_GATEWAY_IP]
	call arpLookupBlocking
	jc .failure
	mov [DHCP_GATEWAY_MAC], rax
	
	; Success! We're configured!

	lea rax, [dhcpConfigurationComplete]
	call debugPrint
	
	clc
	jmp .end
.failure:
.timeout:
	mov dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_START
	stc
.end:
	pop rcx
	pop rbx
	pop rax
	ret
	
; eax contains target ip
; rax gets mac address
; carry clear if success, set if failure
arpLookupBlocking:
	push rbx
	push rcx
	
	mov [ARP_LOOKUP_TARGET_IP], eax
	mov dword [ARP_LOOKUP_CURRENT_STATE], ARP_LOOKUP_STATE_PENDING
	
	call acquireTxPacket
	imul ecx, eax, VIRTQ_DESC_SIZE
	movzx ecx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_NEXT_OFFSET]
	imul ecx, ecx, VIRTQ_DESC_SIZE
	mov rcx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_ADDR_OFFSET] ; Address
	; Setup ethernet header
	mov dword [rcx], 0xFFFFFFFF ; Dst mac 0-3
	mov word [rcx + 4], 0xFFFF ; Dst mac 4-5
	mov ebx, [VIRTIO_NET_MAC]
	mov [rcx + 6], ebx ; Src mac 0-3
	mov bx, [VIRTIO_NET_MAC + 4]
	mov [rcx + 10], bx ; Src mac 4-5
	mov word [rcx + 12], HTON16(ETHERTYPE_ARP)
	add rcx, ETHERNET_HEADER_SIZE
	mov word [rcx + ARP_HEADER_HARDWARE_TYPE_OFFSET], HTON16(ARP_HARDWARE_TYPE_ETHERNET)
	mov word [rcx + ARP_HEADER_PROTOCOL_TYPE_OFFSET], HTON16(ETHERTYPE_IPV4)
	mov byte [rcx + ARP_HEADER_HARDWARE_ADDRESS_SIZE_OFFSET], 6 ; 6 byte mac addresses
	mov byte [rcx + ARP_HEADER_PROTOCOL_ADDRESS_SIZE_OFFSET], 4 ; 4 byte IPv4 addresses
	mov word [rcx + ARP_HEADER_OPCODE_OFFSET], HTON16(ARP_OP_REQUEST)
	mov rbx, [VIRTIO_NET_MAC]
	mov [rcx + ARP_HEADER_SENDER_MAC_ADDRESS_OFFSET], rbx
	mov ebx, [VIRTIO_NET_IP_ADDRESS]
	mov [rcx + ARP_HEADER_SENDER_IP_ADDRESS_OFFSET], ebx
	xor ebx, ebx
	mov [rcx + ARP_HEADER_TARGET_MAC_ADDRESS_OFFSET], rbx
	mov ebx, [ARP_LOOKUP_TARGET_IP]
	mov [rcx + ARP_HEADER_TARGET_IP_ADDRESS_OFFSET], ebx
	mov ebx, ETHERNET_HEADER_SIZE + ARP_HEADER_SIZE
	call queueTxPacket
	
	mov eax, [PIT_CLOCK_MILLISECONDS]
.waitForResponse:
	hlt
	mov ebx, [PIT_CLOCK_MILLISECONDS]
	sub ebx, eax
	cmp ebx, 4000
	jge .timeout
	cmp dword [ARP_LOOKUP_CURRENT_STATE], ARP_LOOKUP_STATE_SUCCESS
	jne .waitForResponse
	
	mov rax, [ARP_LOOKUP_RESULT_MAC]
	clc
	jmp .end
.timeout:
	mov dword [ARP_LOOKUP_CURRENT_STATE], ARP_LOOKUP_STATE_FAILURE
	stc
.end:
	pop rcx
	pop rbx
	ret

sendTestARPPacket:
	push rax
	push rbx
	push rcx
	call acquireTxPacket
	imul ecx, eax, VIRTQ_DESC_SIZE
	movzx ecx, word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_NEXT_OFFSET]
	imul ecx, ecx, VIRTQ_DESC_SIZE
	mov rcx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_DESC_OFFSET + ecx + VIRTQ_DESC_ADDR_OFFSET]

	; Setup ethernet header
	mov dword [rcx], 0xFFFFFFFF ; Dst mac 0-3
	mov word [rcx + 4], 0xFFFF ; Dst mac 4-5
	mov ebx, [VIRTIO_NET_MAC]
	mov [rcx + 6], ebx ; Src mac 0-3
	mov bx, [VIRTIO_NET_MAC + 4]
	mov [rcx + 10], bx ; Src mac 4-5
	mov word [ecx + 12], HTON16(ETHERTYPE_ARP)
	add ecx, ETHERNET_HEADER_SIZE
	; Setup ARP header
	mov word [ecx], HTON16(ARP_HARDWARE_TYPE_ETHERNET)
	mov word [ecx + 2], HTON16(ETHERTYPE_IPV4)
	mov byte [ecx + 4], 6 ; Hardware size, mac address is 6 bytes
	mov byte [ecx + 5], 4 ; Protocol size, IPv4 is 4 bytes
	mov word [ecx + 6], HTON16(ARP_OP_REQUEST)
	mov ebx, [VIRTIO_NET_MAC]
	mov [ecx + 8], ebx ; Sender mac 0-3
	mov bx, [VIRTIO_NET_MAC + 4]
	mov [ecx + 12], bx ; Sender mac 4-5
	mov dword [ecx + 14], 0 ; Sender IP (don't have one yet)
	mov dword [ecx + 18], 0 ; Target mac 0-3
	mov word [ecx + 22], 0 ; Target mac 4-5
	mov dword [ecx + 24], (10 | (0 << 8) | (2 << 16) | (2 << 24)) ; Target IP, 10.0.2.2
	add ecx, ARP_PACKET_SIZE
	
	

	mov ebx, ETHERNET_HEADER_SIZE + ARP_PACKET_SIZE ; packet size
	call queueTxPacket
	
	; DO got here

	pop rcx
	pop rbx
	pop rax
	ret








;///// BEGIN TCP CODE /////






allocTCPBlocks:
	push rax
	push rbx
	push rcx
	push rsi
	push rdi

	mov rax, TCP_CONNECTIONS_OFFSET
	mov rbx, TCP_CONNECTIONS_OFFSET + (((TCP_BLOCK_SIZE * TCP_MAX_CONNECTIONS) + 0xFFF) & ~0xFFF)
.allocBlocksLoop:
	xor esi, esi
	call mapPhysicalToVirtual
	jc .failed
	add rax, 0x1000
	cmp rax, rbx
	jne .allocBlocksLoop

	mov rax, TCP_CONNECTION_HASH_TABLE_OFFSET
	xor esi, esi
	call mapPhysicalToVirtual
	jc .failed
	mov eax, 0xFFFFFFFF
	mov rdi, TCP_CONNECTION_HASH_TABLE_OFFSET
	mov ecx, 0x1000/4
	rep stosd

	mov ecx, TCP_TIMEOUT_QUEUE_BYTE_SIZE
	mov rax, TCP_CONNECTION_HASH_TABLE_OFFSET
	add rax, 0x1000
.timeoutQueueAllocLoop:
	xor esi, esi
	call mapPhysicalToVirtual
	jc .failed
	add rax, 0x1000
	sub ecx, 0x1000
	cmp ecx, 0
	jnle .timeoutQueueAllocLoop

	xor esi, esi
	mov rax, TCP_CONNECTIONS_OFFSET
	mov rbx, TCP_CONNECTIONS_OFFSET + TCP_BLOCK_SIZE * TCP_MAX_CONNECTIONS
	mov rcx, TCP_TIMEOUT_QUEUE_OFFSET
	mov dword [TCP_FREE_LIST_HEAD_IDX], 0
.initBlocksLoop:
	mov [rax + TCP_TCB_BLOCK_IDX_OFFSET], esi
	mov [rax + TCP_TCB_TIMER_QUEUE_IDX_OFFSET], esi
	mov dword [rax + TCP_TCB_NEXT_CHECK_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	mov [rcx + rsi * 2], si
	add esi, 1
	mov [rax + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], esi
	mov dword [rax + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], TCP_SEND_QUEUE_INVALID_NEXT_PTR

	; Set send and receive buffers
	lea rdi, [rax + TCP_TCB_SIZE]
	mov [rax + TCP_TCB_SEND_BUFFER_OFFSET], rdi
	add rdi, (TCP_BLOCK_SIZE - TCP_TCB_SIZE) / 2
	mov [rax + TCP_TCB_RECEIVE_BUFFER_OFFSET], rdi
	mov edi, (TCP_BLOCK_SIZE - TCP_TCB_SIZE) / 2
	mov dword [rax + TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET], edi
	mov dword [rax + TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET], edi
	
	; Debug low receive buffer test
	;mov dword [rax + TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET], 64

	add rax, TCP_BLOCK_SIZE
	cmp rax, rbx
	jne .initBlocksLoop
	mov dword [rbx - TCP_BLOCK_SIZE + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], TCP_CONNECTION_BLOCK_INVALID_IDX

	mov dword [TCP_SEND_QUEUE_HEAD], TCP_CONNECTION_BLOCK_INVALID_IDX
	mov dword [TCP_SEND_QUEUE_TAIL], TCP_CONNECTION_BLOCK_INVALID_IDX
	mov dword [TCP_NOTIFY_QUEUE_HEAD], TCP_CONNECTION_BLOCK_INVALID_IDX
	
	mov dword [TCP_IS_INITIALIZED], 1

	clc
	jmp .end
.failed:
	stc
.end:

	pop rdi
	pop rsi
	pop rcx
	pop rbx
	pop rax
	ret

; rcx contains packed srcIP | (dstIP << 32)
; edx contains packed srcPort | (dstPort << 16)
; ebx gets the hash table index of 
tcpHashGetIdx:
	push rax
	push rsi
	push rdi

	mov eax, 0xFFFFFFFF
	crc32 rax, rcx
	crc32 eax, edx
	and eax, TCP_CONNECTION_HASH_TABLE_MASK
	mov ebx, eax
.lookForSlot:
	mov rsi, TCP_CONNECTION_HASH_TABLE_OFFSET
	movzx esi, word [rsi + rbx * 2]
	cmp esi, TCP_CONNECTION_HASH_EMPTY
	je .lookForSlotBreak
	imul esi, esi, TCP_BLOCK_SIZE
	mov rdi, TCP_CONNECTIONS_OFFSET
	add rdi, rsi
	cmp dword [rdi + TCP_TCB_CONNECTION_INFO_PORTS_OFFSET], edx
	jne .slotWasWrong
	cmp qword [rdi + TCP_TCB_CONNECTION_INFO_IPS_OFFSET], rcx
	je .lookForSlotBreak
.slotWasWrong:
	add ebx, 1
	and ebx, TCP_CONNECTION_HASH_TABLE_MASK
	cmp eax, ebx
	jne .lookForSlot
.lookForSlotFailure:
	mov ebx, TCP_CONNECTION_HASH_INVALID_IDX
.lookForSlotBreak:

	pop rdi
	pop rsi
	pop rax
	ret

; rcx:edx contains ConnectionInfo struct
; ebx gets the connection index
tcpHashLookup:
	call tcpHashGetIdx
	cmp ebx, TCP_CONNECTION_HASH_INVALID_IDX
	je .invalidIdx
	push rax
	mov rax, TCP_CONNECTION_HASH_TABLE_OFFSET
	movzx ebx, word [rax + rbx * 2]
	pop rax
	ret
.invalidIdx:
	mov ebx, TCP_CONNECTION_HASH_EMPTY
	ret

; rcx contains packed srcIP | (dstIP << 32)
; edx contains packed srcPort | (dstPort << 16)
; ebx contains the connection index to be inserted
; carry set if fail, clear if success
tcpHashInsert:
	push rax
	push rsi
	mov esi, ebx
	call tcpHashGetIdx
	cmp ebx, TCP_CONNECTION_HASH_INVALID_IDX
	je .tableFull
	mov rax, TCP_CONNECTION_HASH_TABLE_OFFSET
	mov [rax + rbx * 2], si
	clc
	jmp .end
.tableFull:
	stc
.end:
	mov ebx, esi
	pop rsi
	pop rax
	ret

; rcx contains packed srcIP | (dstIP << 32)
; edx contains packed srcPort | (dstPort << 16)
tcpHashDelete:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi

	call tcpHashGetIdx
	cmp ebx, TCP_CONNECTION_HASH_INVALID_IDX
	je .end

	mov eax, ebx
.displacedElementCleanupLoop:
	add eax, 1
	and eax, TCP_CONNECTION_HASH_TABLE_MASK
	cmp eax, ebx
	je .displacedElementCleanupBreak
	mov rsi, TCP_CONNECTION_HASH_TABLE_OFFSET
	movzx esi, word [rsi + rax * 2]
	cmp si, TCP_CONNECTION_HASH_EMPTY
	je .displacedElementCleanupBreak
	imul esi, esi, TCP_BLOCK_SIZE
	mov rdx, TCP_CONNECTIONS_OFFSET
	add rdx, rsi
	mov ecx, 0xFFFFFFFF
	crc32 rcx, qword [rdx + TCP_TCB_REMOTE_IP_OFFSET]
	crc32 ecx, dword [rdx + TCP_TCB_REMOTE_PORT_OFFSET]
	and ecx, TCP_CONNECTION_HASH_TABLE_MASK
	cmp eax, ebx
	jl .checkIdxWrapped
	cmp ecx, ebx
	jle .fixDisplacedElement
	cmp ecx, eax
	jg .fixDisplacedElement
	jmp .displacedElementCleanupLoop
.checkIdxWrapped:
	cmp ecx, ebx
	jg .displacedElementCleanupLoop
	cmp ecx, eax
	jle .displacedElementCleanupLoop
.fixDisplacedElement:
	mov rdx, TCP_CONNECTION_HASH_TABLE_OFFSET
	mov si, [rdx + rax * 2]
	mov [rdx + rbx * 2], si
	mov ebx, eax
	jmp .displacedElementCleanupLoop
.displacedElementCleanupBreak:
	mov rdx, TCP_CONNECTION_HASH_TABLE_OFFSET
	mov word [rdx + rbx * 2], TCP_CONNECTION_HASH_EMPTY

.end:
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; ebx gets connection block index, TCP_CONNECTION_BLOCK_INVALID_IDX if failure
tcpAlloc:
	push rax
	push rcx

	mov ebx, [TCP_FREE_LIST_HEAD_IDX]
	cmp ebx, TCP_CONNECTION_BLOCK_INVALID_IDX
	je .end
	DEBUG_SEND_STR tcpAllocStr
	DEBUG_SEND_NUM ebx
	imul eax, ebx, TCP_BLOCK_SIZE
	mov rcx, TCP_CONNECTIONS_OFFSET
	add rcx, rax
	mov eax, [rcx + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET]
	mov dword [rcx + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], TCP_NOTIFY_QUEUE_INVALID_NEXT_PTR
	mov [TCP_FREE_LIST_HEAD_IDX], eax
	

.end:
	pop rcx
	pop rax
	ret

tcpNotifyQueuePtrWasStillValidStr db "TCP Notify Queue Ptr Still valid, this could be bad!",0x0a,0x00
tcpAllocStr db "TCPAlloc",0x0a,0x00
tcpFreeStr db "TCPFree",0x0a,0x00

; ebx contains connection block index
tcpFree:
	push rax
	push rcx
	pushf
	cli

	mov rax, TCP_CONNECTIONS_OFFSET
	imul ecx, ebx, TCP_BLOCK_SIZE
	add rax, rcx
	mov ecx, [TCP_FREE_LIST_HEAD_IDX]
	cmp dword [rax + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], TCP_NOTIFY_QUEUE_INVALID_NEXT_PTR
	je .nextPtrOk
	DEBUG_PRINT_STR tcpNotifyQueuePtrWasStillValidStr
	DEBUG_SEND_STR tcpNotifyQueuePtrWasStillValidStr
.nextPtrOk:
	DEBUG_SEND_STR tcpFreeStr
	mov [rax + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], ecx
	mov [TCP_FREE_LIST_HEAD_IDX], ebx

	pop rcx
	test ecx, EFLAGS_INTERRUPT_FLAG
	jz .end
	sti
.end:
	pop rcx
	pop rax
	ret

; TODO: CHECK IF INVALID_TIMEOUT_VALUE WORKS RIGHT
; ebx contains block index
tcpTimerPriorityQueueUpdateElement:
	; This is a high register pressure function
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14

	mov r12, TCP_TIMEOUT_QUEUE_OFFSET
	mov r11, TCP_CONNECTIONS_OFFSET
	movzx eax, word [r12 + rbx * 2] ; blockIdx
	imul ecx, eax, TCP_BLOCK_SIZE
	add rcx, r11 ; block

	mov edx, [rcx + TCP_TCB_NEXT_CHECK_TIME_OFFSET] ; blockNextCheckTime
	mov esi, [PIT_CLOCK_MILLISECONDS]
	add esi, 2000000000
	cmp edx, TCP_INVALID_TIMEOUT_VALUE
	cmove edx, esi ; if invalid, make it current time + 2000000000, should float it to the bottom

	; Float up to its position in the priority queue
.floatUpLoop:
	test ebx, ebx
	jz .floatUpLoopBreak
	lea esi, [ebx - 1]
	shr esi, 1 ; parentIdx
	movzx edi, word [r12 + rsi * 2] ; parentBlockIdx
	imul r8d, edi, TCP_BLOCK_SIZE
	add r8, r11 ; parentBlock
	mov r9d, [r8 + TCP_TCB_NEXT_CHECK_TIME_OFFSET]
	mov r10d, [PIT_CLOCK_MILLISECONDS]
	add r10d, 2000000000 ; If invalid, make it some arbitrarily far point in the future
	cmp r9d, TCP_INVALID_TIMEOUT_VALUE
	cmove r9d, r10d
	mov r10d, edx
	sub r10d, r9d
	cmp r10d, 0 ; blockNextCheckTime >= parentBlockEndMillis
	jge .floatUpLoopBreak
	; swap parent and child
	mov [r12 + rbx * 2], di ; timeoutQueue[idx] = parentBlockIdx
	mov [r12 + rsi * 2], ax ; timeoutQueue[parentIdx] = blockIdx
	mov [rcx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET], si ; block.timeoutQueueIdx = parentIdx
	mov [r8 + TCP_TCB_TIMER_QUEUE_IDX_OFFSET], bx ; parentBlock.timeoutQueueIdx = idx
	mov ebx, esi ; idx = parentIdx
	jmp .floatUpLoop
.floatUpLoopBreak:
	
	; Float down to its position in the priority queue
.floatDownLoop:
	lea edi, [ebx * 2 + 1] ; firstChildIdx
	lea esi, [ebx * 2 + 2] ; secondChildIdx
	cmp edi, TCP_MAX_CONNECTIONS
	jge .floatDownLoopBreak
	movzx r8d, word [r12 + rdi * 2] ; firstChildBlockIdx
	imul r9d, r8d, TCP_BLOCK_SIZE
	add r9, r11 ; firstChildBlock
	mov r13d, [r9 + TCP_TCB_NEXT_CHECK_TIME_OFFSET] ; firstChildEndMillis
	movzx r10d, word [r12 + rsi * 2] ; secondChildBlockIdx
	imul r12d, r10d, TCP_BLOCK_SIZE
	add r11, r12 ; secondChildBlock
	mov r12d, [r11 + TCP_TCB_NEXT_CHECK_TIME_OFFSET] ; secondChildEndMillis
	mov r14d, [PIT_CLOCK_MILLISECONDS]
	add r14d, 2000000000 ; If invalid, make it some arbitrarily far point in the future
	cmp r13d, TCP_INVALID_TIMEOUT_VALUE
	cmove r13d, r14d
	cmp r12d, TCP_INVALID_TIMEOUT_VALUE
	cmove r12d, r14d

	cmp esi, TCP_MAX_CONNECTIONS ; secondChildIdx < timeoutQueueSize
	jnl .floatDownLoopCheckSwapWithFirstChild
	mov r14d, r12d ; secondChildEndMillis
	sub r14d, r13d ; firstChildEndMillis
	cmp r14d, 0 ; secondChildEndMillis < firstChildEndMillis
	jnl .floatDownLoopCheckSwapWithFirstChild
	sub r12d, edx
	cmp r12d, 0 ; secondChildEndMillis < blockNextCheckTime
	jnl .floatDownLoopCheckSwapWithFirstChild
	mov r12, TCP_TIMEOUT_QUEUE_OFFSET
	mov [r12 + rbx * 2], r10w
	mov [r12 + rsi * 2], ax
	mov [rcx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET], si
	mov [r11 + TCP_TCB_TIMER_QUEUE_IDX_OFFSET], bx
	mov ebx, esi
	mov r11, TCP_CONNECTIONS_OFFSET
	jmp .floatDownLoop
.floatDownLoopCheckSwapWithFirstChild:
	sub r13d, edx
	cmp r13d, 0 ; firstChildEndMillis < blockNextCheckTime
	jnl .floatDownLoopBreak
	mov r12, TCP_TIMEOUT_QUEUE_OFFSET
	mov [r12 + rbx * 2], r8w
	mov [r12 + rdi * 2], ax
	mov [rcx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET], di
	mov [r9 + TCP_TCB_TIMER_QUEUE_IDX_OFFSET], bx
	mov ebx, edi
	mov r11, TCP_CONNECTIONS_OFFSET
	jmp .floatDownLoop
.floatDownLoopBreak:

	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret
	
tcpDeleteStr db "TCPDelete",0x0a,0x00

; rbx contains block address
; ebx ges old block index
tcpDelete:
	push rcx
	push rdx
	DEBUG_SEND_STR tcpDeleteStr

	mov dword [rbx + TCP_TCB_NOTIFY_FLAGS_OFFSET], 0
	mov dword [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	mov rcx, rbx
	mov ebx, [rbx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET]
	call tcpTimerPriorityQueueUpdateElement
	mov rbx, rcx
	; Load ConnectionInfo struct
	mov rcx, [rbx + TCP_TCB_REMOTE_IP_OFFSET]
	mov edx, [rbx + TCP_TCB_REMOTE_PORT_OFFSET]
	call tcpHashDelete
	mov ebx, [rbx + TCP_TCB_BLOCK_IDX_OFFSET]
	call tcpFree

	pop rdx
	pop rcx
	ret

; rbx contains block address
; ecx contains flags
tcpMarkForDelete:
	push rax
	push rcx
	push rdx

	mov eax, ecx ; save flags
	mov dword [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	mov dword [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	mov rcx, rbx
	mov ebx, [rbx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET]
	call tcpTimerPriorityQueueUpdateElement
	mov rbx, rcx
	; Load ConnectionInfo struct
	mov rcx, [rbx + TCP_TCB_REMOTE_IP_OFFSET]
	mov edx, [rbx + TCP_TCB_REMOTE_PORT_OFFSET]
	call tcpHashDelete
	mov ecx, eax ; get flags back
	call tcpNotify
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_CLOSED

	pop rdx
	pop rcx
	pop rax
	ret

tcpNotifyStr db "TCP Notify Call",0x0a,0x00

; To be called only from an interrupt
; rbx contains block address
; ecx contains flags
tcpNotify:
	push rdx

	;DEBUG_PRINT_STR tcpNotifyStr
	or [rbx + TCP_TCB_NOTIFY_FLAGS_OFFSET], ecx
	cmp dword [rbx + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], TCP_NOTIFY_QUEUE_INVALID_NEXT_PTR
	jne .alreadyOnNotifyQueue
	mov edx, [TCP_NOTIFY_QUEUE_HEAD]
	mov [rbx + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], edx
	mov edx, [rbx + TCP_TCB_BLOCK_IDX_OFFSET]
	mov [TCP_NOTIFY_QUEUE_HEAD], edx
.alreadyOnNotifyQueue:

	pop rdx
	ret

sendingFinStr db "Sending FIN",0x0a,0x00

; rbx contains block address
; ecx contains max packets
; edx contains congestionWindow
; carry clear if sent data, set if did not send data
tcpSendData:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	mov r13, [rsp + 12 * 8]

	xor r8d, r8d ; didSendData = false
	mov r9d, [rbx + TCP_TCB_SEND_WINDOW_OFFSET]
	cmp r9d, edx
	cmovg r9d, edx ; window
	mov r10d, ecx ; maxPackets
	mov r11, rbx
	mov r12d, [r11 + TCP_TCB_RESEND_NEXT_OFFSET]
	add r12d, [r11 + TCP_TCB_SEND_DATA_SIZE_OFFSET]
	mov ecx, [r11 + TCP_TCB_CLOSE_REQUESTED_OFFSET]
	mov edx, ecx
	shr ecx, 1 ; get TCP_TCB_CLOSE_REQUESTED_FIN_SENT_BIT
	xor ecx, 1 ; invert it to get 1 if fin was not yet sent
	and ecx, edx ; and it with TCP_TCB_CLOSE_REQUESTED_REQUEST_BIT. If close requested and not fin sent yet, lastSendSeq += 1
	mov r13d, ecx
	add r12d, ecx ; lastSendSeq
	jmp .sendDataLoopContinue
.sendDataLoop:
	mov ecx, [r11 + TCP_TCB_SEND_DATA_SIZE_OFFSET]
	mov esi, [r11 + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	cmp ecx, esi
	cmovg ecx, esi ; dataTransfer = min(sendDataSize, sendMaximumSegmentSize)
	mov edi, [r11 + TCP_TCB_RESEND_NEXT_OFFSET]
	mov edx, r9d
	sub edx, edi
	add edx, [r11 + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp ecx, edx
	cmovg ecx, edx ; dataTransfer = min(dataTransfer, window - (resendNext - sendUnacknowledged))
	mov edx, edi ; next seq to use = resendNext
	add edi, ecx ; resendNext += dataTransfer
	sub [r11 + TCP_TCB_SEND_DATA_SIZE_OFFSET], ecx

	mov ebx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	cmp dword [r11 + TCP_TCB_CLOSE_REQUESTED_OFFSET], TCP_TCB_CLOSE_REQUESTED_REQUEST_BIT
	jne .noConnectionFin ; no fin if not requested or already sent
	cmp dword [r11 + TCP_TCB_SEND_DATA_SIZE_OFFSET], 0
	jne .noConnectionFin
	mov ebx, TCP_HEADER_FLAG_ACKNOWLEDGE | TCP_HEADER_FLAG_FINALIZE ; flags
	or dword [r11 + TCP_TCB_CLOSE_REQUESTED_OFFSET], TCP_TCB_CLOSE_REQUESTED_FIN_SENT_BIT
	add edi, 1
	;DEBUG_PRINT_STR sendingFinStr
.noConnectionFin:
	
	mov [r11 + TCP_TCB_RESEND_NEXT_OFFSET], edi
	mov esi, edi
	mov eax, [r11 + TCP_TCB_SEND_NEXT_OFFSET]
	sub esi, eax
	cmp esi, 0 ; resendNext >= sendNext
	cmovge eax, edi
	mov [r11 + TCP_TCB_SEND_NEXT_OFFSET], eax


	mov edi, edx ; seq
	push r12 ; save lastSendSeq
	mov r12, rcx ; save dataTransfer
	movzx edx, word [r11 + TCP_TCB_REMOTE_PORT_OFFSET] ; dst port
	shl edx, 16
	mov dx, [r11 + TCP_TCB_LOCAL_PORT_OFFSET] ; src port
	mov eax, [r11 + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	mov ecx, 0xFFFF
	cmp ecx, eax
	cmovg ecx, eax
	shl rcx, 32
	or rcx, rdx ; rcx = srcPort | (dstPort << 16) | (window << 32)

	call acquireTxPacket
	push rax ; push descriptor head

	mov edx, [r11 + TCP_TCB_REMOTE_IP_OFFSET]
	mov esi, [r11 + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	call encodeTCPIPv4Ethernet

	lea rdi, [rbx + rax] ; dst = packetBase + packetLength
	mov esi, [r11 + TCP_TCB_SEND_DATA_POS_OFFSET]
	add rsi, [r11 + TCP_TCB_SEND_BUFFER_OFFSET] ; src = sendBuffer + sendDataPos
	mov rcx, r12 ; retrieve dataTransfer
	add eax, ecx ; packet.length += dataTransfer
	mov edx, [r11 + TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET]
	sub edx, [r11 + TCP_TCB_SEND_DATA_POS_OFFSET] ; dataUntilEnd
	cmp ecx, edx
	jnle .needsTwoCopies
	; memcpy(packet, block.sendBuffer + block.sendDataPos, dataTransfer)
	rep movsb
	add [r11 + TCP_TCB_SEND_DATA_POS_OFFSET], ecx
	jmp .needsOneCopy
.needsTwoCopies:
	xchg ecx, edx ; copy size = dataUntilEnd
	rep movsb
	mov rsi, [r11 + TCP_TCB_SEND_BUFFER_OFFSET] ; src = sendBuffer
	sub edx, ecx
	mov ecx, edx ; copy size = dataTransfer - dataUntilEnd
	rep movsb
	mov [r11 + TCP_TCB_SEND_DATA_POS_OFFSET], ecx
.needsOneCopy:
	
	mov edx, [r11 + TCP_TCB_REMOTE_IP_OFFSET]
	call patchTCPIPv4Ethernet
	pop rax ; pop descriptor head
	call queueTxPacket

	mov r8d, 1 ; didSendData = true

	; Start round trip timer if it isn't already running
	mov ecx, [r11 + TCP_TCB_ROUND_TRIP_TIMER_OFFSET]
	cmp ecx, -1
	jne .roundTripTimerAlreadyRunning
	mov ecx, [PIT_CLOCK_MILLISECONDS]
	lea edx, [ecx + 1]
	cmp ecx, -1
	cmove ecx, edx
	mov [r11 + TCP_TCB_ROUND_TRIP_TIMER_OFFSET], ecx
	mov ecx, [r11 + TCP_TCB_RESEND_NEXT_OFFSET]
	mov [r11 + TCP_TCB_ROUND_TRIP_TIMER_SEQUENCE_NUMBER_OFFSET], ecx
.roundTripTimerAlreadyRunning:

	pop r12 ; retrieve lastSendSeq
	sub r10d, 1 ; maxPackets--
.sendDataLoopContinue:
	cmp word [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET], 0
	je .sendDataLoopBreak
	test r10d, r10d ; maxPackets == 0
	jz .sendDataLoopBreak
	mov esi, [r11 + TCP_TCB_RESEND_NEXT_OFFSET]
	cmp r12d, esi
	je .sendDataLoopBreak
	sub esi, [r11 + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp esi, r9d
	jl .sendDataLoop
.sendDataLoopBreak:
	
	cmp [r11 + TCP_TCB_SEND_NEXT_OFFSET], r12d ; sendNext == lastSendSeq
	jne .stateUpdateNotNeeded
	cmp dword [r11 + TCP_TCB_CLOSE_REQUESTED_OFFSET], 0 ; closeRequested != 0
	je .stateUpdateNotNeeded
	mov rbx, r11
	mov ecx, [r11 + TCP_TCB_STATE_OFFSET]
	mov edx, TCP_STATE_FIN_WAIT_1;
	cmp ecx, TCP_STATE_ESTABLISHED
	cmove ecx, edx
	mov edx, TCP_STATE_LAST_ACK
	cmp ecx, TCP_STATE_CLOSE_WAIT
	cmove ecx, edx
	mov [r11 + TCP_TCB_STATE_OFFSET], ecx
.stateUpdateNotNeeded:

	
	; See if we should queue it for more data sending later
	cmp dword [r11 + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], TCP_SEND_QUEUE_INVALID_NEXT_PTR
	jne .shouldNotQueueBlockForMoreTransmission ; Already on queue
	cmp [r11 + TCP_TCB_RESEND_NEXT_OFFSET], r12d ; resendNext != lastSendSeq
	je .shouldNotQueueBlockForMoreTransmission
	mov ecx, [r11 + TCP_TCB_RESEND_NEXT_OFFSET]
	sub ecx, [r11 + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp ecx, r9d ; (resendNext - sendUnacknowledged) < window
	jnl .shouldNotQueueBlockForMoreTransmission
	; Queue it up so that a call to tcpTrySendAllData can deal with the rest of it when more tx packets are available
	mov ecx, [TCP_SEND_QUEUE_HEAD]
	cmp ecx, TCP_CONNECTION_BLOCK_INVALID_IDX
	jne .atLeastOneItemOnQueue
	mov ecx, [r11 + TCP_TCB_BLOCK_IDX_OFFSET]
	mov [r11 + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], ecx
	mov [TCP_SEND_QUEUE_HEAD], ecx
	mov [TCP_SEND_QUEUE_TAIL], ecx
	jmp .noItemsCurrentlyOnQueue
.atLeastOneItemOnQueue:
	mov [r11 + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], ecx
	mov ecx, [r11 + TCP_TCB_BLOCK_IDX_OFFSET]
	mov edi, [TCP_SEND_QUEUE_TAIL]
	imul edi, edi, TCP_BLOCK_SIZE
	mov rsi, TCP_CONNECTIONS_OFFSET
	add rdi, rsi
	mov [rdi + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], ecx
	mov [TCP_SEND_QUEUE_TAIL], ecx
.noItemsCurrentlyOnQueue:
.shouldNotQueueBlockForMoreTransmission:

	test r8d, r8d ; didSendData != 0
	jz .didNotSendData
	clc
	jmp .end
.didNotSendData:
	stc
.end:
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; Called from tx packet available interrupt
tcpTrySendAllData:
	push rax
	push rbx
	push rcx
	push rdx

	jmp .loopContinue
.loop:
	; ebx is tcpSendQueueHead
	imul ebx, ebx, TCP_BLOCK_SIZE
	mov rax, TCP_CONNECTIONS_OFFSET
	add rbx, rax ; block
	mov eax, [rbx + TCP_TCB_STATE_OFFSET]
	cmp eax, TCP_STATE_CLOSED
	je .removeSendQueueBlock
	cmp eax, TCP_STATE_LISTEN
	je .removeSendQueueBlock

	mov ecx, 1
	mov edx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	call tcpSendData

	cmp dword [rbx + TCP_TCB_SEND_DATA_SIZE_OFFSET], 0
	je .removeSendQueueBlock
	mov eax, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	sub eax, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	mov ecx, [rbx + TCP_TCB_SEND_WINDOW_OFFSET]
	cmp ecx, edx
	cmovg ecx, edx ; ecx = min(congestionWindow, sendWindow)
	cmp eax, ecx ; (sendNext - sendUnacknowledged) >= min(congestionWindow, sendWindow)
	jge .removeSendQueueBlock
	; Rotate send blocks (we try to send them in a round robin fashion)
	mov eax, [TCP_SEND_QUEUE_HEAD]
	mov [TCP_SEND_QUEUE_TAIL], eax
	mov eax, [rbx + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET]
	mov [TCP_SEND_QUEUE_HEAD], eax
	jmp .loopContinue
.removeSendQueueBlock:
	mov eax, [TCP_SEND_QUEUE_HEAD]
	mov ecx, [TCP_SEND_QUEUE_TAIL]
	cmp eax, ecx
	jne .removeNotSamePtr
	mov dword [rbx + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], TCP_SEND_QUEUE_INVALID_NEXT_PTR
	mov eax, TCP_CONNECTION_BLOCK_INVALID_IDX
	mov [TCP_SEND_QUEUE_HEAD], eax
	mov [TCP_SEND_QUEUE_TAIL], eax
	jmp .loopContinue
.removeNotSamePtr:
	mov eax, [rbx + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET]
	mov dword [rbx + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], TCP_SEND_QUEUE_INVALID_NEXT_PTR
	mov [TCP_SEND_QUEUE_HEAD], eax
	imul ecx, ecx, TCP_BLOCK_SIZE
	mov rbx, TCP_CONNECTIONS_OFFSET
	add rcx, rbx
	mov [rcx + TCP_TCB_SEND_QUEUE_NEXT_PTR_OFFSET], eax
.loopContinue:
	mov bx, [VIRTIO_NET_TX_QUEUE_OFFSET + VIRTQ_NUM_FREE_DESCRIPTORS_OFFSET]
	test bx, bx
	jz .loopBreak
	mov ebx, [TCP_SEND_QUEUE_HEAD]
	cmp ebx, TCP_CONNECTION_BLOCK_INVALID_IDX
	jne .loop
.loopBreak:

	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; To be called only from user code
; ebx contains block idx
tcpClose:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi

	and ebx, 0xFFFF
	cmp bx, TCP_MAX_CONNECTIONS
	jae .end

	cli

	imul ebx, ebx, TCP_BLOCK_SIZE
	mov rcx, TCP_CONNECTIONS_OFFSET
	add rbx, rcx
	mov eax, [rbx + TCP_TCB_STATE_OFFSET]
	;DEBUG_PRINT_NUM 9898989
	;call debugPrintlnInteger

	cmp eax, TCP_STATE_SYN_SENT
	jne .stateNotSynSent
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_CLOSED
	call tcpDelete
	jmp .end
.stateNotSynSent:
	
	cmp eax, TCP_STATE_SYN_RECEIVED
	jne .stateNotSynReceived
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE | TCP_HEADER_FLAG_FINALIZE ; flags
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_FIN_WAIT_1
	mov ecx, 3000 ; 3 seconds
	call tcpResetDeletionTimeout
.stateNotSynReceived:

	cmp eax, TCP_STATE_ESTABLISHED
	je .stateEstablishedOrCloseWait
	cmp eax, TCP_STATE_CLOSE_WAIT
	jne .stateNotEstablishedNorCloseWait
.stateEstablishedOrCloseWait:
	or dword [rbx + TCP_TCB_CLOSE_REQUESTED_OFFSET], TCP_TCB_CLOSE_REQUESTED_REQUEST_BIT ; closeRequested = true
	mov ecx, 5000 ; 5 seconds
	call tcpResetDeletionTimeout
	; send_data will go to FIN_WAIT_1 (if ESTABLISHED) or LAST_ACK (if CLOSE_WAIT) if it runs out of data to send and closeRequested is true
	mov ecx, 0xFFFFFFFF
	mov edx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	call tcpSendData
.stateNotEstablishedNorCloseWait:

.end:
	sti
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; To be called only from user space
; ebx contains block index
; rcx contains buffer address
; edx contains buffer size
; edx gets sent size
tcpSend:
	push rax
	push rcx
	push rsi
	push rdi

	and ebx, 0xFFFF
	cmp bx, TCP_MAX_CONNECTIONS
	jae .noDataSent

	cmp edx, 0
	jle .noDataSent

	imul ebx, ebx, TCP_BLOCK_SIZE
	mov rax, TCP_CONNECTIONS_OFFSET
	add rbx, rax

	cmp dword [rbx + TCP_TCB_CLOSE_REQUESTED_OFFSET], 0
	jne .noDataSent ; don't try sending anything if the connection is closed

	cli

	; Max data we can put in the send buffer without overwriting other data that needs to be sent
	mov eax, [rbx + TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET]
	sub eax, [rbx + TCP_TCB_SEND_DATA_SIZE_OFFSET]
	sub eax, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	add eax, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp edx, eax
	cmovg edx, eax ; sendSize = min(size, sendBufferSize - sendDataSize - (sendNext - sendUnacknowledged))
	cmp edx, 0
	jle .noDataSent

	call tlbVerifyUserBuffer
	; TODO replace after testing
	;jc .noDataSent

	mov eax, [rbx + TCP_TCB_SEND_DATA_POS_OFFSET]
	add eax, [rbx + TCP_TCB_SEND_DATA_SIZE_OFFSET] ; writePos
	mov edi, [rbx + TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET]
	mov esi, eax
	sub esi, edi
	cmp eax, edi ; writePos >= sendBufferCapacity
	cmovge eax, esi ; writePos = writePos >= sendBufferSize ? writePos - sendBufferSize : writePos
	mov rsi, rcx ; src = buffer
	mov rdi, [rbx + TCP_TCB_SEND_BUFFER_OFFSET]
	add rdi, rax ; dst = sendBuffer + writePos
	mov ecx, [rbx + TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET]
	sub ecx, eax ; dataUntilEnd
	mov eax, edx ; save sendSize
	add [rbx + TCP_TCB_SEND_DATA_SIZE_OFFSET], edx ; sendDataSize += sendSize
	cmp edx, ecx ; sendSize <= dataUntilEnd
	jnle .twoCopiesNeeded
	; memcpy(block.sendBuffer + writePos, buffer, sendSize)
	mov ecx, edx
	rep movsb
	jmp .oneCopyNeeded
.twoCopiesNeeded:
	; memcpy(block.sendBuffer + writePos, buffer, dataUntilEnd)
	rep movsb
	; memcpy(block.sendBuffer, buffer + dataUntilEnd, sendSize - dataUntilEnd)
	; src = buffer + dataUntilEnd
	mov rdi, [rbx + TCP_TCB_SEND_BUFFER_OFFSET] ; dst = sendBuffer
	sub edx, ecx ; edx = sendSize - dataUntilEnd
	mov ecx, edx ; copy size = edx
	rep movsb
.oneCopyNeeded:

	mov ecx, 0xFFFFFFFF
	mov edx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	call tcpSendData

	jmp .end
.noDataSent:
	xor eax, eax ; sendSize = 0
.end:
	mov edx, eax ; get sendSize
	sti
	pop rdi
	pop rsi
	pop rcx
	pop rax
	ret

; ebx contains blockIdx
tcpActiveOpen:
	push rax
	push rbx
	push rcx
	push rdx

	imul eax, ebx, TCP_BLOCK_SIZE
	mov rcx, TCP_CONNECTIONS_OFFSET
	add rax, rcx
	; Load ConnectionInfo
	mov rcx, [rax + TCP_TCB_CONNECTION_INFO_IPS_OFFSET]
	mov edx, [rax + TCP_TCB_CONNECTION_INFO_PORTS_OFFSET]
	call tcpHashInsert
	mov rbx, rax
	call tcpBlockInit
	mov ecx, TCP_HEADER_FLAG_SYNCHRONIZE
	call tcpSendSYNPacket
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_SYN_SENT

	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret


; ecx comtains remote ip
; edx contains remotePort | (localPort << 16)
; ebx gets block idx, or TCP_CONNECTION_BLOCK_INVALID_IDX if failure
tcpOpen:
	push rax
	push rsi
	push rdi

	mov ebx, [VIRTIO_NET_IP_ADDRESS]
	shl rbx, 32
	or rcx, rbx
	call tcpHashLookup
	cmp ebx, TCP_CONNECTION_HASH_EMPTY
	jne .connectionDidNotAlreadyExist
	mov rsi, TCP_CONNECTIONS_OFFSET
	imul eax, ebx, TCP_BLOCK_SIZE
	add rax, rsi
	mov esi, [rax + TCP_TCB_STATE_OFFSET]
	cmp esi, TCP_STATE_CLOSED
	je .resetNotNeeded
	cmp esi, TCP_STATE_LISTEN
	je .resetNotNeeded
	; Instead of returning "connection already exists", we'll just reset it.
	; This isn't a general purpose implementation, and we'll probably never get here anyway.
	push rbx
	push rdx
	mov rbx, rax ; block
	mov edx, TCP_HEADER_FLAG_RESET ; flags
	xor esi, esi ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	pop rdx
	pop rbx
.resetNotNeeded:
	mov dword [rax + TCP_TCB_STATE_OFFSET], TCP_STATE_CLOSED
.connectionDidNotAlreadyExist:

	cmp ebx, TCP_CONNECTION_BLOCK_INVALID_IDX
	jne .blockAllocated
	call tcpAlloc
.blockAllocated:

	cmp ebx, TCP_CONNECTION_BLOCK_INVALID_IDX
	je .end

	mov rsi, TCP_CONNECTIONS_OFFSET
	imul eax, ebx, TCP_BLOCK_SIZE
	add rax, rsi
	; set ConnectionInfo
	mov [rax + TCP_TCB_CONNECTION_INFO_IPS_OFFSET], rcx
	mov [rax + TCP_TCB_CONNECTION_INFO_PORTS_OFFSET], edx
	call tcpActiveOpen

.end:
	pop rdi
	pop rsi
	pop rax
	ret

; Only to be used if no connection blocks are available
; and a connection is absolutely required
; ebx contains blockIdx
; ecx contains dstIP
; edx contains remotePort | (localPort << 16)
tcpCannibalize:
	push rax
	push rsi

	and ebx, 0xFFFF
	cmp bx, TCP_MAX_CONNECTIONS
	jae .end

	mov eax, [VIRTIO_NET_IP_ADDRESS]
	shl rax, 32
	or rcx, rax
	imul eax, ebx, TCP_BLOCK_SIZE
	mov rsi, TCP_CONNECTIONS_OFFSET
	add rax, rsi ; block
	call tcpHashDelete
	mov dword [rax + TCP_TCB_NOTIFY_FLAGS_OFFSET], 0
	mov eax, [rax + TCP_TCB_STATE_OFFSET]
	cmp eax, TCP_STATE_SYN_RECEIVED
	je .sendRst
	cmp eax, TCP_STATE_ESTABLISHED
	je .sendRst
	cmp eax, TCP_STATE_FIN_WAIT_1
	je .sendRst
	cmp eax, TCP_STATE_FIN_WAIT_2
	je .sendRst
	cmp eax, TCP_STATE_CLOSE_WAIT
	jne .setConnectionInfoAndActiveOpen
.sendRst:
	push rdx
	xchg rbx, rax ; block
	mov edx, TCP_HEADER_FLAG_RESET ; flags
	xor esi, esi ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	xchg rax, rbx
	pop rdx
.setConnectionInfoAndActiveOpen:
	; set ConnectionInfo
	mov [rax + TCP_TCB_CONNECTION_INFO_IPS_OFFSET], rcx
	mov [rax + TCP_TCB_CONNECTION_INFO_PORTS_OFFSET], edx
	call tcpActiveOpen
.end:
	pop rsi
	pop rax
	ret

; To be called only from user space
; ebx contains block idx
tcpAbort:
	push rax
	push rbx
	push rcx
	push rdx
	cli

	and ebx, 0xFFFF
	cmp bx, TCP_MAX_CONNECTIONS
	jae .end

	mov rax, TCP_CONNECTIONS_OFFSET
	imul ebx, ebx, TCP_BLOCK_SIZE
	add rbx, rax
	mov eax, [rbx + TCP_TCB_STATE_OFFSET]
	cmp eax, TCP_STATE_SYN_RECEIVED
	je .sendRst
	cmp eax, TCP_STATE_ESTABLISHED
	je .sendRst
	cmp eax, TCP_STATE_FIN_WAIT_1
	je .sendRst
	cmp eax, TCP_STATE_FIN_WAIT_2
	je .sendRst
	cmp eax, TCP_STATE_CLOSE_WAIT
	jne .noRstNeeded
.sendRst:
	mov edx, TCP_HEADER_FLAG_RESET ; flags
	xor esi, esi ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
.noRstNeeded:
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_CLOSED
	call tcpDelete

.end:
	sti
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; To be called only from user space
; rbx contains block idx
; rcx contains buffer address
; edx contains buffer size
; edx gets amount received
tcpReceive:
	push rax
	push rcx
	push rsi
	push rdi
	push rbp

	mov rax, TCP_CONNECTIONS_OFFSET
	imul ebx, ebx, TCP_BLOCK_SIZE
	add rbx, rax
	mov eax, [rbx + TCP_TCB_RECEIVE_DATA_SIZE_OFFSET]
	cmp edx, eax
	cmovg edx, eax ; receiveSize
	cmp edx, 0
	jle .end
	call tlbVerifyUserBuffer
	; TODO replace after testing
	;jc .end
	
	cli
	
	mov eax, [rbx + TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET]
	mov ebp, [rbx + TCP_TCB_RECEIVE_DATA_POS_OFFSET]
	sub eax, ebp ; dataUntilEnd
	mov rdi, rcx ; dst = buffer
	mov rsi, [rbx + TCP_TCB_RECEIVE_BUFFER_OFFSET]
	add rsi, rbp ; receiveDataPos ; src = receiveBuffer + receiveDataPos
	
	cmp edx, eax ; receiveSize <= dataUntilEnd
	jnle .needsTwoCopies

	; memcpy(buffer, block.receiveBuffer + block.receiveDataPos, receiveSize)
	mov ecx, edx
	rep movsb
	add ebp, edx
	mov [rbx + TCP_TCB_RECEIVE_DATA_POS_OFFSET], ebp
	sub [rbx + TCP_TCB_RECEIVE_DATA_SIZE_OFFSET], edx
	mov eax, [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	add [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET], edx
	cmp eax, 0xFFFF
	jnl .end
	jmp .sendWindowUpdate

.needsTwoCopies:
	; memcpy(buffer, block.receiveBuffer + block.receiveDataPos, dataUntilEnd)
	mov ecx, eax ; size = dataUntilEnd
	rep movsb
	; memcpy(buffer + dataUntilEnd, block.receiveBuffer, receiveSize - dataUntilEnd)
	mov rsi, [rbx + TCP_TCB_RECEIVE_BUFFER_OFFSET]
	mov ecx, edx
	sub ecx, eax ; receiveSize - dataUntilEnd
	rep movsb
	mov [rbx + TCP_TCB_RECEIVE_DATA_POS_OFFSET], ecx
	sub [rbx + TCP_TCB_RECEIVE_DATA_SIZE_OFFSET], edx
	mov eax, [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	add [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET], edx
	cmp eax, 0xFFFF
	jnl .end

.sendWindowUpdate:
	mov eax, edx
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	mov edx, eax
.end:
	sti
	pop rbp
	pop rdi
	pop rsi
	pop rcx
	pop rax
	ret

; To be called only from user code
; rax gets (connectionIdx << 32) | flags (zero if none read)
tcpRead:
	push rcx
	cli

	mov eax, TCP_CONNECTION_BLOCK_INVALID_IDX
	shl rax, 32
	mov ecx, [TCP_NOTIFY_QUEUE_HEAD]
	cmp ecx, TCP_CONNECTION_BLOCK_INVALID_IDX
	je .queueWasEmpty
	mov rax, TCP_CONNECTIONS_OFFSET
	push rcx
	imul ecx, ecx, TCP_BLOCK_SIZE
	add rcx, rax
	mov eax, [rcx + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET]
	mov [TCP_NOTIFY_QUEUE_HEAD], eax
	mov dword [rcx + TCP_TCB_FREE_AND_NOTIFY_QUEUE_NEXT_PTR_OFFSET], TCP_NOTIFY_QUEUE_INVALID_NEXT_PTR
	mov eax, [rcx + TCP_TCB_NOTIFY_FLAGS_OFFSET]
	mov dword [rcx + TCP_TCB_NOTIFY_FLAGS_OFFSET], 0
	mov ecx, eax
	pop rax
	shl rax, 32
	or rax, rcx
.queueWasEmpty:

	sti
	pop rcx
	ret



; rbx contains TCB address
; edx contains flags
; esi contains acknowledge number
; edi contains sequence number
tcpSendPacketFromTCB:
	push rax
	push rbx
	push rcx
	push rdx
	push r8

	mov rcx, rbx
	mov ebx, edx ; Put flags in correct register for encodeTCPIPv4Ethernet

	mov r8d, [rcx + TCP_TCB_REMOTE_IP_OFFSET] ; dst ip
	movzx eax, word [rcx + TCP_TCB_REMOTE_PORT_OFFSET] ; dst port
	shl eax, 16
	mov ax, [rcx + TCP_TCB_LOCAL_PORT_OFFSET] ; src port
	mov edx, eax
	mov eax, [rcx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	mov ecx, 0xFFFF
	cmp ecx, eax
	cmovg ecx, eax
	shl rcx, 32
	or rcx, rdx ; rcx = srcPort | (dstPort << 16) | (window << 32)
	

	call acquireTxPacket
	push rax ; push descriptor head

	mov edx, r8d ; restore dst ip
	call encodeTCPIPv4Ethernet

	call patchTCPIPv4Ethernet
	pop rax ; pop descriptor head
	call queueTxPacket

	pop r8
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; ebx contains flags
; rcx contains srcPort | (dstPort << 16) | (window << 32)
; edx contains dst ip
; esi contains acknowledge number
; edi contains sequence number
tcpSendPacket:
	push rax
	push rbx

	call acquireTxPacket
	push rax ; push descriptor head
	call encodeTCPIPv4Ethernet
	call patchTCPIPv4Ethernet
	pop rax ; pop descriptor head
	call queueTxPacket

	pop rbx
	pop rax
	ret

; rbx contains block address
; edx contains flags
tcpSendSYNPacket:
	push rax
	push rbx
	push rcx
	push rdx
	push r8

	mov rcx, rbx ; rcx = TCP_TCB address
	mov ebx, edx ; Put flags in correct register for encodeTCPIPv4Ethernet
	mov edi, [rcx + TCP_TCB_SEND_INITIAL_SEQUENCE_NUMBER_OFFSET]
	mov esi, [rcx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	mov r8d, [rcx + TCP_TCB_REMOTE_IP_OFFSET] ; dst ip
	movzx eax, word [rcx + TCP_TCB_REMOTE_PORT_OFFSET] ; dst port
	shl eax, 16
	mov ax, [rcx + TCP_TCB_LOCAL_PORT_OFFSET] ; src port
	mov edx, eax
	mov eax, [rcx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	mov ecx, 0xFFFF
	cmp ecx, eax
	cmovg ecx, eax
	shl rcx, 32
	or rcx, rdx ; rcx = srcPort | (dstPort << 16) | (window << 32)

	call acquireTxPacket
	push rax ; push descriptor head

	mov edx, r8d ; dst ip
	call encodeTCPIPv4Ethernet

	mov byte [rbx + rax - TCP_HEADER_MIN_SIZE + TCP_HEADER_DATA_OFFSET_OFFSET], 6 << 4 ; Set header length to 6 dwords (we're using one more than normal for initial extensions)
	mov byte [rbx + rax + 0], TCP_OPTION_MAXIMUM_SEGMENT_SIZE_OP
	mov byte [rbx + rax + 1], 4
	mov word [rbx + rax + 2], HTON16(TCP_SEGMENT_SIZE)
	add rax, 4

	call patchTCPIPv4Ethernet
	pop rax ; pop descriptor head
	call queueTxPacket

	pop r8
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

; rbx contains TCP block address
; rcx:edx contains ConnectionInfo struct
tcpBlockInit:
	push rax
	push rbx
	push rsi

	mov eax, [PIT_CLOCK_MILLISECONDS]
	lea esi, [eax + 2500 + TCP_CLOCK_GRANULARITY_MILLISECONDS]
	mov [rbx + TCP_TCB_RETRANSMIT_TIMEOUT_MILLISECONDS_OFFSET], esi
	mov dword [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	lea esi, [eax + 5000]
	mov [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET], esi
	mov [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], esi
	mov dword [rbx + TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET], -1 ; No round trip time yet measured
	mov dword [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET], 536 ; The default maximum segment sized, specified in RFC
	mov dword [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET], 0
	mov dword [rbx + TCP_TCB_RECEIVE_INITIAL_SEQUENCE_NUMBER_OFFSET], 0
	rdrand esi ; Should have something to do with time I think, just randomize it for now
	mov [rbx + TCP_TCB_SEND_INITIAL_SEQUENCE_NUMBER_OFFSET], esi
	mov [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET], esi
	add esi, 1 ; Add one to account for the SYN
	mov [rbx + TCP_TCB_RESEND_NEXT_OFFSET], esi
	mov [rbx + TCP_TCB_SEND_NEXT_OFFSET], esi ; sendNext and resendNext should be the same unless retransmission is needed
	mov dword [rbx + TCP_TCB_SEND_DATA_POS_OFFSET], 1
	mov dword [rbx + TCP_TCB_SEND_DATA_SIZE_OFFSET], 0
	mov dword [rbx + TCP_TCB_RECEIVE_DATA_POS_OFFSET], 1
	mov dword [rbx + TCP_TCB_RECEIVE_DATA_SIZE_OFFSET], 0
	mov dword [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET], 0
	mov dword [rbx + TCP_TCB_SLOW_START_THRESHOLD_OFFSET], 0xFFFF ; Set to some arbitrary high number (could go higher than this?)
	mov [rbx + TCP_TCB_REMOTE_IP_OFFSET], rcx ; Set both connection info IPs
	mov [rbx + TCP_TCB_REMOTE_PORT_OFFSET], edx ; Set both connection info ports
	mov dword [rbx + TCP_TCB_CLOSE_REQUESTED_OFFSET], 0
	mov [rbx + TCP_TCB_ROUND_TRIP_TIMER_SEQUENCE_NUMBER_OFFSET], esi ; set to sendNext
	mov esi, [rbx + TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET]
	mov [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET], esi
	xor esi, esi
	cmp eax, -1
	cmove eax, esi
	mov [rbx + TCP_TCB_ROUND_TRIP_TIMER_OFFSET], eax ; Time the first round trip
	mov dword [rbx + TCP_TCB_NOTIFY_FLAGS_OFFSET], 0
	mov ebx, [rbx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET]
	call tcpTimerPriorityQueueUpdateElement

	pop rsi
	pop rbx
	pop rax
	ret

resetRetransmitTimeoutStr db "Reset Retransmit Timeout: ",0x00

; rbx contains TCP_TCB address
tcpResetRetransmissionTimeout:
	push rbx
	push rcx
	push rdx

	mov ecx, [PIT_CLOCK_MILLISECONDS]
	add ecx, [rbx + TCP_TCB_RETRANSMIT_TIMEOUT_MILLISECONDS_OFFSET]
	lea edx, [ecx + 1]
	cmp ecx, TCP_INVALID_TIMEOUT_VALUE
	cmove ecx, edx
	mov [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET], ecx

	cmp dword [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	je .needsUpdatePriorityQueue
	mov edx, ecx
	sub edx, [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET]
	cmp edx, 0
	jnle .end
.needsUpdatePriorityQueue:
	mov [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], ecx
	mov ebx, [rbx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET]
	call tcpTimerPriorityQueueUpdateElement
.end:
	pop rdx
	pop rcx
	pop rbx
	ret

resetDeletionTimeoutStr db "Reset Deletion Timeout: ",0x00

; rbx contains TCP_TCB address
; ecx contains timeout length in milliseconds
tcpResetDeletionTimeout:
	push rbx
	push rcx
	push rdx

	add ecx, [PIT_CLOCK_MILLISECONDS]
	lea edx, [ecx + 1]
	cmp ecx, TCP_INVALID_TIMEOUT_VALUE
	cmove ecx, edx
	mov [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET], ecx

	cmp dword [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	je .needsUpdatePriorityQueue
	mov edx, ecx
	sub edx, [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET]
	cmp edx, 0
	jnle .end
.needsUpdatePriorityQueue:
	mov [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], ecx
	mov ebx, [rbx + TCP_TCB_TIMER_QUEUE_IDX_OFFSET]
	call tcpTimerPriorityQueueUpdateElement
.end:
	pop rdx
	pop rcx
	pop rbx
	ret

; rbx contains TCP_TCB address
; rcx contains extension data
; edx contains bytes in extensions
tcpProcessExtensions:
	push rax
	push rcx
	push rdx
	push rsi
	push rdi

	test edx, edx
	jz .processLoopEnd
.processLoop:
	mov al, [rcx]
	cmp al, TCP_OPTION_END_OP
	je .processLoopEnd
	add rcx, 1
	sub edx, 1
	cmp al, TCP_OPTION_NOP_OP
	je .processLoopContinue
	cmp edx, 0
	jle .processLoopEnd
	movzx edi, byte [rcx] ; get extension size
	add rcx, 1
	sub edx, 1
	cmp edx, edi
	jl .processLoopEnd ; Make sure we have at least this many bytes left
	cmp al, TCP_OPTION_MAXIMUM_SEGMENT_SIZE_OP
	jne .notMaximumSegmentSizeExt
	cmp edi, 4 ; This extension must have a size of exactly 4
	jne .processLoopEnd
	movbe eax, [rcx]
	mov esi, TCP_SEGMENT_SIZE
	cmp esi, eax
	cmovl eax, esi
	mov [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET], eax ; sendMaximumSegmentSize = min(packet.maxSegmentSize, TCP_SEGMENT_SiZE)
.notMaximumSegmentSizeExt:
	lea rcx, [rcx + rdi + 1]
	sub edx, edi
	sub edx, 1
.processLoopContinue:
	cmp edx, 0
	jg .processLoop
.processLoopEnd:

	mov eax, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	mov ecx, eax
	shl ecx, 2 ; congestionWindow = sendMaximumSegmentSize * 4
	mov edx, ecx
	sub edx, eax
	cmp eax, 1095
	cmovg ecx, edx ; if sendMaximumSegmentSize > 1095, congestionWindow = sendMaximumSegmentSize * 3
	sub edx, eax
	cmp eax, 2190
	cmovg ecx, edx ; if sendMaximumSegmentSize > 2190, congestionWindow = sendMaximumSegmentSize * 2
	mov [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET], ecx

	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rax
	ret

connectionAlreadyExisted db "Connection Already Existed!",0x0a,0x00
makingNewConnection db "Making New Connection",0x0a,0x00
connectionWasStateSynSent db "Connection Was In SYN_SENT",0x0a,0x00
stateNotSynSentStr db "State Not SYN_SENT",0x0a,0x00
dataNotifedStr db "Data Notified",0x0a,0x00
gotAckSendRstStr db "Got ack, sending rst",0x0a,0x00

; This function is extracted from processPacket so I can group it with the other TCP stuff
; rax has packet data address (starting with TCP header)
; ecx has packet data size (not including TCP header)
; rbp points to the top of the local variable table in processPacket
; all registers are caller preserved
; packet has been checksum verified and the TCP header is at least 5 dwords
handleTCPPacket:
	push r8
	push r9
	push r10
	push r11

	cmp word [rbp - 20], 80 ; dstPort (local)
	jne .end ; Only TCP port 80 accepted for now
	mov r8, rax
	mov r9, rcx
	; Swap packet variables to something we can use
	movbe ecx, [rax + TCP_HEADER_SEQUENCE_NUM_OFFSET]
	mov r10d, ecx ; r10 used for packet sequence number
	mov [rax + TCP_HEADER_SEQUENCE_NUM_OFFSET], ecx
	movbe ecx, [rax + TCP_HEADER_ACKNOWLEDGE_NUM_OFFSET]
	mov r11d, ecx ; r11 used for packet acknowledge number
	mov [rax + TCP_HEADER_ACKNOWLEDGE_NUM_OFFSET], ecx
	ror word [rax + TCP_HEADER_WINDOW_OFFSET], 8
	; Collect a ConnectionInfo
	mov ecx, [rbp - 16] ; srcIP
	mov edx, [rbp - 24] ; dstIP
	shl rdx, 32
	or rcx, rdx
	movzx edx, word [rbp - 20] ; dstPort
	shl edx, 16
	mov dx, [rbp - 18] ; srcPort (remote)
	mov sil, [rax + TCP_HEADER_FLAGS_OFFSET] ; flags
	call tcpHashLookup
	cmp ebx, TCP_CONNECTION_HASH_EMPTY
	jne .connectionExists
	; New connection
	test sil, TCP_HEADER_FLAG_RESET
	jnz .end ; Reset flag detected, drop it

	test sil, TCP_HEADER_FLAG_ACKNOWLEDGE
	jz .newConnectionHasNoAck
	DEBUG_SEND_STR gotAckSendRstStr
	; New connections should not have ack bit set, reset
	mov ebx, TCP_HEADER_FLAG_RESET ; flags
	movzx ecx, word [rbp - 18] ; packet srcPort (remote machine port)
	shl ecx, 16
	mov cx, [rbp - 20] ; packet dstPort (local machine port)
	mov edx, [rbp - 16] ; dstIP (the source of the packet is our destination)
	xor esi, esi ; ack
	mov edi, r11d ; seq = packet.ack
	call tcpSendPacket
	jmp .end
.newConnectionHasNoAck:

	test sil, TCP_HEADER_FLAG_SYNCHRONIZE
	jz .newConnectionHasNoSyn
	call tcpAlloc
	cmp ebx, TCP_CONNECTION_BLOCK_INVALID_IDX
	jne .makeNewConnection

	; No connection resources available!
	movzx ecx, word [rbp - 18] ; srcPort (local machine port)
	shl ecx, 16
	mov cx, [rbp - 20] ; dstPort (remote machine port)
	mov edx, [rbp - 16] ; dstIP (the source of the packet is our destination)
	test sil, TCP_HEADER_FLAG_ACKNOWLEDGE
	jz .connectionEmptyNoAck
	mov ebx, TCP_HEADER_FLAG_RESET ; flags
	xor esi, esi ; ack
	mov edi, r11d ; seq = packet.ack
	call tcpSendPacket
	jmp .end
.connectionEmptyNoAck:
	mov ebx, TCP_HEADER_FLAG_ACKNOWLEDGE | TCP_HEADER_FLAG_RESET ; flags
	mov esi, r10d ; get packet.seq
	add esi, r9d ; ack
	xor edi, edi ; seq
	call tcpSendPacket
	jmp .end

.makeNewConnection:
	;DEBUG_PRINT_STR makingNewConnection
	call tcpHashInsert
	
	mov rsi, TCP_CONNECTIONS_OFFSET
	imul ebx, ebx, TCP_BLOCK_SIZE
	add rbx, rsi ; block address
	call tcpBlockInit
	mov esi, r10d ; get packet.seq
	mov [rbx + TCP_TCB_RECEIVE_INITIAL_SEQUENCE_NUMBER_OFFSET], esi
	add esi, 1
	mov [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET], esi ; block.receiveNext = packet.seq + 1 // plus 1, account for the SYN

	mov edx, TCP_HEADER_FLAG_SYNCHRONIZE | TCP_HEADER_FLAG_ACKNOWLEDGE
	call tcpSendSYNPacket
	
	movzx edx, byte [rax + TCP_HEADER_DATA_OFFSET_OFFSET]
	shr edx, 2 ; shift the bits down by 4 to get dwords, then multiply by 4 (shift by 2) to translate dwords to bytes
	sub edx, 20 ; Take off the first 20 bytes to get the amount of extension data
	lea rcx, [rax + 20] ; address of TCP extensions
	call tcpProcessExtensions

	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_SYN_RECEIVED

	jmp .end
.newConnectionHasNoSyn:
	; Should not reach here in normal operation
	jmp .end
.connectionExists:

	;DEBUG_SEND_STR connectionAlreadyExisted

	mov rdx, TCP_CONNECTIONS_OFFSET
	imul ebx, ebx, TCP_BLOCK_SIZE
	add rbx, rdx ; RBX contains block address
	cmp dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_SYN_SENT
	jne .stateNotSynSent

	;DEBUG_PRINT_STR connectionWasStateSynSent

	xor edx, edx ; ackAcceptable = 0

	test sil, TCP_HEADER_FLAG_ACKNOWLEDGE
	jz .synSentNoAck
	; ack bit is on
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_INITIAL_SEQUENCE_NUMBER_OFFSET]
	cmp ecx, 0
	jle .synSentAckInRange
	mov ecx, r11d ; packet.ack
	sub ecx, [rax + TCP_TCB_SEND_NEXT_OFFSET]
	cmp ecx, 0
	jg .synSentAckInRange
	test sil, TCP_HEADER_FLAG_RESET
	jnz .end
	; Send reset
	mov edx, TCP_HEADER_FLAG_RESET ; flags
	mov edi, r11d ; seq = packet.ack
	xor esi, esi ; ack
	call tcpSendPacketFromTCB
	jmp .end
.synSentAckInRange:
	mov ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	sub ecx, r11d ; packet.ack
	cmp ecx, 0
	jnl .synSentAckNotAcceptable
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	jnle .synSentAckNotAcceptable
	mov edx, 1 ; ackAcceptable = true
.synSentAckNotAcceptable:
.synSentNoAck:

	test sil, TCP_HEADER_FLAG_RESET
	jz .synSentNoRst
	; rst bit is on
	test edx, edx ; ackAcceptable
	jz .synSentNoRstNoMarkForDelete
	mov ecx, TCP_NOTIFY_RESET | TCP_NOTIFY_CLOSED
	call tcpMarkForDelete
.synSentNoRstNoMarkForDelete:
	jmp .end
.synSentNoRst:
	
	test sil, TCP_HEADER_FLAG_SYNCHRONIZE
	jz .synSentNoSyn
	test edx, edx
	jnz .synSentHasValidSyn
	test sil, TCP_HEADER_FLAG_ACKNOWLEDGE
	jz .synSentHasValidSyn
	jmp .synSentNoSyn
.synSentHasValidSyn:
	mov ecx, r10d ; get packet.seq
	mov [rbx + TCP_TCB_RECEIVE_INITIAL_SEQUENCE_NUMBER_OFFSET], ecx
	add ecx, 1
	mov [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET], ecx
	test sil, TCP_HEADER_FLAG_ACKNOWLEDGE
	jz .synSentHasValidSynNoAck
	mov ecx, r11d ; packet.ack
	mov [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET], ecx
.synSentHasValidSynNoAck:
	mov ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	sub ecx, [rbx + TCP_TCB_SEND_INITIAL_SEQUENCE_NUMBER_OFFSET]
	cmp ecx, 0
	jng .synSentOurSynNotAcked
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_ESTABLISHED
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	mov ecx, TCP_NOTIFY_OPENED
	call tcpNotify
	jmp .synSentStateChangeEnd
.synSentOurSynNotAcked:
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_SYN_RECEIVED
	mov edx, TCP_HEADER_FLAG_SYNCHRONIZE | TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	call tcpSendSYNPacket
.synSentStateChangeEnd:
	movzx edx, byte [rax + TCP_HEADER_DATA_OFFSET_OFFSET]
	shr edx, 2 ; shift the bits down by 4 to get dwords, then multiply by 4 (shift by 2) to translate dwords to bytes
	sub edx, 20 ; Take off the first 20 bytes to get the amount of extension data
	lea rcx, [rax + 20] ; address of TCP extensions
	call tcpProcessExtensions
	movzx ecx, word [rax + TCP_HEADER_WINDOW_OFFSET]
	mov [rbx + TCP_TCB_SEND_WINDOW_OFFSET], ecx
	mov ecx, r10d ; get packet.seq
	mov [rbx + TCP_TCB_SEND_SEQUENCE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET], ecx
	mov ecx, r11d ; packet.ack
	mov [rbx + TCP_TCB_SEND_ACKNOWLEDGE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET], ecx
	mov ecx, 5000 ; 5 seconds
	call tcpResetDeletionTimeout
.synSentNoSyn:
	jmp .end
.stateNotSynSent:

	;DEBUG_PRINT_STR stateNotSynSentStr
	
	; if packet.len == 0 && block.receiveWindow == 0 && packet.seq != block.receiveNext, drop
	test r9d, r9d ; packet length
	jnz .basicCheckPass2 ; Skip second test as well, since this condition applies to both
	mov ecx, [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	test ecx, ecx
	jnz .basicCheckPass1
	mov ecx, r10d ; get packet.seq
	cmp ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	jne .end
.basicCheckPass1:
	; if packet.len == 0 && block.receiveWindow > 0 && (packet.seq < block.receiveNext || packet.seq >= (block.receiveNext + block.receiveWindow)), drop
	; packet length is zero if we reach here
	cmp dword [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET], 0
	jng .basicCheckPass2
	mov ecx, r10d ; get packet.seq
	sub ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp ecx, 0
	jl .end
	sub ecx, [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	cmp ecx, 0
	jge .end
.basicCheckPass2:
	; if packet.len > 0 && block.receiveWindow == 0, drop
	test r9d, r9d
	jz .basicCheckPass4 ; checks 3 and 4 both require packet.len > 0, skip both
	mov ecx, [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	test ecx, ecx
	jz .end
.basicCheckPass3:
	; u32 packetSeqEnd = packet.seq + packet.len - 1;
	; u32 receiveSeqEnd = block.receiveNext + block.receiveWindow;
	; if packet.len > 0 && block.receiveWindow > 0
	; && (packet.seq < block.receiveNext || packet.seq => receiveSeqEnd) // seq out of range test
	; && (packetSeqEnd < block.receiveNext || packetSeqEnd => receiveSeqEnd)) ; seq end out of range test
	; , drop
	mov esi, r10d ; packet.seq
	mov edi, [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	test edi, edi
	jz .basicCheckPass4
	mov ecx, esi
	sub ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp ecx, 0
	jl .basicCheck4SeqOutOfRange
	mov ecx, esi
	sub ecx, edi
	sub ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp ecx, 0
	jnge .basicCheckPass4
.basicCheck4SeqOutOfRange:
	lea esi, [esi + r9d - 1] ; packetSeqEnd
	mov ecx, esi
	sub ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp ecx, 0
	jl .end
	sub esi, edi
	sub esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp esi, 0
	jge .end
.basicCheckPass4:
	add edi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; receiveSeqEnd

	test byte [rax + TCP_HEADER_FLAGS_OFFSET], TCP_HEADER_FLAG_RESET
	jz .noRst
	; Reset bit is on
	mov ecx, r10d ; packet.seq
	sub ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp ecx, 0
	jl .end
	mov ecx, r10d ; packet.seq
	sub ecx, edi ; compare to receiveSeqEnd
	jge .end
	mov ecx, r10d ; packet.seq
	cmp ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	je .hasRstAndSeqIsReceiveNext
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	jmp .end
.hasRstAndSeqIsReceiveNext:
	cmp dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_SYN_RECEIVED
	jne .hasRstAndStateIsNotSynReceived
	; User was never notified, we can just delete
	call tcpDelete
	jmp .end
.hasRstAndStateIsNotSynReceived:
	mov ecx, TCP_NOTIFY_RESET | TCP_NOTIFY_CLOSED
	call tcpMarkForDelete
	jmp .end
.noRst:

	test byte [rax + TCP_HEADER_FLAGS_OFFSET], TCP_HEADER_FLAG_SYNCHRONIZE
	jz .noSyn
	cmp dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_SYN_RECEIVED
	jne .synPresentAndStateNotSynReceived
	; User was never notified, we can just delete
	call tcpDelete
	jmp .end
.synPresentAndStateNotSynReceived:
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	jmp .end
.noSyn:

	test byte [rax + TCP_HEADER_FLAGS_OFFSET], TCP_HEADER_FLAG_ACKNOWLEDGE
	jz .end
	; Ack bit must be on
	mov ecx, [rbx + TCP_TCB_ROUND_TRIP_TIMER_OFFSET]
	cmp ecx, -1
	je .roundTripTimeNotMeasured
	mov edx, r11d ; packet.ack
	sub edx, [rbx + TCP_TCB_ROUND_TRIP_TIMER_SEQUENCE_NUMBER_OFFSET]
	cmp edx, 0
	jnge .roundTripTimeNotMeasured
	mov esi, [PIT_CLOCK_MILLISECONDS]
	sub esi, ecx ; esi = roundTripTimeMilliseconds
	; round trip time measured, compute new retransmission timeout variables
	cmp dword [rbx + TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET], -1
	jne .notFirstRoundTripTimeMeasurement
	mov [rbx + TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET], esi
	mov ecx, esi
	shr ecx, 1
	mov [rbx + TCP_TCB_ROUND_TRIP_TIME_VARIATION_MILLISECONDS_OFFSET], ecx
	shl ecx, 2
	mov edx, TCP_CLOCK_GRANULARITY_MILLISECONDS
	cmp ecx, edx
	cmovl ecx, edx
	add esi, ecx
	mov [rbx + TCP_TCB_RETRANSMIT_TIMEOUT_MILLISECONDS_OFFSET], esi
	jmp .roundTripTimeMeasuredEnd
.notFirstRoundTripTimeMeasurement:
	; alpha = 1/8
	; beta = 1/4
	; RTTVAR = (1 - beta) * RTTVAR + beta * abs(SRTT - R)
	mov ecx, [rbx + TCP_TCB_ROUND_TRIP_TIME_VARIATION_MILLISECONDS_OFFSET]
	shr ecx, 2
	lea ecx, [ecx + ecx * 2]
	mov edx, [rbx + TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET]
	sub edx, esi
	mov edi, edx
	neg edx
	cmovs edx, edi
	shr edx, 2
	add ecx, edx
	mov [rbx + TCP_TCB_ROUND_TRIP_TIME_VARIATION_MILLISECONDS_OFFSET], ecx
	; SRTT = (1 - alpha) * SRTT + alpha * R
	mov ecx, [rbx + TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET]
	shr ecx, 3
	imul ecx, ecx, 7
	shr esi, 3
	add ecx, esi
	mov [rbx + TCP_TCB_SMOOTHED_ROUND_TRIP_TIME_MILLISECONDS_OFFSET], ecx
	; RTO = SRTT + max(G, K * RTTVAR)
	mov edx, [rdx + TCP_TCB_ROUND_TRIP_TIME_VARIATION_MILLISECONDS_OFFSET]
	shl edx, 2
	mov edi, TCP_CLOCK_GRANULARITY_MILLISECONDS
	cmp edx, edi
	cmovl edx, edi
	add ecx, edx
	mov edx, 500 ; min timeout in milliseconds
	cmp ecx, edx
	cmovl ecx, edx
	mov [rbx + TCP_TCB_RETRANSMIT_TIMEOUT_MILLISECONDS_OFFSET], ecx
.roundTripTimeMeasuredEnd:
	mov dword [rbx + TCP_TCB_ROUND_TRIP_TIMER_OFFSET], -1
.roundTripTimeNotMeasured:
	
	mov ecx, 10000 ; 10 second user timeout
	call tcpResetDeletionTimeout
	
	xor esi, esi ; sendWindowUpdated = false
	cmp dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_SYN_RECEIVED
	jne .currentStateNotSynReceived
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp ecx, 0
	jng .ackReceivedDidNotAckOurSyn
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	cmp ecx, 0
	jnle .ackReceivedDidNotAckOurSyn
	; Our counter syn was acked, the connection is now open
	movzx ecx, word [rax + TCP_HEADER_WINDOW_OFFSET]
	mov [rbx + TCP_TCB_SEND_WINDOW_OFFSET], ecx
	mov ecx, r10d ; packet.seq
	mov [rbx + TCP_TCB_SEND_SEQUENCE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET], ecx
	mov ecx, r11d ; packet.ack
	mov [rbx + TCP_TCB_SEND_ACKNOWLEDGE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET], ecx
	mov esi, 1 ; sendWindowUpdated = true
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_ESTABLISHED
	mov ecx, TCP_NOTIFY_OPENED
	call tcpNotify
	jmp .currentStateNotSynReceived
.ackReceivedDidNotAckOurSyn:
	mov edx, TCP_HEADER_FLAG_RESET ; flags
	mov edi, r11d ; seq = packet.ack
	xor esi, esi ; ack
	call tcpSendPacketFromTCB
	jmp .end
.currentStateNotSynReceived:
	
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	cmp ecx, 0
	jng .ackWasNotGreaterThanSendNext
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	call tcpSendPacketFromTCB
	jmp .end
.ackWasNotGreaterThanSendNext:
	
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp ecx, 0
	jnge .ackWasNotGreaterThanOrEqualToSendUnacknowledged
	mov ecx, r10d ; packet.seq
	sub ecx, [rbx + TCP_TCB_SEND_SEQUENCE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET]
	cmp ecx, 0
	jg .needsUpdateSendWindow
	cmp [rbx + TCP_TCB_SEND_SEQUENCE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET], r10d ; packet.seq
	jne .doesNotNeedUpdateSendWindow
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_ACKNOWLEDGE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET]
	cmp ecx, 0
	jnge .doesNotNeedUpdateSendWindow
.needsUpdateSendWindow:
	movzx ecx, word [rax + TCP_HEADER_WINDOW_OFFSET]
	mov [rbx + TCP_TCB_SEND_WINDOW_OFFSET], ecx
	mov [rbx + TCP_TCB_SEND_SEQUENCE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET], r10d ; packet.seq
	mov [rbx + TCP_TCB_SEND_ACKNOWLEDGE_NUMBER_FOR_LAST_WINDOW_UPDATE_OFFSET], r11d ; packet.ack
.doesNotNeedUpdateSendWindow:
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp ecx, 0
	jng .noNewDataAcked
	; New data was acked
	mov ecx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	cmp ecx, [rbx + TCP_TCB_SLOW_START_THRESHOLD_OFFSET]
	jnle .congestionAvoidance
	mov ecx, r11d ; packet.ack
	sub ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	mov edx, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	cmp ecx, edx
	cmovg ecx, edx
	add [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET], ecx
	jmp .slowStart
.congestionAvoidance:
	mov rdi, rax
	xor edx, edx
	mov ecx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	mov eax, 1
	cmp ecx, 0
	cmovle ecx, eax
	mov eax, TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET
	imul eax, eax
	div ecx
	mov edx, 1
	cmp eax, 0
	cmove eax, edx
	add [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET], eax
	mov rax, rdi
.slowStart:
	call tcpResetRetransmissionTimeout
	cmp dword [rbx + TCP_TCB_DUPLICATE_ACK_COUNT_OFFSET], 3
	jnge .noCongestionWindowDeflateNeeded
	mov ecx, [rbx + TCP_TCB_SLOW_START_THRESHOLD_OFFSET]
	mov [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET], ecx
.noCongestionWindowDeflateNeeded:
	mov dword [rbx + TCP_TCB_DUPLICATE_ACK_COUNT_OFFSET], 0
	mov esi, 1 ; sendWindowUpdated = true
	; calling tcpSendData with zero max packets will put it on the send queue if it isn't there already
	xor ecx, ecx
	mov edx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	call tcpSendData
	jmp .newDataWasAcked
.noNewDataAcked:
	; Duplicate ack check
	mov ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	cmp ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	je .notADuplicateAck
	test r9d, r9d ; packet data size
	jnz .notADuplicateAck
	test byte [rax + TCP_HEADER_FLAGS_OFFSET], TCP_HEADER_FLAG_SYNCHRONIZE | TCP_HEADER_FLAG_FINALIZE
	jnz .notADuplicateAck
	; Duplicate ack detected
	add dword [rbx + TCP_TCB_DUPLICATE_ACK_COUNT_OFFSET], 1
.notADuplicateAck:
.newDataWasAcked:
	mov [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET], r11d ; packet.ack
.ackWasNotGreaterThanOrEqualToSendUnacknowledged:

	mov edx, [rbx + TCP_TCB_STATE_OFFSET]
	mov ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	jne .notDoneSendingData
	cmp edx, TCP_STATE_FIN_WAIT_1
	jne .ackStateNotFinWait1
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_FIN_WAIT_2
	jmp .ackProcessingFinished
.ackStateNotFinWait1:
	cmp edx, TCP_STATE_CLOSING
	jne .ackStateNotClosing
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_TIME_WAIT
	mov ecx, TCP_TIME_WAIT_TIMEOUT
	call tcpResetDeletionTimeout
	jmp .ackProcessingFinished
.ackStateNotClosing:
	cmp edx, TCP_STATE_LAST_ACK
	jne .ackStateNotLastAck
	mov ecx, TCP_NOTIFY_CLOSED
	call tcpMarkForDelete
	jmp .ackProcessingFinished
.ackStateNotLastAck:
.notDoneSendingData:
	cmp edx, TCP_STATE_TIME_WAIT
	jne .ackStateNotTimeWait
	mov ecx, esi ; save sendWindowUpdated
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
	mov esi, ecx ; restore sendWindowUpdated
	mov ecx, TCP_TIME_WAIT_TIMEOUT
	call tcpResetDeletionTimeout
.ackStateNotTimeWait:
.ackProcessingFinished:

	; esi now has sendWindowUpdated in bit 0 (0b1) and needsAck in bit 2 (0b10)
	; Process segment text
	test r9d, r9d ; packet data size
	jz .noProcessSegmentText
	mov ecx, [rbx + TCP_TCB_STATE_OFFSET]
	cmp ecx, TCP_STATE_ESTABLISHED
	je .processSegmentText
	cmp ecx, TCP_STATE_FIN_WAIT_1
	je .processSegmentText
	cmp ecx, TCP_STATE_FIN_WAIT_2
	jne .noProcessSegmentText
.processSegmentText:
	; r10d = start = packet.seq
	lea edx, [r10d + r9d] ; end = start + packet data size
	; if end >= receiveNext + receiveWindow, end = receiveNext + receiveWindow
	mov ecx, edx
	mov edi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	add edi, [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET]
	sub ecx, edi
	cmp ecx, 0
	cmovge edx, edi ; end = receiveNext + receiveWindow (if condition)
	; if start <= block.receiveNext, start = block.receiveNext
	mov ecx, r10d
	mov edi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	sub ecx, edi
	cmp ecx, 0
	mov ecx, r10d
	cmovle ecx, edi
	; ecx = start, edx = end
	push rsi
	push rax

	cmp ecx, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	jne .dataWasNotNextToProcess
	mov [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET], edx ; receiveNext = end
	mov eax, edx
	sub eax, ecx
	add [rbx + TCP_TCB_RECEIVE_DATA_SIZE_OFFSET], eax ; receiveDataSize += end - start
	sub [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET], eax ; receiveWindow -= end - start;
	push r8
	push r9

	xor r9d, r9d ; writePos
	xor eax, eax ; loop counter
	mov r8d, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET]
	test r8d, r8d
	jz .dataWasNextToProcessBufferIsEmpty
.segmentProcessBufferCheckLoop:
	mov esi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + rax * 8 + TCP_RECEIVED_RANGE_START_OFFSET]
	sub esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp esi, 0
	jnle .segmentProcessBufferCheckLoopCantConsumeSegmentRangeYet
	mov edi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + rax * 8 + TCP_RECEIVED_RANGE_END_OFFSET]
	mov esi, edi
	sub esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	cmp esi, 0
	jle .segmentProcessBufferCheckLoopContinue
	mov esi, edi
	sub esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET]
	add [rbx + TCP_TCB_RECEIVE_DATA_SIZE_OFFSET], esi ; receiveDataSize += segmentProcesBuffer[i].end - receiveNext
	sub [rbx + TCP_TCB_RECEIVE_WINDOW_OFFSET], esi ; receiveWindow -= segmentProcesBuffer[i].end - receiveNext
	mov [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET], edi ; receiveNext = segmentProcessBuffer[i].end
	jmp .segmentProcessBufferCheckLoopContinue
.segmentProcessBufferCheckLoopCantConsumeSegmentRangeYet:
	mov rsi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + rax * 8]
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r9 * 8], rsi
	add r9d, 1
.segmentProcessBufferCheckLoopContinue:
	add eax, 1
	cmp eax, r8d
	jl .segmentProcessBufferCheckLoop
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET], r9d ; writePos
.dataWasNextToProcessBufferIsEmpty:
	pop r9
	pop r8

	;DEBUG_PRINT_STR dataNotifedStr
	mov esi, ecx
	mov ecx, TCP_NOTIFY_DATA
	call tcpNotify
	mov ecx, esi
	jmp .dataWasNextToProcess
.dataWasNotNextToProcess:
	push rcx
	push rdx
	push r8
	push r9
	push r10
	push r11

	xor r8d, r8d ; insertPos
	mov r9d, 1 ; shift

	xor r10d, r10d ; loop counter
	mov r11d, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET]
	test r11d, r11d
	jz .dataNotNextToProcessDataCombineLoopBreak
.dataNotNextToProcessDataCombineLoop:
	mov esi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8 + TCP_RECEIVED_RANGE_START_OFFSET]
	mov edi, edx
	sub edi, esi ; newRange.end < range.start
	cmp edi, 0
	jl .dataNotNextToProcessDataCombineLoopBreak
	mov edi, ecx
	sub edi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8 + TCP_RECEIVED_RANGE_END_OFFSET] ; newRange.start > range.end
	cmp edi, 0
	jng .newRangeStartNotGreaterThanRangeEnd
	add r8d, 1 ; insertPos++
	jmp .dataNotNextToProcessDataCombineLoopContinue
.newRangeStartNotGreaterThanRangeEnd:
	mov edi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8 + TCP_RECEIVED_RANGE_END_OFFSET]
	cmp ecx, esi ; newRange.start < range.start
	cmovg ecx, esi ; newRange.start = min(range.start, newRange.start)
	cmp edx, edi ; newRange.end > range.end
	cmovl edx, edi ; newRange.end = max(range.end, newRange.end)
	; range = newRange
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8 + TCP_RECEIVED_RANGE_START_OFFSET], ecx
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8 + TCP_RECEIVED_RANGE_END_OFFSET], edx
	sub r9d, 1
.dataNotNextToProcessDataCombineLoopContinue:
	add r10d, 1
	cmp r10d, r11d
	jl .dataNotNextToProcessDataCombineLoop
.dataNotNextToProcessDataCombineLoopBreak:
	
	cmp r9d, 1
	jne .dataNotNextToProcessDataShiftNot1
	cmp r8d, TCP_TCB_SEGMENT_PROCESS_BUFFER_CAP
	je .dataNotNextToProcessDataNoShiftNeeded
	mov r10d, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET]
	lea esi, [r10d + 1]
	cmp r10d, TCP_TCB_SEGMENT_PROCESS_BUFFER_CAP
	cmovl r10d, esi
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET], r10d
	sub r10d, 1 ; loop counter
	cmp r10d, r8d
	jng .dataNotNextToProcessDataShiftForwardLoopBreak
.dataNotNextToProcessDataShiftForwardLoop:
	mov rsi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8 - 8]
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8], rsi
	sub r10d, 1
	cmp r10d, r8d
	jg .dataNotNextToProcessDataShiftForwardLoop
.dataNotNextToProcessDataShiftForwardLoopBreak:
	shl rdx, 32
	or rcx, rdx
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r8 * 8], rcx
.dataNotNextToProcessDataShiftNot1:

	cmp r9d, 0
	jnl .dataNotNextToProcessDataShiftNotLessThan0
	neg r9d ; shift
	lea r10d, [r8d + 1] ; loop counter
	mov r11d, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET]
	sub r11d, r9d
	cmp r10d, r11d
	jnl .dataNotNextToProcessDataShiftBackwardLoopBreak
.dataNotNextToProcessDataShiftBackwardLoop:
	lea edi, [r10d + r9d]
	mov rsi, [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + rdi * 8]
	mov [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_OFFSET + r10 * 8], rsi
	add r10d, 1
	cmp r10d, r11d
	jl .dataNotNextToProcessDataShiftBackwardLoop
.dataNotNextToProcessDataShiftBackwardLoopBreak:
	sub [rbx + TCP_TCB_SEGMENT_PROCESS_BUFFER_SIZE_OFFSET], r9d
.dataNotNextToProcessDataShiftNotLessThan0:
.dataNotNextToProcessDataNoShiftNeeded:

	pop r11
	pop r10
	pop r9
	pop r8
	pop rdx
	pop rcx
	
.dataWasNextToProcess:

	mov esi, [rbx + TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET]
	cmp esi, 0
	jle .receiveBufferSizeWasWrong ; Should never take this jump, but just in case, make sure a divide by 0 isn't possible
	mov edi, edx
	xor edx, edx
	mov eax, ecx
	sub eax, [rbx + TCP_TCB_RECEIVE_INITIAL_SEQUENCE_NUMBER_OFFSET]
	div esi
	mov ecx, edx ; start = (start - block.receiveInitialSequence) % block.receiveBufferSize
	xor edx, edx
	mov eax, edi
	sub eax, [rbx + TCP_TCB_RECEIVE_INITIAL_SEQUENCE_NUMBER_OFFSET]
	div esi
	; edx = end = (end - block.receiveInitialSequence) % block.receiveBufferSize;
	movzx esi, byte [r8 + TCP_HEADER_DATA_OFFSET_OFFSET]
	shr esi, 2 ; shift the bits down by 4 to get dwords, then multiply by 4 (shift up by 2) to translate dwords to bytes
	add rsi, r8 ; address of TCP data
	mov eax, ecx ; start
	cmp ecx, edx ; start >= end
	jge .receiveRegionNotContiguous
	; memcpy(block.receiveBuffer + start, packet.dataBuffer, end - start)
	mov rdi, [rbx + TCP_TCB_RECEIVE_BUFFER_OFFSET]
	add rdi, rax
	mov ecx, edx
	sub ecx, eax ; size = end - start
	rep movsb
	mov ecx, eax
	jmp .receiveRegionContiguous
.receiveRegionNotContiguous:
	; memcpy(block.receiveBuffer + start, packet.dataBuffer, block.receiveBufferSize - start)
	mov rdi, [rbx + TCP_TCB_RECEIVE_BUFFER_OFFSET]
	add rdi, rax
	mov ecx, [rbx + TCP_TCB_RECEIVE_BUFFER_CAPACITY_OFFSET]
	sub ecx, eax ; size = receiveBufferSize - start
	rep movsb
	; memcpy(block.receiveBuffer, packet.dataBuffer + block.receiveBufferSize - start, end)
	mov rdi, [rbx + TCP_TCB_RECEIVE_BUFFER_OFFSET]
	mov ecx, edx
	rep movsb
	mov ecx, eax
.receiveRegionContiguous:
	
.receiveBufferSizeWasWrong:

	pop rax
	pop rsi
	or esi, 0b10 ; set needsAck
.noProcessSegmentText:


	test byte [rax + TCP_HEADER_FLAGS_OFFSET], TCP_HEADER_FLAG_FINALIZE
	jz .noFin
	mov ecx, [rbx + TCP_TCB_STATE_OFFSET]
	cmp ecx, TCP_STATE_SYN_RECEIVED
	je .finSynReceivedOrEstablished
	cmp ecx, TCP_STATE_ESTABLISHED
	je .finSynReceivedOrEstablished
	cmp ecx, TCP_STATE_FIN_WAIT_1
	je .finFinWait1
	cmp ecx, TCP_STATE_FIN_WAIT_2
	je .finFinWait2
	cmp ecx, TCP_STATE_TIME_WAIT
	je .finTimeWait
	jmp .finSetNeedsAck
.finSynReceivedOrEstablished:
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_CLOSE_WAIT
	add dword [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET], 1
	mov ecx, TCP_NOTIFY_HALF_CLOSED
	call tcpNotify
	jmp .finSetNeedsAck
.finFinWait1:
	mov ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	cmp ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	jne .finNotDoneSendingData
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_TIME_WAIT
	mov ecx, TCP_TIME_WAIT_TIMEOUT
	call tcpResetDeletionTimeout
	jmp .finSetNeedsAck
.finNotDoneSendingData:
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_CLOSING
	jmp .finSetNeedsAck
.finFinWait2:
	mov dword [rbx + TCP_TCB_STATE_OFFSET], TCP_STATE_TIME_WAIT
	add dword [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET], 1
.finTimeWait:
	mov ecx, TCP_TIME_WAIT_TIMEOUT
	call tcpResetDeletionTimeout
.finSetNeedsAck:
	or esi, 0b10 ; set needsAck bit
.noFin:

	call waitForTxPacket

	mov ecx, [rbx + TCP_TCB_DUPLICATE_ACK_COUNT_OFFSET]
	cmp ecx, 3
	jng .duplicateAckCountNotGreaterThan3
	mov edx, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	add [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET], edx
	or esi, 0b1 ; sendWindowUpdated = true
.duplicateAckCountNotGreaterThan3:

	cmp ecx, 3
	je .duplicateAckCountIs3
	test esi, 0b10 ; check needsAck
	jnz .sendAckOrDataWithAck
	test esi, 0b1 ; check sendWindowUpdated
	jnz .trySendDataWithNewSendWindow
	jmp .end
.duplicateAckCountIs3:
	mov ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	sub ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	shr ecx, 1
	mov edx, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	add edx, edx
	cmp ecx, edx
	cmovl ecx, edx
	mov [rbx + TCP_TCB_SLOW_START_THRESHOLD_OFFSET], ecx ; slowStartThreshold = max((sendNext - sendUnacknowledged) / 2, 2 * sendMaximumSegmentSize)
	mov edx, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	lea edx, [edx + edx * 2]
	add ecx, edx
	mov [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET], ecx
	mov edi, [rbx + TCP_TCB_RESEND_NEXT_OFFSET]
	mov edx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	mov [rbx + TCP_TCB_RESEND_NEXT_OFFSET], edx
	; TODO ...
	mov edx, ecx
	mov ecx, 1
	call tcpSendData
	mov [rbx + TCP_TCB_RESEND_NEXT_OFFSET], edi
	jmp .end
.sendAckOrDataWithAck:
	; u32 congestionWindow = block.congestionWindow;
	; if(block.duplicateAckCount > 0 && block.duplicateAckCount <= 2){
	;	congestionWindow += 2 * block.sendMaximumSegmentSize;
	; }
	mov ecx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	mov edx, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	lea edx, [ecx + edx * 2]
	mov edi, [rbx + TCP_TCB_DUPLICATE_ACK_COUNT_OFFSET]
	sub edi, 1
	cmp edi, 2
	cmovl ecx, edx
	mov edx, ecx
	mov ecx, 1
	call tcpSendData
	jnc .trySendDataSuccess
	mov edx, TCP_HEADER_FLAG_ACKNOWLEDGE ; flags
	mov esi, [rbx + TCP_TCB_RECEIVE_NEXT_OFFSET] ; ack
	mov edi, [rbx + TCP_TCB_SEND_NEXT_OFFSET] ; seq
	call tcpSendPacketFromTCB
.trySendDataSuccess:
	jmp .end
.trySendDataWithNewSendWindow:
	mov ecx, 1
	mov edx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	call tcpSendData
.end:
	pop r11
	pop r10
	pop r9
	pop r8
	ret

deletionTimeoutSTr db "Deletion Timeout",0x0a,0x00
retransmitState db "Retransmit State: ",0x00

; RFC 9293 "Transmission Control Protocol (TCP)" section 3.10.8
; rbx contains block address
tcpHandleTimeout:
	push rax
	push rcx
	push rdx

	mov eax, [PIT_CLOCK_MILLISECONDS]
	sub eax, [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET]
	cmp eax, 0
	jnge .noRetransmit
	mov ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	cmp ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	je .allSentDataAcknowledged
	; Algorithm from RFC 5681 "TCP Congestion Control"
	sub ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	shr ecx, 1
	mov edx, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	add edx, edx
	cmp ecx, edx
	cmovl ecx, edx
	mov [rbx + TCP_TCB_SLOW_START_THRESHOLD_OFFSET], ecx ; slowStartThreshold = max((sendNext - sendUnacknowledged) / 2, 2 * sendMaximumSegmentSize);
	; It seems a bit extreme to set the congestion window all the way back to one segment
	; when a single packet times out, but that's what the RFC says.
	mov ecx, [rbx + TCP_TCB_SEND_MAXIMUM_SEGMENT_SIZE_OFFSET]
	mov [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET], ecx
	mov ecx, [rbx + TCP_TCB_SEND_NEXT_OFFSET]
	sub ecx, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	mov edx, [rbx + TCP_TCB_SEND_DATA_POS_OFFSET]
	sub edx, ecx
	mov eax, edx
	add eax, [rbx + TCP_TCB_SEND_BUFFER_CAPACITY_OFFSET]
	cmp edx, 0
	cmovl edx, eax
	mov [rbx + TCP_TCB_SEND_DATA_POS_OFFSET], edx
	add [rbx + TCP_TCB_SEND_DATA_SIZE_OFFSET], ecx
	mov eax, [rbx + TCP_TCB_SEND_UNACKNOWLEDGED_OFFSET]
	mov [rbx + TCP_TCB_RESEND_NEXT_OFFSET], eax
	mov eax, [PIT_CLOCK_MILLISECONDS]
	add eax, [rbx + TCP_TCB_RETRANSMIT_TIMEOUT_MILLISECONDS_OFFSET]
	mov [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET], eax
	and dword [rbx + TCP_TCB_CLOSE_REQUESTED_OFFSET], ~TCP_TCB_CLOSE_REQUESTED_FIN_SENT_BIT
	
	mov eax, [rbx + TCP_TCB_STATE_OFFSET]
	cmp eax, TCP_STATE_SYN_SENT
	je .retransmitStateWasSynSentOrSynReceived
	cmp eax, TCP_STATE_SYN_RECEIVED
	je .retransmitStateWasSynSentOrSynReceived
	mov ecx, 0xFFFFFFFF
	mov edx, [rbx + TCP_TCB_CONGESTION_WINDOW_OFFSET]
	call tcpSendData
	jmp .noRetransmit
.retransmitStateWasSynSentOrSynReceived:
	mov edx, TCP_HEADER_FLAG_SYNCHRONIZE
	mov ecx, TCP_HEADER_FLAG_SYNCHRONIZE | TCP_HEADER_FLAG_ACKNOWLEDGE
	cmp eax, TCP_STATE_SYN_RECEIVED
	cmove edx, ecx
	call tcpSendSYNPacket
	jmp .noRetransmit
.allSentDataAcknowledged:
	mov dword [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
.noRetransmit:
	

	mov eax, [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET]
	cmp eax, TCP_INVALID_TIMEOUT_VALUE
	je .noDeletionTimeout
	sub eax, [PIT_CLOCK_MILLISECONDS]
	cmp eax, 0
	jnle .noDeletionTimeout
	;DEBUG_PRINT_STR deletionTimeoutSTr
	mov eax, [rbx + TCP_TCB_STATE_OFFSET]
	cmp eax, TCP_STATE_ESTABLISHED
	je .deletionTimeoutStateEstablished
	mov ecx, TCP_NOTIFY_TIMED_OUT | TCP_NOTIFY_CLOSED
	mov edx, TCP_NOTIFY_CLOSED
	cmp eax, TCP_STATE_TIME_WAIT
	cmove ecx, edx
	call tcpMarkForDelete
	mov dword [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	jmp .noDeletionTimeout
.deletionTimeoutStateEstablished:
	mov ecx, TCP_NOTIFY_TIMED_OUT
	call tcpNotify
	mov dword [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
.noDeletionTimeout:

	
	mov eax, [rbx + TCP_TCB_NEXT_RETRANSMIT_TIME_OFFSET]
	mov ecx, [rbx + TCP_TCB_DELETION_TIMEOUT_TIME_OFFSET]
	mov edx, ecx
	cmp eax, TCP_INVALID_TIMEOUT_VALUE
	cmove eax, edx
	sub ecx, eax
	cmp ecx, 0
	cmovle eax, edx
	mov [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], eax

	pop rdx
	pop rcx
	pop rax
	ret

; Called from timer interrupt
tcpCoarseClock:
	push rax
	push rbx

.checkQueueLoop:
	mov rax, TCP_TIMEOUT_QUEUE_OFFSET
	movzx eax, word [rax]
	imul eax, eax, TCP_BLOCK_SIZE
	mov rbx, TCP_CONNECTIONS_OFFSET
	add rbx, rax ; block
	cmp dword [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	je .end
	mov eax, [PIT_CLOCK_MILLISECONDS]
	sub eax, [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET]
	cmp eax, 0
	jl .end
	mov dword [rbx + TCP_TCB_NEXT_CHECK_TIME_OFFSET], TCP_INVALID_TIMEOUT_VALUE
	call tcpHandleTimeout
	xor ebx, ebx
	call tcpTimerPriorityQueueUpdateElement
	jmp .checkQueueLoop

.end:
	pop rbx
	pop rax
	ret

tcpConnectionNotifiedStr db "  Connection Notified!",0x0a,0x00
connectionOpenedStr db "  Connection Opened!",0x0a,0x00
closingConnectionStr db "  Closing Connection!",0x0a,0x00
dataReceivedStr db "  Data Received! Size: ",0x00
freeingConnectionStr db "  Freeing Connection!",0x0a,0x00
connectionResetStr db "  Connection Reset!",0x0a,0x00
sendingResponseStr db "Sending response...",0x0a,0x00
responseStr db "Same thing we do every night: try to take over the world!",0x0a,0x00 ; 59

tcpServerLoop:
	push rax
	push rbx
	push rcx
	push rdx
	push r8
	push r9
	push rbp
	mov rbp, rsp
	sub rsp, 256

	xorps xmm0, xmm0
	xor r8d, r8d
	
	xor esi, esi
	mov eax, 1024*1024*512
	call mapPhysicalToVirtual
	mov r9, rax
	add eax, 0x1000
	call mapPhysicalToVirtual
	add eax, 0x1000
	call mapPhysicalToVirtual
	add eax, 0x1000
	call mapPhysicalToVirtual
	add eax, 0x1000
	call mapPhysicalToVirtual
	add eax, 0x1000
	call mapPhysicalToVirtual
	add eax, 0x1000
	call mapPhysicalToVirtual
	add eax, 0x1000
	call mapPhysicalToVirtual

.serverLoop:
.messageLoop:
	call tcpRead
	mov rbx, rax
	mov eax, eax ; flags
	shr rbx, 32 ; ebx = connectionIdx
	cmp ebx, TCP_CONNECTION_BLOCK_INVALID_IDX
	je .messageLoopBreak

	;DEBUG_PRINT_STR tcpConnectionNotifiedStr
	
	
	

	test eax, TCP_NOTIFY_OPENED
	jz .tcpNotOpened
	; DO got here
	DEBUG_PRINT_STR connectionOpenedStr
	DEBUG_SEND_STR connectionOpenedStr
.tcpNotOpened:

	test eax, TCP_NOTIFY_HALF_CLOSED | TCP_NOTIFY_TIMED_OUT | TCP_NOTIFY_CLOSED
	jz .tcpShouldNotClose
	DEBUG_PRINT_STR closingConnectionStr
	call tcpClose
.tcpShouldNotClose:

	test eax, TCP_NOTIFY_DATA
	jz .tcpDoesNotHaveData
	movups [rbp], xmm0
	movups [rbp + 16], xmm0
	movups [rbp + 32], xmm0
	movups [rbp + 48], xmm0
	movups [rbp + 64], xmm0
	movups [rbp + 80], xmm0
	movups [rbp + 96], xmm0
	movups [rbp + 112], xmm0
	mov rcx, r9
.receiveLoop:
	mov edx, 0x8000
	call tcpReceive
	;DEBUG_PRINT_STR dataReceivedStr
	;DEBUG_PRINT_NUM edx
	;DEBUG_PRINT_NUM r8d
	;call debugPrintNewline
	;DEBUG_PRINT_STR rcx
	add r8d, edx
	test edx, edx
	jnz .receiveLoop
	
	cmp r8d, 1024 * 1024
	jl .dontSendResponseYet
	DEBUG_SEND_STR sendingResponseStr
	lea rcx, [responseStr]
	mov edx, 59
	call tcpSend
	sub r8d, 1024 * 1024
	;DEBUG_PRINT_STR rcx
.dontSendResponseYet:
	
.tcpDoesNotHaveData:

	test eax, TCP_NOTIFY_RESET
	jz .tcpDoesNotHaveRst
	DEBUG_PRINT_STR connectionResetStr
	DEBUG_SEND_STR connectionResetStr
.tcpDoesNotHaveRst:

	test eax, TCP_NOTIFY_CLOSED
	jz .tcpShouldNotDelete
	DEBUG_PRINT_STR freeingConnectionStr
	DEBUG_SEND_STR freeingConnectionStr
	call tcpFree
.tcpShouldNotDelete:

	jmp .messageLoop
.messageLoopBreak:
	hlt
	jmp .serverLoop

	leave
	pop r9
	pop r8
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret






;///// END TCP CODE /////





receivedPacketCountStr db "Receive packet count: ",0x0

; Currently called from an interrupt, might change that later
; rax has packet data address, max VIRTIO_NET_PACKET_BUFFER_PACKET_SIZE bytes
processPacket:
	push rax
	push rbx
	push rcx
	push rdx
	push rsi
	push rdi
	push rbp
	mov rbp, rsp
	sub rsp, 32
	; Local variable table
	; [rbp - 4] : dhcp message type
	; [rbp - 8] : dhcp ip lease time
	; [rbp - 12] : dhcp server identifier
	; [rbp - 16] : sender ip address
	; [rbp - 18] : source port (for UDP or TCP, remote machine)
	; [rbp - 20] : destination port (for UDP or TCP, local machine)
	; [rbp - 24] : destination ip address
	; [rbp - 32] : sender mac address

	; Zero local variables
	pxor xmm0, xmm0
	movups [rbp - 16], xmm0
	movups [rbp - 32], xmm0
	
	mov rbx, [VIRTIO_RECEIVED_PACKET_COUNT]
	add rbx, 1
	;DEBUG_PRINT_STR receivedPacketCountStr
	;DEBUG_PRINT_NUM rbx
	mov [VIRTIO_RECEIVED_PACKET_COUNT], rbx

	;cmp ebx, 16
	;jge .end

	; Process ethernet header
	cmp word [rax + 12], HTON16(ETHERTYPE_ARP)
	jne .ethernetPacketNotARP
	cmp dword [ARP_LOOKUP_CURRENT_STATE], ARP_LOOKUP_STATE_PENDING
	jne .end ; make sure we're actually looking for an ARP packet
	add rax, ETHERNET_HEADER_SIZE
	; Process ARP header, see if it's the ARP packet we're looking for
	cmp word [rax + ARP_HEADER_HARDWARE_TYPE_OFFSET], HTON16(ARP_HARDWARE_TYPE_ETHERNET)
	jne .end
	cmp word [rax + ARP_HEADER_PROTOCOL_TYPE_OFFSET], HTON16(ETHERTYPE_IPV4)
	jne .end
	cmp byte [rax + ARP_HEADER_HARDWARE_ADDRESS_SIZE_OFFSET], 6 ; must have 6 byte mac
	jne .end
	cmp byte [rax + ARP_HEADER_PROTOCOL_ADDRESS_SIZE_OFFSET], 4 ; must have 4 byte IPv4
	jne .end
	cmp word [rax + ARP_HEADER_OPCODE_OFFSET], HTON16(ARP_OP_REPLY)
	jne .end
	; DO got here
	mov ecx, [ARP_LOOKUP_TARGET_IP]
	cmp [rax + ARP_HEADER_SENDER_IP_ADDRESS_OFFSET], ecx
	jne .end
	; DO got here?
	mov rcx, [rax + ARP_HEADER_SENDER_MAC_ADDRESS_OFFSET]
	shl rcx, 16 ; take top two bytes off, since mac addresses are only 6 bytes
	shr rcx, 16
	mov [ARP_LOOKUP_RESULT_MAC], rcx
	mov dword [ARP_LOOKUP_CURRENT_STATE], ARP_LOOKUP_STATE_SUCCESS
	jmp .end
.ethernetPacketNotARP:
	cmp word [rax + 12], HTON16(ETHERTYPE_IPV4) ; See if this is an ipv4 ethernet packet
	jne .end
	
	; Save sender mac address
	mov rcx, 0x0000FFFFFFFFFFFF ; mac address mask
	mov rbx, [rax + 6]
	and rbx, rcx
	mov [rbp - 32], rbx ; Save sender mac to local
	; check if target is either us or broadcast mac
	mov rbx, [rax]
	and rbx, rcx
	cmp rbx, rcx
	je .shouldReceiveEthernetIPv4Packet ; broadcast address
	cmp rbx, [VIRTIO_NET_MAC]
	je .shouldReceiveEthernetIPv4Packet ; our address
	jmp .end
.shouldReceiveEthernetIPv4Packet:
	
	; Process IPv4 header
	add rax, ETHERNET_HEADER_SIZE
	mov bl, [rax]
	shr bl, 4
	cmp bl, 4
	jne .end ; Check that ip version is 4
	movzx ebx, byte [rax]
	and ebx, 0xF
	cmp ebx, 5
	jl .end ; Check that the header size is at least 5
	; Checksum the header
	xor edx, edx
	clc
.checksumIPv4Header:
	adc edx, [rax + rbx * 4 - 4]
	dec ebx
	jnz .checksumIPv4Header
	adc edx, 0
	mov ecx, edx
	shr ecx, 16
	add dx, cx
	adc dx, 0
	cmp dx, 0xFFFF
	je .IPv4ChecksumOK
	lea rax, [packetChecksumMismatch]
	call debugPrint
	;DEBUG_SEND_STR packetChecksumMismatch
	movzx eax, dx
	call debugPrintlnInteger
	jmp .end
.IPv4ChecksumOK:
	movzx edx, word [rax + 2]
	ror dx, 8 ; Get length in bytes
	cmp dx, VIRTIO_NET_PACKET_BUFFER_PACKET_SIZE - ETHERNET_HEADER_SIZE
	jg .end ; Packet too big, the buffer doesn't contain all the data
	mov ecx, [rax + 12] ; Get sender address
	mov [rbp - 16], ecx ; Save sender address to local
	; Check if we're the destination
	mov ecx, [rax + 16] ; Get destination address
	mov [rbp - 24], ecx ; Store to local
	cmp ecx, 0xFFFFFFFF
	je .shouldReceiveIPv4Packet ; broadcast address
	cmp ecx, [VIRTIO_NET_IP_ADDRESS]
	je .shouldReceiveIPv4Packet ; our address
	jmp .end
.shouldReceiveIPv4Packet:
	movzx ecx, byte [rax + 9] ; Get protocol type
	; Move past IPv4 header
	movzx ebx, byte [rax]
	and ebx, 0xF
	lea ebx, [ebx * 4] ; Get header size in bytes
	cmp edx, ebx
	jl .end ; Packet size was less than header size, invalid
	sub edx, ebx ; Cap on next layer's size
	lea rax, [rax + rbx]

	cmp ecx, INTERNET_PROTOCOL_UDP
	jne .packetNotUDP
	
	; DO got here
	
	; Process UDP packet
	movzx ecx, word [rax + 4] ; Get length
	ror cx, 8 ; Swap to native order
	cmp ecx, 8
	jl .end ; UDP packet size must be at least 8 (for the header)
	cmp ecx, edx
	jg .end ; UDP packet size should fall within IPv4's bounds
	; Check if we should skip checksum verification
	; If a virtual device talks to another virtual device and
	; checksum offload is enabled, no checksum will actually be computed
	cmp word [rax + 2], HTON16(DHCP_CLIENT_PORT)
	je .udpSkipChecksum
	; If the checksum is 0, it means the sender did not compute a checksum
	cmp word [rax + 2], 0
	je .udpSkipChecksum
	; Verify checksum
	push rcx
	mov rbx, rax ; Base address needs to be in rbx for addIPChecksum
	mov eax, ecx ; Packet size needs to be in eax for addIPChecksum
	xor ecx, ecx ; This will contain our computed checksum
	; UDP pseudo header
	mov ecx, [rbp - 16] ; Src ip
	mov edx, [rbp - 24] ; Add dst ip
	add rcx, rdx
	movzx edx, word [rbx + 4] ; Get UDP length
	shl edx, 16
	or edx, (INTERNET_PROTOCOL_UDP << 8)
	add rcx, rdx
	call addIPChecksum
	mov rax, rcx
	pop rcx
	cmp ax, 0xFFFF
	jne .end ; Checksum incorrect
	mov rax, rbx ; Restore base address to rax
.udpSkipChecksum:
	; Checksum complete
	mov dx, [rax] ; Get src port
	mov [rbp - 18], dx ; Save src port to local
	mov dx, [rax + 2] ; Get dst port
	mov [rbp - 20], dx ; Save dst port to local
	cmp dx, HTON16(DHCP_CLIENT_PORT)
	jne .notDHCPPacket
	cmp dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_WAIT_FOR_OFFER
	je .shouldReceiveDHCPPacket
	cmp dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_PENDING_ACK
	je .shouldReceiveDHCPPacket
	jmp .end
.shouldReceiveDHCPPacket:
	; DO got here
	; receiveDHCPPacket
	add rax, 8
	sub ecx, 8
	cmp ecx, DHCP_SIZE_MIN + 4 ; Plus 4 for the DHCP magic cookie
	jl .end ; Not enough data for a DHCP packet
	cmp byte [rax + DHCP_OP_OFFSET], DHCP_OP_BOOTREPLY
	jne .end ; Needs to be a reply
	cmp byte [rax + DHCP_HTYPE_OFFSET], ARP_HARDWARE_TYPE_ETHERNET
	jne .end ; Needs to be ethernet
	cmp byte [rax + DHCP_HLEN_OFFSET], 6
	jne .end ; hardware address length must be 6 (size of mac address)
	mov ebx, [DHCP_CURRENT_XID]
	cmp [rax + DHCP_XID_OFFSET], ebx
	jne .end ; Not our transaction
	cmp dword [rax + DHCP_OPTIONS_OFFSET], DHCP_OPTION_MAGIC_COOKIE
	jne .end ; This is a bootp packet, not a DHCP one? Probably a mistake
	mov rsi, rax ; Save bootp base before going into options (need it in case there's an option overload option)
	add rax, DHCP_SIZE_MIN + 4 ; options offset
	sub ecx, DHCP_SIZE_MIN + 4 ; size

	; Interpret DHCP options
	xor edi, edi ; edi will store option flags. 0b0001 is message type, 0b0010 is lease time, 0b0100 is server identifier, 0b1000 is option overload
	jmp .dhcpOptionsContinue
.dhcpOptionsLoop:
	movzx ebx, byte [rax]
	cmp ebx, DHCP_OPTION_END
	je .dhcpOptionsEnd
	cmp ebx, DHCP_OPTION_PAD
	jne .optionNotPad
	add rax, 1
	sub ecx, 1
	jmp .dhcpOptionsContinue
.optionNotPad:
	; Regular options, all have a size byte after
	cmp ecx, 2
	jl .dhcpOptionsEnd ; Not enough bytes left
	movzx edx, byte [rax + 1] ; Option size
	add rax, 2
	sub ecx, 2
	cmp edx, ecx ; option size <= total size
	jg .dhcpOptionsEnd ; option size too large
	; See which option it is
	cmp ebx, DHCP_OPTION_DHCP_MESSAGE_TYPE_OP
	jne .optionNotMessageType
	cmp edx, DHCP_OPTION_DHCP_MESSAGE_TYPE_LEN
	jne .dhcpOptionsEnd ; message type option must have one byte
	movzx ebx, byte [rax] ; Get message type
	mov [rbp - 4], ebx ; Store message type to local
	or edi, 0b0001 ; Mark found message type
	jmp .dhcpRegularOptionDone
.optionNotMessageType:
	cmp ebx, DHCP_OPTION_ROUTER_OP
	jne .optionNotRouter
	cmp edx, DHCP_OPTION_ROUTER_LEN
	jl .dhcpOptionsEnd
	mov ebx, [rax] ; get router ip
	mov [DHCP_GATEWAY_IP], ebx
	jmp .dhcpRegularOptionDone
.optionNotRouter:
	cmp ebx, DHCP_OPTION_IP_ADDRESS_LEASE_TIME_OP
	jne .optionNotLeaseTime
	cmp edx, DHCP_OPTION_IP_ADDRESS_LEASE_TIME_LEN
	jne .dhcpOptionsEnd ; must be a 4 byte time in seconds
	mov ebx, [rax] ; Get time
	bswap ebx ; Swap to little endian
	mov [rbp - 8], ebx ; Store time to local
	or edi, 0b0010 ; Mark found lease time
	jmp .dhcpRegularOptionDone
.optionNotLeaseTime:
	cmp ebx, DHCP_OPTION_SERVER_IDENTIFIER_OP
	jne .optionNotServerIdentifier
	cmp edx, DHCP_OPTION_SERVER_IDENTIFIER_LEN
	jne .dhcpOptionsEnd ; must be a 4 byte IP address
	mov ebx, [rax] ; Get server identifier
	mov [rbp - 12], ebx ; Store server identifier to local
	or edi, 0b0100 ; Mark found server identifier
	jmp .dhcpRegularOptionDone
.optionNotServerIdentifier:
	cmp ebx, DHCP_OPTION_OPTION_OVERLOAD_OP
	jne .optionNotOptionOverload
	cmp edx, DHCP_OPTION_OPTION_OVERLOAD_LEN
	jne .dhcpOptionsEnd ; must have exactly one byte
	test edi, 0b1000 ; See if already in an overload
	jnz .dhcpOptionsEnd ; If so, can't do it again
	or edi, 0b1000 ; Found an option overload
	movzx ebx, byte [rax]
	cmp ebx, 1
	jne .optionOverloadNotFile
	lea rax, [rsi + DHCP_FILE_OFFSET] ; Use the file field as part of options
	mov ecx, 128 ; Size of file field
	jmp .dhcpOptionsContinue
.optionOverloadNotFile:
	cmp ebx, 2
	jne .optionOverloadNotSname
	lea rax, [rsi + DHCP_SNAME_OFFSET] ; Use the sname filed as part of options
	mov ecx, 64 ; Size of sname field
	jmp .dhcpOptionsContinue
.optionOverloadNotSname:
	cmp ebx, 3
	jne .optionOverloadNotFileAndSname
	lea rax, [rsi + DHCP_SNAME_OFFSET] ; Use both file and sname fields as part of options (sname comes directly before file)
	mov ecx, 128 + 64 ; Size of file + size of sname fields
	jmp .dhcpOptionsContinue
.optionOverloadNotFileAndSname:
	jmp .dhcpOptionsEnd ; value was not 1, 2, or 3. Invalid.
.optionNotOptionOverload:
	; Unknown option, just skip it
.dhcpRegularOptionDone:
	add rax, rdx ; Add option size to pointer
	sub ecx, edx ; Subtract option size from total size
.dhcpOptionsContinue:
	cmp ecx, 1 ; See if we have another option left
	jge .dhcpOptionsLoop
.dhcpOptionsEnd:
	
	cmp dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_WAIT_FOR_OFFER
	jne .notWaitForOffer
	cmp dword [rbp - 4], DHCPOFFER ; See if local message type is DHCPOFFER
	jne .end ; Wasn't an offer message
	and edi, 0b0111
	cmp edi, 0b0111 ; Must have message type, lease time, and server identifier
	jne .end
	mov rbx, [PIT_CLOCK_MILLISECONDS]
	mov [DHCP_LEASE_START], rbx
	mov ebx, [rbp - 8] ; Get DHCP lease time from local
	imul rbx, rbx, 1000 ; Convert to milliseconds
	mov [DHCP_LEASE_TIME], rbx
	mov ebx, [rbp - 12] ; Get server identifier from local
	mov [DHCP_SERVER_IDENTIFIER], ebx
	mov ebx, [rsi + DHCP_YIADDR_OFFSET] ; Get our offered ip address
	mov [DHCP_OFFERED_IP], ebx
	cmp dword [DHCP_GATEWAY_IP], 0
	jne .dhcpRouterFieldPresent
	mov ebx, [rbp - 16]
	mov [DHCP_GATEWAY_IP], ebx
.dhcpRouterFieldPresent:
	mov dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_OFFER_RECEIVED

	jmp .dhcpPacketProcessed
.notWaitForOffer:
	cmp dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_PENDING_ACK
	jne .notPendingAck
	cmp dword [rbp - 4], DHCPACK ; See if local message type is DHCPACK
	jne .pendingAckNotAck
	mov dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_CONFIGURED
	jmp .dhcpPacketProcessed
.pendingAckNotAck:
	cmp dword [rbp - 4], DHCPNAK ; See if local message type is DHCPNAK
	jne .pendingAckNotNak
	mov dword [DHCP_CURRENT_STATE], DHCP_CLIENT_STATE_DENIED
	jmp .dhcpPacketProcessed
.pendingAckNotNak:
	jmp .dhcpPacketProcessed
.notPendingAck:
.dhcpPacketProcessed:

	lea rax, [dhcpPacketReceived]
	call debugPrint
	jmp .end
.notDHCPPacket:

	; See if we have a DNS packet
	cmp word [rbp - 20], HTON16(DNS_CLIENT_PORT) ; Compare UDP dst port
	jne .notDNSPacket
	cmp word [rbp - 18], HTON16(DNS_SERVER_PORT) ; Compare UDP src port
	jne .notDNSPacket
	cmp dword [DNS_LOOKUP_RESULT_CODE], DNS_LOOKUP_RESULT_LOOKUP_PENDING
	jne .end
	add rax, 8
	sub ecx, 8
	cmp ecx, DNS_MESSAGE_HEADER_SIZE
	jl .end
	mov dx, [rax + DNS_MESSAGE_HEADER_ID_OFFSET]
	cmp word [DNS_LOOKUP_MESSAGE_ID], dx
	jne .end
	test word [rax + DNS_MESSAGE_HEADER_FLAGS_OFFSET], HTON16(DNS_MESSAGE_HEADER_FLAG_RESPONSE)
	jz .end

	; Alright, we have our DNS response. Time to process it and see if it contains the IP address we need
	DEBUG_PRINT_STR dnsAnswerReceived
	mov dword [DNS_LOOKUP_RESULT_CODE], DNS_LOOKUP_RESULT_FAILURE ; Set to failure now, will be set to success if we find the answer we want
	movzx ebx, word [rax + DNS_MESSAGE_HEADER_FLAGS_OFFSET]
	ror bx, 8 ; swap to little endian
	and ebx, DNS_MESSAGE_HEADER_FLAG_RESPONSE_CODE_MASK
	cmp ebx, DNS_MESSAGE_RESPONSE_CODE_SUCCESS
	jne .end
	mov rdi, rax ; Save packet base
	add rax, DNS_MESSAGE_HEADER_SIZE
	sub ecx, DNS_MESSAGE_HEADER_SIZE

	; Skip all questions, don't care about those
	ror word [rdi + DNS_MESSAGE_HEADER_QUESTION_COUNT_OFFSET], 8 ; swap to little endian
	cmp word [rdi + DNS_MESSAGE_HEADER_QUESTION_COUNT_OFFSET], 0
	je .noDNSQuestions
.dnsProcessQuestionsLoop:
	cmp ecx, 5 ; Make sure we have at least a null terminator and type and class left
	jl .end ; Invalid, not enough data
.dnsQuestionNameProcessLoop:
	movzx ebx, byte [rax]
	test ebx, ebx
	jnz .dnsQuestionNameProcessNotNullTerminator
	; Skip null terminator and break
	add rax, 1
	sub ecx, 1
	jmp .dnsQuestionNameProcessed
.dnsQuestionNameProcessNotNullTerminator:
	and bl, 0b11000000
	cmp bl, 0b11000000
	jne .dnsQuestionNameProcessNotPointer
	; Pointer, don't care about the contents, just skip it
	add rax, 2
	sub ecx, 2
	jmp .dnsQuestionNameProcessed
.dnsQuestionNameProcessNotPointer:
	test bl, bl
	jnz .end ; Invalid bit pattern
	movzx ebx, byte [rax]
	add ebx, 1
	cmp ebx, ecx
	jg .end
	add rax, rbx
	sub ecx, ebx
	cmp ecx, 5 ; Make sure we have at least a null terminator and type and class left
	jl .end
	jmp .dnsQuestionNameProcessLoop
.dnsQuestionNameProcessed:
	add rax, 4 ; Skip type and class
	sub ecx, 4
	sub word [rdi + DNS_MESSAGE_HEADER_QUESTION_COUNT_OFFSET], 1
	cmp word [rdi + DNS_MESSAGE_HEADER_QUESTION_COUNT_OFFSET], 0
	jne .dnsProcessQuestionsLoop
.noDNSQuestions:

	; Restore packet base and size
	sub rax, rdi
	add ecx, eax
	mov ebx, eax ; packetIdx
	mov rax, rdi

	; Go through answers, see if any match our name
	ror word [rax + DNS_MESSAGE_HEADER_ANSWER_COUNT_OFFSET], 8 ; swap to little endian
	cmp word [rax + DNS_MESSAGE_HEADER_ANSWER_COUNT_OFFSET], 0
	je .noDNSAnswers
.dnsProcessAnswersLoop:
	mov ebp, ecx ; ebp = length
	sub ebp, ebx ; ebp = length - packetIdx
	cmp ebp, DNS_RESOURCE_RECORD_RDATA_OFFSET + 1
	jl .end ; if length - packetIdx < DNS_RESOURCE_RECORD_RDATA_OFFSET + 1, not enough data left
	mov rsi, [DNS_LOOKUP_NAME_PTR] ; name
	mov edi, ebx ; labelReadIdx = packetIdx
	mov byte [DNS_LOOKUP_RESULT_TTL_MILLISECONDS], 0 ; Use the TTL variable as a temporary recursion counter
.dnsAnswerNameProcessLoop:
	movzx edx, byte [rax + rdi] ; segmentLength
	test edx, edx
	jnz .dnsAnswerNameSegmentLengthNotZero
	add edi, 1 ; labelReadIdx++
	cmp edi, ebx ; labelReadIdx > packetIdx
	cmovg ebx, edi ; packetIdx = max(packetIdx, labelReadIdx)
	jmp .dnsAnswerNameProcessed
.dnsAnswerNameSegmentLengthNotZero:
	and dl, 0b11000000
	cmp dl, 0b11000000
	jne .dnsAnswerNotPointer
	; Pointer
	cmp byte [DNS_LOOKUP_RESULT_TTL_MILLISECONDS], 32 ; See if the temporary recursion counter is greater than 32 (arbitrary cutoff)
	jge .end ; If so, this is probably a broken message, end
	add byte [DNS_LOOKUP_RESULT_TTL_MILLISECONDS], 1 ; Increment recursion counter
	movzx ebp, word [rax + rdi] ; newOffset = packetBase[labelReadIdx]
	ror bp, 8 ; newOffset = bswap16(newOffset)
	and bp, 0xFFFF >> 2 ; take off high indicator bits
	cmp ebp, edi ; newOffset >= labelReadIdx
	jge .end ; pointer did not point backwards
	add edi, 2
	cmp edi, ebx
	cmovg ebx, edi ; packetIdx = max(packetIdx, labelReadIdx + 2)
	mov edi, ebp ; labelReadIdx = newOffset
	jmp .dnsAnswerNameProcessLoop
.dnsAnswerNotPointer:
	test dl, dl
	jnz .end ; Indicator bit patterns 10 and 01 are invalid
	movzx edx, byte [rax + rdi] ; segment length
	add edx, 1 ; segmentLengthPlus1
	add edi, edx ; labelReadIdx + segmentLengthPlus1
	cmp edx, ecx ; labelReadIdx + segmentLengthPlus1 > length
	jg .end ; Too few bytes in packet
	sub edi, edx ; Restore labelReadIdx
	push rcx ; push length
	test rsi, rsi
	jz .dnsAnswerNameCompareEnd ; name was nullptr
	mov ebp, 1 ; i iterator
	cmp ebp, edx ; i == segmentLengthPlus1
	je .dnsAnswerNameCompareEnd ; Empty string
.dnsAnswerNameCompareLoop:
	add ebp, edi ; i += labelReadIdx
	mov cl, [rax + rbp]
	sub ebp, edi ; i -= labelReadIdx
	cmp cl, '-'
	je .dnsAnswerNameCompareLabelCharacterValid
	cmp cl, '0'
	jl .dnsAnswerNameCompareNotEqual
	cmp cl, '9'
	jle .dnsAnswerNameCompareLabelCharacterValid
	cmp cl, 'A'
	jl .dnsAnswerNameCompareNotEqual
	cmp cl, 'Z'
	jle .dnsAnswerNameCompareLabelCharacterValid
	cmp cl, 'a'
	jl .dnsAnswerNameCompareNotEqual
	cmp cl, 'z'
	jg .dnsAnswerNameCompareNotEqual
.dnsAnswerNameCompareLabelCharacterValid:
	cmp cl, [rsi + rbp - 1] ; character == name[i - 1]
	je .dnsAnswerNameCompareOk
.dnsAnswerNameCompareNotEqual:
	xor esi, esi ; name = nullptr
	jmp .dnsAnswerNameCompareEnd
.dnsAnswerNameCompareOk:
	add ebp, 1 ; i++
	cmp ebp, edx ; i != segmentLengthPlus1
	jne .dnsAnswerNameCompareLoop
.dnsAnswerNameCompareEnd:
	pop rcx ; pop length
	test rsi, rsi
	jz .dnsAnswerNameWasNull
	lea rsi, [rsi + rdx - 1] ; name += segmentLengthPlus1 - 1
	cmp byte [rsi], '.'
	jne .dnsAnswerNameNoDot
	add rsi, 1 ; name++
.dnsAnswerNameNoDot:
.dnsAnswerNameWasNull:
	add edi, edx ; labelReadIdx += segmentLengthPlus1
	cmp edi, ebx ; labelReadIdx > packetIdx
	cmovg ebx, edi ; packetIdx = max(packetIdx, labelReadIdx)
	mov ebp, ecx ; lengthLeft = length
	sub ebp, ebx ; lengthLeft = length - packetIdx
	cmp ebp, DNS_RESOURCE_RECORD_RDATA_OFFSET + 1 ; lengthLeft < DNS_RESOURCE_RECORD_RDATA_OFFSET + 1	
	jl .end ; Not enough data left
	jmp .dnsAnswerNameProcessLoop
.dnsAnswerNameProcessed:
	test rsi, rsi ; name == nullptr
	jz .dnsAnswerNotValid ; name was null
	cmp byte [rsi], 0 ; name[0] == '\0'
	jne .dnsAnswerNotValid ; name didn't end on a null terminator
	cmp word [rax + rbx + DNS_RESOURCE_RECORD_TYPE_OFFSET], HTON16(DNS_TYPE_A)
	jne .dnsAnswerNotValid
	cmp word [rax + rbx + DNS_RESOURCE_RECORD_CLASS_OFFSET], HTON16(DNS_CLASS_IN)
	jne .dnsAnswerNotValid
	cmp word [rax + rbx + DNS_RESOURCE_RECORD_RDATA_LENGTH_OFFSET], HTON16(4)
	jne .dnsAnswerNotValid
	mov ebp, ecx
	sub ebp, ebx
	cmp ebp, DNS_RESOURCE_RECORD_RDATA_OFFSET + 4
	jl .end ; Didn't have 4 bytes left
	; Found the answer we were looking for
	mov ecx, [rax + rbx + DNS_RESOURCE_RECORD_TTL_OFFSET]
	bswap ecx ; swap to little endian
	imul rcx, rcx, 1000 ; seconds to milliseconds
	mov [DNS_LOOKUP_RESULT_TTL_MILLISECONDS], rcx
	mov ecx, [rax + rbx + DNS_RESOURCE_RECORD_RDATA_OFFSET]
	mov [DNS_LOOKUP_RESULT_IP], ecx
	mov dword [DNS_LOOKUP_RESULT_CODE], DNS_LOOKUP_RESULT_SUCCESS
	DEBUG_PRINT_STR dnsAddressFound
	jmp .end
.dnsAnswerNotValid:
	add ebx, DNS_RESOURCE_RECORD_RDATA_OFFSET
	sub word [rax + DNS_MESSAGE_HEADER_ANSWER_COUNT_OFFSET], 1
	cmp word [rax + DNS_MESSAGE_HEADER_ANSWER_COUNT_OFFSET], 0
	jne .dnsProcessAnswersLoop
.noDNSAnswers:

.notDNSPacket:
	jmp .end
.packetNotUDP:
	cmp ecx, INTERNET_PROTOCOL_TCP
	jne .packetNotTCP

	cmp dword [TCP_IS_INITIALIZED], 0
	je .end ; Not done initializing TCP yet (why are we getting TCP packets this early?)

	;DEBUG_SEND_STR debugGotHere2

	; Process TCP packet
	movzx ecx, byte [rax + TCP_HEADER_DATA_OFFSET_OFFSET] ; Get TCP data offset
	shr ecx, 2 ; shift the bits down by 4 to get dwords, then multiply by 4 (shift by 2) to translate dwords to bytes
	cmp ecx, 20
	jl .end ; TCP header size must be at least 20
	cmp ecx, edx
	jg .end ; TCP header size should fall within IPv4's bounds
	mov esi, ecx ; esi = header size
	mov edi, edx ; edi = total TCP size
	; Verify checksum
	mov rbx, rax ; Base address needs to be in rbx for addIPChecksum
	mov eax, edi ; Packet size needs to be in eax for addIPChecksum
	xor ecx, ecx ; This will contain our computed checksum
	; TCP pseudo header
	mov ecx, [rbp - 16] ; Src ip
	mov edx, [rbp - 24] ; Add dst ip
	add rcx, rdx
	mov edx, edi ; Get TCP length (including header, not including pseudoheader)
	ror dx, 8 ; swap to network byte order
	shl edx, 16
	or edx, (INTERNET_PROTOCOL_TCP << 8)
	add rcx, rdx
	call addIPChecksum
	cmp cx, 0xFFFF
	jne .end ; Checksum incorrect
	mov ecx, edi ; Put packet size in ecx for handleTCPPacket
	sub ecx, esi ; handleTCPPacket expects the size without the header
	mov rax, rbx ; Restore base address to rax
	; Checksum complete
	movbe dx, [rax + TCP_HEADER_SRC_PORT_OFFSET] ; Get src port
	mov [rbp - 18], dx ; Save src port to local
	movbe dx, [rax + TCP_HEADER_DST_PORT_OFFSET] ; Get dst port
	mov [rbp - 20], dx ; Save dst port to local
	;DEBUG_PRINT_STR handlingTCPPacket
	push rax
	rdtsc
	mov ebx, eax
	shl rdx, 32
	or rbx, rdx
	pop rax
	push rbx
	call handleTCPPacket
	pop rbx
	rdtsc
	shl rdx, 32
	or rax, rdx
	sub rax, rbx
	;call debugPrintlnInteger

	;DEBUG_PRINT_STR handlingTCPPacketComplete
.packetNotTCP:
.end:
	add rsp, 32
	pop rbp
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop rbx
	pop rax
	ret

debugGotHere db "GotHere",0x0a,0x00
debugGotHere2 db "GotHere2",0x0a,0x00
debugGotHere3 db "GotHere3",0x0a,0x00
faultOccured db "Fault Occured",0x0a,0x00
nameAssignedNull db "Name Assigned Null",0x0a,0x00
functionEndStr db "Function End",0x0a,0x00
packetChecksumMismatch db "Packet Checksum Mismatch",0x0a,0x00
dhcpPacketReceived db "DHCP Packet Received",0x0a,0x00
dhcpConfigurationComplete db "DHCP Configuration Complete!",0x0a,0x00
dnsAnswerReceived db "DNS Answer Received!",0x0a,0x00
dnsAddressFound db "DNS Address Found!",0x0a,0x00
webtestDotDrillengineDotNet db "webtest.drillengine.net",0x00
googleDotCom db "google.com",0x00
handlingTCPPacket db "Handling TCP Packet",0x0a,0x00
handlingTCPPacketComplete db "Handling TCP Packet Complete",0x0a,0x00
serverStartupStr db "Server Startup",0x0a,0x00

times 512 - ($-$$) % 512 db 0
endBootloader: