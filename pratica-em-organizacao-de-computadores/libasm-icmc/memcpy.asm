; Este programa copia valores de um vetor para outro

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
	; Declaracao do vetor v
	v: var #5

	; Populando o vetor v
	static v + #0, #1
	static v + #1, #2
	static v + #2, #3
	static v + #3, #4
	static v + #4, #5

	; Declaracao do vetor v_dest
	v_dest: var #5

main:
	loadn r0, #v ; carrega o endereco do vetor em r0
	loadn r1, #5 ; carrega o tamanho do vetor em r1
	loadn r2, #v_dest ; carrega o tamanho do vetor destino em r2
	
	call memcpy
	
	; Imprime os vetores para conferir se copiou
	loadn r0, #v
	loadn r1, #0
	loadn r2, #5

	call printv

	loadn r0, #v_dest
	loadn r1, #80
	loadn r2, #5

	call printv

	halt
; Fim do programa

; --------------------------------------------------------------------------------------------
; memcpy: copia um vetor
; Parametros
; 	r0: vetor (origem)
; 	r1: tamanho
; 	r2: destino (com o tamanho necessario ja reservado)
; --------------------------------------------------------------------------------------------
memcpy:
	push fr
	push r0 ; vetor
	push r1 ; tamanho
	push r2 ; destino
	push r3 ; auxiliar

	; Confere o tamanho
	loadn r3, #0
	cmp r1, r3
	jeq memcpy_exit

	; Loop que faz a copia
	memcpy_do:
		loadi  r3, r0
		storei r2, r3
		inc    r0
		inc    r2
		dec    r1
		jnz memcpy_do

	; Sai da rotina
	memcpy_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts


; -----------------------------------------------------------------
; printv: imprime um vetor de numeros
; Parametros
; 	r0: endereco do comeco do vetor
; 	r1: posicao inicial da tela
; 	r2: tamanho do vetor
; -----------------------------------------------------------------
printv:
	push fr
	push r0 ; vetor
	push r1 ; posicao da tela
	push r2 ; tamanho do vetor
	push r3 ; caractere separador
	push r4 ; auxiliar

	; Confere se o tamanho != 0
	loadn r4, #0
	cmp r2, r4
	jeq printv_exit

	; Define o carectere separador
	loadn r3, #','

	; Loop de impressao
	printv_loop:
		mov r4, r0
		loadi r0, r0   ; carrega o numero
		call  printd ; imprime
		mov r0, r4

		outchar r3, r1
		inc r1
		inc r1 ; espaco em branco
		inc r0 ; proximo numero
		dec r2
		jnz printv_loop

		; Apaga o ultimo caractere separador
		dec r1
		dec r1
		loadn r3, #' '
		outchar r3, r1

	; Sai da rotina
	printv_exit:
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; -----------------------------------------------------------------
; printd: imprime um numero inteiro sem sinal
; Parametros
; 	r0: numero
; 	r1: posicao inicial da tela
; -----------------------------------------------------------------
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
