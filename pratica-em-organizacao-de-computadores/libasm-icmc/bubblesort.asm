; Este programa ordena um vetor em ordem crescente usando o algoritmo bubblesort

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

	; Populando o vetor
	static v + #0, #5
	static v + #1, #3
	static v + #2, #1
	static v + #3, #10
	static v + #4, #0
main:
	; Ordenando com o bubblesort
	loadn r0, #v      ; carrega endereco do vetor em r0
	loadn r1, #5      ; carrega tamanho do vetor em r1
	call  bubblesort  ; chama a rotina do bubblesort

	; Imprime o vetor ordenado
	mov   r2, r1
	loadn r1, #0
	call  printv

	halt
; Fim do programa


; ------------------------------------------------------
; bubblesort: ordena um vetor em ordem crescente
; Parametros
; 	r0: endereco do vetor
; 	r1: tamanho do vetor
; Algoritmo
; 	for (i = 0; i < n-1; i++)
; 		for (j = 1; j < n-i; j++)
; 			if (v[j-1] > v[j])
; 				swap(j, j-1);
; ------------------------------------------------------
bubblesort:
	push fr
	push r0 ; vetor
	push r1 ; tamanho
	push r2 ; auxiliar
	push r3 ; auxiliar
	push r4 ; contador i
	push r5 ; contador j
	push r6 ; n-1 / auxiliar
	push r7 ; n-i / auxiliar

	; i = 0
	loadn r4, #0
	; n-1
	mov r6, r1
	dec r6

	; loop1 (externo)
	bubblesort_loop1:
	; i < n-1
	cmp r4, r6
	jeq bubblesort_loop1_exit

		; j = 1
		loadn r5, #1
		; n-i
		sub r7, r1, r4

		; loop2 (interno)
		bubblesort_loop2:
		; j < n-i
		cmp  r5, r7
		jeq  bubblesort_loop2_exit

		; push em r6 e r7
		; para poder usa-los como auxiliar dentro deste loop
		push r6
		push r7

		; v[j-1] em r2
		; v+j-1  em r6
		push  r0
		mov   r2, r5
		dec   r2
		add   r0, r0, r2
		loadi r2, r0
		mov   r6, r0
		pop   r0

		; v[j] em r3
		; v+j  em r7
		push  r0
		mov   r3, r5
		add   r0, r0, r3
		loadi r3, r0
		mov   r7, r0
		pop   r0

		; if (v[j-1] > v[j])
		cmp r2, r3
		cgr bubblesort_swap

		; j++
		inc r5

		; pop dos auxiliares neste loop
		pop r7
		pop r6

		jmp bubblesort_loop2

			; Sai do loop2 e volta para o loop1
			bubblesort_loop2_exit:
				; i++
				inc r4
				jmp bubblesort_loop1

			; Faz a troca dos valores
			; swap(j, j-1)
			bubblesort_swap:
				storei r6, r3
				storei r7, r2
				rts

	; Sai do loop1, ou seja, fim do bubblesort
	bubblesort_loop1_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; ============================================================================
; printv: imprime um vetor de numeros
; Parametros
; 	r0: endereco do comeco do vetor
; 	r1: posicao inicial da tela
; 	r2: tamanho do vetor
printv:
	push fr
	push r0 ; vetor
	push r1 ; posicao da tela
	push r2 ; tamanho do vetor
	push r3 ; contador

	loadn r3, #0 ; inicializa o contador com zero

	; Loop de impressao
	printv_loop:
		; Confere a condicao de parada
		; if (contador == tamanho) break;
		cmp r3, r2
		jeq printv_exit

		push  r0
		loadi r0, r0   ; carrega o numero
		call  printd ; imprime
		pop   r0

		inc r1 ; espaco em branco
		inc r0 ; proximo numero
		inc r3 ; incrementa contador

		jmp printv_loop

	; Sai da rotina
	printv_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; ============================================================================
; printd: imprime um numero inteiro decimal sem sinal
; Parametros
; 	r0: numero
; 	r1: posicao inicial da tela
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
