    section .text


; Принимает код возврата и завершает текущий процесс
exit:
    mov rax, 60
    syscall
    ret

; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
    xor rax, rax
    .loop:
        cmp byte [rdi], 0
        jz .exit
        inc rdi
        inc rax
        jmp .loop
    .exit:
    ret

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
    xor rax, rax
    push rdi
    call string_length
    pop rdi
    mov rsi, rdi
    mov rdx, rax
    mov rdi, 1
    mov rax, 1
    syscall
    ret

; Принимает код символа и выводит его в stdout
print_char:
    xor rax, rax
    push rdi
    mov rsi, rsp
    mov rdi, 1
    mov rdx, 1
    mov rax, 1
    syscall
    pop rdi
    ret

; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    xor rax, rax
    mov rdi, 0xA
    call print_char
    ret

; Выводит беззнаковое 8-байтовое число в десятичном формате
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    xor rax, rax
    mov r10, rsp
    push 0
    cmp rdi, 0
    jnz .loop
    mov rdi, 0x30
    call print_char
    pop rax
    ret
    .loop: 
        cmp rdi, 0
        jz .exit
        mov rdx, 0
        mov rax, rdi
        mov r11, 10
        div r11
        mov rdi, rax
        or rdx, 0x30
        shl rdx, 56
        push rdx
        add rsp, 7
        jmp .loop
    .exit:
        mov rdi, rsp
        call print_string    
        mov rsp, r10        
    ret

; Выводит знаковое 8-байтовое число в десятичном формате
print_int:
    xor rax, rax
    cmp rdi, 0
    jl .signed
    jmp print_uint
    .signed:
        push rdi
        mov rdi, '-'
        call print_char
        pop rdi
        neg rdi
        jmp print_uint

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    .loop:
        mov r10b,byte [rdi]
        mov r11b,byte [rsi]
        inc rdi
        inc rsi
        cmp r10b, r11b
        jne .not
        cmp r10b, 0
        je .yes
        jmp .loop
    .yes:
        mov rax, 1
        ret
    .not:
        mov rax, 0
        ret
; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    xor rax, rax
    mov rax, 0
    push 0
    mov rsi, rsp
    mov rdx, 1
    mov rdi, 0
    syscall
    cmp rax, 0
    jle .aaa
    pop rax
    ret
    .aaa:
        pop rax
        mov rax, 0
        ret

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
    push r12
    push r13
    push r15
    push rbx
    mov r12, rdi
    mov r13, rsi
    mov r15, rdi
    mov rbx, 0
    dec r13

    .loop:
        mov rax, 0
        cmp r13, 0
        jle .return
        call read_char
        push rax
        cmp rax, 0
        jnz .rax_not_0
        cmp r12, r15
        jz .return_poprax
        jmp .is_ws
        .rax_not_0:
        jz .is_ws
        mov rdi, rax
        call is_whitespace
        cmp rax, 0
        jz .is_not_ws
        .is_ws:
            pop rax
            cmp r15, r12
            je .loop
            mov [r12], byte 0
            mov rax, r15
            mov rdx, rbx
            jmp .return
        .is_not_ws:
            pop rax
            mov byte [r12], al
            inc rbx 
            inc r12
            dec r13
            jmp .loop
    .return_poprax:
        pop rax
        xor rax, rax
        xor rdx, rdx
    .return:
        pop rbx
        pop r15
        pop r13
        pop r12 
        ret

is_whitespace:
    cmp rdi, 0x20
    je .ret
    cmp rdi, 0x9
    je .ret
    cmp rdi, 0xA
    je .ret
    mov rax, 0
    ret
    .ret:   
        mov rax, 1
        ret


; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax, rax
    mov r11, 0
    push r13
    push r12
    mov r13, 10
    mov r12, 0
    .loop:
        cmp byte[rdi+r11], 0
        je .return
        mov r12b, byte [rdi + r11]
        cmp r12, 0x39
        ja .return_err
        cmp r12, 0x30
        jb .return_err
        sub r12, 0x30
        push rdx
        mul r13
        pop rdx
        add rax, r12
        inc r11
        jmp .loop
    .return_err:
    .return:
        pop r12
        pop r13
        mov rdx, r11
        ret

; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был)
; rdx = 0 если число прочитать не удалось
parse_int:
    xor rax, rax
    cmp byte[rdi], '-'
    jne parse_uint
    inc rdi
    call parse_uint 
    neg rax
    inc rdx
    retcmp byte[rdi], '-'
    jne parse_uint
    inc rdi
    call parse_uint 
    neg rax
    inc rdx
    ret

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
; rdi -> rsi -> rdx
string_copy:
    push rdi
    push rsi
    push rdx
    call string_length
    pop rdx
    pop rsi
    pop rdi
    cmp rax, rdx
    jnl .loop
    mov rdx, rax
    inc rdx
    mov r11, 0
    .loop:
        cmp r11, rdx
        jnl .loop_exit
        mov r10b, byte [rdi + r11]
        mov byte [rsi + r11], r10b
        inc r11
        jmp .loop
    .loop_exit:
        mov r11, rax
        inc r11
        cmp r11, rdx
        jg .aaaa
        ret
    .aaaa:
        mov rax, 0
        ret
