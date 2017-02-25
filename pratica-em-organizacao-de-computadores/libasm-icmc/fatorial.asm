; Este programa calcula e imprime o fatorial de um numero

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
	; Define as mensagens
	msg1: string "O fatorial de "
	msg2: string " eh "

	loadn r1, #0 ; posicao inicial da tela
main:
	; Imprime a primeira mensagem
	loadn r0, #msg1
	call  printstr

	; Imprime o numero 5
	loadn r0, #5
	call  printd

	; Calcula o fatorial de 5
	call fatorial

	; Imprime a segunda mensagem
	loadn r0, #msg2
	call  printstr

	; Imprime o resultado (fatorial de 5)
	mov  r0, r7
	call printd
	halt
; Fim do programa

; -----------------------------------------------------------------
; fatorial: calcula iterativamente o fatorial de um numero
; Parametros
; 	r0: numero
; Retorno
; 	r7: resultado
; -----------------------------------------------------------------
fatorial:
	push fr
	push r0 ; numero
	push r1
	push r2

	loadn r7, #1 ; carrega 1 em r7 (r7 retorna o resultado do fatorial)
	loadn r1, #1 ; carrega 1 em r1 para fazer a comparacoes
	loadn r2, #0 ; carrega 0 em r2 para fazer comparacao

	; Se eh zero: nao calcula
	cmp r0, r2
	jeq fatorial_exit

	; Se nao eh zero: entra no loop
	; Loop para calcular o fatorial
	fatorial_loop:
		; Confere a condicao de parada
		; if (numero == 1)
		cmp r0, r1
		jeq fatorial_exit

		mul r7, r7, r0 ; multiplica e acumula o resultado fatorial em r7
		dec r0         ; decrementa o numero

		jmp fatorial_loop

	; Sai da rotina
	fatorial_exit:
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
