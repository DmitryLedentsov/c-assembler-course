; hello_mmap.asm
%define O_RDONLY 0 
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2
%define SYS_WRITE 1
%define SYS_OPEN 2
%define SYS_MMAP 9
%define SYS_EXIT 60
%define FD_STDOUT 1

section .data
    ; This is the file name. You are free to change it.
    fname: db 'hello.txt', 0

section .text
global _start

; use exit system call to shut down correctly
exit:
    mov  rax, SYS_EXIT
    xor  rdi, rdi
    syscall

; These functions are used to print a null terminated string
; rdi holds a string pointer
print_string:
    push rdi
    call string_length
    pop  rsi
    mov  rdx, rax 
    mov  rax, SYS_WRITE
    mov  rdi, FD_STDOUT
    syscall
    ret

string_length:
    xor  rax, rax
.loop:
    cmp  byte [rdi+rax], 0
    je   .end 
    inc  rax
    jmp .loop 
.end:
    ret

; This function is used to print a substring with given length
; rdi holds a string pointer
; rsi holds a substring length
print_substring:
    mov  rdx, rsi 
    mov  rsi, rdi
    mov  rax, SYS_WRITE
    mov  rdi, FD_STDOUT
    syscall
    ret

_start:
    ; Вызовите open и откройте fname в режиме read only.
    mov  rax, SYS_OPEN
    mov  rdi, fname
    mov  rsi, O_RDONLY    ; Open file read only
    mov  rdx, 0         ; We are not creating a file
                          ; so this argument has no meaning
    syscall

    push rax
    
    mov r8, rax
    mov rax, 9 ; mmap number
    mov rdi, 0 ; operating system will choose mapping destination
    mov rsi, 4096 ; page size
    mov rdx, PROT_READ ; new memory region will be marked read only
    mov r10, MAP_PRIVATE ; pages will not be shared
    mov r9, 0 ; offset inside test.txt
    syscall ; now rax will point to mapped location

    mov rdi, rax
    call print_string

    mov r9, rax
    pop rax

    push r9
    push rax

    mov rax, 11 ; mmap number
    mov rdi, r9 ; operating system will choose mapping destination
    mov rsi, 4096 ; page size
    syscall ; now rax will point to mapped location

    pop rax


    ;закрываем файл
    mov  rax, 3
    mov  rdi, fname
    mov  rsi, O_RDONLY
    mov  rdx, rax
    syscall

    call exit