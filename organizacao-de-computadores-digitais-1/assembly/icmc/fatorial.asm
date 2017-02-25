; Este programa calcula e imprime o fatorial de um numero
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
	call  printint
	
	; Calcula o fatorial de 5
	call fatorial

	; Imprime a segunda mensagem
	loadn r0, #msg2
	call  printstr

	; Imprime o resultado (fatorial de 5)
	mov  r0, r7
	call printint
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
