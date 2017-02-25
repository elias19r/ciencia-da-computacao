; Este programa concatena duas strings e imprime o resultado

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
	; Define as strings
	str1: string "Este eh "
	str2: string "um exemplo do strcat."

	; Define o destino (onde as strings ficarao concatenadas)
	dest: var #30
main:
	loadn r0, #str1 ; carrega o endereco de str1 em r0
	loadn r1, #str2 ; carrega o endereco de str2 em r1
	loadn r2, #dest ; carrega o endereco de dest em r2

	call strcat ; chama a rotina para concatenar str1 + str 2 em dest

	; Imprime a nova string
	mov   r0, r2
	loadn r1, #0
	call  printstr
	halt
; Fim do programa

; -----------------------------------------------------------------
; strcat: concatena duas strings
; Parametros
; 	r0: string1
; 	r1: string2
; 	r2: destino (com o tamanho necessario ja reservado)
; -----------------------------------------------------------------
strcat:
	push fr
	push r0 ; string1
	push r1 ; string2
	push r2 ; destino
	push r3 ; auxiliar
	push r4 ; caractere nulo '\0'

	loadn r4, #0 ; carrega o caractere nulo '\0' em r4

	; Loop que copia a primeira string
	strcat_copy1:
		; Confere a condicao de parada
		; if (*str1 == '\0')
		loadi r3, r0
		cmp   r3, r4
		jeq   strcat_copy2

		; Copia o caractere para dest
		storei r2, r3 ; *dest = *str1
		inc    r0     ; str1++
		inc    r2     ; dest++

		jmp strcat_copy1

	; Loop que copia a segunda string
	strcat_copy2:
		; Confere a condicao de parada
		; if (*str2 == '\0')
		loadi r3, r1
		cmp   r3, r4
		jeq   strcat_exit

		; Copia o caractere para dest
		storei r2, r3 ; *dest = *str2
		inc    r1     ; str2++
		inc    r2     ; dest++

		jmp strcat_copy2

	; Sai da rotina
	strcat_exit:
		; Insere o caractere '\0' no final de dest
		storei r2, r4

		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; -----------------------------------------------------------------
; printstr: imprime uma string na tela
; Parametros
; 	r0: endereco do comeco da string
; 	r1: posicao inicial da tela
; Algoritmo
; 	while (*str != '\0') {
; 		printf("%c", *str);
; 		str++;
; 	}
; -----------------------------------------------------------------
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
