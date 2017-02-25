; Este programa imprime um vetor na tela

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
	loadn r0, #v ; carrega o endereco do vetor em r0
	loadn r1, #0 ; carrega a posicao inicial da tela em r1
	loadn r2, #5 ; carrega o tamanho do vetor em r2

	call printvector ; chama a rotina de impressao de vetor
	halt
; Fim do programa

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
