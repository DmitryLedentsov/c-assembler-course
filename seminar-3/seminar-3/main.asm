section .text
%include "lib.inc"
extern print_hex
global _start
_start:
    
    mov  rdi, 0x1122334455667788
    call print_hex
    call exit