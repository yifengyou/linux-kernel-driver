#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/sched.h>
#include <linux/export.h>
#include <asm/paravirt.h>
#include <asm/paravirt_types.h>

static unsigned long vaddr = 0;
static unsigned long cr0, cr3;

// https://blog.csdn.net/weixin_43690845/article/details/105650221

void get_pgtable_macro(void)
{
    // cr0 = read_cr0();
    // cr3 = read_cr3();
    asm volatile("mov %%cr3, %0\n\t"
                 : "=r"(cr3), "=m"(__force_order));
    asm volatile("mov %%cr0, %0\n\t"
                 : "=r"(cr0), "=m"(__force_order));

    printk("cr0 = 0x%016lx, cr3 = 0x%016lx\n", cr0, cr3);

    printk("PGDIR SHIFT: %d-47[%d]\n", PGDIR_SHIFT, 47 - PGDIR_SHIFT + 1);
    printk("PUD SHIFT: %d-%d[%d]\n", PUD_SHIFT, PGDIR_SHIFT - 1, PGDIR_SHIFT - 1 - PUD_SHIFT + 1);
    printk("PMD SHIFT: %d-%d[%d]\n", PMD_SHIFT, PUD_SHIFT - 1, PUD_SHIFT - 1 - PMD_SHIFT + 1);
    printk("PAGE TALBE SHIFT: %d - %d[%d]\n", PAGE_SHIFT, PMD_SHIFT - 1, PMD_SHIFT - 1 - PAGE_SHIFT + 1);
    printk("PAGE OFFSET: %d - %d[%d]\n", 0, PAGE_SHIFT - 1, PAGE_SHIFT - 1 - 0 + 1);

    printk("PTRS_PER_PGD: %d\n", PTRS_PER_PGD);
    printk("PTRS_PER_PUD: %d\n", PTRS_PER_PUD);
    printk("PTRS_PER_PMD: %d\n", PTRS_PER_PMD);
    printk("PTRS_PER_PTE: %d\n", PTRS_PER_PTE);
    printk("PAGE_MASK: 0x%016lx\n", PAGE_MASK);
}

static int vaddr_2_paddr(unsigned long vaddr)
{
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned long paddr = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;

    pgd = pgd_offset(current->mm, vaddr);
    printk("pgd_val = 0x%016lx, pgd_index = %lu\n", pgd_val(*pgd), pgd_index(vaddr));
    if (pgd_none(*pgd))
    {
        printk("not map int pgd\n");
        return -1;
    }

    pud = pud_offset((p4d_t *)pgd, vaddr);
    if (pud_none(*pud))
    {
        printk("not map int pud\n");
        return -1;
    }

    pmd = pmd_offset(pud, vaddr);
    printk("pmd_val = 0x%016lx, pmd_index = %lu\n", pmd_val(*pmd), pmd_index(vaddr));
    if (pmd_none(*pmd))
    {
        printk("not map int pmd\n");
        return -1;
    }

    pte = pte_offset_kernel(pmd, vaddr);
    printk("pte_val = 0x%016lx, pte_index = %lu\n", pte_val(*pte), pte_index(vaddr));
    if (pte_none(*pte))
    {
        printk("not map int pte\n");
        return -1;
    }

    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = vaddr & ~PAGE_MASK;
    paddr = page_addr | page_offset;

    printk("page_addr:0x%016lx,page_offset:0x%016lx\n", page_addr, page_offset);
    printk("vaddr:0x%016lx,paddr:0x%016lx\n", vaddr, paddr);

    return paddr;
}

static int __init vaddr_to_paddr_init(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Loading vaddr_to_paddr Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n", __FILE__, __func__, __LINE__);

    //基础信息
    get_pgtable_macro();

    vaddr = __get_free_page(GFP_KERNEL);
    if (vaddr == 0)
    {
        printk("get free page error\n");
        return 0;
    }

    sprintf((char *)vaddr, "hello world");
    printk(KERN_INFO "__get_free_page success!\n");
    printk(KERN_INFO " 'hello world' string vaddr:%lx\n", vaddr);

    //完成地址转化
    vaddr_2_paddr(vaddr);

    return 0;
}

static void __exit vaddr_to_paddr_exit(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Removing vaddr_to_paddr Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n", __FILE__, __func__, __LINE__);
}

module_init(vaddr_to_paddr_init);
module_exit(vaddr_to_paddr_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("vaddr_to_paddr Module");
MODULE_AUTHOR("nicyou");
