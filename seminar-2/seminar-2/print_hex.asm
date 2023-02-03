; print_hex.asm
section .data
codes:
    db   '0123456789ABCDEF'

section .text
global _start

print_char: 
    push rdi ;кладем код символа в память, потому что системный вызов берет адрес символа в rsi
    mov rdx, 1     
    mov rsi, rsp ;получаем адрес символа через указатель на стек
    pop rdi
    mov rax, 1      ; Код системного вызова "Вывод"
    mov rdi, 1      ; Дескриптор stdout
    syscall         ; Системный вызов
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    xor rax, rax
    mov rdi, 0xA ;загружаем символ перевода строки и вызываем print_char
    call print_char
    ret
print_hex:
    mov  rdx, 1
    mov  rcx, 64
    .loop:
        push rdi
        sub  rcx, 4
        ; cl is a register, smallest part of rcx
        ; rax -- eax -- ax -- ah + al
        ; rcx -- ecx -- cx -- ch + cl
        sar  rdi, cl
        and  rdi, 0xf

        lea  rsi, [codes + rdi]
        mov  rax, 1
        mov rdi,1
        ; syscall leaves rcx and r11 changed
        push  rcx
        syscall
        pop  rcx

        pop  rdi
        ; test can be used for the fastest 'is it a zero?' check
        ; see docs for 'test' command
        test rcx, rcx
        jnz .loop

        call print_newline
        ret

print_3:
    sub rsp,24
    mov rcx,rsp
    mov qword[rcx], 0xaa
    mov qword[rcx+8], 0xbb
    mov qword[rcx+16], 0xff
    mov rdi, [rcx]
    call print_hex
    mov rdi, [rcx+8]
    call print_hex
    mov rdi, [rcx+16]
    call print_hex
    mov rsp,rcx
    ret



_start:
    ; number 1122... in hexadecimal format
    call print_3

    mov  rax, 60            ; invoke 'exit' system call
    xor  rdi, rdi
    syscall

    ; Each 4 bits should be output as one hexadecimal digit
    ; Use shift and bitwise AND to isolate them
    ; the result is the offset in 'codes' array