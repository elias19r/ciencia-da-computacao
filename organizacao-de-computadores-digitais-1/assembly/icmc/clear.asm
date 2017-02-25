; -------------------------------------------------------------------------
; clear: Apaga a tela imprimindo o caractere espaco em todas as posicoes
; -------------------------------------------------------------------------
clear:
	push r0
	push r1
	
	loadn r0, #1200 ; quantidade de posicoes na tela
	loadn r1, #' '  ; caractere espaco
	
	clear_loop:
		dec r0
		outchar r1, r0
		jnz clear_loop

	pop r1
	pop r0
	rts

