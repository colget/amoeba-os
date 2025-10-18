ENTRY(_start)

PHDRS
{
    multiboot PT_LOAD FLAGS(5); /* Read + Execute */
    text PT_LOAD FLAGS(5);      /* Read + Execute */
    data PT_LOAD FLAGS(6);      /* Read + Write */
}

SECTIONS
{
    . = 0x100000; /* Kernel load address */

    .multiboot : {
        *(.multiboot)
    } :multiboot

    .text : {
        *(.text)
        *(.text.*)
    } :text

    .rodata : {
        *(.rodata)
        *(.rodata.*)
    } :text

    .data : {
        *(.data)
    } :data

    .bss : {
        *(.bss)
    } :data

    . = 0x20000;
    __stack_top = .;

    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note.GNU-stack)
        *(.iplt)
        *(.rel.dyn)
        *(.got)
        *(.got.plt)
        *(.igot.plt)
    }
}
