section .data
	buf : times 256 db 0

section .text

; принимает указатель на строку, возвращает её длину. rax - счетчик
string_length:
	xor rax, rax ; обнуляем аккумулятор
.loop:
	cmp byte[rax + rdi], 0 					; идем туда, где лежит строка
	je .exit								; пустая строка - выход
	inc rax									; увеличиваем счетчик
	jmp .loop
.exit:
	ret

;-------------------------------------------

; принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
	call string_length
	mov rsi, rdi 							; откуда выводить
	mov rdi, 1 								; куда вывод - stdout
	mov rdx, rax 							; длина строки - в счетчике
	mov rax, 1 								; номер системного вызова write
	syscall
	xor rax, rax							; чистим аккумулятор
	ret

;-------------------------------------------

; принимает код символа и выводит его в stdout
print_char:
	push rdi 								; положили символ на стек
	mov rsi, rsp 							; индекс источника - вершина стека
	mov rdi, 1 								; куда вывод - stdout
	mov rdx, 1 								; длина строки - 1
	mov rax, 1 								; номер системного вызова write
	syscall
	pop rdi									; возвращаем стек как было
ret

;-------------------------------------------

; перевод на новую строку (0xA)
print_newline:
	mov rdi, 10 							; кладем в нужный регистр значение конца строки
	call print_char
	xor rax, rax
ret

;-----------------------------------------—

; вывод беззнакового 8-байтового числа в десятичной с/с
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
	push r11 								; кладем значение на стек
	mov r11, rsp 							; сохраняем в r11 адрес вершины стека
	mov rax, rdi 							; сохраняем полученное число в аккумулятор
	mov rdi, 10 							; заносим делитель в rdi
	sub rsp, 256 							; выделяем место в стеке
	dec r11 								; выделяем место под 0 на вершине стэка (нультермируем для вывода)
	mov byte[r11], 0 						; кладем 0 на вершину стека
.loop:
	dec r11 								; сдвигаем метку для следующей цифры
	xor rdx, rdx 							; обнуление rdx
	div rdi 								; целочисленное деление на 10, остаток - в rdx
	add rdx, '0' 							; перевод
	mov byte[r11], dl 						; сохраняем 1 цифру на стек
	test rax, rax							; установка флагов
	jnz .loop								; если rax !=0, продолжает цикл
	mov rdi, r11							; помещаем аргумент для print_string
	call print_string
	add rsp, 256							; возвращаем буфер как было 
	pop r11									; восстанавливаем значение регистра
	ret

;-----------------------------------------—

; вывод знакового 8-байтового числа в десятичной с/с
; Выводит знаковое 8-байтовое число в десятичном формате
print_int:
	xor rax, rax
	test rdi, rdi 							; установка флагов
	jns print_uint 							; если положительное - вывести
	push rdi 								; сохранить переданное число
	mov rdi, '-' 							; запись минуса в rdi
	call print_char 						; вывести минус
	pop rdi 
	neg rdi 								; делаем положительное число
	jmp print_uint

;-----------------------------------------—

; принимает 2 указателя на строки, сравнивает посимвольно.
; Возвращает 1, если равны, иначе 0
string_equals:
	call string_length
	mov r11, rax 							; в r11 - длина первой строки
	mov rdi, rsi 							; в rdi - адрес второй строки
	call string_length
	cmp r11, rax  							; сравнение длин и установка флага
	jne .exit 								; если не равны, выход
	repe cmpsb 								; повторять пока равны + устанавливать флаги
	jne .exit 								; если перебор закончился
	mov rax, 1 								; возврат 1
	ret
.exit:
	mov rax, 0 								; возврат 0
	ret
;-----------------------------------------—

; прочитать с потока ввода один символ
read_char:
	dec rsp 								; сюда пишем символ
	mov rax, 0 								; код системного вызова sys_read
	mov rdx, 1 								; читаем 1 символ
	mov rdi, 0 								; откуда ввод - stdin
	mov rsi, rsp 							; указатель на начало (и конец)
	syscall
	mov rax, [rsp] 							; сохраняем символ в rax
	inc rsp 								; указатель стека - на место
	ret
	
;-----------------------------------------—

; прочитать с потока ввода следующее слово, пропустив перед ним произвольное кол-во пробельных символов
; rax - указатель на буфер, rdx - длина прочитанного слова
read_word:
	push rbx 								; сохраняем значение rbx
	mov r8, rsi 							; сохраняем размер буфера
	mov r9, rdi 							; сохраняем адрес буфера
	xor rbx, rbx 							; обнуление счетчика длины
	xor rdi, rdi 							; откуда ввод - stdin
	mov rdx, 1 								; читаем 1 символ
.skip:
	xor rax, rax 							; код системного вызова sys_read
	mov rsi, buf 							; заносим адрес, куда будем считавать
	syscall
	cmp al, 0 								; сравниваем с концом строки
	je .finally 							; если равно, переходим к завершению
	cmp byte[buf], 0x21 					; сравниваем с кодом последнего 'плохого' символа
	jb .skip 								; если меньше, пропускаем его
	inc rbx 								; иначе учитываем
.read:
	xor rax, rax 							; код системного вызова sys_read
	lea rsi, [buf + rbx] 					; записываем эффективный адрес в rsi (откуда читаем)
	syscall
	cmp byte [buf + rbx], 0x21 				; сравниваем c последним не нужным символом
	jb .finally 							; если меньше (значит символ нам не нужен), -> завершаем чтение
	cmp r8, rbx 							; иначе проверяем умещается ли очередной символ
	jbe .exit 								; если нет, возвращаем 0
	inc rbx 								; иначе учитываем символ
	jmp .read 								; и читаем следующий
.finally:
	mov byte[buf + rbx], 0 					; нультермируем строку в буфере
	mov rdx, rbx 							; возвращаем длину строки
	mov rax, buf 							; возвращаем указатель на буфер
	pop rbx 								; восстанавливаем значение rbx
	ret
.exit:
	xor rdx, r8 							; записываем длину буфера (=сколько считали символов)
	xor rax, rax 							; возвращаем 0
	pop rbx 								; восстанавливаем значение rbx
	ret
;-------------------------------------------

; принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
	mov rax, 0
	mov rsi, 0 
	mov rcx, 0
	mov rdx, 0 
	mov r11, 10 

.loop:
	mov sil, [rdi+rcx] 						; перемещаем символ
	cmp sil, '0' 							; проверяем, что этот символ цифра
	jl .return								; если нет - выход
	cmp sil, '9' 							; проверяем, что этот символ цифра
	jg .return 								; если нет - выход
	inc rcx
	sub sil, '0' 							; преобразование символа в число
	mul r11									; умножаем на 10 + добавляем
	add rax, rsi
	jmp .loop
.return:
	mov rdx, rcx
	ret

;-----------------------------------------—

; принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
	cmp byte[rdi], '-' 						; знак - минус?
	jne parse_uint 							; нет - ищем число
	inc rdi 								; двигаемся по строке
	call parse_uint 
	neg rax									; делаем положительное число
	inc rdx 								; увеличиваем счетчик
	ret

;-------------------------------------------

; принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
; в rsi- указатель куда, в rdi - указатель откуда, rdx - длинна строки
	xor rcx, rcx 							; обнуляем счетчик
	push r11 								; сохраним значение регистра
.loop:
	cmp rcx, rdx 							; сравниваем счетчик и количество символов в строке
	jz .complete 							; если все символы скопированы - выходим
	mov r11, [rdi + rcx] 					; сохраним в r11 текущий символ
	mov [rsi + rcx], r11 					; скопируем
	cmp r11, 0
	je .exit
	inc rcx 								; увеличиваем счетчик
	jmp .loop
.complete:
	mov rax, 0
.exit:
	pop r11 								; вернем значение r11
	ret