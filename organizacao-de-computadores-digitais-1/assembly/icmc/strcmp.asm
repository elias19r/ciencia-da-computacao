; Este programa compara duas strings

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
	call printint
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
		rts

; --------------------------------------------------------------------------------------------
; printint: imprime um numero inteiro sem sinal
; Parametros
; 	r0: numero
; 	r1: posicao inicial da tela
; --------------------------------------------------------------------------------------------
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
