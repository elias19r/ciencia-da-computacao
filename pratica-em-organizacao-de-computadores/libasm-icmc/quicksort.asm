; Este programa ordena um vetor em ordem crescente usando o algoritmo
; quicksort recursivo e imprime na tela os dois vetores: ordenado e nao-ordenado

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

jmp main
	; --------------------------------
	; CONFIGURACOES (altere aqui)
	; --------------------------------
	
		; -------------------------
		; Declaracao das variaveis
		; -------------------------
		v:       var #10
		v_size:  var #1
		v_left:  var #1
		v_right: var #1
	
		; -------------------------------------------------
		; Define: TAMANHO, posicao inicial e posicao final
		; -------------------------------------------------
		static v_size  + #0, #10
		static v_left  + #0, #0
		static v_right + #0, #9

		; --------------------------
		; Define: valores do vetor
		; --------------------------
		static v + #0,  #5
		static v + #1,  #3
		static v + #2,  #1
		static v + #3,  #10
		static v + #4,  #129
		static v + #5,  #0
		static v + #6,  #5
		static v + #7,  #65
		static v + #8,  #33
		static v + #9,  #99

	; -------------------------------------
	; NAO ALTERAR ESTAS DEFINICOES
	; -------------------------------------
		v_return: var #20 ; vetor de retorno (com o tamanho maximo alocado)
		min_size: var #1  ; tamanho minimo que o vetor pode ter
		max_size: var #1  ; tamanho maximo que o vetor pode ter
		static min_size + #0, #1
		static max_size + #0, #20
		msg_1:     string "nao-ordenado"
		msg_2:     string "ordenado"
		msg_error: string "Interrompido. Vetor invalido."


; -----------------------
; Comeco do programa
; -----------------------
main:
	; Define os argumentos para o quicksort e ordena
	; ----------------------------------------------
	loadn r0, #v      ; o vetor
	load  r1, v_left  ; numero da posicao inicial
	load  r2, v_right ; numero da posicao final
	call quicksort    ; chama o quicksort
	
	; Analisa o retorno (r7) do quicksort
	; -----------------------------------
	loadn r0, #0
	cmp r7, r0
	jne result
	
	; Imprime mensagem de erro na tela (se r7 == 0)
	; ----------------------------------------------
	loadn r0, #msg_error ; endereco da mensagem de erro
	loadn r1, #0         ; posicao na tela
	call printstr        ; chama impressao de string
	halt                 ; finaliza o programa

	; Imprime os vetores ordenado e nao-ordenado (se r7 != 0)
	; -------------------------------------------------------
	result:
		; nao-ordenado
		; ------------
		loadn r0, #msg_1 ; endereco da mensagem "nao-ordenado"
		loadn r1, #0     ; posicao na tela
		call printstr    ; chama impressao de string
		loadn r0, #v     ; endereco do vetor
		loadn r1, #40    ; posicao na tela
		load  r2, v_size ; tamanho do vetor
		call printvector ; chama impressao de vetor

		; ordenado
		; ------------
		loadn r0, #msg_2 ; endereco da mensagem "ordenado"
		loadn r1, #280   ; posicao na tela
		call printstr    ; chama impressao de string
		mov   r0, r7     ; enderenco do vetor ordenado em r0
		loadn r1, #320   ; posicao na tela
		call printvector ; chama impressao de vetor

		halt             ; finaliza o programa
; -----------------------
; Fim do programa
; -----------------------


; ------------------------------------------------------
; quicksort: ordena um vetor em ordem crescente
; Parametros
; 	r0: endereco do vetor
; 	r1: numero da posicao inicial
; 	r2: numero da posicao final
; Retorno
; 	r7: endereco do vetor ordenado
; Algoritmo:
; 	void quicksort_swap(int *v, int i, int j) {
; 		int aux = v[i];
; 		v[i] = v[j];
; 		v[j] = aux;
; 	}
; 	int quicksort_partition(int *v, int left, int right) {
; 		int i, j;
; 	
; 		i = left;
; 		for (j = left+1; j <= right; j++) {
; 			if (v[j] < v[left]) {
; 				i++;
; 				quicksort_swap(v, i, j);
; 			}
; 		}
; 		quicksort_swap(v, left, i);
; 	
; 		return i;
; 	}
; 	void quicksort(int *v, int left, int right) {
; 		if (right > left) {
; 			int cutoff = quicksort_partition(v, left, right);
; 			quicksort(v, left, cutoff-1);
; 			quicksort(v, cutoff+1, right);
; 		}
; 	}
; ------------------------------------------------------
quicksort:
	push r0 ; endereco do vetor (v)
	;    r1 ; numero da posicao inicial (left)
	;    r2 ; numero da posicao final (right)
	push r3 ; tamanho minimo permitido
	push r4 ; tamanho maximo permitido
	;    r7 ; retorno
	
	; Empilha r1, r2 pois serao utilizados como argumento da vectorcpy
	push r1
	push r2
	
	; Pega o tamanho do vetor em r1
	load r1, v_size
	
	; Confere se tamanho eh valido
	; Se sim, continua a execucao. Senao, interrompe.
	load r3, min_size
	load r4, max_size
	cmp r1, r3
	jle quicksort_interrupt
	cmp r1, r4
	jgr quicksort_interrupt

	; Faz a copia do vetor (v)
	; Obtem em r0 uma copia do vetor e trabalha em cima dessa copia
	; Copia (mov) r0 em r7, pois o retorno eh esperado em r7
	loadn r2, #v_return
	
	; recebe em r0 o endereco do vetor origem, em r1 size e r2 vetor destino
	call vectorcpy
	
	mov r0, r2	; argumento para o quicksortr
	mov r7, r2	; vetor de retorno em r7
	
	; Desempilha e restaura os valores de r1 e r2
	pop r2
	pop r1
	
	; Chama, de fato, o quicksort
	call quicksortr
	jmp quicksort_exit

	; Interrompe o quicksort e sai da rotina
	quicksort_interrupt:
		pop r2
		pop r1
		loadn r7, #0	; codigo de erro

	; Sai da rotina
	quicksort_exit:
		pop r4
		pop r3
		pop r0
		rts	


; O quicksort, de fato
quicksortr:
	;    r0 ; endereco do vetor (v)
	push r1 ; numero da posicao inicial (left)
	push r2 ; numero da posicao final (right)
	push r3 ; cutoff
	push r4 ; copia de r2 (copia do right)
	
	; Faz copia de r2 em r4
	mov r4, r2

	; if (right > left)
	cmp r2, r1
	jel quicksortr_exit
	
	; cutoff = quicksort_partition(v, left, right);
	call quicksortr_partition

	; quicksort(v, left, cutoff-1)
	dec r3
	mov r2, r3
	call quicksortr
	inc r3

	; quicksort(v, cutoff+1, right)
	mov r2, r4
	inc r3
	mov r1, r3
	call quicksortr
	
	; Sai da rotina
	quicksortr_exit:
		pop r4
		pop r3
		pop r2
		pop r1
		rts

		; Encontra a posicao certa do valor do pivo (left)
		quicksortr_partition:
			push r4 ; i
			push r5 ; j
			push r6 ; auxiliar
			push r7 ; auxiliar
	
			; i = left
			mov r4, r1
	
			; j = left+1
			mov r5, r1
			inc r5

			; v[left] em r7
			add   r7, r0, r1
			loadi r7, r7
	
			; Loop que faz as comparacoes
			quicksortr_partition_loop:
				; j <= right
				cmp r5, r2
				jgr quicksortr_partition_exit

				; v[j] em r6
				add   r6, r0, r5
				loadi r6, r6
		
				; if (v[j] < v[left])
				cmp r6, r7
				jeg quicksortr_partition_loop_continue
		
				; i++
				inc r4

				; quicksort_swap(v, i, j);
				call quicksortr_swap
					
				quicksortr_partition_loop_continue:
					; j++
					inc r5
					jmp quicksortr_partition_loop

			quicksortr_partition_exit:
				; return i
				mov r3, r4
				
				; quicksort_swap(v, left, i);
				mov r5, r4
				mov r4, r1
				call quicksortr_swap
				
				pop r7
				pop r6
				pop r5
				pop r4
				rts
	
		; Faz a troca de valores (swap)
		quicksortr_swap:
			push r1
			push r2
			push r6
			push r7
	
			; v+i  em r1
			; v[i] em r6
			add   r1, r0, r4
			loadi r6, r1

			; v+j  em r2
			; v[j] em r7	
			add   r2, r0, r5
			loadi r7, r2
			
			; troca valores	
			storei r1, r7
			storei r2, r6
	
			pop r7
			pop r6
			pop r2
			pop r1
			rts


; --------------------------------------------------------------------------------------------
; vectorcpy: copia um vetor
; Parametros
; 	r0: vetor (origem)
; 	r1: tamanho
; 	r2: destino (com o tamanho necessario ja reservado)
; --------------------------------------------------------------------------------------------
vectorcpy:
	push r0 ; vetor
	push r1 ; tamanho
	push r2 ; destino
	push r3 ; auxiliar

	; Confere o tamanho
	loadn r3, #0
	cmp r1, r3
	jeq vectorcpy_exit
	
	; Loop que faz a copia
	vectorcpy_loop:
		loadi  r3, r0
		storei r2, r3
		inc    r0
		inc    r2
		dec    r1
		jnz vectorcpy_loop
	
	; Sai da rotina
	vectorcpy_exit:
		pop r3
		pop r2
		pop r1
		pop r0
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
	push r0 ; string
	push r1 ; posicao inicial da tela
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
		pop r1
		pop r0
		rts

; -----------------------------------------------------------------
; printvector: imprime um numero vetor
; Parametros
; 	r0: endereco do comeco do vetor
; 	r1: posicao inicial da tela
; 	r2: tamanho do vetor
; -----------------------------------------------------------------
printvector:
	push r0 ; vetor
	push r1 ; posicao da tela
	push r2 ; tamanho do vetor
	push r3 ; caractere separador
	push r4 ; auxiliar
	
	; Confere se o tamanho != 0
	loadn r4, #0
	cmp r2, r4
	jeq printvector_exit
	
	; Define o carectere separador
	loadn r3, #','

	; Loop de impressao
	printvector_loop:
		mov r4, r0
		loadi r0, r0   ; carrega o numero
		call  printint ; imprime
		mov r0, r4

		outchar r3, r1
		inc r1
		inc r1 ; espaco em branco
		inc r0 ; proximo numero
		dec r2
		jnz printvector_loop

		; Apaga o ultimo caractere separador
		dec r1
		dec r1
		loadn r3, #' '
		outchar r3, r1

	; Sai da rotina
	printvector_exit:
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

; -----------------------------------------------------------------
; printint: imprime um numero inteiro sem sinal
; Parametros
; 	r0: numero
; 	r1: posicao inicial da tela
; -----------------------------------------------------------------
printint:
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
	jne printint_insert
	
	; Se eh zero: coloca na pilha e manda imprimir
	loadn r0, #48        ; carrega o carectere '0' em r0
	push  r0             ; empilha
	jmp   printint_print ; imprime

	; Loop que insere os digitos na pilha
	printint_insert:
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
		jeq printint_print
		
		; Se nao eh zero: continua pegando e inserindo os digitos na pilha
		jmp printint_insert

	; Loop que desempilha e imprime os digitos
	printint_print:
		pop     r3     ; desempilha
		outchar r3, r1 ; imprime
		inc     r1     ; incrementa a posicao na tela
		
		; Confere se ja terminou
		mov r3, sp
		cmp r3, r2
		jeq printint_exit
		jmp printint_print

	; Sai da impressao
	printint_exit:
		pop r4
		pop r3
		pop r2
;		pop r1
		pop r0
		rts
