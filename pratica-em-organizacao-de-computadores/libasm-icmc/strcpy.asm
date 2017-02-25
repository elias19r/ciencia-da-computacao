; Este programa copia uma string e imprime

; Copyright (C) 2013  Elias Rodrigues <https://github.com/elias19r>
; 
; This program is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.
; 
; This program is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.
; 
; You should have received a copy of the GNU General Public License
; along with this program.  If not, see <http://www.gnu.org/licenses/>.

; Comeco do programa
jmp main
	; Define a string
	str: string "Este eh um exemplo."

	; Define o destino (para onde sera copiada a string)
	dest: var #30
main:
	loadn r0, #str  ; carrega o endereco de str em r0
	loadn r1, #dest ; carrega o endereco de dest em r1

	call strcpy ; chama a rotina para copiar str em dest

	mov   r2, r1
	loadn r1, #0
	call printstr ; imprime string original
	mov   r0, r2
	call printstr ; imprime a nova string
	halt
; Fim do programa

; -------------------------------------------------------------------
; strcpy: copia uma string
; Parametros
; 	r0: string
; 	r1: destino (com o tamanho necessario ja reservado)
; -------------------------------------------------------------------
strcpy:
	push fr
	push r0 ; string
	push r1 ; destino
	push r2 ; auxiliar
	push r3 ; caractere nulo '\0'

	loadn r3, #0 ; carrega o caractere nulo '\0' em r3

	; Loop que faz a copia
	strcpy_do:
		; Confere a condicao de parada
		; if (*str == '\0')
		loadi r2, r0
		cmp   r2, r3
		jeq   strcpy_exit

		; Copia o caractere para dest
		storei r1, r2 ; *dest = *str
		inc    r0     ; str++
		inc    r1     ; dest++

		jmp strcpy_do

	; Sai da rotina
	strcpy_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; -------------------------------------------------------------------
; printstr: imprime uma string na tela
; Parametros
; 	r0: endereco do comeco da string
; 	r1: posicao inicial da tela
; Algoritmo
; 	while (*str != '\0') {
; 		printf("%c", *str);
; 		str++;
; 	}
; -------------------------------------------------------------------
printstr:
	push fr
	push r0 ; string
;	push r1 ; posicao inicial da tela
	push r2 ; caractere nulo '\0'
	push r3 ; auxiliar

	loadn r2, #0 ; carrega o caractere nulo em r2

	; Loop de impressao
	printstr_print:
		; while (*str != '\0')
		loadi   r3, r0
		cmp     r3, r2
		jeq     printstr_exit

		outchar r3, r1 ; printf("%c", *str)
		inc     r0     ; str++
		inc     r1     ; incrementa a posicao da tela
		jmp     printstr_print

	; Sai da rotina, terminou de imprimir
	printstr_exit:
		pop r3
		pop r2
;		pop r1
		pop r0
		pop fr
		rts
