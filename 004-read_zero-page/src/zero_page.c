#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/export.h>
#include <linux/memblock.h>

typedef union _cr3
{
    unsigned long cr3_value;
    struct cr3_struct_t
    {
        unsigned long no_use1 : 3;     // 0-2
        unsigned long PWT : 1;         // 3 Page-level Write-Through
        unsigned long PCD : 1;         // 4 Page-level Cache Disable
        unsigned long no_use2 : 7;     // 5-11
        unsigned long pml4t_base : 52; // 12-63, maybe 40,maybe 52, depends on design,all ok
    } cr3_struct;
} cr3_t;

typedef union _pml4e
{
    unsigned long pml4e_value;
    struct
    {
        unsigned long present : 1;            // Must be 1, region invalid if 0.
        unsigned long rw : 1;                 // If 0, writes not allowed.
        unsigned long user_supervisor : 1;    // If 0, user-mode accesses not allowed.
        unsigned long page_write_through : 1; // Determines the memory type used to access PDPT.
        unsigned long page_cache : 1;         // Determines the memory type used to access PDPT.
        unsigned long accessed : 1;           // If 0, this entry has not been used for translation.
        unsigned long Ignored1 : 1;
        unsigned long page_size : 1; // Must be 0 for PML4E.
        unsigned long Ignored2 : 4; // 12
        unsigned long pfn : 36; // The page frame number of the PDPT of this PML4E.
        unsigned long reserved : 4;
        unsigned long Ignored3 : 11;
        unsigned long nx : 1; // If 1, instruction fetches not allowed.
    } plm4e_struct;
} plm4e_t;

typedef union _virt_addr_t
{
    unsigned long value;
    struct virt_addr_t
    {
        unsigned long offset : 12;
        unsigned long pt_index : 9;
        unsigned long pd_index : 9;
        unsigned long pdpt_index : 9;
        unsigned long pml4_index : 9;
        unsigned long reserved : 16;
    } value_struct;
} virt_addr_t;

void parseVaddr(unsigned long vaddr)
{
    unsigned long entry_size = 8;
    unsigned long shifted_address;
    unsigned long pte_offset, pde_offset, pdpte_offset, pmle_offset, offset;
    virt_addr_t vaddr_to_struct;

    offset = vaddr & 0xfff;
    shifted_address = vaddr >> 12;
    pte_offset = shifted_address & 0x1ff;

    shifted_address = shifted_address >> 9;
    pde_offset = shifted_address & 0x1ff;

    shifted_address = shifted_address >> 9;
    pdpte_offset = shifted_address & 0x1ff;

    shifted_address = shifted_address >> 9;
    pmle_offset = shifted_address & 0x1ff;

    printk(KERN_INFO "PGD/PUD/PMD/PT Entry_size : %ld", entry_size);
    printk(KERN_INFO "Type 1 : shift ops");
    printk(KERN_INFO "[0x%lX] Entry index: PML4:0x%lX PDPT:0x%lX PD:0x%lX PT:0x%lX OFFSET:0x%lX", vaddr, pmle_offset, pdpte_offset, pde_offset, pte_offset, offset);
    printk(KERN_INFO "[0x%lX] Offset(index*entry_size): PML4:0x%lX PDPT:0x%lX PD:0x%lX PT:0x%lX", vaddr, pmle_offset * entry_size, pdpte_offset * entry_size, pde_offset * entry_size, pte_offset * entry_size);

    vaddr_to_struct.value = vaddr;
    printk(KERN_INFO "Type 2 : to union struct");
    printk(KERN_INFO "[0x%lX] Entry index: PML4:0x%x PDPT:0x%x PD:0x%x PT:0x%x OFFSET:0x%x",
           vaddr_to_struct.value,
           vaddr_to_struct.value_struct.pml4_index,
           vaddr_to_struct.value_struct.pdpt_index,
           vaddr_to_struct.value_struct.pd_index,
           vaddr_to_struct.value_struct.pt_index,
           vaddr_to_struct.value_struct.offset);

    printk(KERN_INFO "[0x%lX] Offset(index*entry_size): PML4:0x%lX PDPT:0x%lX PD:0x%lX PT:0x%lX",
           vaddr_to_struct.value,
           vaddr_to_struct.value_struct.pml4_index * entry_size,
           vaddr_to_struct.value_struct.pdpt_index * entry_size,
           vaddr_to_struct.value_struct.pd_index * entry_size,
           vaddr_to_struct.value_struct.pt_index * entry_size,
           vaddr_to_struct.value_struct.offset * entry_size);
}

void walkpml4e(virt_addr_t pml4e)
{
    plm4e_t pml4e_paddr;
    unsigned long pml4e_pfn, pdpt_vaddr;
    // pml4
    pml4e_paddr.pml4e_value = pml4e.value;
    pml4e_pfn = pml4e_paddr.plm4e_struct.pfn;
    pdpt_vaddr = phys_to_virt(pml4e_paddr.plm4e_struct.pfn << 12);
    // pdpt

    // pd

    // pt
    printk(KERN_INFO " PGD: pfn : 0x%lX pfn base paddr : 0x%lX pfn vaddr : 0x%lX \n", pml4e_pfn, pml4e_pfn << 12, pdpt_vaddr);
}

static int __init
zero_page_init(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Loading zero_page Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n", __FILE__, __func__, __LINE__);
    printk(KERN_INFO "mem_map = 0x%ld\n", mem_map);
    cr3_t cr3;
    unsigned long *cr3_virt_addr;

    // 32bit cr3=32bit 64bit cr3=64bit
    asm volatile("mov %%cr3, %0\n\t"
                 : "=r"(cr3.cr3_value), "=m"(__force_order));

    printk("CR3 Value = 0x%016lx (Physical Addr with special bits) PWT(1b): %x PCD(1b): %x\n", cr3.cr3_value, cr3.cr3_struct.PWT, cr3.cr3_struct.PCD);
    printk("CR3 Value = 0x%016lx (Physical linear Addr) \n", cr3.cr3_struct.pml4t_base << 12);

    cr3_virt_addr = phys_to_virt(cr3.cr3_struct.pml4t_base << 12);
    printk("CR3 Value = 0x%016lx (Virtual Addr)\n", cr3_virt_addr);
    parseVaddr(cr3_virt_addr);
    unsigned long *pml4e_cursor = (unsigned long *)cr3_virt_addr;
    // 读取PML4E，只有4K，每项8字节，则一共有512条，内核、用户态平分
    unsigned long i = 0;
    for (; i < 512; i++)
    {
        if (0 != *pml4e_cursor)
        {
            printk("[%03d] PML4E Addr: 0x%016lx Value(Paddr): 0x%016lx [*]\n", i, pml4e_cursor, *pml4e_cursor);
            walkpml4e(*(union _virt_addr_t *)pml4e_cursor);
        }
        //  else {
        //     printk("[%03d] PML4E Addr: 0x%016lx Value: 0x%016lx\n", i, pml4e_cursor, *pml4e_cursor);
        //  }
        pml4e_cursor++;
    }

    return 0;
}

static void __exit zero_page_exit(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Removing zero_page Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n", __FILE__, __func__, __LINE__);
}

module_init(zero_page_init);
module_exit(zero_page_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("zero_page Module");
MODULE_AUTHOR("nicyou");
