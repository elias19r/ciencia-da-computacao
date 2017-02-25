; Este programa imprime um numero inteiro decimal sem sinal na tela

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

main:
	loadn r0, #65535 ; carrega o numero a ser impresso em r0
	loadn r1, #0     ; carrega a posicao inicial da tela em r1
	call  printd     ; chama a rotina de impressao
	halt
; Fim do programa

; -----------------------------------------------------------------
; printd: imprime um numero inteiro sem sinal
; Parametros
; 	r0: numero
; 	r1: posicao inicial da tela
; -----------------------------------------------------------------
printd:
	push fr
	push r0 ; numero
	push r1 ; posicao inicial da tela
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
	jmp   printd_print   ; imprime

	; Loop que insere os digitos na pilha
	printd_insert:
		; Pega o digito em r4 fazendo mod por 10
		loadn r3, #10
		mod   r4, r0, r3
		div   r0, r0, r3
		loadn r3, #48    ; carrega o carectere '0' em r3
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
		pop r1
		pop r0
		pop fr
		rts
