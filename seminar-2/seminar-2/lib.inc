section .text
 
; Принимает указатель на нуль-терминированную строку, возвращает её дли
; Принимает указатель на нуль-терминированную строку, выводит её в stdout
exit:
    mov  rax, 60
    xor  rdi, rdi
    syscall

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rcx,rcx ;i=0
    .loop: ;while(str[i]!=0) i++
        cmp byte[rcx+rdi],0 ;если конец -> переход на .end
        je .end
        inc rcx ;i++
        jmp .loop
    .end:
        mov rax, rcx
        ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    xor rax, rax
    call string_length
    mov rdx, rax ;rdx = len(str)
    mov rsi, rdi ;rsi = адрес строки
    mov rdi, 1 ;дескриптор stdout
    mov rax, 1 ;установка номера системного вызова
    syscall
    ret
; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax, rax
    xor rcx, rcx
    mov r8, 10 ;основание системы
    .loop: ;movzx - extend with 0
        movzx r9, byte[rdi+rcx] ;r9 = s[i]
        cmp r9,0 ;if r9==0 end
        je .end
        cmp r9b, '0' ;проверка на то что цифра. то есть код меньше 9 и больше 0.
        jl .end     ;если не цифра, дальше не парсим.
        cmp r9b, '9'
        jg .end

        mul r8 
        sub r9b, '0' ;получаем цифру,  вычитая из кода символа код 0. (ascii)
        add rax, r9 ;rax = rax*10 + digit. то есть сдвигаем на рязряд и добавляем цифру
        inc rcx ;i++
        jmp .loop

    .end
        mov rdx, rcx ;записываем в rdx длину. rdx = i
        ret
