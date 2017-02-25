; ----------------------------------------------------------------
; Jogo da Forca
; 2013/04/06
; Elias Italiano Rodrigues, 7987251, Org (Pratica), BCC, ICMC-USP
; ----------------------------------------------------------------


jmp main

	word:   var #20 ; palavra do jogo
	buffer: var #20 ; palavra com as letras que o jogador for acertando
	                ; o jogo acaba quando strcmp(word, buffer) == 0 (ganhar)
	              	; ou quando attempts_left == 0 (perder)
	tip:           var #30 ; uma dica sobre a palavra
	tip_max_size:  var #1  ; o tamanho maximo que a dica pode ter
	word_max_size: var #1  ; o tamanho maximo que pode ter a palavra do jogo / buffer
	word_size:     var #1  ; o tamanho da palavra do jogo / buffer
	attempts:      var #1  ; quantidade de vezes que o jogador pode errar
	attempts_left: var #1  ; quantidade de erros restantes
	char:          var #1  ; letra
	flag:          var #1  ; flag para sinalizar quando o jogador acertou a letra (1) ou errou (0)
	
	static word_max_size + #0, #19
	static  tip_max_size + #0, #29

	; Tela inicial
	; ------------
	home_word_00: string "                                        "
	home_word_01: string "            JOGO DA FORCA               "
	home_word_02: string "           ---------------              "
	home_word_03: string "                                        "
	home_word_04: string "                                        "
	home_word_05: string "                                        "
	home_word_06: string "   Digite uma palavra para comecar      "
	home_word_07: string "                                        "
	home_word_08: string "     Ela nao aparecera na tela ;)       "
	home_word_09: string "                                        "
	home_word_10: string "                                        "
	home_word_11: string "                                        "
	home_word_12: string "      Apos digitar, tecle ENTER         "
	home_word_13: string "                                        "
	home_word_14: string "                                        "
	home_word_15: string "                                        "
	home_word_16: string "                                        "
	home_word_17: string "                                        "
	home_word_18: string "                                        "
	home_word_19: string "                                        "
	home_word_20: string "                                        "
	home_word_21: string "                                        "
	home_word_22: string "                                        "
	home_word_23: string "                                        "
	home_word_24: string "                                        "
	home_word_25: string "                                        "
	home_word_26: string "                                        "
	home_word_27: string "                                        "
	home_word_28: string "                                        "
	home_word_29: string "                                        "

	; Tela de dica
	; ------------
	home_tip_00: string "                                        "
	home_tip_01: string "            JOGO DA FORCA               "
	home_tip_02: string "           ---------------              "
	home_tip_03: string "                                        "
	home_tip_04: string "                                        "
	home_tip_05: string "                                        "
	home_tip_06: string "   Digite agora alguma dica sobre a     "
	home_tip_07: string "                                        "
	home_tip_08: string "          palavra informada:            "
	home_tip_09: string "                                        "
	home_tip_10: string "                                        "
	home_tip_11: string "                                        "
	home_tip_12: string " >                                      "
	home_tip_13: string "                                        "
	home_tip_14: string "                                        "
	home_tip_15: string "                                        "
	home_tip_16: string "      Apos digitar, tecle ENTER         "
	home_tip_17: string "                                        "
	home_tip_18: string "                                        "
	home_tip_19: string "                                        "
	home_tip_20: string "                                        "
	home_tip_21: string "                                        "
	home_tip_22: string "                                        "
	home_tip_23: string "                                        "
	home_tip_24: string "                                        "
	home_tip_25: string "                                        "
	home_tip_26: string "                                        "
	home_tip_27: string "                                        "
	home_tip_28: string "                                        "
	home_tip_29: string "                                        "
	
	; Tela de jogo
	; ------------
	in_game_00: string "                                        "
	in_game_01: string "            JOGO DA FORCA               "
	in_game_02: string "           ---------------              "
	in_game_03: string "                                        "
	in_game_04: string "                                        "
	in_game_05: string "    _______                             "
	in_game_06: string "   |       |                            "
	in_game_07: string "   |                                    "
	in_game_08: string "   |                                    "
	in_game_09: string "   |                                    "
	in_game_10: string "   |                                    "
	in_game_11: string "   |                                    "
	in_game_12: string "   |                                    "
	in_game_13: string "   |                                    "
	in_game_14: string "   |                                    "
	in_game_15: string "  ===                                   "
	in_game_16: string "                                        "
	in_game_17: string "                                        "
	in_game_18: string "                                        "
	in_game_19: string "                                        "
	in_game_20: string "                                        "
	in_game_21: string "                                        "
	in_game_22: string "                                        "
	in_game_23: string "  Dica:                                 "
	in_game_24: string "                                        "
	in_game_25: string "  Tamanho:                              "
	in_game_26: string "                                        "
	in_game_27: string "  Tentativas:                           "
	in_game_28: string "                                        "
	in_game_29: string "                                        "

	; Tela de fim de jogo (ganhou)
	; ----------------------------
	game_over_win_00: string "                                        "
	game_over_win_01: string "                                        "
	game_over_win_02: string "                                        "
	game_over_win_03: string "               GAME OVER                "
	game_over_win_04: string "              -----------               "
	game_over_win_05: string "                                        "
	game_over_win_06: string "                                        "
	game_over_win_07: string "                                        "
	game_over_win_08: string "             Y O U   W I N              "
	game_over_win_09: string "                                        "
	game_over_win_10: string "                                        "
	game_over_win_11: string "                                        "
	game_over_win_12: string "                                        "
	game_over_win_13: string "                                        "
	game_over_win_14: string "               **      **               "
	game_over_win_15: string "               **      **               "
	game_over_win_16: string "                                        "
	game_over_win_17: string "                                        "
	game_over_win_18: string "                                        "
	game_over_win_19: string "           *                *           "
	game_over_win_20: string "           *                *           "
	game_over_win_21: string "            ****************            "
	game_over_win_22: string "                                        "
	game_over_win_23: string "                                        "
	game_over_win_24: string "                                        "
	game_over_win_25: string "                                        "
	game_over_win_26: string "      Deseja jogar de novo? (y/n)       "
	game_over_win_27: string "                                        "
	game_over_win_28: string "                                        "
	game_over_win_29: string "                                        "

	; Tela de fim de jogo (perdeu)
	; ----------------------------
	game_over_lose_00: string "                                        "
	game_over_lose_01: string "                                        "
	game_over_lose_02: string "                                        "
	game_over_lose_03: string "               GAME OVER                "
	game_over_lose_04: string "              -----------               "
	game_over_lose_05: string "                                        "
	game_over_lose_06: string "                                        "
	game_over_lose_07: string "                                        "
	game_over_lose_08: string "            Y O U   L O S E ...         "
	game_over_lose_09: string "                                        "
	game_over_lose_10: string "                                        "
	game_over_lose_11: string "                                        "
	game_over_lose_12: string "                                        "
	game_over_lose_13: string "                                        "
	game_over_lose_14: string "              *         *               "
	game_over_lose_15: string "               *       *                "
	game_over_lose_16: string "              * *     * *               "
	game_over_lose_17: string "                                        "
	game_over_lose_18: string "                                        "
	game_over_lose_19: string "                                        "
	game_over_lose_20: string "             *************              "
	game_over_lose_21: string "            *             *             "
	game_over_lose_22: string "            *             *             "
	game_over_lose_23: string "                                        "
	game_over_lose_24: string "                                        "
	game_over_lose_25: string "                                        "
	game_over_lose_26: string "      Deseja jogar de novo? (y/n)       "
	game_over_lose_27: string "                                        "
	game_over_lose_28: string "                                        "
	game_over_lose_29: string "                                        "
	
	; Tela de termino
	; ---------------
	end_00: string "                                        "
	end_01: string "                                        "
	end_02: string "                                        "
	end_03: string "                                        "
	end_04: string "                                        "
	end_05: string "                                        "
	end_06: string "                                        "
	end_07: string "                                        "
	end_08: string "                                        "
	end_09: string "                                        "
	end_10: string "                                        "
	end_11: string "                                        "
	end_12: string "                                        "
	end_13: string "              T H E   E N D             "
	end_14: string "                                        "
	end_15: string "                                        "
	end_16: string "                                        "
	end_17: string "                                        "
	end_18: string "                                        "
	end_19: string "                                        "
	end_20: string "                                        "
	end_21: string "                                        "
	end_22: string "                                        "
	end_23: string "                                        "
	end_24: string "                                        "
	end_25: string "                                        "
	end_26: string "                                        "
	end_27: string "                                        "
	end_28: string "                                        "
	end_29: string "                                        "

main:
	; Inicializa um jogo novo
	call forca_new
	
	; Roda o jogo em um loop
	forca_loop:
		call forca_input_char ; recebe a letra
		call forca_check_char ; confere se a letra esta na palavra
		call forca_check_end  ; confere se o jogo acabou
		                      ; se sim: faz uma operacao que resulta em zero,
		                      ; forcando assim a saida deste loop
		jnz forca_loop
	
	; Finaliza/Reinicia o jogo
	forca_exit:
		push r0
		push r1
		
		; Recebe letra e confere se eh 'y'
		; Se sim, recomeca o jogo. Senao, termina.
		call forca_input_char
		load  r0, char
		loadn r1, #'y'
		cmp r0, r1

		pop r1
		pop r0
		jeq main

		push r0
		loadn r0, #end_00
		call forca_draw_screen
		pop r0
		halt

; ------------------------
; Cria um jogo novo
; ------------------------
forca_new:
	push r0

	; Inicializa valores
	loadn r0, #5
	store attempts,      r0
	store attempts_left, r0

	loadn r0, #0
	store word_size, r0
	store char,      r0
	store flag,      r0
	
	; Recebe palavra e a dica para comecar o jogo e inicializa o buffer
	call forca_input_word
	call forca_input_tip
	call forca_new_buffer
	
	; Desenha tela de jogo, o buffer, a dica e o tamanho da palavra
	loadn r0, #in_game_00
	call forca_draw_screen
	call forca_draw_buffer
	call forca_draw_tip
	call forca_draw_word_size

	; Sai da rotina
	pop r0
	rts

; ---------------------------
; Le uma letra do teclado
; ---------------------------
forca_input_char:
	push r0
	push r1
	
	loadn r1, #255
	
	forca_input_char_loop:
		inchar r0
		cmp r0, r1
		jeq forca_input_char_loop
	
	store char, r0
	pop r1
	pop r0	
	rts

; ----------------------------------------
; Confere se a letra lida esta na palavra
; ----------------------------------------
forca_check_char:
	push r0
	push r1
	push r2
	push r3
	push r4
	
	; flag = 0
	loadn r0, #0
	store flag, r0

	loadn r0, #buffer
	load  r1, char
	loadn r2, #word
	load  r3, word_size

	; Compara se a letra existe na palavra: se sim, armazena no buffer
	forca_check_char_loop:
		loadi r4, r2
		cmp r4, r1
		ceq forca_store_buffer
		inc r0
		inc r2
		dec r3
		jnz forca_check_char_loop

	; Se flag == 0 significa que o jogador errou a letra
	; Entao escreve a letra em "Tentativas" e desenha mais uma parte do homem na forca
	;
	; if (flag == 0) {
	; 	draw_attempts();
	; 	draw_man();
	; }
	loadn r0, #0
	load  r1, flag
	cmp r1, r0
	ceq forca_draw_attempts
	ceq forca_draw_man
	
	; Sai da rotina
	call forca_draw_buffer
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	rts

; ------------------------------------
; Confere se o jogo acabou
; ------------------------------------
forca_check_end:
	push r0
	push r1
	push r2
	push r3
	push r7
	
	; r3 eh usado como flag para sinalizar o final do jogo
	; Se r3 for incrementado, resulta numa operacao diferente de zero, o jogo continua
	; Se r3 for decrementado, resulta numa operacao igual a zero, o jogo termina
	loadn r3, #1
	
	; if (attempts_left == 0)
	; 	endgame(LOSE)
	;
	load  r1, attempts_left
	loadn r2, #0
	cmp r1, r2
	jeq forca_check_end_lose
	
	; if (strcmp(word, buffer) == 0)
	; 	endgame(WIN)
	;
	loadn r0, #word
	loadn r1, #buffer
	call strcmp
	cmp r7, r2
	jeq forca_check_end_win
	
	; Incrementa a flag (r3)
	inc r3

	; Sai da rotina
	forca_check_end_exit:
		pop r7
		pop r3
		pop r2
		pop r1
		pop r0
		rts
	
	; Desenha tela de perdeu o jogo
	forca_check_end_lose:
		loadn r0, #game_over_lose_00
		call forca_draw_screen
		dec r3 ; decrementa a flag (r3) resultando em um valor igual a zero, ou seja, fim de jogo
		jmp forca_check_end_exit

	; Desenha tela de ganhou o jogo
	forca_check_end_win:
		loadn r0, #game_over_win_00
		call forca_draw_screen
		dec r3 ; decrementa a flag (r3) resultando em um valor igual a zero, ou seja, fim de jogo
		jmp forca_check_end_exit


; ------------------------------------
; Desenha as tentativas na tela
; ------------------------------------
forca_draw_attempts:
	push r0
	push r1
	push r2
	push r3
	
	load  r0, char
	load  r1, attempts
	load  r2, attempts_left
	
	; Calcula posicao para imprimir a letra
	loadn   r3, #1094
	sub     r1, r1, r2
	shiftl0 r1, #1
	add     r3, r3, r1
	
	; Imprime a letra
	outchar r0, r3

	; Decrementa e salva attemps_left
	dec r2
	store attempts_left, r2
	
	; Sai da rotina
	pop r3
	pop r2
	pop r1
	pop r0
	rts

; ------------------------------------
; Desenha o homem na forca
; ------------------------------------
forca_draw_man:
	push r0
	push r1
	
	; Faz um switch-case do valor de attempts_left
	load  r0, attempts_left
	loadn r1, #4
	
	; Case 4
	cmp r0, r1
	jeq forca_draw_man_head
	dec r1

	; Case 3
	cmp r0, r1
	jeq forca_draw_man_body
	dec r1
	
	; Case 2
	cmp r0, r1
	jeq forca_draw_man_arms
	dec r1
		
	; Case 1
	cmp r0, r1
	jeq forca_draw_man_legs
	dec r1
	
	; Case 0
	cmp r0, r1
	jeq forca_draw_man_exit

	; Desenha a cabeca
	forca_draw_man_head:
		loadn r0, #'0'
		loadn r1, #291
		outchar r0, r1
		jmp forca_draw_man_exit
	
	; Desenha o corpo
	forca_draw_man_body:
		loadn r0, #'|'
		loadn r1, #331
		outchar r0, r1
		loadn r1, #371
		outchar r0, r1
		jmp forca_draw_man_exit
	
	; Desenha os bracos
	forca_draw_man_arms:
		loadn r0, #'\\'
		loadn r1, #330
		outchar r0, r1

		loadn r0, #'/'
		loadn r1, #332
		outchar r0, r1

		jmp forca_draw_man_exit
	
	; Desenha as pernas
	forca_draw_man_legs:
		loadn r0, #'/'
		loadn r1, #410
		outchar r0, r1

		loadn r0, #'\\'
		loadn r1, #412
		outchar r0, r1

	; Sai da rotina
	forca_draw_man_exit:
		pop r1
		pop r0	
		rts


; -------------------------------------------------
; Armazena em buffer a letra lida e seta flag = 1
; Parametros
; 	r0: endereco da posicao do buffer
; 	r1: char lido
; -------------------------------------------------
forca_store_buffer:
	push r0
	push r1
	
	storei r0, r1
	loadn r1, #1
	store flag, r1
	
	pop r1
	pop r0
	rts


; ------------------------------------
; Recebe a palavra para o jogo
; ------------------------------------
forca_input_word:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	
	; Desenha tela inicial
	loadn r0, #home_word_00
	call forca_draw_screen

	loadn r0, #word
	load  r2, word_max_size
	loadn r3, #0  ; contador
	loadn r4, #13 ; carectere ENTER
	loadn r5, #0  ; caractere nulo '\0'

	forca_input_word_loop:
		call forca_input_char
		load r1, char
		storei r0, r1
		cmp r1, r4
		jeq forca_input_word_exit
		inc r0
		inc r3
		dec r2
		jnz forca_input_word_loop

	forca_input_word_exit:
		; Nao permite a insercao de uma palavra de tamanho zero
		cmp r3, r5
		jeq forca_input_word_loop

		; Define o tamanho da palavra
		store word_size, r3
		
		; Coloca caractere nulo '\0' no final da palavra
		storei r0, r5

		pop r5		
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts
		
; --------------------------------------
; Recebe a dica sobre a palavra do jogo
; --------------------------------------
forca_input_tip:
	push r0
	push r1
	push r2
	push r3
	push r4
	
	; Desenha tela de dica
	loadn r0, #home_tip_00
	call forca_draw_screen

	loadn r0, #tip
	load  r2, tip_max_size
	loadn r3, #13  ; carectere ENTER
	loadn r4, #483 ; posicao na tela

	forca_input_tip_loop:
		call forca_input_char
		load r1, char
		storei r0, r1
		cmp r1, r3
		jeq forca_input_tip_exit
		outchar r1, r4
		inc r4
		inc r0
		dec r2
		jnz forca_input_tip_loop

	forca_input_tip_exit:
		; Coloca caractere nulo '\0' no final da dica
		loadn  r3, #0
		storei r0, r3
		
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

; ---------------------------
; Inicializa o buffer
; ---------------------------
forca_new_buffer:
	push r0
	push r1
	push r2
	
	load  r0, word_size
	loadn r1, #buffer
	loadn r2, #'_'
	
	; Coloca caracteres '_' nas posicoes do buffer
	forca_new_buffer_loop:
		storei r1, r2
		inc r1
		dec r0
		jnz forca_new_buffer_loop
	
	loadn  r2, #0 ; coloca  o caractere nulo '\0' no final do buffer
	storei r1, r2

	; Sai da rotina	
	pop r2
	pop r1
	pop r0
	rts

; ------------------------
; Desenha uma tela
; ------------------------
forca_draw_screen:
	push r0
	push r1
	push r2
	push r3

	call clear
	
	loadn r1, #0  ; posicao inicial
	loadn r2, #40 ; incrementador da posicao (quantidade de colunas)
	loadn r3, #30 ; contador (quantidade de linhas)
	
	; Loop de impressao das linhas
	forca_draw_screen_loop:
		call printstr
		add r0, r0, r2
		inc r0
		add r1, r1, r2
		dec r3
		jnz forca_draw_screen_loop

	; Sai da rotina
	pop r3
	pop r2
	pop r1
	pop r0
	rts

; --------------------------------------
; Imprime a dica na tela
; --------------------------------------
forca_draw_tip:
	push r0
	push r1
	
	loadn r0, #tip
	loadn r1, #928
	
	call printstr
	
	pop r1
	pop r0
	rts


; --------------------------------------
; Desenha o tamanho da palavra
; --------------------------------------
forca_draw_word_size:
	push r0
	push r1
	
	load  r0, word_size
	loadn r1, #1011
	
	call printint
	
	pop r1
	pop r0
	rts

; --------------------------------------
; Desenha os campos das letras na tela
; --------------------------------------
forca_draw_buffer:
	push r0
	push r1
	push r2
	push r3
	
	loadn r0, #buffer
	load  r2, word_size
	loadn r3, #721

	forca_draw_buffer_loop:
		loadi r1, r0
		outchar r1, r3
		inc r0
		inc r3
		inc r3
		dec r2
		jnz forca_draw_buffer_loop

	pop r3
	pop r2
	pop r1
	pop r0
	rts

; ----------------------------------------------------------------------------------------------------
; clear: Apaga a tela imprimindo o caractere espaco em todas as posicoes
; Observacao: nao ha necessidade de apagar a tela nesse jogo,
;             mas esta sendo usado so para dar um efeito de transicao ;)
; ----------------------------------------------------------------------------------------------------
clear:
	push r0
	push r1

	loadn r0, #1200 ; quantidade de posicoes na tela
	loadn r1, #'.'  ; caractere

	clear_loop:
		dec r0
		outchar r1, r0
		jnz clear_loop

	pop r1
	pop r0
	rts

; ---------------------------------------
; printstr: imprime uma string na tela
; Parametros
; 	r0: endereco do comeco da string
; 	r1: posicao inicial da tela
; Algoritmo
; 	while (*str != '\0') {
; 		printf("%c", *str);
; 		str++;
; 	}
; ---------------------------------------
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

; -----------------------------------------------------------------
; printint: imprime um numero inteiro sem sinal
; Parametros
; 	r0: numero
; 	r1: posicao inicial da tela
; -----------------------------------------------------------------
printint:
	push r0 ; numero
	push r1 ; posicao inicial da tela
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
		
		; Se eh zero: terminou, entao manda para o loop que desempilha e imprime
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
		pop r1
		pop r0
		rts

