; string.asm
global world
global print_string
global print_file
section .rodata
message: db ", world!", 10, 0

%define O_RDONLY 0 
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2
%define SYS_WRITE 1
%define SYS_OPEN 2
%define SYS_MMAP 9
%define SYS_MUNMAP 11
%define SYS_CLOSE 3
%define SYS_FSTAT 5
%define SYS_EXIT 60
%define FD_STDOUT 1

section .data
    ; This is the file name. You are free to change it.
    fname: db 'hello.txt', 0

section .text

string_length:
    mov rax, 0
    .loop:
      xor  rax, rax
    .count:
      cmp  byte [rdi+rax], 0
      je   .end
      inc  rax
      jmp  .count
    .end:
      ret

world:
    mov  rdi, message

print_string:
    push rax
    push rdi
    call string_length
    mov  rsi, [rsp]
    mov  rdx, rax
    mov  rax, 1
    mov  rdi, 1
    push rcx
    syscall
    pop  rcx
    pop  rdi
    pop  rax
    ret

print_file:
    ; Вызовите open и откройте fname в режиме read only.
    mov  rax, SYS_OPEN
    ;mov  rdi, fname
    mov  rsi, O_RDONLY    ; Open file read only
    mov  rdx, 0 	      ; We are not creating a file
                          ; so this argument has no meaning
    syscall
    ; rax holds the opened file descriptor now
    push rax
    ; Вызовите mmap c правильными аргументами
    ; Дайте операционной системе самой выбрать, куда отобразить файл
    ; Размер области возьмите в размер страницы 
    ; Область не должна быть общей для нескольких процессов 
    ; и должна выделяться только для чтения.

    
    mov r8, rax
    
    mov rax, SYS_MMAP
    mov rdi, 0
    mov rsi, 4096 ; replace
    mov rdx, PROT_READ
    mov r10, MAP_PRIVATE
    
    mov r9, 0
    syscall
    ; now rax pointed to the mapped location
    ; с помощью print_string теперь можно вывести его содержимое
	
    mov rdi, [rsp]
    push rax
    sub rsp, 144
    
    mov rax, SYS_FSTAT
    mov rsi, rsp
    syscall
    
    mov rsi, [rsp+48]
    add rsp, 144
    
    mov rdi, [rsp]
    call print_string
    
    
    mov rax, SYS_MUNMAP
    pop rdi
    mov rsi, 4096  
    syscall
    
    mov rax, SYS_CLOSE
    pop rdi
    syscall
    

    ret
