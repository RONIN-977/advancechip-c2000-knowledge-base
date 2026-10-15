MEMORY { CODE : origin=0x10000 length=0x20000
         DATA : origin=0x20000000 length=0x400000 }
SECTIONS { .text : > CODE
           .rodata : > CODE
           .const : > CODE
           .data : > DATA
           .bss : > DATA
           .stack : > DATA }
