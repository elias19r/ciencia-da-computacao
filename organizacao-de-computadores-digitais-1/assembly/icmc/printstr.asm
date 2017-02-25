; Este programa imprime uma string na tela

; Comeco do programa
jmp main
	msg: string "Exemplo de texto." ; define a string

main:
	loadn r0, #msg ; carrega o endereco da string em r0
	loadn r1, #0   ; carrega a posicao inicial da tela em r1
	call  printstr ; chama a rotina de impressao
	halt
; Fim do programa

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
