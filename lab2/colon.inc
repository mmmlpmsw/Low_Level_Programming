%define next 0
%macro colon 2 	; начало макроса - принимает 2 параметра: строку и указатель
%2: 
dq next 		; временная переменная для хранения указателя на следующую строку
db %1, 0
%define next %2 ; константа для адреса следующей строки
%endmacro
