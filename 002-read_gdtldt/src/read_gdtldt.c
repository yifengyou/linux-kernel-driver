#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
// #include <asm/desc_defs.h>

// http://charette.no-ip.com:81/programming/2009-12-30_Virtualization/2009-12-30_Virtualization.c

typedef struct
{
    // Lots of *really bad* examples on the web where they use char[2]
    // to store the output of SIDT and SGDT.  This is wrong!
    //
    // In 32-bit mode, the output is 6 bytes, where the first 2 is
    // the limit and the upper 4 bytes is the 32-bit base address.
    //
    // In 64-bit mode, the output is 10 bytes, where the first 2 is
    // the limit and the upper 8 bytes is the 64-bit base address.
    //
    // This is described on page 299 of AMD64 Architecture Programmer's
    // Manual Volume 3: General-Purpose and System Instructions:
    // http://www.amd.com/us-en/assets/content_type/white_papers_and_tech_docs/24594.pdf

    unsigned long limit : 16;
    unsigned long base : 64;

} __attribute__((packed)) dt;

/*
 * FIXME: Accessing the desc_struct through its fields is more elegant,
 * and should be the one valid thing to do. However, a lot of open code
 * still touches the a and b accessors, and doing this allow us to do it
 * incrementally. We keep the signature as a struct, rather than an union,
 * so we can get rid of it transparently in the future -- glommer
 */
/* 8 byte segment descriptor */
// struct desc_struct  linux 4-6
// {
//     u16 limit0;
//     u16 base0;
//     u16 base1 : 8, type : 4, s : 1, dpl : 2, p : 1;
//     u16 limit1 : 4, avl : 1, l : 1, d : 1, g : 1, base2 : 8;
// } __attribute__((packed));

unsigned long getGDT(void)
{
    // Global Descriptor Table Register
    //
    // This is the "no pill" variation on the original "red pill".
    struct desc_struct *gdt_pt;
    dt gdt = {0, 0};
    unsigned long i = 0;
    asm volatile("sgdt %0"
                 : "=m"(gdt));
    printk(KERN_INFO "GDTR value : limit(16bit)=%04d, base(64bit)=%016lx\n", gdt.limit, gdt.base);

    printk(KERN_INFO "GDT Entry define:\n"
                     "struct desc_struct  linux 4-6\n"
                     "{\n"
                     "  u16 limit0;\n"
                     "  u16 base0;\n"
                     "  u16 base1 : 8, type : 4, s : 1, dpl : 2, p : 1;\n"
                     "  u16 limit1 : 4, avl : 1, l : 1, d : 1, g : 1, base2 : 8;\n"
                     "} __attribute__((packed));\n");
    // see svg : https://raw.githubusercontent.com/jmolloy/JMTK/master/doc/gdt-descriptor.svg
    printk(KERN_INFO "base0/1/2 - no use in 64bit long mode"
                     "limit0/limit1 - no use in 64bit long mode"
                     "p - Is segment present? (1 = Yes)\n"
                     "dpl - Descriptor privilege level - Ring 0 - 3.\n"
                     "type - Segment type - code segment / data segment. for code X,E,W,A ; for data X,C,R,A\n"
                     "avl - Available for system use (always zero).\n"
                     "g - Granularity (0 = 1 byte, 1 = 1kbyte)\n"
                     "l - long mode(1=64bit 0=32bit)\n"
                     "d - Operand size (0 = 16bit, 1 = 32bit)\n");
    printk(KERN_INFO " sizeof(struct desc_struct) = %ld Bytes", sizeof(struct desc_struct));
    gdt_pt = (struct desc_struct *)gdt.base;
    for (; i < gdt.limit;)
    {
        if (gdt_pt->p)
        {
            printk(KERN_INFO " *GDT ENTRY[%03ld] Addr=0x%p limit0(16b)=%04x limit1(4b)=%x base(16b)=%04x base1(8b)=%02x base2(8b)=%x type(4b)=%01x s(1b)=%x dpl(2b)=%x p(1b)=%x avl(1b)=%x l(1b)=%x d(1b)=%x g(1b)=%x\n",
                   i / sizeof(struct desc_struct),
                   gdt_pt,
                   gdt_pt->limit0,
                   gdt_pt->limit1,
                   gdt_pt->base0,
                   gdt_pt->base1,
                   gdt_pt->base2,
                   gdt_pt->type,
                   gdt_pt->s,
                   gdt_pt->dpl,
                   gdt_pt->p,
                   gdt_pt->avl,
                   gdt_pt->l,
                   gdt_pt->d,
                   gdt_pt->g);
        }
        else {
            printk(KERN_INFO "  GDT ENTRY[%03ld] Addr=0x%p limit0(16b)=%04x limit1(4b)=%x base(16b)=%04x base1(8b)=%02x base2(8b)=%x type(4b)=%01x s(1b)=%x dpl(2b)=%x p(1b)=%x avl(1b)=%x l(1b)=%x d(1b)=%x g(1b)=%x\n",
                   i / sizeof(struct desc_struct),
                   gdt_pt,
                   gdt_pt->limit0,
                   gdt_pt->limit1,
                   gdt_pt->base0,
                   gdt_pt->base1,
                   gdt_pt->base2,
                   gdt_pt->type,
                   gdt_pt->s,
                   gdt_pt->dpl,
                   gdt_pt->p,
                   gdt_pt->avl,
                   gdt_pt->l,
                   gdt_pt->d,
                   gdt_pt->g);
        }
        gdt_pt++;
        i += sizeof(struct desc_struct);
    }

    return gdt.base;
}

// struct gate_struct_dev
// {
//     u16 offset_low;
//     u16 segment;
//     struct idt_bits bits;
//     u16 offset_middle;
//     u32 offset_high;
//     u32 reserved;
// } __attribute__((packed));

unsigned long getIDT(void)
{
    // Interrupt Descriptor Table Register
    //
    // This is the original "red pill" and "scoopy_doo" method of determining
    // whether or not we're running in a VM.  The check would be against the
    // base address returned by SIDT.  If the table was not at the usual
    // location, then it was assumed we were running in a VM.  However, this
    // check can return false-positives with multi-core and multi-processor,
    // since the table is per-cpu/core.

    struct gate_struct *idt_pt;
    unsigned long i = 0;
    dt idt = {0, 0};
    asm volatile("sidt %0"
                 : "=m"(idt));
    printk(KERN_INFO "IDT: limit=%04d, base=%016lx\n", idt.limit, idt.base);

    printk(KERN_INFO "IDT Entry define:\n"
                     "struct gate_struct {\n"
                     "    u16 offset_low;\n"
                     "    u16 segment;\n"
                     "    struct idt_bits bits;\n"
                     "    u16 offset_middle;\n"
                     "#ifdef CONFIG_X86_64\n"
                     "    u32 offset_high;\n"
                     "    u32 reserved;\n"
                     "#endif\n"
                     "} __attribute__((packed));\n");

    // printk(KERN_INFO "base0/1/2 - no use in 64bit long mode"
    //                  "limit0/limit1 - no use in 64bit long mode"
    //                  "p - Is segment present? (1 = Yes)\n"
    //                  "dpl - Descriptor privilege level - Ring 0 - 3.\n"
    //                  "type - Segment type - code segment / data segment. 对于数据段来说，这4位分别是X,E,W,A，对于代码段来说这4位分别是X,C,R,A\n"
    //                  "avl - Available for system use (always zero).\n"
    //                  "g - Granularity (0 = 1 byte, 1 = 1kbyte)\n"
    //                  "l - long mode(1=64bit 0=32bit)\n"
    //                  "d - Operand size (0 = 16bit, 1 = 32bit)\n");

    printk(KERN_INFO " sizeof(struct gate_struct) = %ld Bytes", sizeof(struct gate_struct));
    idt_pt = (struct gate_struct *)idt.base;
    for (; i < idt.limit;)
    {
        // printk(KERN_INFO "  idt ENTRY[%03ld] segment(16)=0x%p\n",
        //        i,
        //        idt_pt);

        printk(KERN_INFO "idt ENTRY[%03ld] Addr=[%p] offset_high(32b)=0x%08x offset_middle(16b)=0x%04x offset_low(16b)=0x%04x segment(16b)=0x%04x idt_bits(16b)=0x%04x p(1b)=%x dpl(2b)=%x type(5b)=%02x ist(3b)=%x\n",
               i / sizeof(struct gate_struct),
               idt_pt,
               idt_pt->offset_high,
               idt_pt->offset_middle,
               idt_pt->offset_low,
               idt_pt->segment,
               *((unsigned short *)&(idt_pt->bits)),
               idt_pt->bits.p,
               idt_pt->bits.dpl,
               idt_pt->bits.type,
               idt_pt->bits.ist);
        // break;
        // idt_pt = (struct gate_struct *)(idt.base + sizeof(struct gate_struct) * i);
        idt_pt++;
        i += sizeof(struct gate_struct);
    }

    return idt.base;
}

unsigned short getLDT(void)
{
    // Local Descriptor Table Register
    //
    // This one is slightly different than IDT and idt.  It returns to us just
    // the 16-bit segment selector.  This is always a 16-bit value, regardless
    // of whether or not we're running in legacy i386 or in AMD64.

    unsigned long ldt = -1; // -1 will help us "see" this is a 16-bit value
    asm volatile("sldt %0"
                 : "=m"(ldt));
    printk(KERN_INFO "LDT: %04lx\n [ Linux doesn't use ldt! ]", ldt);

    return ldt;
}

unsigned short getSTR(void)
{
    // Task Register Selector
    //
    // Apparently this can be used to determine if this is within or outside
    // the "Matrix, but I get the same value whether I'm running natively or
    // within VirtualBox.
    // http://www.s21sec.com/descargas/vmware-eng.pdf
    unsigned long str = -1; // -1 will help us "see" this is a 16-bit value
    asm volatile("str %0"
                 : "=m"(str));
    printk(KERN_INFO "STR: %04lx\n", str);

    return str;
}

typedef struct
{
    unsigned short rpl : 2;
    unsigned short t1 : 1;
    unsigned short index : 13;
} segment_t;

void getAllSegment(void)
{
    segment_t cs;
    segment_t ds;
    segment_t gs;
    segment_t ss;
    segment_t es;
    segment_t fs;

    __asm__ __volatile__("mov %%cs, %[cs]"
                         : /* output */[cs] "=rm"(cs));
    __asm__ __volatile__("mov %%ds, %[ds]"
                         : /* output */[ds] "=rm"(ds));
    __asm__ __volatile__("mov %%gs, %[gs]"
                         : /* output */[gs] "=rm"(gs));
    __asm__ __volatile__("mov %%ss, %[ss]"
                         : /* output */[ss] "=rm"(ss));
    __asm__ __volatile__("mov %%es, %[es]"
                         : /* output */[es] "=rm"(es));
    __asm__ __volatile__("mov %%fs, %[fs]"
                         : /* output */[fs] "=rm"(fs));

    printk(KERN_INFO "Data in CS segement is - 0x%04x | 0x%04x(%d) 0x%x 0x%x\n", *(unsigned short *)&cs, cs.index, cs.index, cs.t1, cs.rpl);
    printk(KERN_INFO "Data in DS segement is - 0x%04x | 0x%04x(%d) 0x%x 0x%x\n", *(unsigned short *)&ds, ds.index, ds.index, ds.t1, ds.rpl);
    printk(KERN_INFO "Data in GS segement is - 0x%04x | 0x%04x(%d) 0x%x 0x%x\n", *(unsigned short *)&gs, gs.index, gs.index, gs.t1, gs.rpl);
    printk(KERN_INFO "Data in FS segement is - 0x%04x | 0x%04x(%d) 0x%x 0x%x\n", *(unsigned short *)&fs, fs.index, fs.index, fs.t1, fs.rpl);
    printk(KERN_INFO "Data in SS segement is - 0x%04x | 0x%04x(%d) 0x%x 0x%x\n", *(unsigned short *)&ss, ss.index, ss.index, ss.t1, ss.rpl);
    printk(KERN_INFO "Data in ES segement is - 0x%04x | 0x%04x(%d) 0x%x 0x%x\n", *(unsigned short *)&es, es.index, es.index, es.t1, es.rpl);
}
static int __init read_gdtldt_init(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Loading read_gdtldt Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n", __FILE__, __func__, __LINE__);

    getGDT();
    getIDT();
    getLDT();
    getSTR();
    getAllSegment();

    return 0;
}

static void __exit read_gdtldt_exit(void)
{
    printk(KERN_INFO "--------------------------------------------\n");
    printk(KERN_INFO "Removing read_gdtldt Module\n");
    printk(KERN_INFO "file:%s func:%s line:%d\n", __FILE__, __func__, __LINE__);
}

module_init(read_gdtldt_init);
module_exit(read_gdtldt_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("read_gdtldt Module");
MODULE_AUTHOR("nicyou");
