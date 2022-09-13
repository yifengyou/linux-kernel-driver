#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

// ffffffff9b404400 B boot_params
// ffffffffc0466000 d boot_params	[boot_params]
// extern struct boot_params boot_params;
struct boot_params *boot_params = (struct boot_params *)0xffffffff9b404400;
// ffffffff9b402008 B saved_command_line
// char *saved_command_line;
void * saved_command_line_ptr = (void *)0xffffffff99402008;


static int __init
boot_params_init(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Loading boot_params Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);

    printk(KERN_INFO "%d\n", boot_params->hdr.setup_sects);

    printk(KERN_INFO " saved_command_line %p\n", saved_command_line_ptr);
    
    return 0;
    // struct boot_params
    {
        // struct screen_info screen_info;         /* 0x000 */
        printk(KERN_INFO "struct screen_info screen_info : %p\n", &(boot_params->screen_info));
        // struct apm_bios_info apm_bios_info;     /* 0x040 */
        printk(KERN_INFO "struct apm_bios_info apm_bios_info : %p\n", &(boot_params->apm_bios_info));
        // __u8 _pad2[4];                          /* 0x054 */
        // __u64 tboot_addr;                       /* 0x058 */
        // struct ist_info ist_info;               /* 0x060 */
        printk(KERN_INFO "struct ist_info ist_info : %p\n", &(boot_params->ist_info));
        // __u8 _pad3[16];                         /* 0x070 */
        // __u8 hd0_info[16]; /* obsolete! */      /* 0x080 */
        // __u8 hd1_info[16]; /* obsolete! */      /* 0x090 */
        // struct sys_desc_table sys_desc_table;   /* 0x0a0 */
        printk(KERN_INFO "struct sys_desc_table sys_desc_table : %p\n", &(boot_params->sys_desc_table));
        // struct olpc_ofw_header olpc_ofw_header; /* 0x0b0 */
        printk(KERN_INFO "struct olpc_ofw_header olpc_ofw_header : %p\n", &(boot_params->olpc_ofw_header));
        // __u32 ext_ramdisk_image;                /* 0x0c0 */
        // __u32 ext_ramdisk_size;                 /* 0x0c4 */
        // __u32 ext_cmd_line_ptr;                 /* 0x0c8 */
        // printk(KERN_INFO "__u32 ext_cmd_line_ptr : %d\n", boot_params->ext_cmd_line_ptr);
        // __u8 _pad4[116];                        /* 0x0cc */
        // struct edid_info edid_info;             /* 0x140 */
        // struct efi_info efi_info;               /* 0x1c0 */
        // __u32 alt_mem_k;                        /* 0x1e0 */
        // __u32 scratch; /* Scratch field! */     /* 0x1e4 */
        // __u8 e820_entries;                      /* 0x1e8 */
        // __u8 eddbuf_entries;                    /* 0x1e9 */
        // __u8 edd_mbr_sig_buf_entries;           /* 0x1ea */
        // __u8 kbd_status;                        /* 0x1eb */
        // __u8 _pad5[3];                          /* 0x1ec */
        // __u8 sentinel;                              /* 0x1ef */
        // __u8 _pad6[1];                              /* 0x1f0 */
        // struct setup_header hdr; /* setup header */ /* 0x1f1 */
        printk(KERN_INFO "struct setup_header hdr : %p\n", &(boot_params->hdr));
        // struct setup_header
        {
            // __u8 setup_sects;
            printk(KERN_INFO "  __u8 setup_sects : %d\n", boot_params->hdr.setup_sects);
            // __u16 root_flags;
            printk(KERN_INFO "  __u16 root_flags : %d\n", boot_params->hdr.root_flags);
            // __u32 syssize;
            printk(KERN_INFO "  __u32 syssize : %d\n", boot_params->hdr.syssize);
            // __u16 ram_size;
            printk(KERN_INFO "  __u16 ram_size : %d\n", boot_params->hdr.ram_size);
            // __u16 vid_mode;
            printk(KERN_INFO "  __u16 vid_mode : %d\n", boot_params->hdr.vid_mode);
            // __u16 root_dev;
            printk(KERN_INFO "  __u16 root_dev : %d\n", boot_params->hdr.root_dev);
            // __u16 boot_flag;
            printk(KERN_INFO "  __u16 boot_flag : %d\n", boot_params->hdr.boot_flag);
            // __u16 jump;
            printk(KERN_INFO "  __u16 jump : %d\n", boot_params->hdr.jump);
            // __u32 header;
            printk(KERN_INFO "  __u32 header : %d\n", boot_params->hdr.header);
            // __u16 version;
            printk(KERN_INFO "  __u16 version : %d\n", boot_params->hdr.version);
            // __u32 realmode_swtch;
            printk(KERN_INFO "  __u32 realmode_swtch : %d\n", boot_params->hdr.realmode_swtch);
            // __u16 start_sys;
            printk(KERN_INFO "  __u16 start_sys : %d\n", boot_params->hdr.start_sys_seg);
            // __u16 kernel_version;
            printk(KERN_INFO "  __u16 kernel_version : %d\n", boot_params->hdr.kernel_version);
            // __u8 type_of_loader;
            printk(KERN_INFO "  __u8 type_of_loader : %d\n", boot_params->hdr.type_of_loader);
            // __u8 loadflags;
            printk(KERN_INFO "  __u8 loadflags : %d\n", boot_params->hdr.loadflags);
            // __u16 setup_move_size;
            printk(KERN_INFO "  __u16 setup_move_size : %d\n", boot_params->hdr.setup_move_size);
            // __u32 code32_start; // 保护模式内核入口地址 pm.c:134
            printk(KERN_INFO "  __u32 code32_start : %d\n", boot_params->hdr.code32_start);
            // __u32 ramdisk_image;
            printk(KERN_INFO "  __u32 ramdisk_image : %d\n", boot_params->hdr.ramdisk_image);
            // __u32 ramdisk_size;
            printk(KERN_INFO "  __u32 ramdisk_size : %d\n", boot_params->hdr.ramdisk_size);
            // __u32 bootsect_kludge;
            printk(KERN_INFO "  __u32 bootsect_kludge : %d\n", boot_params->hdr.bootsect_kludge);
            // __u16 heap_end_ptr;
            printk(KERN_INFO "  __u16 heap_end_ptr : %d\n", boot_params->hdr.heap_end_ptr);
            // __u8 ext_loader_ver;
            printk(KERN_INFO "  __u8 ext_loader_ver : %d\n", boot_params->hdr.ext_loader_ver);
            // __u8 ext_loader_type;
            printk(KERN_INFO "  __u8 ext_loader_type : %d\n", boot_params->hdr.ext_loader_type);
            // __u32 cmd_line_ptr;
            // __u32 initrd_addr_max;
            // __u32 kernel_alignment;
            // __u8 relocatable_kernel;
            printk(KERN_INFO "  __u8 relocatable_kernel : %d\n", boot_params->hdr.relocatable_kernel);
            // __u8 min_alignment;
            printk(KERN_INFO "  __u8 min_alignment : %d\n", boot_params->hdr.min_alignment);
            // __u16 xloadflags;
            printk(KERN_INFO "  __u16 xloadflags : %d\n", boot_params->hdr.xloadflags);
            // __u32 cmdline_size;
            printk(KERN_INFO "  __u16 cmdline_size : %d\n", boot_params->hdr.cmdline_size);
            // __u32 hardware_subarch;
            // __u64 hardware_subarch_data;
            // __u32 payload_offset;
            // __u32 payload_length;
            // __u64 setup_data;
            // __u64 pref_address;
            // __u32 init_size;
            printk(KERN_INFO "  __u32 init_size : %d\n", boot_params->hdr.init_size);
            // __u32 handover_offset;
            printk(KERN_INFO "  __u32 handover_offset : %d\n", boot_params->hdr.handover_offset);
        }
        // __u8 _pad7[0x290 - 0x1f1 - sizeof(struct setup_header)];
        // __u32 edd_mbr_sig_buffer[EDD_MBR_SIG_MAX]; /* 0x290 */
        // struct e820entry e820_map[E820MAX];        /* 0x2d0 */
        // __u8 _pad8[48];                            /* 0xcd0 */
        // struct edd_info eddbuf[EDDMAXNR];          /* 0xd00 */
        // __u8 _pad9[276];                           /* 0xeec */
    }

    return 0;
}

static void __exit 
boot_params_exit(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Removing boot_params Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n",__FILE__,__func__,__LINE__);
}

module_init(boot_params_init);
module_exit(boot_params_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("boot_params Module");
MODULE_AUTHOR("nicyou");
