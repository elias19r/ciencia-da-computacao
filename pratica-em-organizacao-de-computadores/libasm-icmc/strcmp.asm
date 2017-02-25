; Este programa compara duas strings

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
	str1: string "AAB"
	str2: string "ABC"

main:
	loadn r0, #str1 ; carrega o endereco de str1 em r0
	loadn r1, #str2 ; carrega o endereco de str2 em r1

	call strcmp ; chama a rotina para comparar as strings

	; Imprime o resultado
	mov   r0, r7
	loadn r1, #0
	call printd
	halt
; Fim do programa

; --------------------------------------------------------------------------------------------
; strcmp: compara duas strings
; Parametros
; 	r0: string1
; 	r1: string2
; Retorno
; 	r7: 0 se iguais, 1 se a primeira for menor, 2 se a segunda for menor, alfabeticamente
; --------------------------------------------------------------------------------------------
strcmp:
	push fr
	push r0 ; string1
	push r1 ; string2
	push r2 ; auxiliar
	push r3 ; auxiliar

	loadn r7, #0 ; carrega o caractere nulo '\0' em r7

	; Loop que faz a comparacao dos caracteres
	strcmp_loop:
		; Compara se os caracteres atuais de ambas strings sao '\0'
		; Se sim, as strings sao iguais
		loadi r2, r0
		loadi r3, r1

		; if (*str1 == '\0') {
		; 	if (*str2 == '\0') {
		; 		return 0;
		; 	} else {
		; 		return 1;
		; 	}
		; }
		cmp r2, r7
		jeq strcmp_return01

		; if (*str2 == '\0') {
		; 	return 2;
		; }
		cmp r3, r7
		jeq strcmp_return2

		; if (*str1 == *str2) continue;
		; if (*str1 < *str2)  return 1;
		; if (*str1 > *str2)  return 2;
		cmp r2, r3
		inc r0
		inc r1
		jeq strcmp_loop
		jle strcmp_return01
		jgr strcmp_return2

	; Define o retorno entre 0 ou 1
	strcmp_return01:
		cmp   r3, r7
		jeq   strcmp_exit
		loadn r7, #1
		jmp   strcmp_exit

	; Define o retorna para 2
	strcmp_return2:
		loadn r7, #2
		jmp   strcmp_exit

	; Sai da rotina
	strcmp_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; --------------------------------------------------------------------------------------------
; printd: imprime um numero inteiro sem sinal
; Parametros
; 	r0: numero
; 	r1: posicao inicial da tela
; --------------------------------------------------------------------------------------------
printd:
	push fr
	push r0 ; numero
;	push r1 ; posicao inicial da tela
	push r2 ; frame
	push r3 ; auxiliar
	push r4 ; digito

	mov r2, sp ; copia sp em r2 (frame)

	; Compara o numero com zero
	loadn r3, #0
	cmp   r0, r3

	; Se nao eh zero: vai para o loop de inserir os digitos na pilha
	jne printd_insert

	; Se eh zero: coloca na pilha e manda imprimir
	loadn r0, #48        ; carrega o carectere '0' em r0
	push  r0             ; empilha
	jmp   printd_print ; imprime

	; Loop que insere os digitos na pilha
	printd_insert:
		; Pega o digito em r4 fazendo mod por 10
		loadn r3, #10
		mod   r4, r0, r3
		div   r0, r0, r3
		loadn r3, #48    ; carrega o carectere '0' em r0
		add   r4, r4, r3 ; define o caractere do digito
		push  r4         ; coloca o digito na pilha

		; Compara o numero com zero
		loadn r3, #0
		cmp   r0, r3

		; Se eh zero: terminou, então manda para o loop que desempilha e imprime
		jeq printd_print

		; Se nao eh zero: continua pegando e inserindo os digitos na pilha
		jmp printd_insert

	; Loop que desempilha e imprime os digitos
	printd_print:
		pop     r3     ; desempilha
		outchar r3, r1 ; imprime
		inc     r1     ; incrementa a posicao na tela

		; Confere se ja terminou
		mov r3, sp
		cmp r3, r2
		jeq printd_exit
		jmp printd_print

	; Sai da impressao
	printd_exit:
		pop r4
		pop r3
		pop r2
;		pop r1
		pop r0
		pop fr
		rts
