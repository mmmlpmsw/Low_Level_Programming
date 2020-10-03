section .data
	msg: db "No word", 0

global _start	
	
section .text
%include "colon.inc"
%include "words.inc"

extern read_word
extern find_word
extern string_length
extern print_string
extern print_newline

_start: 
	
	mov rsi, 255	; максимальная длина слова, что мы можем записать в буфер
	sub rsp, 256	; освободим место под буфер в стеке
	
	mov rdi, rsp	; передадим вершину буфера в read_word
	call read_word	; начало слова - в rax
	
	cmp rax, 0		; проверяем, не 0 ли там (есть ли слово)
	je .false
	 
	mov rdi, rax	; сохраним слово в rdi
	mov rsi, next	; адрес посл. слва - в rsi
	call find_word	; делаем поиск - возвращает указатель на строку
	cmp rax, 0		; проверка на ноль, установка флагов
	je .false		; сюда, если слово не найдено
	add rax, 8		; указатель на начало строки, а не на адрес следующего слова	
	mov r10, rax 	; сохраняем значение rax
	mov rdi, rax	; передаем rax как аргкмент для string_length
					; дальше нам надо найти строку по метке 
					;(для этого надо найти сначала метку, которая лежит после строки)
		 						
	call string_length
	add r10, rax	; сдвинем указатель на начало метки
	inc r10			; учитываем нуль-терминатор
	mov rdi, r10	; ссылка на слово - в rdi
	
	mov r13, 1		; дескриптор для stdout
	call print_string
	
.exit:	
	call print_newline
	mov rax, 60 	;
	;mov rdi, rax 	; чтобы echo выводило то, что у нас возвращает find_word
	syscall
	
.false:
	mov rdi, msg	; если нет слова, сообщаем
	call string_length
	mov rsi, rax
	
	mov r13, 2		; дескриптор для ошибок
	call print_string
	jmp .exit