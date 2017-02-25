; ------------------------------------------------------------------------------
; Universidade de Sao Paulo
; Instituto de Ciencias Matematicas e de Computacao
; SSC0112 Organizacao de Computadores Digitais I
;
; Trabalho 1: Conversor de Base
; 
; Carlos Humberto 7987456
; Elias Rodrigues 7987251
; Ricardo Fukuda  7986729
;
; 2015-10-07
;
; ------------------------------------------------------------------------------
; NOTE: como a arquitetura do ICMC trabalha somente com valores de 16 bits,
; esta implementacao suporta somente valores ate 16 bits, i.e. [0, 65535].
; ------------------------------------------------------------------------------
; NOTE: o programa suporta tanto caracteres maiusculos como minusculos para
; a base hexadecimal e para as letras da base.
; ------------------------------------------------------------------------------
; NOTE: os casos de testes foram registrados com screenshots do simulador e
; encontram-se no diretorio "./testes".
; ------------------------------------------------------------------------------

jmp main

	; String de 16 caracteres + '\0'
	; E' usada para armazenar a string do numero informado pelo usuario
	str_number_src:   string "                "

	; String de 1 caractere + '\0'
	; E' usada para armazenar a string da base do numero informado, onde
	; B : binario
	; O : octal
	; H : hexadecimal
	; D : decimal
	str_base_src:     string " "

	; Valor numerico do numero informado
	value_number_src: var #1

	; Valor numerico da base do numero informado
	value_base_src:   var #1

	; Idem as duas anteriores, porem estas sao para a base de destino
	str_base_dest:    string " "
	value_base_dest:  var #1

	; Posicao de memoria para guardar 1 char lido do teclado
	char: var #1

	; Mensagens ao usuario
	msg_title1: string "          CONVERSOR DE BASE             "
	msg_title2: string "          -----------------             "

	msg_howto1:	string " B = binario                            "
	msg_howto2:	string " O = octal                              "
	msg_howto3:	string " D = decimal                            "
	msg_howto4:	string " H = hexadecimal                        "

	msg_input1:	string " Informe a base de origem  :            "
	msg_input2:	string " Informe o numero :                     "
	msg_input3:	string " Informe a base de destino :            "

	msg_result:	string " Resultado :                            "

	msg_error1: string " ERRO: base invalida                    "
	msg_error2: string " ERRO: numero invalido                  "
	msg_error3: string " ERRO: numero maior que 16 bits         "

	msg_end1:	string " Deseja executar novamente? (y/n)       "
	msg_end2:	string " Programa encerrado.                    "

main:
	; Limpa toda a tela
	call clear

	; Imprime mensagens ao usuario
	loadn r0, #msg_title1 ; endereco da mensagem
	loadn r1, #40         ; posicao na tela
	call prints           ; rotina que imprime string

	; Idem para as demais mensagens
	loadn r0, #msg_title2
	loadn r1, #120
	call prints

	loadn r0, #msg_howto1
	loadn r1, #200
	call prints

	loadn r0, #msg_howto2
	loadn r1, #280
	call prints

	loadn r0, #msg_howto3
	loadn r1, #360
	call prints

	loadn r0, #msg_howto4
	loadn r1, #440
	call prints

	; Pede por base de origem
	loadn r0, #msg_input1
	loadn r1, #560
	call prints
	loadn r0, #str_base_src ; endereco onde a string sera salva
	loadn r1, #1            ; tamanho maximo da string
	loadn r2, #589          ; posicao para escrever na tela
	call gets
	; Verifica se base e' valida e define seu valor numerico
	call verify_base_src

	; Pede por numero
	loadn r0, #msg_input2
	loadn r1, #640
	call prints
	loadn r0, #str_number_src
	loadn r1, #16
	loadn r2, #661
	call gets
	; Verifica se numero e' valido e define seu valor numerico
	call verify_number

	; Pede por base de destino
	loadn r0, #msg_input3
	loadn r1, #720
	call prints
	loadn r0, #str_base_dest
	loadn r1, #1
	loadn r2, #749
	call gets
	; Verifica se base e' valida e define seu valor numerico
	call verify_base_dest

	; Imprime resultado
	loadn r0, #msg_result
	loadn r1, #840
	call prints
	load  r0, value_base_dest
	load  r1, value_number_src
	loadn r2, #854
	call  printn

	run_again:
		; Pergunta se quer rodar novamente
		loadn r0, #msg_end1
		loadn r1, #1040
		call prints

		; Le resposta do usuario
		call getc
		load r0, char

		; Se 'y' ou 'Y', executa o programa novamente
		loadn r1, #'y'
		cmp r1, r0
		jeq main
		loadn r1, #'Y'
		cmp r1, r0
		jeq main

	; Imprime mensagem de programa encerrado e finaliza execucao
	loadn r0, #msg_end2
	loadn r1, #1040
	call prints

	halt

; -------------------------------------------------------------
; Pega um char do teclado e salva na posicao de memoria #char
; -------------------------------------------------------------
getc:
	push fr
	push r0
	push r1
	loadn r1, #255

	getc_loop:
		inchar r0     ; pega char
		cmp r0, r1    ; comparar com 255
		jeq getc_loop ; se igual, fica neste loop

	; Salva char na posicao de memoria #char
	store char, r0

	getc_exit:
		pop r1
		pop r0
		pop fr
		rts

; ------------------------------------------------------------------------
; Pega uma string do teclado e vai imprimindo-a conforme o usuario digita
;     r0: endereco onde a string sera salva
;     r1: tamanho maximo da string
;     r2: posicao inicial na tela
;
; NOTE:
;     Tecla ENTER finaliza a string
;     Nao e' permitido string vazia
;     Se o tamanho maximo e' atingido, ele para de pegar a string
; BUG:
;     Teclas BACKSPACE e DELETE eram para funcionar apagando um caractere,
;     porem o simulador nao captura essas teclas...
;     Portanto, para apagar um caractere digitado, use a tecla SPACE
; ------------------------------------------------------------------------
gets:
	push fr
	push r0 ; endereco onde a string sera salva
	push r1 ; tamanho maximo da string
	push r2 ; posicao inicial da tela
	push r3 ; contador
	push r4 ; caractere ENTER / BACKSPACE / nulo '\0'
	push r5 ; auxiliar

	loadn r3, #0 ; inicializa contador

	jmp gets_loop

	; Se for BACKSPACE/DELETE, executa essa rotina
	gets_del:
		; Tratamento para apagar somente se algo foi digitado
		loadn r5, #0  
		cmp r5, r3
		jeq gets_loop
		
		dec r2         ; decrementa posicao na tela
		dec r3         ; decrementa o contador
		dec r0         ; decrementa posicao da string
		inc r1         ; incrementa tamanho maximo
		loadn r5, #32  ; carrega caractere SPACE
		outchar r5, r2 ; imprime caractere SPACE

	gets_loop:
		call getc     ; pega char
		load r5, char ; carrega char lido em r5
		storei r0, r5 ; armazena char lido na string

		loadn r4, #13 ; carectere ENTER
		cmp r4, r5    ; compara com tecla ENTER
		jeq gets_exit ; se e' ENTER, sai do loop

		;loadn r4, #8  ; carectere BACKSPACE (nao funciona)
		loadn r4, #' ' ; carectere SPACE ("fix" para poder apagar...)
		cmp r4, r5     ; compara com tecla BACKSPACE
		jeq gets_del   ; se igual, deleta ultimo char lido

		;loadn r4, #127 ; carectere DELETE (nao funciona)
		;cmp r4, r5     ; compara com tecla DELETE
		;jeq gets_del   ; se igual, deleta ultimo char lido

		outchar r5, r2 ; imprime char na tela para o usuario ver enqto digita
		inc r2         ; incrementa posicao na tela
		inc r3         ; incrementa o contador
		inc r0         ; incrementa posicao da string
		dec r1         ; decrementa tamanho maximo
		jnz gets_loop

		; Se tamanho maximo foi atingido, so pode receber ENTER ou BACKSPACE/DELETE
		gets_max_length:
			call getc     ; pega char
			load r5, char ; carrega char lido em r5
			storei r0, r5 ; armazena char lido na string

			loadn r4, #13 ; carectere ENTER
			cmp r4, r5    ; compara com tecla ENTER
			jeq gets_exit ; se e' ENTER, sai do loop

			;loadn r4, #8  ; carectere BACKSPACE (nao funciona)
			loadn r4, #' ' ; carectere SPACE ("fix" para poder apagar...)
			cmp r4, r5     ; compara com tecla BACKSPACE
			jeq gets_del   ; se igual, deleta ultimo char lido

			;loadn r4, #127 ; carectere DELETE (nao funciona)
			;cmp r4, r5     ; compara com tecla DELETE
			;jeq gets_del   ; se igual, deleta ultimo char lido

			jmp gets_max_length

	gets_exit:
		; Nao permite a insercao de uma palavra de tamanho zero
		loadn r5, #0
		cmp r3, r5
		jeq gets_loop

		; Coloca caractere nulo '\0' no final da palavra
		storei r0, r5

		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; ---------------------------------------
; Imprime uma string na tela
;     r0: endereco inicial da string
;     r1: posicao inicial na tela
; ---------------------------------------
prints:
	push fr
	push r0 ; endereco do caractere a ser impresso
	push r1 ; posicao na tela
	push r2 ; condicao de parada
	push r3 ; caractere a ser impresso

	; Carrega condicao de parada (caractere de valor zero)
	loadn r2, #0
	
	prints_loop:
		loadi r3, r0    ; carrega o caractere
		cmp r2, r3      ; compara com zero
		jeq prints_exit ; se e' zero, sai da rotina

		outchar r3, r1  ; se nao eh zero, imprime caractere
		inc r0          ; incrementa endereco da string
		inc r1          ; incrementa posicao na tela
		jmp prints_loop ; volta para o loop de impressao

	; Desempilha os registradores e retorna
	prints_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; -----------------------------------------------------
; Imprime caractere SPACE em toda a tela para limpa-la
; -----------------------------------------------------
clear:
	push fr
	push r0
	push r1

	loadn r0, #1200 ; quantidade de posicoes na tela
	loadn r1, #' '  ; caractere SPACE

	clear_loop:
		dec r0         ; decrementa posicao da tela
		outchar r1, r0 ; imprime SPACE
		jnz clear_loop ; volta para o loop se ainda tem posicao

	pop r1
	pop r0
	pop fr
	rts

; ----------------------------------------
; Verifica se a base informada e' valida
; ----------------------------------------
verify_base_src:
	push fr
	push r0
	push r1
	push r2 ; auxiliar

	load  r0, str_base_src    ; caractere da base
	loadn r1, #value_base_src ; endereco do valor numerico da base
	jmp verify_base

verify_base_dest:
	push fr
	push r0
	push r1
	push r2 ; auxiliar

	load  r0, str_base_dest    ; caractere da base
	loadn r1, #value_base_dest ; endereco do valor numerico da base
	jmp verify_base

verify_base:
	; Confere se e' igual a 'b' ou 'B'
	loadn r2, #'b'              ; carrega 'b'
	cmp r2, r0                  ; compara com valor lido
	jeq verify_base_set_value_b ; se igual, define valor numerico
	loadn r2, #'B'              ; carrega 'B'
	cmp r2, r0                  ; compara com valor lido
	jeq verify_base_set_value_b ; se igual, define valor numerico

	; Idem para as demais bases:

	; Confere se e' igual a 'o' ou 'O'
	loadn r2, #'o'
	cmp r2, r0
	jeq verify_base_set_value_o
	loadn r2, #'O'
	cmp r2, r0
	jeq verify_base_set_value_o

	; Confere se e' igual a 'd' ou 'D'
	loadn r2, #'d'
	cmp r2, r0
	jeq verify_base_set_value_d
	loadn r2, #'D'
	cmp r2, r0
	jeq verify_base_set_value_d

	; Confere se e' igual a 'h' ou 'H'
	loadn r2, #'h'
	cmp r2, r0
	jeq verify_base_set_value_h
	loadn r2, #'H'
	cmp r2, r0
	jeq verify_base_set_value_h

	; Se nao e' igual a nenhum dos caracteres acima, entao deu erro!
	; Imprime mensagem de erro "base invalida"
	; e faz jump para "executar novamente"
	verify_base_error:
		loadn r0, #msg_error1
		loadn r1, #840
		call prints
		pop r2
		pop r1
		pop r0
		pop fr
		jmp run_again

	verify_base_set_value_b:
		loadn  r2, #2 ; carrega valor numerico da base
		storei r1, r2 ; salva na memoria
		jmp verify_base_exit ; sai

	; Idem para as demais bases
	verify_base_set_value_o:
		loadn  r2, #8
		storei r1, r2
		jmp verify_base_exit
	
	verify_base_set_value_d:
		loadn  r2, #10
		storei r1, r2
		jmp verify_base_exit
	
	verify_base_set_value_h:
		loadn  r2, #16
		storei r1, r2
		jmp verify_base_exit

	verify_base_exit:
		pop r2
		pop r1
		pop r0
		pop fr
		rts

; ---------------------------------------------------------------------
; Verifica se o numero informado e' valido e define seu valor numerico
; ---------------------------------------------------------------------
verify_number:
	push fr
	push r0 ; caractere da base
	push r1 ; endereco do stringo do numero
	push r2 ; valor zero para comparacao
	push r3 ; um caractere do numero
	push r4 ; auxiliar
	push r5 ; contador de caracteres (pra saber qtos caracteres o numero tem)
	push r6 ; auxiliar
	push r7 ; auxiliar
	
	load  r0, value_base_src  ; carrega o valor da base de origem em r0
	loadn r1, #str_number_src ; carrega endereco da string do numero em r1
	loadn r2, #0              ; carrega valor zero r2

	loadn r5, #0 ; inicializa contador

	dec r1 ; FIX so' para o loop ficar do tipo "do-while" ;)
	verify_number_loop:
		inc r1
		loadi r3, r1 ; carrega em r3 um caractere da string do numero

		; Confere condicao de parada (se o caractere eh '\0')
		cmp r2, r3
		jeq verify_number_set_value

		inc r5 ; incrementa contador caracteres

		; Confere para base binaria
		loadn r4, #2        ; carrega #2
		cmp r4, r0          ; compara com valor lido da base de origem
		jeq verify_number_b ; se igual, confere para base binaria

		; Idem para as demais bases

		; Confere para base octal
		loadn r4, #8
		cmp r4, r0
		jeq verify_number_o

		; Confere para base decimal
		loadn r4, #10
		cmp r4, r0
		jeq verify_number_d

		; Confere para base hexadecimal
		loadn r4, #16
		cmp r4, r0
		jeq verify_number_h

		verify_number_h:
			loadn r4, #'f'         ; carrega caractere valido
			cmp r4, r3             ; compara com o caractere da string
			jeq verify_number_loop ; se igual, continua
			loadn r4, #'F'
			cmp r4, r3
			jeq verify_number_loop

			; Idem para os demais caracteres
			loadn r4, #'e'
			cmp r4, r3
			jeq verify_number_loop
			loadn r4, #'E'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'d'
			cmp r4, r3
			jeq verify_number_loop
			loadn r4, #'D'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'c'
			cmp r4, r3
			jeq verify_number_loop
			loadn r4, #'C'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'b'
			cmp r4, r3
			jeq verify_number_loop
			loadn r4, #'B'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'a'
			cmp r4, r3
			jeq verify_number_loop
			loadn r4, #'A'
			cmp r4, r3
			jeq verify_number_loop

		verify_number_d:
			loadn r4, #'9'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'8'
			cmp r4, r3
			jeq verify_number_loop

		verify_number_o:
			loadn r4, #'7'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'6'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'5'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'4'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'3'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'2'
			cmp r4, r3
			jeq verify_number_loop

		verify_number_b:
			loadn r4, #'1'
			cmp r4, r3
			jeq verify_number_loop

			loadn r4, #'0'
			cmp r4, r3
			jeq verify_number_loop

			; Se nao foi igual a nenhum dos caracteres listados acima,
			; entao deu erro!
			; Imprime mensagem de erro "numero invalido"
			; e faz jump para "executar novamente"
			verify_number_error_invalid:
				loadn r0, #msg_error2
			verify_number_error:
				loadn r1, #840
				call prints
				pop r7
				pop r6
				pop r5
				pop r4
				pop r3
				pop r2
				pop r1
				pop r0
				pop fr
				jmp run_again

			; Caso tenha ocorrido overflow em "verify_number_set_value",
			; Imprime mensagem de erro "numero maior que 16 bits"
			; e faz jump para "executar novamente"
			verify_number_error_overflow:
				loadn r0, #msg_error3
				jmp verify_number_error

	verify_number_set_value:
		loadn r6, #0 ; r6 tera' o valor numerico da string do numero
		loadn r7, #1 ; valor para ir multiplicando pela base
		
		verify_number_set_value_loop:
			dec r5
			dec r1
			loadi r3, r1 ; carrega caractere do numero em r3

			; Compara esse caractere com os valores de "digitos"
			loadn r4, #'f'                ; carrega caractere do "digito"
			cmp r4, r3                    ; compara com caractere da string
			jeq verify_number_set_value_f ; se igual, define valor do "digito"
			loadn r4, #'F'
			cmp r4, r3
			jeq verify_number_set_value_f

			; Idem para os demais "digitos" possiveis
			loadn r4, #'e'
			cmp r4, r3
			jeq verify_number_set_value_e
			loadn r4, #'E'
			cmp r4, r3
			jeq verify_number_set_value_e

			loadn r4, #'d'
			cmp r4, r3
			jeq verify_number_set_value_d
			loadn r4, #'D'
			cmp r4, r3
			jeq verify_number_set_value_d

			loadn r4, #'c'
			cmp r4, r3
			jeq verify_number_set_value_c
			loadn r4, #'C'
			cmp r4, r3
			jeq verify_number_set_value_c

			loadn r4, #'b'
			cmp r4, r3
			jeq verify_number_set_value_b
			loadn r4, #'B'
			cmp r4, r3
			jeq verify_number_set_value_b

			loadn r4, #'a'
			cmp r4, r3
			jeq verify_number_set_value_a
			loadn r4, #'A'
			cmp r4, r3
			jeq verify_number_set_value_a

			loadn r4, #'9'
			cmp r4, r3
			jeq verify_number_set_value_9

			loadn r4, #'8'
			cmp r4, r3
			jeq verify_number_set_value_8

			loadn r4, #'7'
			cmp r4, r3
			jeq verify_number_set_value_7

			loadn r4, #'6'
			cmp r4, r3
			jeq verify_number_set_value_6

			loadn r4, #'5'
			cmp r4, r3
			jeq verify_number_set_value_5

			loadn r4, #'4'
			cmp r4, r3
			jeq verify_number_set_value_4

			loadn r4, #'3'
			cmp r4, r3
			jeq verify_number_set_value_3

			loadn r4, #'2'
			cmp r4, r3
			jeq verify_number_set_value_2

			loadn r4, #'1'
			cmp r4, r3
			jeq verify_number_set_value_1

			loadn r4, #'0'
			cmp r4, r3
			jeq verify_number_set_value_0

		verify_number_set_value_f:
			loadn r4, #15 ; carrega valor numerico do "digito"
			jmp verify_number_set_value_do ; define valor numerico

		; Idem para os demais "digitos"
		verify_number_set_value_e:
			loadn r4, #14
			jmp verify_number_set_value_do

		verify_number_set_value_d:
			loadn r4, #13
			jmp verify_number_set_value_do

		verify_number_set_value_c:
			loadn r4, #12
			jmp verify_number_set_value_do

		verify_number_set_value_b:
			loadn r4, #11
			jmp verify_number_set_value_do

		verify_number_set_value_a:
			loadn r4, #10
			jmp verify_number_set_value_do

		verify_number_set_value_9:
			loadn r4, #9
			jmp verify_number_set_value_do

		verify_number_set_value_8:
			loadn r4, #8
			jmp verify_number_set_value_do

		verify_number_set_value_7:
			loadn r4, #7
			jmp verify_number_set_value_do

		verify_number_set_value_6:
			loadn r4, #6
			jmp verify_number_set_value_do

		verify_number_set_value_5:
			loadn r4, #5
			jmp verify_number_set_value_do

		verify_number_set_value_4:
			loadn r4, #4
			jmp verify_number_set_value_do

		verify_number_set_value_3:
			loadn r4, #3
			jmp verify_number_set_value_do

		verify_number_set_value_2:
			loadn r4, #2
			jmp verify_number_set_value_do

		verify_number_set_value_1:
			loadn r4, #1
			jmp verify_number_set_value_do

		verify_number_set_value_0:
			loadn r4, #0
			jmp verify_number_set_value_do

		verify_number_set_value_do:
			mul r4, r4, r7          ; multiplica pela base
			add r6, r6, r4          ; soma no resultado
			jc  verify_number_error_overflow ; se deu overflow: erro!
			mul r7, r7, r0          ; aumenta a potencia da base
			cmp r5, r2              ; testa se pode continuar loop
			jne verify_number_set_value_loop

	verify_number_exit:
		; Salva valor numerico da string do numero
		store value_number_src, r6

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

; ------------------------------------------------------
; Imprime na tela um numero em uma dada base
;     r0: valor da base
;     r1: valor do numero
;     r2: posicao inicial na tela
; ------------------------------------------------------
printn:
	push fr
	push r0
	push r1
	push r2
	push r3 ; condicao de parada
	push r4 ; contador
	push r5 ; auxiliar
	push r6 ; resto

	; Carrega condicao de parada
	loadn r3, #0

	; Inicializa contador
	loadn r4, #0

	printn_loop:
		mod r6, r1, r0 ; resto de numero/base
		div r1, r1, r0 ; quociente de numero/10

		; Empilha caractere que representa o resto
		loadn r5, #0      ; carrega valor do digito
		cmp r5, r6        ; compara
		jeq printn_push_0 ; se eh igual, vai empilhar o caractere desse "digito"

		; Idem para os demais "digitos"
		loadn r5, #1
		cmp r5, r6
		jeq printn_push_1

		loadn r5, #2
		cmp r5, r6
		jeq printn_push_2

		loadn r5, #3
		cmp r5, r6
		jeq printn_push_3

		loadn r5, #4
		cmp r5, r6
		jeq printn_push_4

		loadn r5, #5
		cmp r5, r6
		jeq printn_push_5

		loadn r5, #6
		cmp r5, r6
		jeq printn_push_6

		loadn r5, #7
		cmp r5, r6
		jeq printn_push_7

		loadn r5, #8
		cmp r5, r6
		jeq printn_push_8

		loadn r5, #9
		cmp r5, r6
		jeq printn_push_9

		loadn r5, #10
		cmp r5, r6
		jeq printn_push_a

		loadn r5, #11
		cmp r5, r6
		jeq printn_push_b

		loadn r5, #12
		cmp r5, r6
		jeq printn_push_c

		loadn r5, #13
		cmp r5, r6
		jeq printn_push_d

		loadn r5, #14
		cmp r5, r6
		jeq printn_push_e

		loadn r5, #15
		cmp r5, r6
		jeq printn_push_f

		printn_loop_continue:
			inc  r4 ; incrementa contador

			; Se (quociente == 0) vai para "printn_pop"
			cmp r1, r3
			jeq printn_pop
			; Senao, volta para "printn_loop"
			jmp printn_loop

	printn_push_0:
		loadn r5, #'0' ; carrega valor ASCII do "digito" 0
		push r5        ; empilha
		jmp printn_loop_continue ; volta para o loop

	; Idem para os demais "digitos"
	printn_push_1:
		loadn r5, #'1'
		push r5
		jmp printn_loop_continue

	printn_push_2:
		loadn r5, #'2'
		push r5
		jmp printn_loop_continue

	printn_push_3:
		loadn r5, #'3'
		push r5
		jmp printn_loop_continue

	printn_push_4:
		loadn r5, #'4'
		push r5
		jmp printn_loop_continue

	printn_push_5:
		loadn r5, #'5'
		push r5
		jmp printn_loop_continue

	printn_push_6:
		loadn r5, #'6'
		push r5
		jmp printn_loop_continue

	printn_push_7:
		loadn r5, #'7'
		push r5
		jmp printn_loop_continue

	printn_push_8:
		loadn r5, #'8'
		push r5
		jmp printn_loop_continue

	printn_push_9:
		loadn r5, #'9'
		push r5
		jmp printn_loop_continue

	printn_push_a:
		loadn r5, #'a'
		push r5
		jmp printn_loop_continue

	printn_push_b:
		loadn r5, #'b'
		push r5
		jmp printn_loop_continue

	printn_push_c:
		loadn r5, #'c'
		push r5
		jmp printn_loop_continue

	printn_push_d:
		loadn r5, #'d'
		push r5
		jmp printn_loop_continue

	printn_push_e:
		loadn r5, #'e'
		push r5
		jmp printn_loop_continue

	printn_push_f:
		loadn r5, #'f'
		push r5
		jmp printn_loop_continue

	; Desempilha os caracteres empilhados e imprime
	printn_pop:
		pop r5 ; desempilha caractere
		outchar r5, r2 ; imprime na tela
		inc r2 ; incrementa posicao na tela
		dec r4 ; decrementa contador
		jz   printn_exit
		jmp  printn_pop

	; Desempilha os registradores e retorna
	printn_exit:
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		pop fr
		rts
