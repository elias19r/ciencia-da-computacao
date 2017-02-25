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
	vectorcpy_do:
		loadi  r3, r0
		storei r2, r3
		inc    r0
		inc    r2
		dec    r1
		jnz vectorcpy_do
	
	; Sai da rotina
	vectorcpy_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		rts

