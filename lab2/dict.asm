global find_word
extern string_equals

%include "colon.inc"

global _start

section .text

find_word:
					; rdi - адрес нуль-терм. имени
					; rsi - адрес последнего слова
					; адрес, если найдено, иначе rax = 0
mov r8, rdi									
.loop:
	mov r9, rsi
	cmp rsi, 0 			; проверяем, не пустой ли список слов
	je .exit_false 		; выйдем и скажем, что ничего не нашли
	mov rdi, r8			; положим строку в rdi
	add rsi, 8			; смотрим что там за строка
	call string_equals	; сравниваем строки
	mov rsi, r9			; возвращаем rsi на место (после string_equals укзатель на конец строки)
	cmp rax, 0 			; проверяем, что вернула функция
	jnz .exit_true		; если не 0 - слова совпали
	mov rsi, [rsi]		; иначе смотрим следующее слово
    jmp .loop
	
.exit_true:				; если нашли строку - сохраняем указатель на нее
	mov rax, rsi
	ret 
	
.exit_false:
	mov rax, 0			; если не нашли - сохраняем 0
	ret