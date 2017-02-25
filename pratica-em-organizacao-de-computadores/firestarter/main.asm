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
;
; ---------------------------------------------------------------
; ===============================================================
;
; Jogo "Firestarter" v1.1
; 2013/05/24
; Elias Italiano Rodrigues, 7987251, Org (Pratica), BCC, ICMC-USP
;
; ---------------------------------------------------------------
; ===============================================================
;
; ABOUT
; -----
; 	Este jogo foi desenvolvido em assembly do Simoes (processador ICMC)
; 	como trabalho da disciplina de Organizacao de Computadores Digitais.
;
; 	Neste jogo voce controla um tanque-de-guerra.
;
; 	Teclas:
; 		Use W-A-S-D para movimenta-lo.
; 		Aperte J ou K para atirar.
; 		Para pausar o jogo, pressione Enter.
;
; 	Objetivo:
; 		Destruir o maior numero de mobs dentro do tempo.
;
; CONFIG
; ------
; 	Para rodar o jogo adequadamente, ajuste os valores nas seguintes funcoes:
; 		delay
; 		dec_time
;

jmp main
	; Texto sobre como jogar
	; ----------------------
	text_help_keys_00: string "     Use W-A-S-D to move the tank.      "
	text_help_keys_01: string "                                        "
	text_help_keys_02: string "     Hit J or K to fire.                "
	text_help_keys_03: string "                                        "
	text_help_keys_04: string "     Press Enter to pause the game.     "

	text_help_target_00: string "     Kill all mobs on the way,          "
	text_help_target_01: string "                                        "
	text_help_target_02: string "     before you run out of time!        "

	; Texto de fim de jogo (ganhou)
	; -----------------------------
	text_game_over_win_00: string "      G A M E   O V E R                 "
	text_game_over_win_01: string "                                        "
	text_game_over_win_02: string "                                        "
	text_game_over_win_03: string "                                        "
	text_game_over_win_04: string "             Y O U   W I N              "

	; Texto de fim de jogo (perdeu)
	; -----------------------------
	text_game_over_lose_00: string "      G A M E   O V E R                 "
	text_game_over_lose_01: string "                                        "
	text_game_over_lose_02: string "                                        "
	text_game_over_lose_03: string "                                        "
	text_game_over_lose_04: string "             Y O U   L O S E            "

	; Texto de sucesso
	; ----------------
	text_great_00: string "     You did it! You killed all mobs    "
	text_great_01: string "                                        "
	text_great_02: string "     in time!                           "

	; Texto de fracasso
	; -----------------
	text_try_again_00: string "     The mobs owned you, what a shame!  "
	text_try_again_01: string "                                        "
	text_try_again_02: string "     Try again...                       "

	; Mensagens
	; ---------
	msg_go:          string "   GO, DESTROY THEM!                    "
	msg_damn:        string "   DAMN MOBS!                           "
	msg_life:        string "   TAKE CARE, OR YOU WILL DIE...        "
	msg_kill:        string "   I'M GONNA KILL Y'ALL!                "
	msg_fire:        string "   FIRE, FIRE, FIRE!                    "
	msg_time:        string "   TIME IS RUNNING OUT!                 "
	msg_boost:       string "   YOU GOT BOOSTER POWER!               "
	msg_timer:       string "   TIME INCREASED BY 15!                "
	msg_pause:       string "   PAUSE...                             "

	msg_enter_name:  string "     Enter your name :                  "
	msg_not_enough:  string "     Your score is not enough...        "
	msg_press_enter: string "        Press enter to continue         "
	msg_time_out:    string "            T I M E   O U T             "
	msg_the_end:     string "             T H E   E N D              "
	msg_version:     string "                            version 1.1 "

	; Menus
	; -----
	menu_start:      string "  Start Game (S)                        "
	menu_records:    string "  Records (R)                           "
	menu_help:       string "  Help (H)                              "
	menu_back:       string "  Back (B)                              "
	menu_play_again: string "     Do you wanna play again? (Y/N)     "

	; Declaracao de variaveis
	; -----------------------
		; Do jogo
		; -------
		char:         var #1 ; armazena char lido do teclado
		time:         var #1 ; tempo de jogo
		life:         var #1 ; quantidade de vezes que o jogador pode morrer (vidas)
		score:        var #1 ; pontos do jogador
		kill_mob_1:   var #1 ; quantidade de hits para matar um mob do tipo 1
		kill_mob_2:   var #1 ; quantidade de hits para matar um mob do tipo 2
		score_mob_1:  var #1 ; quantidade de pontos por mata um mob do tipo 1
		score_mob_2:  var #1 ; quantidade de pontos por mata um mob do tipo 2
		seed:         var #1 ; semente para os numeros aleatorios de cada partida

		; Contadores
		; ----------
		booster_fire: var #1 ; contador de quantos tiros com boost o tank pode dar
		sec:          var #1 ; contador para sincronizar os "segundos"
		mobs_speed:   var #1 ; contador para controlar a velocidade dos mobs
		mobs_time:    var #1 ; contador determinar a frequencia que vai gerar mobs
		rand:         var #1 ; contador para indexar o vetor de numeros aleatorios
		shots:        var #1 ; contador para indexar o vetor de tiros
		mobs_1:       var #1 ; contador para indexar o vetor de mobs tipo 1
		mobs_2:       var #1 ; contador para indexar o vetor de mobs tipo 2
		msg_swap:     var #1 ; contador para o tempo que uma mensagem fica exibida

		; Flags
		; -----
		booster:       var #1 ; flag: seta para 1 quando houver booster na tela
		                      ;       seta para 2 quando tank pegar o item de booster
		timer:         var #1 ; flag: seta para 1 quando houver timer na tela
		booster_block: var #1 ; flag: seta para 1 para nao gerar mais boosters
		timer_block:   var #1 ; flag: seta para 1 para nao gerar mais timers
		msg:           var #1 ; flag: usada para definir uma mensagem para mostrar no topo da tela
		hit:           var #1 ; flag: quando acertar o mob, seta para o valor de score_mob_1 ou score_mob_2
		damage:        var #1 ; flag: quando atingir tank, seta para 1
		game_over:     var #1 ; flag: se fim de jogo por tempo, seta para 1
		                      ;       se fim de jogo por morrer, seta para 2

		; Posicoes de objetos
		; -------------------
			; Tank
			; ----
			tank_pre_pos: var #1 ; posicao anterior do tank
			tank_cur_pos: var #1 ; posicao atual do tank

			; Tiros
			; -----
			shots_pre_pos: var #10 ; posicao anterior de cada tiro
			shots_cur_pos: var #10 ; posicao atual de cada tiro

			; Posicoes dos mobs tipo 1
			; ------------------------
			mobs_pre_pos_1: var #20 ; posicao anterior de cada mob do tipo 1
			mobs_cur_pos_1: var #20 ; posicao atual de cada mob do tipo 1

			; Posicoes dos mobs tipo 2
			; ------------------------
			mobs_pre_pos_2: var #20 ; posicao anterior de cada mob do tipo 2
			mobs_cur_pos_2: var #20 ; posicao atual de cada mob do tipo 2

			; Booster
			; -------
			booster_cur_pos: var #1 ; posicao do booster

			; Timer
			; -------
			timer_cur_pos: var #1 ; posicao do timer

		; Dano dos mobs
		; -------------
		mobs_damage_1: var #20 ; vetor com o valor do dano dos mobs do tipo 1
		mobs_damage_2: var #20 ; vetor com o valor do dano dos mobs do tipo 2

		; Records
		; -------
			; Vetor de nomes
			; --------------
			records_names:  var #8

			; Nomes (3 letras + '\0')
			; -----------------------
			records_name_0: var #4
			records_name_1: var #4
			records_name_2: var #4
			records_name_3: var #4
			records_name_4: var #4
			records_name_5: var #4
			records_name_6: var #4
			records_name_7: var #4

			; Pontos
			; ------
			records_scores: var #8

			; Vidas
			; -----
			records_lifes:  var #8

		; Numeros aleatorios
		; ------------------
		rand_numbers: var #1013
		static rand_numbers + #0, #3314
		static rand_numbers + #1, #4585
		static rand_numbers + #2, #501
		static rand_numbers + #3, #1030
		static rand_numbers + #4, #1761
		static rand_numbers + #5, #7804
		static rand_numbers + #6, #6031
		static rand_numbers + #7, #2002
		static rand_numbers + #8, #10740
		static rand_numbers + #9, #9621
		static rand_numbers + #10, #9640
		static rand_numbers + #11, #4121
		static rand_numbers + #12, #9930
		static rand_numbers + #13, #10412
		static rand_numbers + #14, #9134
		static rand_numbers + #15, #1944
		static rand_numbers + #16, #277
		static rand_numbers + #17, #10524
		static rand_numbers + #18, #6380
		static rand_numbers + #19, #9726
		static rand_numbers + #20, #1387
		static rand_numbers + #21, #1872
		static rand_numbers + #22, #7009
		static rand_numbers + #23, #10924
		static rand_numbers + #24, #7307
		static rand_numbers + #25, #4495
		static rand_numbers + #26, #1445
		static rand_numbers + #27, #796
		static rand_numbers + #28, #7956
		static rand_numbers + #29, #9557
		static rand_numbers + #30, #255
		static rand_numbers + #31, #283
		static rand_numbers + #32, #5566
		static rand_numbers + #33, #756
		static rand_numbers + #34, #3725
		static rand_numbers + #35, #7327
		static rand_numbers + #36, #10971
		static rand_numbers + #37, #9756
		static rand_numbers + #38, #9330
		static rand_numbers + #39, #10724
		static rand_numbers + #40, #10801
		static rand_numbers + #41, #10394
		static rand_numbers + #42, #6270
		static rand_numbers + #43, #1168
		static rand_numbers + #44, #1243
		static rand_numbers + #45, #6828
		static rand_numbers + #46, #5523
		static rand_numbers + #47, #3931
		static rand_numbers + #48, #8776
		static rand_numbers + #49, #3327
		static rand_numbers + #50, #2670
		static rand_numbers + #51, #10164
		static rand_numbers + #52, #5199
		static rand_numbers + #53, #9679
		static rand_numbers + #54, #1525
		static rand_numbers + #55, #3930
		static rand_numbers + #56, #5598
		static rand_numbers + #57, #5381
		static rand_numbers + #58, #4727
		static rand_numbers + #59, #4979
		static rand_numbers + #60, #3951
		static rand_numbers + #61, #7393
		static rand_numbers + #62, #7673
		static rand_numbers + #63, #942
		static rand_numbers + #64, #10561
		static rand_numbers + #65, #411
		static rand_numbers + #66, #10680
		static rand_numbers + #67, #10545
		static rand_numbers + #68, #1591
		static rand_numbers + #69, #447
		static rand_numbers + #70, #1707
		static rand_numbers + #71, #3816
		static rand_numbers + #72, #2265
		static rand_numbers + #73, #10388
		static rand_numbers + #74, #4984
		static rand_numbers + #75, #3508
		static rand_numbers + #76, #6229
		static rand_numbers + #77, #1931
		static rand_numbers + #78, #7439
		static rand_numbers + #79, #6429
		static rand_numbers + #80, #5258
		static rand_numbers + #81, #1533
		static rand_numbers + #82, #8017
		static rand_numbers + #83, #1882
		static rand_numbers + #84, #226
		static rand_numbers + #85, #9542
		static rand_numbers + #86, #8223
		static rand_numbers + #87, #8235
		static rand_numbers + #88, #6348
		static rand_numbers + #89, #1963
		static rand_numbers + #90, #2227
		static rand_numbers + #91, #1723
		static rand_numbers + #92, #781
		static rand_numbers + #93, #9901
		static rand_numbers + #94, #2665
		static rand_numbers + #95, #355
		static rand_numbers + #96, #10312
		static rand_numbers + #97, #4770
		static rand_numbers + #98, #10900
		static rand_numbers + #99, #3328
		static rand_numbers + #100, #5217
		static rand_numbers + #101, #1620
		static rand_numbers + #102, #7144
		static rand_numbers + #103, #7483
		static rand_numbers + #104, #1021
		static rand_numbers + #105, #3553
		static rand_numbers + #106, #2415
		static rand_numbers + #107, #9661
		static rand_numbers + #108, #5484
		static rand_numbers + #109, #1279
		static rand_numbers + #110, #7515
		static rand_numbers + #111, #10743
		static rand_numbers + #112, #2812
		static rand_numbers + #113, #4545
		static rand_numbers + #114, #4049
		static rand_numbers + #115, #5449
		static rand_numbers + #116, #5512
		static rand_numbers + #117, #1285
		static rand_numbers + #118, #2698
		static rand_numbers + #119, #3284
		static rand_numbers + #120, #5660
		static rand_numbers + #121, #4925
		static rand_numbers + #122, #5007
		static rand_numbers + #123, #6441
		static rand_numbers + #124, #6250
		static rand_numbers + #125, #10084
		static rand_numbers + #126, #6796
		static rand_numbers + #127, #7987
		static rand_numbers + #128, #3867
		static rand_numbers + #129, #6709
		static rand_numbers + #130, #328
		static rand_numbers + #131, #9084
		static rand_numbers + #132, #10741
		static rand_numbers + #133, #7472
		static rand_numbers + #134, #7991
		static rand_numbers + #135, #3186
		static rand_numbers + #136, #38
		static rand_numbers + #137, #10407
		static rand_numbers + #138, #4272
		static rand_numbers + #139, #7934
		static rand_numbers + #140, #699
		static rand_numbers + #141, #800
		static rand_numbers + #142, #10101
		static rand_numbers + #143, #5922
		static rand_numbers + #144, #7756
		static rand_numbers + #145, #3163
		static rand_numbers + #146, #385
		static rand_numbers + #147, #4692
		static rand_numbers + #148, #6859
		static rand_numbers + #149, #3083
		static rand_numbers + #150, #7976
		static rand_numbers + #151, #1532
		static rand_numbers + #152, #10419
		static rand_numbers + #153, #1997
		static rand_numbers + #154, #7973
		static rand_numbers + #155, #5683
		static rand_numbers + #156, #1094
		static rand_numbers + #157, #6193
		static rand_numbers + #158, #2683
		static rand_numbers + #159, #7372
		static rand_numbers + #160, #4327
		static rand_numbers + #161, #5422
		static rand_numbers + #162, #5469
		static rand_numbers + #163, #6492
		static rand_numbers + #164, #1907
		static rand_numbers + #165, #4885
		static rand_numbers + #166, #9678
		static rand_numbers + #167, #4357
		static rand_numbers + #168, #6716
		static rand_numbers + #169, #2963
		static rand_numbers + #170, #3715
		static rand_numbers + #171, #9826
		static rand_numbers + #172, #6174
		static rand_numbers + #173, #2829
		static rand_numbers + #174, #4761
		static rand_numbers + #175, #5355
		static rand_numbers + #176, #8403
		static rand_numbers + #177, #5146
		static rand_numbers + #178, #10047
		static rand_numbers + #179, #4275
		static rand_numbers + #180, #10640
		static rand_numbers + #181, #7037
		static rand_numbers + #182, #5808
		static rand_numbers + #183, #10073
		static rand_numbers + #184, #9034
		static rand_numbers + #185, #5205
		static rand_numbers + #186, #7180
		static rand_numbers + #187, #1552
		static rand_numbers + #188, #2823
		static rand_numbers + #189, #9863
		static rand_numbers + #190, #8924
		static rand_numbers + #191, #7150
		static rand_numbers + #192, #6709
		static rand_numbers + #193, #5817
		static rand_numbers + #194, #2655
		static rand_numbers + #195, #8616
		static rand_numbers + #196, #2126
		static rand_numbers + #197, #3757
		static rand_numbers + #198, #4397
		static rand_numbers + #199, #8842
		static rand_numbers + #200, #9132
		static rand_numbers + #201, #8112
		static rand_numbers + #202, #7681
		static rand_numbers + #203, #4319
		static rand_numbers + #204, #2365
		static rand_numbers + #205, #1456
		static rand_numbers + #206, #9674
		static rand_numbers + #207, #10768
		static rand_numbers + #208, #9013
		static rand_numbers + #209, #159
		static rand_numbers + #210, #4057
		static rand_numbers + #211, #8667
		static rand_numbers + #212, #7196
		static rand_numbers + #213, #9865
		static rand_numbers + #214, #10164
		static rand_numbers + #215, #7654
		static rand_numbers + #216, #6494
		static rand_numbers + #217, #8768
		static rand_numbers + #218, #9206
		static rand_numbers + #219, #9317
		static rand_numbers + #220, #7644
		static rand_numbers + #221, #9554
		static rand_numbers + #222, #7891
		static rand_numbers + #223, #5777
		static rand_numbers + #224, #6795
		static rand_numbers + #225, #1970
		static rand_numbers + #226, #3406
		static rand_numbers + #227, #8922
		static rand_numbers + #228, #5728
		static rand_numbers + #229, #10215
		static rand_numbers + #230, #6777
		static rand_numbers + #231, #3873
		static rand_numbers + #232, #9751
		static rand_numbers + #233, #5883
		static rand_numbers + #234, #10603
		static rand_numbers + #235, #1130
		static rand_numbers + #236, #7339
		static rand_numbers + #237, #715
		static rand_numbers + #238, #911
		static rand_numbers + #239, #7776
		static rand_numbers + #240, #874
		static rand_numbers + #241, #4968
		static rand_numbers + #242, #7867
		static rand_numbers + #243, #8070
		static rand_numbers + #244, #6257
		static rand_numbers + #245, #9455
		static rand_numbers + #246, #7148
		static rand_numbers + #247, #4176
		static rand_numbers + #248, #7236
		static rand_numbers + #249, #7778
		static rand_numbers + #250, #4917
		static rand_numbers + #251, #6304
		static rand_numbers + #252, #6345
		static rand_numbers + #253, #1822
		static rand_numbers + #254, #1094
		static rand_numbers + #255, #4564
		static rand_numbers + #256, #6203
		static rand_numbers + #257, #4501
		static rand_numbers + #258, #2499
		static rand_numbers + #259, #944
		static rand_numbers + #260, #6140
		static rand_numbers + #261, #701
		static rand_numbers + #262, #7228
		static rand_numbers + #263, #4904
		static rand_numbers + #264, #6584
		static rand_numbers + #265, #6845
		static rand_numbers + #266, #8445
		static rand_numbers + #267, #5347
		static rand_numbers + #268, #7560
		static rand_numbers + #269, #9357
		static rand_numbers + #270, #2136
		static rand_numbers + #271, #10845
		static rand_numbers + #272, #5749
		static rand_numbers + #273, #1428
		static rand_numbers + #274, #10339
		static rand_numbers + #275, #3431
		static rand_numbers + #276, #10883
		static rand_numbers + #277, #6500
		static rand_numbers + #278, #7607
		static rand_numbers + #279, #7133
		static rand_numbers + #280, #3291
		static rand_numbers + #281, #1537
		static rand_numbers + #282, #2450
		static rand_numbers + #283, #1060
		static rand_numbers + #284, #5770
		static rand_numbers + #285, #3545
		static rand_numbers + #286, #5624
		static rand_numbers + #287, #3398
		static rand_numbers + #288, #10457
		static rand_numbers + #289, #10535
		static rand_numbers + #290, #4342
		static rand_numbers + #291, #8021
		static rand_numbers + #292, #249
		static rand_numbers + #293, #584
		static rand_numbers + #294, #4349
		static rand_numbers + #295, #9244
		static rand_numbers + #296, #9840
		static rand_numbers + #297, #1808
		static rand_numbers + #298, #3604
		static rand_numbers + #299, #8824
		static rand_numbers + #300, #2589
		static rand_numbers + #301, #8151
		static rand_numbers + #302, #8682
		static rand_numbers + #303, #8338
		static rand_numbers + #304, #9579
		static rand_numbers + #305, #10445
		static rand_numbers + #306, #782
		static rand_numbers + #307, #9476
		static rand_numbers + #308, #5958
		static rand_numbers + #309, #10800
		static rand_numbers + #310, #8033
		static rand_numbers + #311, #9249
		static rand_numbers + #312, #3762
		static rand_numbers + #313, #10483
		static rand_numbers + #314, #10309
		static rand_numbers + #315, #956
		static rand_numbers + #316, #5452
		static rand_numbers + #317, #7357
		static rand_numbers + #318, #4354
		static rand_numbers + #319, #4922
		static rand_numbers + #320, #6905
		static rand_numbers + #321, #8697
		static rand_numbers + #322, #1956
		static rand_numbers + #323, #7154
		static rand_numbers + #324, #705
		static rand_numbers + #325, #6306
		static rand_numbers + #326, #5411
		static rand_numbers + #327, #1969
		static rand_numbers + #328, #10525
		static rand_numbers + #329, #439
		static rand_numbers + #330, #10793
		static rand_numbers + #331, #2127
		static rand_numbers + #332, #8591
		static rand_numbers + #333, #10899
		static rand_numbers + #334, #1889
		static rand_numbers + #335, #9594
		static rand_numbers + #336, #10357
		static rand_numbers + #337, #2672
		static rand_numbers + #338, #10494
		static rand_numbers + #339, #7739
		static rand_numbers + #340, #4896
		static rand_numbers + #341, #7540
		static rand_numbers + #342, #6001
		static rand_numbers + #343, #8658
		static rand_numbers + #344, #9448
		static rand_numbers + #345, #7734
		static rand_numbers + #346, #9615
		static rand_numbers + #347, #3913
		static rand_numbers + #348, #4104
		static rand_numbers + #349, #2982
		static rand_numbers + #350, #260
		static rand_numbers + #351, #23
		static rand_numbers + #352, #3103
		static rand_numbers + #353, #4627
		static rand_numbers + #354, #9588
		static rand_numbers + #355, #6219
		static rand_numbers + #356, #2357
		static rand_numbers + #357, #6424
		static rand_numbers + #358, #8188
		static rand_numbers + #359, #1895
		static rand_numbers + #360, #6863
		static rand_numbers + #361, #10405
		static rand_numbers + #362, #6433
		static rand_numbers + #363, #6878
		static rand_numbers + #364, #10317
		static rand_numbers + #365, #8323
		static rand_numbers + #366, #7897
		static rand_numbers + #367, #1111
		static rand_numbers + #368, #2419
		static rand_numbers + #369, #7404
		static rand_numbers + #370, #8850
		static rand_numbers + #371, #7315
		static rand_numbers + #372, #6369
		static rand_numbers + #373, #6275
		static rand_numbers + #374, #4987
		static rand_numbers + #375, #7241
		static rand_numbers + #376, #3022
		static rand_numbers + #377, #3615
		static rand_numbers + #378, #2578
		static rand_numbers + #379, #9538
		static rand_numbers + #380, #9008
		static rand_numbers + #381, #2838
		static rand_numbers + #382, #985
		static rand_numbers + #383, #1125
		static rand_numbers + #384, #9877
		static rand_numbers + #385, #10573
		static rand_numbers + #386, #7344
		static rand_numbers + #387, #1247
		static rand_numbers + #388, #6010
		static rand_numbers + #389, #6957
		static rand_numbers + #390, #5554
		static rand_numbers + #391, #4298
		static rand_numbers + #392, #6375
		static rand_numbers + #393, #3411
		static rand_numbers + #394, #2600
		static rand_numbers + #395, #8117
		static rand_numbers + #396, #747
		static rand_numbers + #397, #10497
		static rand_numbers + #398, #652
		static rand_numbers + #399, #3166
		static rand_numbers + #400, #9326
		static rand_numbers + #401, #927
		static rand_numbers + #402, #10482
		static rand_numbers + #403, #7119
		static rand_numbers + #404, #7202
		static rand_numbers + #405, #6893
		static rand_numbers + #406, #3373
		static rand_numbers + #407, #1649
		static rand_numbers + #408, #1932
		static rand_numbers + #409, #5951
		static rand_numbers + #410, #200
		static rand_numbers + #411, #10940
		static rand_numbers + #412, #214
		static rand_numbers + #413, #1185
		static rand_numbers + #414, #3489
		static rand_numbers + #415, #10091
		static rand_numbers + #416, #3182
		static rand_numbers + #417, #2258
		static rand_numbers + #418, #2762
		static rand_numbers + #419, #617
		static rand_numbers + #420, #9215
		static rand_numbers + #421, #10727
		static rand_numbers + #422, #4915
		static rand_numbers + #423, #4603
		static rand_numbers + #424, #3152
		static rand_numbers + #425, #9926
		static rand_numbers + #426, #4144
		static rand_numbers + #427, #6310
		static rand_numbers + #428, #861
		static rand_numbers + #429, #4797
		static rand_numbers + #430, #9477
		static rand_numbers + #431, #1611
		static rand_numbers + #432, #5724
		static rand_numbers + #433, #396
		static rand_numbers + #434, #8730
		static rand_numbers + #435, #1939
		static rand_numbers + #436, #9700
		static rand_numbers + #437, #3527
		static rand_numbers + #438, #5999
		static rand_numbers + #439, #645
		static rand_numbers + #440, #9478
		static rand_numbers + #441, #6199
		static rand_numbers + #442, #598
		static rand_numbers + #443, #1116
		static rand_numbers + #444, #9795
		static rand_numbers + #445, #6499
		static rand_numbers + #446, #220
		static rand_numbers + #447, #1991
		static rand_numbers + #448, #8757
		static rand_numbers + #449, #5394
		static rand_numbers + #450, #2608
		static rand_numbers + #451, #6985
		static rand_numbers + #452, #5134
		static rand_numbers + #453, #9934
		static rand_numbers + #454, #3012
		static rand_numbers + #455, #8286
		static rand_numbers + #456, #297
		static rand_numbers + #457, #7157
		static rand_numbers + #458, #3610
		static rand_numbers + #459, #1158
		static rand_numbers + #460, #967
		static rand_numbers + #461, #4511
		static rand_numbers + #462, #2769
		static rand_numbers + #463, #6691
		static rand_numbers + #464, #7318
		static rand_numbers + #465, #2923
		static rand_numbers + #466, #54
		static rand_numbers + #467, #6031
		static rand_numbers + #468, #6450
		static rand_numbers + #469, #6054
		static rand_numbers + #470, #6676
		static rand_numbers + #471, #7353
		static rand_numbers + #472, #3677
		static rand_numbers + #473, #9685
		static rand_numbers + #474, #8469
		static rand_numbers + #475, #4897
		static rand_numbers + #476, #5197
		static rand_numbers + #477, #114
		static rand_numbers + #478, #6888
		static rand_numbers + #479, #5378
		static rand_numbers + #480, #5508
		static rand_numbers + #481, #920
		static rand_numbers + #482, #1376
		static rand_numbers + #483, #2066
		static rand_numbers + #484, #2278
		static rand_numbers + #485, #4389
		static rand_numbers + #486, #10353
		static rand_numbers + #487, #2575
		static rand_numbers + #488, #2970
		static rand_numbers + #489, #5387
		static rand_numbers + #490, #6145
		static rand_numbers + #491, #3937
		static rand_numbers + #492, #1322
		static rand_numbers + #493, #8914
		static rand_numbers + #494, #2052
		static rand_numbers + #495, #8640
		static rand_numbers + #496, #3262
		static rand_numbers + #497, #4517
		static rand_numbers + #498, #6095
		static rand_numbers + #499, #9712
		static rand_numbers + #500, #1995
		static rand_numbers + #501, #4195
		static rand_numbers + #502, #8489
		static rand_numbers + #503, #8084
		static rand_numbers + #504, #2893
		static rand_numbers + #505, #5972
		static rand_numbers + #506, #1994
		static rand_numbers + #507, #10502
		static rand_numbers + #508, #8497
		static rand_numbers + #509, #306
		static rand_numbers + #510, #4893
		static rand_numbers + #511, #3018
		static rand_numbers + #512, #3637
		static rand_numbers + #513, #8681
		static rand_numbers + #514, #5084
		static rand_numbers + #515, #5915
		static rand_numbers + #516, #4494
		static rand_numbers + #517, #6861
		static rand_numbers + #518, #10901
		static rand_numbers + #519, #7464
		static rand_numbers + #520, #3672
		static rand_numbers + #521, #6059
		static rand_numbers + #522, #2825
		static rand_numbers + #523, #7405
		static rand_numbers + #524, #3987
		static rand_numbers + #525, #4877
		static rand_numbers + #526, #7469
		static rand_numbers + #527, #7249
		static rand_numbers + #528, #818
		static rand_numbers + #529, #2577
		static rand_numbers + #530, #8385
		static rand_numbers + #531, #2814
		static rand_numbers + #532, #6772
		static rand_numbers + #533, #5888
		static rand_numbers + #534, #10898
		static rand_numbers + #535, #1090
		static rand_numbers + #536, #3284
		static rand_numbers + #537, #4316
		static rand_numbers + #538, #3016
		static rand_numbers + #539, #794
		static rand_numbers + #540, #4622
		static rand_numbers + #541, #7909
		static rand_numbers + #542, #6223
		static rand_numbers + #543, #8259
		static rand_numbers + #544, #8014
		static rand_numbers + #545, #2731
		static rand_numbers + #546, #3187
		static rand_numbers + #547, #1521
		static rand_numbers + #548, #9593
		static rand_numbers + #549, #3101
		static rand_numbers + #550, #409
		static rand_numbers + #551, #4689
		static rand_numbers + #552, #585
		static rand_numbers + #553, #3234
		static rand_numbers + #554, #1108
		static rand_numbers + #555, #6983
		static rand_numbers + #556, #10522
		static rand_numbers + #557, #8577
		static rand_numbers + #558, #3245
		static rand_numbers + #559, #354
		static rand_numbers + #560, #2579
		static rand_numbers + #561, #3054
		static rand_numbers + #562, #5579
		static rand_numbers + #563, #9351
		static rand_numbers + #564, #8942
		static rand_numbers + #565, #7901
		static rand_numbers + #566, #1865
		static rand_numbers + #567, #3650
		static rand_numbers + #568, #1230
		static rand_numbers + #569, #4881
		static rand_numbers + #570, #4444
		static rand_numbers + #571, #5852
		static rand_numbers + #572, #4215
		static rand_numbers + #573, #10667
		static rand_numbers + #574, #3124
		static rand_numbers + #575, #1242
		static rand_numbers + #576, #2412
		static rand_numbers + #577, #8722
		static rand_numbers + #578, #2764
		static rand_numbers + #579, #3429
		static rand_numbers + #580, #3247
		static rand_numbers + #581, #3173
		static rand_numbers + #582, #10529
		static rand_numbers + #583, #3832
		static rand_numbers + #584, #8819
		static rand_numbers + #585, #3061
		static rand_numbers + #586, #10815
		static rand_numbers + #587, #8354
		static rand_numbers + #588, #3063
		static rand_numbers + #589, #5484
		static rand_numbers + #590, #132
		static rand_numbers + #591, #5642
		static rand_numbers + #592, #10950
		static rand_numbers + #593, #5711
		static rand_numbers + #594, #6417
		static rand_numbers + #595, #329
		static rand_numbers + #596, #2625
		static rand_numbers + #597, #8283
		static rand_numbers + #598, #3980
		static rand_numbers + #599, #3855
		static rand_numbers + #600, #2177
		static rand_numbers + #601, #8424
		static rand_numbers + #602, #1131
		static rand_numbers + #603, #6392
		static rand_numbers + #604, #10516
		static rand_numbers + #605, #6666
		static rand_numbers + #606, #10046
		static rand_numbers + #607, #4352
		static rand_numbers + #608, #4401
		static rand_numbers + #609, #4234
		static rand_numbers + #610, #10192
		static rand_numbers + #611, #10060
		static rand_numbers + #612, #7407
		static rand_numbers + #613, #9734
		static rand_numbers + #614, #5316
		static rand_numbers + #615, #7650
		static rand_numbers + #616, #1809
		static rand_numbers + #617, #7556
		static rand_numbers + #618, #5018
		static rand_numbers + #619, #7283
		static rand_numbers + #620, #4464
		static rand_numbers + #621, #5150
		static rand_numbers + #622, #1938
		static rand_numbers + #623, #4427
		static rand_numbers + #624, #2286
		static rand_numbers + #625, #10766
		static rand_numbers + #626, #4757
		static rand_numbers + #627, #7322
		static rand_numbers + #628, #10473
		static rand_numbers + #629, #161
		static rand_numbers + #630, #2602
		static rand_numbers + #631, #1664
		static rand_numbers + #632, #8585
		static rand_numbers + #633, #3733
		static rand_numbers + #634, #10467
		static rand_numbers + #635, #10525
		static rand_numbers + #636, #10400
		static rand_numbers + #637, #950
		static rand_numbers + #638, #3890
		static rand_numbers + #639, #6225
		static rand_numbers + #640, #5184
		static rand_numbers + #641, #5506
		static rand_numbers + #642, #7709
		static rand_numbers + #643, #1605
		static rand_numbers + #644, #4254
		static rand_numbers + #645, #2039
		static rand_numbers + #646, #679
		static rand_numbers + #647, #8474
		static rand_numbers + #648, #9595
		static rand_numbers + #649, #5697
		static rand_numbers + #650, #4770
		static rand_numbers + #651, #3072
		static rand_numbers + #652, #2272
		static rand_numbers + #653, #9119
		static rand_numbers + #654, #9911
		static rand_numbers + #655, #6969
		static rand_numbers + #656, #322
		static rand_numbers + #657, #3681
		static rand_numbers + #658, #3304
		static rand_numbers + #659, #10796
		static rand_numbers + #660, #6253
		static rand_numbers + #661, #5906
		static rand_numbers + #662, #3884
		static rand_numbers + #663, #6262
		static rand_numbers + #664, #1064
		static rand_numbers + #665, #5775
		static rand_numbers + #666, #5801
		static rand_numbers + #667, #477
		static rand_numbers + #668, #6726
		static rand_numbers + #669, #1115
		static rand_numbers + #670, #9113
		static rand_numbers + #671, #923
		static rand_numbers + #672, #6622
		static rand_numbers + #673, #5836
		static rand_numbers + #674, #4939
		static rand_numbers + #675, #2300
		static rand_numbers + #676, #10286
		static rand_numbers + #677, #5619
		static rand_numbers + #678, #2198
		static rand_numbers + #679, #318
		static rand_numbers + #680, #329
		static rand_numbers + #681, #9379
		static rand_numbers + #682, #5801
		static rand_numbers + #683, #5012
		static rand_numbers + #684, #7511
		static rand_numbers + #685, #4725
		static rand_numbers + #686, #994
		static rand_numbers + #687, #10244
		static rand_numbers + #688, #10817
		static rand_numbers + #689, #4299
		static rand_numbers + #690, #10053
		static rand_numbers + #691, #6083
		static rand_numbers + #692, #1629
		static rand_numbers + #693, #2950
		static rand_numbers + #694, #1359
		static rand_numbers + #695, #2693
		static rand_numbers + #696, #8726
		static rand_numbers + #697, #9571
		static rand_numbers + #698, #5581
		static rand_numbers + #699, #6876
		static rand_numbers + #700, #2110
		static rand_numbers + #701, #6119
		static rand_numbers + #702, #7799
		static rand_numbers + #703, #8732
		static rand_numbers + #704, #3379
		static rand_numbers + #705, #1752
		static rand_numbers + #706, #45
		static rand_numbers + #707, #2678
		static rand_numbers + #708, #7371
		static rand_numbers + #709, #4654
		static rand_numbers + #710, #5407
		static rand_numbers + #711, #10111
		static rand_numbers + #712, #3046
		static rand_numbers + #713, #221
		static rand_numbers + #714, #4137
		static rand_numbers + #715, #10557
		static rand_numbers + #716, #7358
		static rand_numbers + #717, #7542
		static rand_numbers + #718, #1239
		static rand_numbers + #719, #9599
		static rand_numbers + #720, #3265
		static rand_numbers + #721, #305
		static rand_numbers + #722, #7107
		static rand_numbers + #723, #4895
		static rand_numbers + #724, #5667
		static rand_numbers + #725, #10877
		static rand_numbers + #726, #9999
		static rand_numbers + #727, #5817
		static rand_numbers + #728, #9461
		static rand_numbers + #729, #4594
		static rand_numbers + #730, #1706
		static rand_numbers + #731, #584
		static rand_numbers + #732, #10713
		static rand_numbers + #733, #9505
		static rand_numbers + #734, #741
		static rand_numbers + #735, #3105
		static rand_numbers + #736, #270
		static rand_numbers + #737, #3197
		static rand_numbers + #738, #8194
		static rand_numbers + #739, #10052
		static rand_numbers + #740, #7852
		static rand_numbers + #741, #2614
		static rand_numbers + #742, #601
		static rand_numbers + #743, #10898
		static rand_numbers + #744, #5246
		static rand_numbers + #745, #7149
		static rand_numbers + #746, #1893
		static rand_numbers + #747, #4028
		static rand_numbers + #748, #3704
		static rand_numbers + #749, #3132
		static rand_numbers + #750, #2641
		static rand_numbers + #751, #6970
		static rand_numbers + #752, #5848
		static rand_numbers + #753, #1172
		static rand_numbers + #754, #3289
		static rand_numbers + #755, #2939
		static rand_numbers + #756, #1062
		static rand_numbers + #757, #2301
		static rand_numbers + #758, #8756
		static rand_numbers + #759, #1947
		static rand_numbers + #760, #6895
		static rand_numbers + #761, #10462
		static rand_numbers + #762, #4942
		static rand_numbers + #763, #9032
		static rand_numbers + #764, #405
		static rand_numbers + #765, #8094
		static rand_numbers + #766, #3561
		static rand_numbers + #767, #3086
		static rand_numbers + #768, #305
		static rand_numbers + #769, #3179
		static rand_numbers + #770, #2152
		static rand_numbers + #771, #10568
		static rand_numbers + #772, #5793
		static rand_numbers + #773, #2753
		static rand_numbers + #774, #1903
		static rand_numbers + #775, #2464
		static rand_numbers + #776, #1326
		static rand_numbers + #777, #3796
		static rand_numbers + #778, #8903
		static rand_numbers + #779, #5030
		static rand_numbers + #780, #9339
		static rand_numbers + #781, #2968
		static rand_numbers + #782, #3424
		static rand_numbers + #783, #6612
		static rand_numbers + #784, #4140
		static rand_numbers + #785, #6713
		static rand_numbers + #786, #9551
		static rand_numbers + #787, #7613
		static rand_numbers + #788, #9015
		static rand_numbers + #789, #7321
		static rand_numbers + #790, #9560
		static rand_numbers + #791, #7334
		static rand_numbers + #792, #9207
		static rand_numbers + #793, #5927
		static rand_numbers + #794, #7791
		static rand_numbers + #795, #9612
		static rand_numbers + #796, #3034
		static rand_numbers + #797, #365
		static rand_numbers + #798, #1712
		static rand_numbers + #799, #5750
		static rand_numbers + #800, #3545
		static rand_numbers + #801, #6275
		static rand_numbers + #802, #7742
		static rand_numbers + #803, #762
		static rand_numbers + #804, #452
		static rand_numbers + #805, #9646
		static rand_numbers + #806, #5637
		static rand_numbers + #807, #1778
		static rand_numbers + #808, #4866
		static rand_numbers + #809, #3554
		static rand_numbers + #810, #6808
		static rand_numbers + #811, #5630
		static rand_numbers + #812, #6522
		static rand_numbers + #813, #1657
		static rand_numbers + #814, #1255
		static rand_numbers + #815, #2087
		static rand_numbers + #816, #8370
		static rand_numbers + #817, #10806
		static rand_numbers + #818, #9700
		static rand_numbers + #819, #8809
		static rand_numbers + #820, #9551
		static rand_numbers + #821, #10685
		static rand_numbers + #822, #7568
		static rand_numbers + #823, #7772
		static rand_numbers + #824, #5625
		static rand_numbers + #825, #4372
		static rand_numbers + #826, #8808
		static rand_numbers + #827, #83
		static rand_numbers + #828, #4737
		static rand_numbers + #829, #10520
		static rand_numbers + #830, #5834
		static rand_numbers + #831, #10693
		static rand_numbers + #832, #8219
		static rand_numbers + #833, #2589
		static rand_numbers + #834, #469
		static rand_numbers + #835, #8671
		static rand_numbers + #836, #3659
		static rand_numbers + #837, #8517
		static rand_numbers + #838, #10449
		static rand_numbers + #839, #8526
		static rand_numbers + #840, #1084
		static rand_numbers + #841, #8682
		static rand_numbers + #842, #3169
		static rand_numbers + #843, #10018
		static rand_numbers + #844, #10339
		static rand_numbers + #845, #6835
		static rand_numbers + #846, #1118
		static rand_numbers + #847, #10133
		static rand_numbers + #848, #9065
		static rand_numbers + #849, #10818
		static rand_numbers + #850, #10367
		static rand_numbers + #851, #7630
		static rand_numbers + #852, #10516
		static rand_numbers + #853, #6948
		static rand_numbers + #854, #6826
		static rand_numbers + #855, #7565
		static rand_numbers + #856, #333
		static rand_numbers + #857, #4647
		static rand_numbers + #858, #7649
		static rand_numbers + #859, #5070
		static rand_numbers + #860, #6592
		static rand_numbers + #861, #4907
		static rand_numbers + #862, #7188
		static rand_numbers + #863, #3824
		static rand_numbers + #864, #9907
		static rand_numbers + #865, #10068
		static rand_numbers + #866, #3920
		static rand_numbers + #867, #2580
		static rand_numbers + #868, #7598
		static rand_numbers + #869, #5793
		static rand_numbers + #870, #2530
		static rand_numbers + #871, #8683
		static rand_numbers + #872, #3488
		static rand_numbers + #873, #5699
		static rand_numbers + #874, #10125
		static rand_numbers + #875, #5251
		static rand_numbers + #876, #3958
		static rand_numbers + #877, #256
		static rand_numbers + #878, #6809
		static rand_numbers + #879, #2036
		static rand_numbers + #880, #87
		static rand_numbers + #881, #6189
		static rand_numbers + #882, #9666
		static rand_numbers + #883, #2028
		static rand_numbers + #884, #2150
		static rand_numbers + #885, #7916
		static rand_numbers + #886, #9593
		static rand_numbers + #887, #4894
		static rand_numbers + #888, #3988
		static rand_numbers + #889, #8666
		static rand_numbers + #890, #1388
		static rand_numbers + #891, #2004
		static rand_numbers + #892, #2586
		static rand_numbers + #893, #8576
		static rand_numbers + #894, #8239
		static rand_numbers + #895, #1507
		static rand_numbers + #896, #7657
		static rand_numbers + #897, #1172
		static rand_numbers + #898, #6498
		static rand_numbers + #899, #4269
		static rand_numbers + #900, #6966
		static rand_numbers + #901, #9028
		static rand_numbers + #902, #4376
		static rand_numbers + #903, #1878
		static rand_numbers + #904, #6151
		static rand_numbers + #905, #3514
		static rand_numbers + #906, #9541
		static rand_numbers + #907, #1533
		static rand_numbers + #908, #6181
		static rand_numbers + #909, #7774
		static rand_numbers + #910, #3569
		static rand_numbers + #911, #8679
		static rand_numbers + #912, #5387
		static rand_numbers + #913, #4660
		static rand_numbers + #914, #10707
		static rand_numbers + #915, #7537
		static rand_numbers + #916, #1589
		static rand_numbers + #917, #738
		static rand_numbers + #918, #1444
		static rand_numbers + #919, #7988
		static rand_numbers + #920, #9404
		static rand_numbers + #921, #2832
		static rand_numbers + #922, #9992
		static rand_numbers + #923, #3415
		static rand_numbers + #924, #2833
		static rand_numbers + #925, #7245
		static rand_numbers + #926, #7333
		static rand_numbers + #927, #10490
		static rand_numbers + #928, #10828
		static rand_numbers + #929, #5255
		static rand_numbers + #930, #6183
		static rand_numbers + #931, #9218
		static rand_numbers + #932, #5707
		static rand_numbers + #933, #1983
		static rand_numbers + #934, #2521
		static rand_numbers + #935, #3282
		static rand_numbers + #936, #7908
		static rand_numbers + #937, #3486
		static rand_numbers + #938, #4815
		static rand_numbers + #939, #3102
		static rand_numbers + #940, #273
		static rand_numbers + #941, #8384
		static rand_numbers + #942, #795
		static rand_numbers + #943, #5660
		static rand_numbers + #944, #2057
		static rand_numbers + #945, #2926
		static rand_numbers + #946, #4621
		static rand_numbers + #947, #6058
		static rand_numbers + #948, #6075
		static rand_numbers + #949, #8476
		static rand_numbers + #950, #5470
		static rand_numbers + #951, #6904
		static rand_numbers + #952, #2732
		static rand_numbers + #953, #4476
		static rand_numbers + #954, #1743
		static rand_numbers + #955, #5565
		static rand_numbers + #956, #3145
		static rand_numbers + #957, #9076
		static rand_numbers + #958, #7480
		static rand_numbers + #959, #5397
		static rand_numbers + #960, #3344
		static rand_numbers + #961, #2676
		static rand_numbers + #962, #3629
		static rand_numbers + #963, #475
		static rand_numbers + #964, #4660
		static rand_numbers + #965, #8561
		static rand_numbers + #966, #3757
		static rand_numbers + #967, #3992
		static rand_numbers + #968, #1060
		static rand_numbers + #969, #8572
		static rand_numbers + #970, #7095
		static rand_numbers + #971, #3744
		static rand_numbers + #972, #8380
		static rand_numbers + #973, #10301
		static rand_numbers + #974, #828
		static rand_numbers + #975, #1862
		static rand_numbers + #976, #4651
		static rand_numbers + #977, #5449
		static rand_numbers + #978, #10331
		static rand_numbers + #979, #10727
		static rand_numbers + #980, #5349
		static rand_numbers + #981, #4814
		static rand_numbers + #982, #9055
		static rand_numbers + #983, #8081
		static rand_numbers + #984, #714
		static rand_numbers + #985, #10798
		static rand_numbers + #986, #5071
		static rand_numbers + #987, #3859
		static rand_numbers + #988, #311
		static rand_numbers + #989, #3975
		static rand_numbers + #990, #681
		static rand_numbers + #991, #6066
		static rand_numbers + #992, #9062
		static rand_numbers + #993, #6721
		static rand_numbers + #994, #6541
		static rand_numbers + #995, #5146
		static rand_numbers + #996, #4295
		static rand_numbers + #997, #10298
		static rand_numbers + #998, #9139
		static rand_numbers + #999, #7766
		static rand_numbers + #1000, #10294
		static rand_numbers + #1001, #5247
		static rand_numbers + #1002, #523
		static rand_numbers + #1003, #7687
		static rand_numbers + #1004, #6972
		static rand_numbers + #1005, #1351
		static rand_numbers + #1006, #973
		static rand_numbers + #1007, #636
		static rand_numbers + #1008, #9211
		static rand_numbers + #1009, #317
		static rand_numbers + #1010, #2787
		static rand_numbers + #1011, #3573
		static rand_numbers + #1012, #5132

; ===================================================================
;                           Main function
; ===================================================================
main:
	; Inicializa valores que nao se alteram a cada jogo (os records)
	; --------------------------------------------------------------
	call initialize

	; Comeca um jogo
	; --------------
	start:
		; Valores para comparar com as flags
		; ----------------------------------
		loadn r0, #0
		loadn r1, #1

		; Comeca o jogo
		; -------------
		call new

		; Loop do jogo
		; ------------
		loop:
			; Funcoes do tank
			; ---------------
			call tank_action ; pega acao do tank
			call tank_clear  ; apaga o tank
			call tank_draw   ; redesenha o tank

			; Funcoes dos tiros
			; -----------------
			call shots_clear ; apaga os tiros
			call shots_draw  ; redesenha os tiros

			; Funcoes do booster
			; ------------------
			call booster_draw
			call booster_check

			; Funcoes do timer
			; ----------------
			call timer_draw
			call timer_check

			; Funcoes dos mobs
			; ----------------
			call mobs_rand   ; cria mobs
			call mobs_clear  ; apaga os mobs
			call mobs_draw   ; redesenha os mobs
			call mobs_check  ; confere colisoes com os tiros e com o tank

			; Funcoes do jogo
			; ---------------
			call dec_time     ; decrementa o tempo
			call inc_score    ; incrementa os pontos
			call dec_life     ; decrementa a vida
			call show_message ; mostra uma mensagem no topo da tela
			call check_end    ; confere se o jogo acabou
			call delay        ; delay padrao

			; Confere flag de fim
			; -------------------
			load r7, game_over
			cmp  r7, r0
			jeq loop ; se nao acabou, volta para o loop do jogo

			cmp r7, r1
			jne game_over_lose

			; Game over (enter name)
			; ----------------------
				call clear

				; Titulo
				; ------
				loadn r0, #129
				call render_title

				loadn r0, #msg_time_out
				loadn r1, #400
				call printstr

				loadn r0, #msg_press_enter
				loadn r1, #840
				call printstr

				call getenter

				call clear

				; Titulo
				; ------
				loadn r0, #129
				call render_title

				; Texto
				; -----
				loadn r0, #text_game_over_win_00
				loadn r1, #400
				loadn r2, #5
				call printblock

				; Score e Life
				; ------------
				call render_score_life

				; Insere nome e salva record
				; --------------------------
				call records_update

			; Game over (win)
			; ----------------
				call clear

				; Titulo
				; ------
				loadn r0, #129
				call render_title

				; Texto
				; -----
				loadn r0, #text_game_over_win_00
				loadn r1, #400
				loadn r2, #5
				call printblock

				loadn r0, #text_great_00
				loadn r1, #800
				loadn r2, #3
				call printblock

				loadn r0, #menu_play_again
				loadn r1, #1040
				call printstr

				call change_seed

				jmp play_again_loop

			; Game over (lose)
			; ----------------
			game_over_lose:
				call clear

				; Titulo
				; ------
				loadn r0, #129
				call render_title

				; Texto
				; -----
				loadn r0, #text_game_over_lose_00
				loadn r1, #400
				loadn r2, #5
				call printblock

				loadn r0, #text_try_again_00
				loadn r1, #800
				loadn r2, #3
				call printblock

				loadn r0, #menu_play_again
				loadn r1, #1040
				call printstr

				call change_seed

			; Reinicializa / Finaliza o jogo
			; ------------------------------
			play_again_loop:
				call getchar  ; pega resposta
				load r0, char

				; Case 'y' or 'Y' : goto start
				loadn r1, #'y'
				cmp r0, r1
				jeq start
				loadn r1, #'Y'
				cmp r0, r1
				jeq start

				; Case 'n' or 'N' : end
				loadn r1, #'n'
				cmp r0, r1
				jeq end
				loadn r1, #'N'
				cmp r0, r1
				jeq end

				jmp play_again_loop

				; The End
				; -------
				end:
					call clear

					; Titulo
					; ------
					loadn r0, #129
					call render_title

					; Mensagem
					; --------
					loadn r0, #msg_the_end
					loadn r1, #440
					call printstr

					halt

; ===================================================================
;                           Game functions
; ===================================================================
clear:
	push r0
	push r1

	loadn r0, #1200 ; quantidade de posicoes na tela
	loadn r1, #' '  ; caractere

	clear_loop:
		dec r0
		outchar r1, r0
		jnz clear_loop

	pop r1
	pop r0
	rts

splash:
	push r0
	push r1

	loadn r0, #370
	call render_booster

	loadn r0, #543
	call render_timer

	loadn r0, #129
	call render_title

	loadn r0, #295
	loadn r1, #'+'
	call render_mob

	loadn r0, #387
	loadn r1, #'@'
	call render_mob

	loadn r0, #526
	loadn r1, #'+'
	call render_mob

	loadn r0, #737
	call render_tank

	loadn r0, #456
	call render_shot

	loadn r0, #577
	call render_shot

	loadn r0, #menu_start
	loadn r1, #920
	call printstr

	loadn r0, #menu_records
	loadn r1, #1000
	call printstr

	loadn r0, #menu_help
	loadn r1, #1080
	call printstr

	loadn r0, #msg_version
	loadn r1, #1160
	call printstr

	pop r1
	pop r0
	rts

initialize:
	push r0
	push r1

	loadn r0, #757
	store seed, r0

	loadn r0, #records_names

	loadn r1, #records_name_0
	storei r0, r1
	inc r0

	loadn r1, #records_name_1
	storei r0, r1
	inc r0

	loadn r1, #records_name_2
	storei r0, r1
	inc r0

	loadn r1, #records_name_3
	storei r0, r1
	inc r0

	loadn r1, #records_name_4
	storei r0, r1
	inc r0

	loadn r1, #records_name_5
	storei r0, r1
	inc r0

	loadn r1, #records_name_6
	storei r0, r1
	inc r0

	loadn r1, #records_name_7
	storei r0, r1

	pop r1
	pop r0
	rts

change_seed:
	push r0 ; seed
	push r1 ; rand
	push r2 ; score
	push r3 ; life
	push r4 ; time
	push r5 ; rand_number
	push r6

	loadn r6, #1789

	load r0, seed
	mod r0, r0, r6

	load r1, rand
	mod r1, r1, r6

	load r2, score
	load r3, life
	load r4, time

	loadn r6, #1013
	mod r6, r1, r6
	loadn r5, #rand_numbers
	add r5, r5, r6
	loadi r5, r5

	add r0, r0, r1
	add r0, r0, r2
	add r0, r0, r3
	add r0, r0, r4
	add r0, r0, r5
	store seed, r0

	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	rts

new:
	push r0
	push r1
	push r2
	push r3

	; Reseta valores
	new_reset_shots:
		loadn r0, #0
		loadn r1, #0
		loadn r2, #9

		new_reset_shots_loop:
			cmp r1, r2
			jgr new_reset_mobs

			loadn r3, #shots_pre_pos
			add r3, r3, r1
			storei r3, r0

			loadn r3, #shots_cur_pos
			add r3, r3, r1
			storei r3, r0

			inc r1
			jmp new_reset_shots_loop

	new_reset_mobs:
		loadn r0, #0
		loadn r1, #0
		loadn r2, #19

		new_reset_mobs_loop:
			cmp r1, r2
			jgr new_reset_vars

			loadn r3, #mobs_pre_pos_1
			add r3, r3, r1
			storei r3, r0

			loadn r3, #mobs_cur_pos_1
			add r3, r3, r1
			storei r3, r0

			loadn r3, #mobs_damage_1
			add r3, r3, r1
			storei r3, r0

			loadn r3, #mobs_pre_pos_2
			add r3, r3, r1
			storei r3, r0

			loadn r3, #mobs_cur_pos_2
			add r3, r3, r1
			storei r3, r0

			loadn r3, #mobs_damage_2
			add r3, r3, r1
			storei r3, r0

			inc r1
			jmp new_reset_mobs_loop

	new_reset_vars:
		loadn r0, #90
		store time, r0

		loadn r0, #3
		store life,       r0
		store kill_mob_1, r0

		loadn r0, #5
		store kill_mob_2, r0

		loadn r0, #6
		store score_mob_1, r0

		loadn r0, #10
		store score_mob_2, r0

		loadn r0, #0
		store char,            r0
		store score,           r0
		store timer,           r0
		store timer_block,     r0
		store booster,         r0
		store booster_block,   r0
		store booster_fire,    r0
		store booster_cur_pos, r0
		store msg,             r0
		store hit,             r0
		store damage,          r0
		store game_over,       r0

		store sec,        r0
		store mobs_speed, r0
		store mobs_time,  r0
		store shots,      r0
		store mobs_1,     r0
		store mobs_2,     r0
		store msg_swap,   r0

		load r0, seed
		store rand, r0

		loadn r0, #779
		store tank_cur_pos, r0

	new_home:
		call clear
		call splash

		loadn r2, #0
		loadn r3, #1789
		new_home_loop:
			; Pega um tecla
			call inputchar
			load r0, char

			; Case 's' or 'S' : start!
			loadn r1, #'s'
			cmp r0, r1
			jeq new_start
			loadn r1, #'S'
			cmp r0, r1
			jeq new_start

			; Case 'h' or 'H' : show help
			loadn r1, #'h'
			cmp r0, r1
			jeq new_help
			loadn r1, #'H'
			cmp r0, r1
			jeq new_help

			; Case 'r' or 'R' : show records
			loadn r1, #'r'
			cmp r0, r1
			jeq new_records
			loadn r1, #'R'
			cmp r0, r1
			jeq new_records

			inc r2
			mod r2, r2, r3

			jmp new_home_loop

		new_start:
			load r0, rand
			add r0, r0, r2
			mod r0, r0, r3
			store rand, r0

			call clear

			; Desenha tela de jogo
			loadn r0, #80
			call render_game

			; Desenha as tentativas (vidas)
			load  r0, life
			loadn r1, #1090
			call printd

			; Desenha o tempo
			load  r0, time
			loadn r1, #1099
			call printd

			; Desenha o score
			load  r0, score
			loadn r1, #1111
			call printd

			loadn r0, #0
			store msg, r0

			; Desenha o tank e sai
			call tank_draw
			jmp new_exit

		new_help:
			call clear

			; Desenha titulo
			loadn r0, #129
			call render_title

			loadn r0, #text_help_keys_00
			loadn r1, #440
			loadn r2, #5
			call printblock

			loadn r0, #text_help_target_00
			loadn r1, #840
			loadn r2, #3
			call printblock

			loadn r2, #2048
			loadn r1, #325

			loadn r0, #'K'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'E'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'Y'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'S'
			add r0, r0, r2
			outchar r0, r1

			loadn r1, #725

			loadn r0, #'T'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'A'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'R'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'G'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'E'
			add r0, r0, r2
			outchar r0, r1
			inc r1

			loadn r0, #'T'
			add r0, r0, r2
			outchar r0, r1

			loadn r0, #menu_back
			loadn r1, #1080
			call printstr

			new_help_loop:
				call getchar
				load r0, char

				; Case 'b' or 'B' : back to home screen
				loadn r1, #'b'
				cmp r0, r1
				jeq new_home
				loadn r1, #'B'
				cmp r0, r1
				jeq new_home

				jmp new_help_loop

		new_records:
			call clear

			loadn r0, #129
			call render_title

			; Mostra tela de records
			call records_show

			new_records_loop:
				call getchar
				load r0, char

				; Case 'b' or 'B' : back to home screen
				loadn r1, #'b'
				cmp r0, r1
				jeq new_home
				loadn r1, #'B'
				cmp r0, r1
				jeq new_home

				jmp new_records_loop

	; Sai da rotina
	new_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		rts

dec_time:
	push r0
	push r1
	push r2
	push r3
	push r7

	; Confere valor do contador sec
	load  r0, sec
	inc r0
	store sec, r0

	loadn r1, #35  ; valor arbitrario para tentar definir uma unidade de tempo
	mod r7, r0, r1

	; Se o contador sec for divisivel pelo valor escolhido,
	; entao decrementa o time e imprime
	loadn r1, #0
	cmp r7, r1
	jne dec_time_exit

	load r0, time
	dec r0
	store time, r0

	; Limpa o numero do tempo anterior
	loadn r2, #' '
	loadn r1, #1101

	outchar r2, r1
	dec r1

	outchar r2, r1
	dec r1

	outchar r2, r1

	call printd

	loadn r3, #65
	cmp r0, r3
	jeq dec_time_booster

	loadn r3, #40
	cmp r0, r3
	jeq dec_time_timer

	loadn r3, #21
	cmp r0, r3
	jle dec_time_msg

	; Sai da rotina
	dec_time_exit:
		pop r7
		pop r3
		pop r2
		pop r1
		pop r0
		rts

	dec_time_msg:
		loadn r3, #5
		store msg, r3
		jmp dec_time_exit

	dec_time_timer:
		loadn r1, #1
		load r0, timer_block
		cmp r0, r1
		jeq dec_time_exit

		load r0, rand
		inc r0
		store rand, r0
		loadn r1, #1013
		mod r0, r0, r1

		loadn r1, #rand_numbers
		add r1, r1, r0
		loadi r0, r1

		loadn r1, #32
		mod r0, r0, r1
		loadn r1, #644
		add r0, r0, r1

		store timer_cur_pos, r0

		loadn r1, #1
		store timer, r1
		store timer_block, r1

		jmp dec_time_exit

	dec_time_booster:
		load r0, rand
		inc r0
		store rand, r0
		loadn r1, #1013
		mod r0, r0, r1

		loadn r1, #rand_numbers
		add r1, r1, r0
		loadi r0, r1

		loadn r1, #32
		mod r0, r0, r1
		loadn r1, #484
		add r0, r0, r1

		store booster_cur_pos, r0

		loadn r1, #1
		store booster, r1

		jmp dec_time_exit

inc_score:
	push r0
	push r1
	push r2
	push r3

	loadn r1, #0

	load r2, hit
	cmp r2, r1
	jeq inc_score_exit

	load r0, score
	add r0, r0, r2
	store score, r0

	store hit, r1

	loadn r1, #1115

	loadn r2, #' '
	outchar r2, r1
	dec r1

	outchar r2, r1
	dec r1

	outchar r2, r1
	dec r1

	outchar r2, r1
	dec r1

	call printd

	loadn r3, #600
	cmp r0, r3
	jgr inc_score_msg_2

	loadn r3, #500
	cmp r0, r3
	jgr inc_score_booster

	loadn r3, #200
	cmp r0, r3
	jgr inc_score_msg_1

	; Sai da rotina
	inc_score_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		rts

	inc_score_msg_1:
		loadn r3, #3
		store msg, r3
		jmp inc_score_exit

	inc_score_msg_2:
		loadn r3, #4
		store msg, r3
		jmp inc_score_exit

	inc_score_booster:
		loadn r1, #1
		load r0, booster_block
		cmp r0, r1
		jeq inc_score_exit

		load r0, rand
		inc r0
		store rand, r0
		loadn r1, #1013
		mod r0, r0, r1

		loadn r1, #rand_numbers
		add r1, r1, r0
		loadi r0, r1

		loadn r1, #32
		mod r0, r0, r1
		loadn r1, #324
		add r0, r0, r1

		store booster_cur_pos, r0

		loadn r1, #1
		store booster, r1
		store booster_block, r1

		jmp inc_score_exit


dec_life:
	push r0
	push r1
	push r2
	push r3

	loadn r1, #1
	load r0, damage
	cmp r0, r1
	jne dec_life_exit

	loadn r0, #2
	store damage, r0

	load r0, tank_cur_pos
	call render_tank_damage

	load r0, life
	dec r0
	store life, r0

	loadn r1, #1090
	loadn r2, #' '
	outchar r2, r1
	call printd

	loadn r3, #2
	cmp r0, r3
	jeq dec_life_msg_1

	loadn r3, #1
	cmp r0, r3
	jeq dec_life_msg_2

	dec_life_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		rts

	dec_life_msg_1:
		loadn r3, #1
		store msg, r3
		jmp dec_life_exit

	dec_life_msg_2:
		loadn r3, #2
		store msg, r3
		jmp dec_life_exit

records_update:
	push r0
	push r1
	push r2
	push r3
	push r4

	; Confere se o score eh maior que o do ultimo colocado
	; 	se sim, pega o nome, sobrescreve o ultimo, ordena.

	loadn r2, #7

	load  r0, score
	loadn r1, #records_scores
	add r3, r1, r2
	loadi r4, r3

	cmp r0, r4
	jel records_update_not_enough

	storei r3, r0

	load  r0, life
	loadn r1, #records_lifes
	add r1, r1, r2
	storei r1, r0

	loadn r0, #msg_enter_name
	loadn r1, #1040
	call printstr

	loadn r1, #1063
	loadn r4, #2048
	loadn r0, #'_'
	add r0, r0, r4
	outchar r0, r1
	inc r1
	inc r1
	outchar r0, r1
	inc r1
	inc r1
	outchar r0, r1

	loadn r0, #records_names
	add r0, r0, r2
	loadi r0, r0
	loadn r1, #3
	loadn r2, #1063
	call getstr

	call records_sort
	jmp records_update_exit

	records_update_not_enough:
		loadn r0, #msg_not_enough
		loadn r1, #1040
		call printstr

	records_update_exit:
		call getenter

		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

records_sort:
	push r0 ; vetor
	push r1 ; tamanho
	push r2 ; auxiliar
	push r3 ; auxiliar
	push r4 ; contador i
	push r5 ; contador j
	push r6 ; n-1 / auxiliar
	push r7 ; n-i / auxiliar

	loadn r0, #records_scores
	loadn r1, #8

	; i = 0
	loadn r4, #0
	; n-1
	mov r6, r1
	dec r6

	; loop1 (externo)
	records_sort_loop1:
	; i < n-1
	cmp r4, r6
	jeq records_sort_loop1_exit

		; j = 1
		loadn r5, #1
		; n-i
		sub r7, r1, r4

		; loop2 (interno)
		records_sort_loop2:
		; j < n-i
		cmp  r5, r7
		jeq  records_sort_loop2_exit

		; push em r6 e r7
		; para poder usa-los como auxiliar dentro deste loop
		push r6
		push r7

		; v[j-1] em r2
		; v+j-1  em r6
		push  r0
		mov   r2, r5
		dec   r2
		add   r0, r0, r2
		loadi r2, r0
		mov   r6, r0
		pop   r0

		; v[j] em r3
		; v+j  em r7
		push  r0
		mov   r3, r5
		add   r0, r0, r3
		loadi r3, r0
		mov   r7, r0
		pop   r0

		; if (v[j-1] < v[j])
		cmp r2, r3
		cle records_sort_swap

		; j++
		inc r5

		; pop dos auxiliares neste loop
		pop r7
		pop r6

		jmp records_sort_loop2

			; Sai do loop2 e volta para o loop1
			records_sort_loop2_exit:
				; i++
				inc r4
				jmp records_sort_loop1

			; Faz a troca dos valores
			; swap(j, j-1)
			records_sort_swap:
				push r0
				push r1
				push r2
				push r3
				push r5

				; Troca os scores
				storei r6, r3
				storei r7, r2

				; Trocas as vidas
				loadn r0, #records_lifes
				add r1, r0, r5
				dec r5
				add r0, r0, r5
				loadi r2, r0
				loadi r3, r1
				storei r1, r2
				storei r0, r3

				; Trocas as vidas
				loadn r0, #records_names
				add r1, r0, r5
				inc r5
				add r0, r0, r5
				loadi r2, r0
				loadi r3, r1
				storei r1, r2
				storei r0, r3

				pop r5
				pop r3
				pop r2
				pop r1
				pop r0
				rts

	; Sai do loop1, ou seja, fim do bubblesort
	records_sort_loop1_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

records_show:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	loadn r0, #280
	call render_records

	loadn r0, #menu_back
	loadn r1, #1080
	call printstr

	loadn r1, #988
	loadn r2, #8
	loadn r3, #63

	loadn r4, #records_names
	loadn r5, #records_scores
	loadn r6, #records_lifes

	loadn r7, #7
	add r4, r4, r7
	add r5, r5, r7
	add r6, r6, r7

	; Faz loop imprimindo dos dados
	records_show_loop:
		mov r0, r6
		loadi r0, r0
		dec r6
		loadn r7, #0
		cmp r0, r7
		jeq records_show_score
		call printd

		records_show_score:
			mov r0, r5
			loadi r0, r0
			dec r5
			loadn r7, #9
			sub r1, r1, r7
			loadn r7, #0
			cmp r0, r7
			jeq records_show_name
			call printd

		records_show_name:
			mov r0, r4
			loadi r0, r0
			dec r4
			loadn r7, #8
			sub r1, r1, r7
			call printstr

		sub r1, r1, r3
		dec r2
		jnz records_show_loop

	pop r7
	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	rts


check_end:
	push r0
	push r1

	loadn r0, #0
	load  r1, time
	cmp r1, r0
	jne check_end_life

	; if (time == 0) {
	; 	game_over = 1;
	; }
	loadn r1, #1
	store game_over, r1
	jmp check_end_exit

	; else if (life == 0) {
	; 	game_over = 2;
	; }
	check_end_life:
		load r1, life
		cmp r1, r0
		jne check_end_exit

		loadn r1, #2
		store game_over, r1

	check_end_exit:
		pop r1
		pop r0
		rts

delay:
	push r0
	push r1

	loadn r0, #220

	delay_loop1:
		loadn r1, #220

		delay_loop2:
			dec r1
			jnz delay_loop2
			dec r0
			jnz delay_loop1

	pop r1
	pop r0
	rts


show_message:
	push r0
	push r1
	push r2

	load  r0, msg_swap
	inc r0
	store msg_swap, r0
	loadn r1, #120 ; valor arbitrario
	mod r0, r0, r1
	jnz show_message_exit

	loadn r1, #40
	load r0, msg

	loadn r2, #0
	cmp r0, r2
	jeq show_message_0

	loadn r2, #1
	cmp r0, r2
	jeq show_message_1

	loadn r2, #2
	cmp r0, r2
	jeq show_message_2

	loadn r2, #3
	cmp r0, r2

	jeq show_message_3

	loadn r2, #4
	cmp r0, r2
	jeq show_message_4

	loadn r2, #5
	cmp r0, r2
	jeq show_message_5

	loadn r2, #6
	cmp r0, r2
	jeq show_message_6

	loadn r2, #7
	cmp r0, r2
	jeq show_message_7

	show_message_exit:
		pop r2
		pop r1
		pop r0
		rts

	show_message_0:
		loadn r0, #msg_go
		call printstr
		jmp show_message_exit

	show_message_1:
		loadn r0, #msg_damn
		call printstr
		jmp show_message_exit

	show_message_2:
		loadn r0, #msg_life
		call printstr
		jmp show_message_exit

	show_message_3:
		loadn r0, #msg_kill
		call printstr
		jmp show_message_exit

	show_message_4:
		loadn r0, #msg_fire
		call printstr
		jmp show_message_exit

	show_message_5:
		loadn r0, #msg_time
		call printstr
		jmp show_message_exit

	show_message_6:
		loadn r0, #msg_boost
		call printstr
		jmp show_message_exit

	show_message_7:
		loadn r0, #msg_timer
		call printstr
		jmp show_message_exit

; ===================================================================
;                           Basic functions
; ===================================================================
printd:
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

		; Se eh zero: terminou, entao manda para o loop que desempilha e imprime
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
		pop r1
		pop r0
		rts

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

printblock:
	push r0
	push r1
	push r2
	push r3

	loadn r3, #40 ; incrementador da posicao (quantidade de colunas)

	; Loop de impressao das linhas
	draw_loop:
		call printstr
		add r0, r0, r3
		inc r0
		add r1, r1, r3
		dec r2
		jnz draw_loop

	; Sai da rotina
	pop r3
	pop r2
	pop r1
	pop r0
	rts

inputchar:
	push r0
	push r1

	inchar r0
	store char, r0

	pop r1
	pop r0
	rts

getchar:
	push r0
	push r1

	loadn r1, #255

	getchar_loop:
		inchar r0
		cmp r0, r1
		jeq getchar_loop

	store char, r0
	pop r1
	pop r0
	rts

getenter:
	push r0
	push r1

	loadn r1, #13

	getenter_loop:
		inchar r0
		cmp r0, r1
		jne getenter_loop

	store char, r0
	pop r1
	pop r0
	rts

getstr:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6

	loadn r3, #0  ; contador
	loadn r4, #13 ; carectere ENTER
	loadn r5, #0  ; caractere nulo '\0'

	getstr_loop:
		call getchar
		load r6, char
		storei r0, r6
		cmp r6, r4
		jeq getstr_exit
		outchar r6, r2
		inc r2
		inc r2 ; incrementa r2 duas vezes, so um espaco a mais
		inc r0
		inc r3
		dec r1
		jnz getstr_loop

	getstr_exit:
		; Nao permite a insercao de uma palavra de tamanho zero
		cmp r3, r5
		jeq getstr_loop

		; Coloca caractere nulo '\0' no final da palavra
		storei r0, r5

		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

; ===================================================================
;                           Tank functions
; ===================================================================
tank_action:
	push r0
	push r1
	push r2
	push r3
	push r4

	; Recebe um char direto
	call inputchar
	load r0, char

	; Case 'w' or 'W' : move up
	loadn r1, #'w'
	cmp r0, r1
	jeq tank_action_up
	loadn r1, #'W'
	cmp r0, r1
	jeq tank_action_up

	; Case 'a' or 'A' : move left
	loadn r1, #'a'
	cmp r0, r1
	jeq tank_action_left
	loadn r1, #'A'
	cmp r0, r1
	jeq tank_action_left

	; Case 's' or 'S' : move down
	loadn r1, #'s'
	cmp r0, r1
	jeq tank_action_down
	loadn r1, #'S'
	cmp r0, r1
	jeq tank_action_down

	; Case 'd' or 'D' : move right
	loadn r1, #'d'
	cmp r0, r1
	jeq tank_action_right
	loadn r1, #'D'
	cmp r0, r1
	jeq tank_action_right

	; Case 'j' or 'k'; 'J' or 'K' : fire!
	loadn r1, #'j'
	cmp r0, r1
	jeq tank_action_fire
	loadn r1, #'k'
	cmp r0, r1
	jeq tank_action_fire
	loadn r1, #'J'
	cmp r0, r1
	jeq tank_action_fire
	loadn r1, #'K'
	cmp r0, r1
	jeq tank_action_fire

	; Case 13 : pause
	loadn r1, #13
	cmp r0, r1
	jeq tank_action_pause

	; Sai da rotina
	tank_action_exit:
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

	; Calcula posicao do tank para uma linha acima
	tank_action_up:
		; posicao_atual <- posicao_anterior - 40
		loadn r1, #40
		load  r0, tank_pre_pos
		sub r0, r0, r1

		; Se nao eh uma posicao valida, sai
		loadn r1, #200
		cmp r0, r1
		jle tank_action_exit

		; Seta nova posicao
		store tank_cur_pos, r0

		jmp tank_action_exit

	; Calcula posicao do tank um para esquerda
	tank_action_left:
		; posicao_atual <- posicao_anterior - 1
		load r0, tank_pre_pos
		dec r0

		; Se nao eh uma posicao valida, sai
		loadn r2, #40
		mod r1, r0, r2
		loadn r2, #3
		cmp r1, r2
		jel tank_action_exit

		; Seta nova posicao
		store tank_cur_pos, r0

		jmp tank_action_exit

	; Calcula posicao do tank para uma linha abaixo
	tank_action_down:
		; posicao_atual <- posicao_anterior + 40
		loadn r1, #40
		load  r0, tank_pre_pos
		add r0, r0, r1

		; Se nao eh uma posicao valida, sai
		loadn r1, #959
		cmp r0, r1
		jgr tank_action_exit

		; Seta nova posicao
		store tank_cur_pos, r0

		jmp tank_action_exit

	; Calcula posicao do tank um para direita
	tank_action_right:
		; posicao_atual <- posicao_anterior + 1
		load r0, tank_pre_pos
		inc r0

		; Se nao eh uma posicao valida, sai
		loadn r2, #40
		mod r1, r0, r2
		loadn r2, #36
		cmp r1, r2
		jeg tank_action_exit

		; Seta nova posicao
		store tank_cur_pos, r0

		jmp tank_action_exit

	; Define a posicao inicial de um tiro saindo do tank
	tank_action_fire:
		; Calcula a posicao do tiro em r0
		loadn r1, #120
		load r0, tank_cur_pos
		sub r0, r0, r1

		; Se nao eh uma posicao de tiro valida, sai
		loadn r1, #160
		cmp r0, r1
		jle tank_action_exit

		; Pega posicao do vetor de tiros
		load r2, shots
		inc r2
		store shots, r2
		loadn r1, #10
		mod r2, r2, r1

		; Define a posicao inicial deste tiro
		loadn r3, #shots_cur_pos
		add r3, r3, r2
		storei r3, r0

		loadn r2, #2
		load r0, booster
		cmp r0, r2
		jne tank_action_exit

		load r0, booster_fire
		dec r0
		store booster_fire, r0

		jmp tank_action_exit

	; Pausa o jogo
	tank_action_pause:
		; Imprime mensagem
		loadn r0, #msg_pause
		loadn r1, #40
		call printstr

		; Espera teclar ENTER para continuar
		call getenter

		; Imprime mensagem
		loadn r1, #119
		store msg_swap, r1
		call show_message

		jmp tank_action_exit

tank_clear:
	push r0
	push r1
	push r2

	; Confere se posicao foi alterada
	load r1, tank_cur_pos
	load r0, tank_pre_pos
	cmp r0, r1
	jeq tank_clear_exit

	; Limpa o tank
	call blank_tank

	tank_clear_exit:
		pop r2
		pop r1
		pop r0
		rts

tank_draw:
	push r0
	push r1

	; Confere se posicao foi alterada
	load r0, tank_cur_pos
	load r1, tank_pre_pos
	cmp r1, r0
	jeq tank_draw_exit

	; Redefine posicao anterior
	store tank_pre_pos, r0

	; Renderiza tank
	call render_tank

	tank_draw_exit:
		pop r1
		pop r0
		rts


; ===================================================================
;                           Shots functions
; ===================================================================
shots_clear:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	loadn r1, #0   ; posicao inicial do vetor de tiros
	loadn r2, #9   ; posicao final do vetor de tiros
	loadn r3, #shots_pre_pos
	loadn r4, #120 ; valor para validar

	shots_clear_loop:
		cmp r1, r2
		jgr shots_clear_exit

		add r5, r3, r1
		loadi r0, r5

		; Se nao eh valido, continua
		cmp r0, r4
		jle shots_clear_reset

		load r6, booster
		loadn r7, #2
		cmp r6, r7
		jne shots_clear_normal

		dec r0
		call blank_shot
		inc r0
		call blank_shot
		inc r0
		call blank_shot
		jmp shots_clear_loop_continue

		shots_clear_normal:
			call blank_shot

		shots_clear_loop_continue:
			inc r1
			jmp shots_clear_loop

		shots_clear_reset:
			loadn r0, #0
			storei r5, r0
			jmp shots_clear_loop_continue

	; Sai da rotina
	shots_clear_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

shots_draw:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	loadn r1, #0  ; posicao inicial do vetor de tiros
	loadn r2, #9  ; posicao final do vetor de tiros
	loadn r5, #40
	loadn r7, #120 ; valor de validacao

	loadn r3, #shots_cur_pos
	loadn r4, #shots_pre_pos

	shots_draw_loop:
		cmp r1, r2
		jgr shots_draw_exit

		add r0, r3, r1
		loadi r0, r0

		cmp r0, r7
		jle shots_draw_reset

		push r0
		load r0, booster
		loadn r6, #2
		cmp r0, r6
		pop r0
		jne shots_draw_normal

		inc r0
		call render_shot
		dec r0
		dec r0
		call render_shot
		inc r0
		call render_shot
		jmp shots_draw_update_pos

		shots_draw_normal:
			call render_shot

		shots_draw_update_pos:
			; pre_pos <- cur_pos
			add r6, r4, r1
			storei r6, r0

			; cur_pos <- cur_pos - 40
			sub r0, r0, r5
			add r6, r3, r1
			storei r6, r0

		shots_draw_loop_continue:
			inc r1
			jmp shots_draw_loop

		shots_draw_reset:
			loadn r0, #0

			add r6, r4, r1
			storei r6, r0

			add r6, r3, r1
			storei r6, r0

			jmp shots_draw_loop_continue

	; Sai da rotina
	shots_draw_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts


; ===================================================================
;                           Mobs functions
; ===================================================================
mobs_rand:
	push r0
	push r1
	push r2
	push r3

	loadn r3, #0

	load r0, mobs_time
	inc r0
	store mobs_time, r0

	loadn r1, #17 ; valor arbitrario
	mod r2, r0, r1
	cmp r2, r3
	ceq mobs_rand_create_1

	loadn r1, #31 ; valor arbitrario
	mod r2, r0, r1
	cmp r2, r3
	ceq mobs_rand_create_2

	mobs_rand_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		rts

	mobs_rand_create_1:
		load r0, rand
		inc r0
		store rand, r0
		loadn r1, #1013
		mod r0, r0, r1

		loadn r1, #rand_numbers
		add r1, r1, r0
		loadi r0, r1

		loadn r1, #32
		mod r0, r0, r1
		loadn r1, #124
		add r0, r0, r1

		load r1, mobs_1
		inc r1
		store mobs_1, r1
		loadn r2, #20
		mod r1, r1, r2

		loadn r2, #mobs_cur_pos_1
		add r2, r2, r1
		storei r2, r0

		rts

	mobs_rand_create_2:
		load r0, rand
		inc r0
		store rand, r0
		loadn r1, #1013
		mod r0, r0, r1

		loadn r1, #rand_numbers
		add r1, r1, r0
		loadi r0, r1

		loadn r1, #32
		mod r0, r0, r1
		loadn r1, #124
		add r0, r0, r1

		load r1, mobs_2
		inc r1
		store mobs_2, r1
		loadn r2, #20
		mod r1, r1, r2

		loadn r2, #mobs_cur_pos_2
		add r2, r2, r1
		storei r2, r0

		rts

mobs_clear:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	load r0, mobs_speed
	inc r0
	store mobs_speed, r0

	loadn r2, #0

	loadn r1, #13 ; valor arbitrario
	mod r0, r0, r1
	cmp r0, r2
	jne mobs_clear_next

	loadn r1, #0
	loadn r2, #19
	loadn r3, #mobs_pre_pos_1
	loadn r4, #124
	loadn r5, #960

	mobs_clear_loop_1:
		cmp r1, r2
		jgr mobs_clear_next

		add r6, r3, r1
		loadi r0, r6

		cmp r0, r4
		jle mobs_clear_reset_1
		cmp r0, r5
		jgr mobs_clear_reset_1

		call blank_mob

		mobs_clear_loop_1_continue:
			loadn r7, #mobs_cur_pos_1
			add r7, r7, r1
			loadi r7, r7
			loadn r0, #0
			cmp r7, r0
			jne mobs_clear_loop_1_continue_inc

			storei r6, r0

			mobs_clear_loop_1_continue_inc:
				inc r1
				jmp mobs_clear_loop_1

		mobs_clear_reset_1:
			loadn r0, #0
			storei r6, r0
			jmp mobs_clear_loop_1_continue

	mobs_clear_next:
		load r0, mobs_speed

		loadn r2, #0

		loadn r1, #4 ; valor arbitrario
		mod r0, r0, r1
		cmp r0, r2
		jne mobs_clear_exit

		loadn r1, #0
		loadn r2, #19
		loadn r3, #mobs_pre_pos_2
		loadn r4, #124
		loadn r5, #960

	mobs_clear_loop_2:
		cmp r1, r2
		jgr mobs_clear_exit

		add r6, r3, r1
		loadi r0, r6

		cmp r0, r4
		jle mobs_clear_reset_2
		cmp r0, r5
		jgr mobs_clear_reset_2

		call blank_mob

		mobs_clear_loop_2_continue:
			loadn r7, #mobs_cur_pos_2
			add r7, r7, r1
			loadi r7, r7
			loadn r0, #0
			cmp r7, r0
			jne mobs_clear_loop_2_continue_inc

			storei r6, r0

			mobs_clear_loop_2_continue_inc:
				inc r1
				jmp mobs_clear_loop_2

		mobs_clear_reset_2:
			loadn r0, #0
			storei r6, r0
			jmp mobs_clear_loop_2_continue

	mobs_clear_exit:
		load r7, mobs_speed
		loadn r2, #13
		loadn r4, #0

		mod r5, r7, r2
		cmp r5, r4
		jeq mobs_clear_bite_tank

		mobs_clear_exit_now:
			pop r7
			pop r6
			pop r5
			pop r4
			pop r3
			pop r2
			pop r1
			pop r0
			rts

		mobs_clear_bite_tank:
			loadn r1, #1
			load r0, damage
			cmp r0, r1
			jle mobs_clear_exit_now

			loadn r0, #0
			store damage, r0

			load r0, tank_cur_pos
			call render_tank_damage

			jmp mobs_clear_exit_now


mobs_draw:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	load r0, mobs_speed
	loadn r1, #13 ; valor arbitrario
	loadn r2, #0

	mod r0, r0, r1
	cmp r0, r2
	jne mobs_draw_next

	loadn r1, #0
	loadn r2, #19
	loadn r3, #mobs_cur_pos_1
	loadn r4, #mobs_pre_pos_1
	loadn r5, #124
	loadn r6, #960

	mobs_draw_loop_1:
		cmp r1, r2
		jgr mobs_draw_next

		add r0, r3, r1
		loadi r0, r0

		cmp r0, r5
		jle mobs_draw_reset_1
		cmp r0, r6
		jgr mobs_draw_reset_1

		push r1
		loadn r1, #'+'
		call render_mob
		pop r1

		; pre_pos <- cur_pos
		add r7, r4, r1
		storei r7, r0

		; cur_pos <- cur_pos + 40
		loadn r7, #40
		add r0, r0, r7
		add r7, r3, r1
		storei r7, r0

		mobs_draw_loop_1_continue:
			inc r1
			jmp mobs_draw_loop_1

		mobs_draw_reset_1:
			loadn r0, #0

			add r7, r4, r1
			storei r7, r0

			add r7, r3, r1
			storei r7, r0

			jmp mobs_draw_loop_1_continue

	mobs_draw_next:
		load r0, mobs_speed
		loadn r1, #4 ; valor arbitrario
		loadn r2, #0

		mod r0, r0, r1
		cmp r0, r2
		jne mobs_draw_exit

		loadn r1, #0
		loadn r2, #19
		loadn r3, #mobs_cur_pos_2
		loadn r4, #mobs_pre_pos_2
		loadn r5, #124
		loadn r6, #960

	mobs_draw_loop_2:
		cmp r1, r2
		jgr mobs_draw_exit

		add r0, r3, r1
		loadi r0, r0

		cmp r0, r5
		jle mobs_draw_reset_2
		cmp r0, r6
		jgr mobs_draw_reset_2

		push r1
		loadn r1, #'@'
		call render_mob
		pop r1

		; pre_pos <- cur_pos
		add r7, r4, r1
		storei r7, r0

		; cur_pos <- cur_pos + 40
		loadn r7, #40
		add r0, r0, r7
		add r7, r3, r1
		storei r7, r0

		mobs_draw_loop_2_continue:
			inc r1
			jmp mobs_draw_loop_2

		mobs_draw_reset_2:
			loadn r0, #0

			add r7, r4, r1
			storei r7, r0

			add r7, r3, r1
			storei r7, r0

			jmp mobs_draw_loop_2_continue

	mobs_draw_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

mobs_check:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	loadn r2, #0
	loadn r3, #19
	loadn r4, #mobs_cur_pos_1
	loadn r5, #mobs_cur_pos_2
	load  r6, tank_cur_pos

	mobs_check_loop:
		cmp r2, r3
		jgr mobs_check_next

		add r0, r4, r2
		add r1, r5, r2
		call mobs_shots_taken
		call mobs_bite_tank

		inc r2
		jmp mobs_check_loop

	mobs_check_next:
		loadn r2, #0
		loadn r3, #19
		loadn r4, #mobs_damage_1
		loadn r5, #mobs_damage_2
		load r6, kill_mob_1
		load r7, kill_mob_2

	mobs_check_damage_loop:
		cmp r2, r3
		jgr mobs_check_exit

		add r0, r4, r2
		loadi r1, r0
		cmp r1, r6
		jeg mobs_check_reset_1

		add r0, r5, r2
		loadi r1, r0
		cmp r1, r7
		jeg mobs_check_reset_2

		mobs_check_damage_loop_continue:
			inc r2
			jmp mobs_check_damage_loop

		mobs_check_reset_1:
			load r1, score_mob_1
			store hit, r1

			loadn r1, #0
			storei r0, r1

			loadn r0, #mobs_cur_pos_1
			add r0, r0, r2
			storei r0, r1

			jmp mobs_check_damage_loop_continue

		mobs_check_reset_2:
			load r1, score_mob_2
			store hit, r1

			loadn r1, #0
			storei r0, r1

			loadn r0, #mobs_cur_pos_2
			add r0, r0, r2
			storei r0, r1

			jmp mobs_check_damage_loop_continue

	mobs_check_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

mobs_bite_tank:
	push r3
	push r4
	push r5
	push r6
	push r7

	loadn r3, #0
	loadn r4, #1

	loadi r5, r0
	mov r7, r6

	dec r5
	dec r6
	cmp r5, r6
	jeq mobs_bite_tank_1
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_1
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_1

	inc r5
	mov r6, r7
	dec r6
	cmp r5, r6
	jeq mobs_bite_tank_1
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_1
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_1

	inc r5
	mov r6, r7
	dec r6
	cmp r5, r6
	jeq mobs_bite_tank_1
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_1
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_1

	loadi r5, r1
	mov r6, r7

	dec r5
	dec r6
	cmp r5, r6
	jeq mobs_bite_tank_2
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_2
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_2

	inc r5
	mov r6, r7
	dec r6
	cmp r5, r6
	jeq mobs_bite_tank_2
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_2
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_2

	inc r5
	mov r6, r7
	dec r6
	cmp r5, r6
	jeq mobs_bite_tank_2
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_2
	inc r6
	cmp r5, r6
	jeq mobs_bite_tank_2

	mobs_bite_tank_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		rts

	mobs_bite_tank_1:
		storei r0, r3
		store damage, r4
		loadn r5, #mobs_damage_1
		add r5, r5, r2
		storei r5, r3
		jmp mobs_bite_tank_exit

	mobs_bite_tank_2:
		storei r1, r3
		store damage, r4
		loadn r5, #mobs_damage_2
		add r5, r5, r2
		storei r5, r3
		jmp mobs_bite_tank_exit


mobs_shots_taken:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6
	push r7

	loadn r3, #shots_cur_pos
	loadn r4, #10

	loadi r5, r0
	loadi r6, r1
	loadn r7, #0

	mobs_shots_taken_loop:
		dec r4

		add r0, r3, r4
		loadi r1, r0

		cmp r1, r7
		jeq mobs_shots_taken_loop_continue

		push r4
		push r7

		loadn r7, #2
		load r4, booster
		cmp r4, r7
		jne mobs_shots_taken_normal
		mov r4, r1

		dec r1

		mov r7, r5
		dec r7
		cmp r1, r7
		ceq mobs_hit_1
		inc r7
		cmp r1, r7
		ceq mobs_hit_1
		inc r7
		cmp r1, r7
		ceq mobs_hit_1

		inc r1

		mov r7, r5
		dec r7
		cmp r1, r7
		ceq mobs_hit_1
		inc r7
		cmp r1, r7
		ceq mobs_hit_1
		inc r7
		cmp r1, r7
		ceq mobs_hit_1

		inc r1

		mov r7, r5
		dec r7
		cmp r1, r7
		ceq mobs_hit_1
		inc r7
		cmp r1, r7
		ceq mobs_hit_1
		inc r7
		cmp r1, r7
		ceq mobs_hit_1

		mov r1, r4

		dec r1

		mov r7, r6
		dec r7
		cmp r1, r7
		ceq mobs_hit_2
		inc r7
		cmp r1, r7
		ceq mobs_hit_2
		inc r7
		cmp r1, r7
		ceq mobs_hit_2

		inc r1

		mov r7, r6
		dec r7
		cmp r1, r7
		ceq mobs_hit_2
		inc r7
		cmp r1, r7
		ceq mobs_hit_2
		inc r7
		cmp r1, r7
		ceq mobs_hit_2

		inc r1

		mov r7, r6
		dec r7
		cmp r1, r7
		ceq mobs_hit_2
		inc r7
		cmp r1, r7
		ceq mobs_hit_2
		inc r7
		cmp r1, r7
		ceq mobs_hit_2

		pop r7
		pop r4
		jmp mobs_shots_taken_loop_continue

		mobs_shots_taken_normal:
			mov r7, r5
			dec r7
			cmp r1, r7
			ceq mobs_hit_1
			inc r7
			cmp r1, r7
			ceq mobs_hit_1
			inc r7
			cmp r1, r7
			ceq mobs_hit_1

			mov r7, r6
			dec r7
			cmp r1, r7
			ceq mobs_hit_2
			inc r7
			cmp r1, r7
			ceq mobs_hit_2
			inc r7
			cmp r1, r7
			ceq mobs_hit_2

			pop r7
			pop r4

		mobs_shots_taken_loop_continue:
			cmp r4, r7
			jeq mobs_shots_taken_exit
			jmp mobs_shots_taken_loop

	mobs_shots_taken_exit:
		pop r7
		pop r6
		pop r5
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

mobs_hit_1:
	push r0
	push r1
	push r2
	push r3

	loadn r3, #0
	storei r0, r3

	loadn r0, #mobs_damage_1
	add r0, r0, r2
	loadi r3, r0
	inc r3
	storei r0, r3

	mov r0, r5
	loadn r1, #40
	sub r0, r0, r1
	loadn r1, #124
	cmp r0, r1
	jle mobs_hit_1_exit
	loadn r1, #'+'
	call render_mob_damage

	mobs_hit_1_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		rts

mobs_hit_2:
	push r0
	push r1
	push r2
	push r3

	loadn r3, #0
	storei r0, r3

	loadn r0, #mobs_damage_2
	add r0, r0, r2
	loadi r3, r0
	inc r3
	storei r0, r3

	mov r0, r6
	loadn r1, #40
	sub r0, r0, r1
	loadn r1, #124
	cmp r0, r1
	jle mobs_hit_2_exit
	loadn r1, #'@'
	call render_mob_damage

	mobs_hit_2_exit:
		pop r3
		pop r2
		pop r1
		pop r0
		rts

; ===================================================================
;                           Booster functions
; ===================================================================
booster_draw:
	push r0
	push r1

	load r0, booster
	loadn r1, #1
	cmp r0, r1
	jne booster_draw_exit

	load r0, booster_cur_pos
	call render_booster

	booster_draw_exit:
		pop r1
		pop r0
		rts

booster_check:
	push r0
	push r1
	push r2
	push r3
	push r4

	load r0, booster

	loadn r1, #0
	cmp r0, r1
	jeq booster_check_exit

	loadn r1, #1
	cmp r0, r1
	jeq booster_check_1

	loadn r1, #2
	cmp r0, r1
	jeq booster_check_2

	booster_check_exit:
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

	booster_check_1:
		load r0, tank_cur_pos
		load r1, booster_cur_pos
		mov r2, r0
		mov r3, r1

		loadn r4, #80
		sub r0, r0, r4
		cmp r0, r1
		jeq booster_check_got
		loadn r4, #40
		add r0, r0, r4
		cmp r0, r1
		jeq booster_check_got
		add r0, r0, r4

		inc r0
		cmp r0, r1
		jeq booster_check_got
		dec r0
		cmp r0, r1
		jeq booster_check_got
		dec r0
		cmp r0, r1
		jeq booster_check_got

		add r0, r0, r4

		cmp r0, r1
		jeq booster_check_got
		inc r0
		cmp r0, r1
		jeq booster_check_got
		inc r0
		cmp r0, r1
		jeq booster_check_got

		dec r1
		mov r0, r2

		loadn r4, #80
		sub r0, r0, r4
		cmp r0, r1
		jeq booster_check_got
		loadn r4, #40
		add r0, r0, r4
		cmp r0, r1
		jeq booster_check_got
		add r0, r0, r4

		inc r0
		cmp r0, r1
		jeq booster_check_got
		dec r0
		cmp r0, r1
		jeq booster_check_got
		dec r0
		cmp r0, r1
		jeq booster_check_got

		add r0, r0, r4

		cmp r0, r1
		jeq booster_check_got
		inc r0
		cmp r0, r1
		jeq booster_check_got
		inc r0
		cmp r0, r1
		jeq booster_check_got

		jmp booster_check_exit

		booster_check_got:
			mov r0, r3
			call blank_booster

			mov r0, r2
			call render_tank

			loadn r1, #2
			store booster, r1

			loadn r1, #100
			store booster_fire, r1

			loadn r1, #6
			store msg, r1

			loadn r1, #119
			store msg_swap, r1

			jmp booster_check_exit

	booster_check_2:
		loadn r0, #0
		load r1, booster_fire
		cmp r1, r0
		jne booster_check_exit

		call shots_clear ; apaga os tiros
		store booster, r0

		loadn r1, #119
		store msg_swap, r1
		call show_message

		jmp booster_check_exit

; ===================================================================
;                           Timer functions
; ===================================================================
timer_draw:
	push r0
	push r1

	load r0, timer
	loadn r1, #1
	cmp r0, r1
	jne timer_draw_exit

	load r0, timer_cur_pos
	call render_timer

	timer_draw_exit:
		pop r1
		pop r0
		rts

timer_check:
	push r0
	push r1
	push r2
	push r3
	push r4

	load r0, timer

	loadn r1, #0
	cmp r0, r1
	jeq timer_check_exit

	loadn r1, #1
	cmp r0, r1
	jeq timer_check_1

	loadn r1, #2
	cmp r0, r1
	jeq timer_check_2

	timer_check_exit:
		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

	timer_check_1:
		load r0, tank_cur_pos
		load r1, timer_cur_pos
		mov r2, r0
		mov r3, r1

		loadn r4, #80
		sub r0, r0, r4
		cmp r0, r1
		jeq timer_check_got
		loadn r4, #40
		add r0, r0, r4
		cmp r0, r1
		jeq timer_check_got
		add r0, r0, r4

		inc r0
		cmp r0, r1
		jeq timer_check_got
		dec r0
		cmp r0, r1
		jeq timer_check_got
		dec r0
		cmp r0, r1
		jeq timer_check_got

		add r0, r0, r4

		cmp r0, r1
		jeq timer_check_got
		inc r0
		cmp r0, r1
		jeq timer_check_got
		inc r0
		cmp r0, r1
		jeq timer_check_got

		dec r1
		mov r0, r2

		loadn r4, #80
		sub r0, r0, r4
		cmp r0, r1
		jeq timer_check_got
		loadn r4, #40
		add r0, r0, r4
		cmp r0, r1
		jeq timer_check_got
		add r0, r0, r4

		inc r0
		cmp r0, r1
		jeq timer_check_got
		dec r0
		cmp r0, r1
		jeq timer_check_got
		dec r0
		cmp r0, r1
		jeq timer_check_got

		add r0, r0, r4

		cmp r0, r1
		jeq timer_check_got
		inc r0
		cmp r0, r1
		jeq timer_check_got
		inc r0
		cmp r0, r1
		jeq timer_check_got

		jmp timer_check_exit

		timer_check_got:
			mov r0, r3
			call blank_timer

			mov r0, r2
			call render_tank

			loadn r1, #2
			store timer, r1

			loadn r1, #7
			store msg, r1

			loadn r1, #119
			store msg_swap, r1

			jmp timer_check_exit

	timer_check_2:
		loadn r0, #0
		store timer, r0

		loadn r0, #16

		load r1, time
		add r1, r1, r0
		store time, r1

		jmp timer_check_exit

; ===================================================================
;                           Rendering functions
; ===================================================================
render_title:
	push r0
	push r1
	push r2
	push r3

	mov r1, r0
	loadn r2, #2304

	loadn r0, #'F'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'I'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'R'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'E'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r2, #3072

	inc r1
	loadn r0, #'S'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'T'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'A'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'R'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'T'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'E'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #'R'
	add r0, r0, r2
	outchar r0, r1

	loadn r2, #2048

	loadn r0, #'#'
	loadn r3, #20
	add r1, r1, r3

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	add r0, r0, r2
	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	pop r3
	pop r2
	pop r1
	pop r0
	rts

render_tank_damage:
	push r0
	push r1
	push r2
	push r3
	push r4

	mov r1, r0

	loadn r2, #1280
	loadn r3, #1280

	jmp render_tank_do

render_tank:
	push r0
	push r1
	push r2
	push r3
	push r4

	mov r1, r0

	loadn r2, #2048
	loadn r3, #256

	render_tank_do:

		loadn r4, #80
		sub r1, r1, r4

		loadn r0, #';'
		add r0, r0, r3
		outchar r0, r1

		loadn r4, #40
		add r1, r1, r4

		loadn r0, #'['
		add r0, r0, r2
		outchar r0, r1
		dec r1

		add r1, r1, r4

		loadn r0, #'%'
		add r0, r0, r3
		outchar r0, r1
		inc r1

		loadn r0, #']'
		add r0, r0, r3
		outchar r0, r1
		inc r1

		loadn r0, #'\\'
		add r0, r0, r3
		outchar r0, r1

		loadn r4, #38
		add r1, r1, r4

		loadn r0, #'~'
		add r0, r0, r2
		outchar r0, r1
		inc r1

		loadn r0, #'^'
		add r0, r0, r3
		outchar r0, r1
		inc r1

		loadn r0, #'`'
		add r0, r0, r2
		outchar r0, r1

		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

render_shot:
	push r0
	push r1
	push r2

	mov r1, r0
	loadn r2, #2816
	loadn r0, #'*'
	add r0, r0, r2

	outchar r0, r1

	pop r2
	pop r1
	pop r0
	rts

render_mob_damage:
	push r0
	push r1
	push r2
	push r3
	push r4

	mov r4, r1
	mov r1, r0

	loadn r2, #1280
	jmp render_mob_do

render_mob:
	push r0
	push r1
	push r2
	push r3
	push r4

	mov r4, r1
	mov r1, r0

	loadn r2, #768

	render_mob_do:

		dec r1
		loadn r0, #'<'
		add r0, r0, r2
		outchar r0, r1
		inc r1

		loadn r0, #'='
		add r0, r0, r2
		outchar r0, r1
		inc r1

		loadn r0, #'>'
		add r0, r0, r2
		outchar r0, r1

		loadn r3, #38
		add r1, r1, r3

		loadn r0, #'&'
		add r0, r0, r2
		outchar r0, r1
		inc r1

		add r4, r4, r2
		outchar r4, r1
		inc r1

		loadn r0, #'$'
		add r0, r0, r2
		outchar r0, r1

		pop r4
		pop r3
		pop r2
		pop r1
		pop r0
		rts

render_game:
	push r0
	push r1
	push r2
	push r3
	push r4
	push r5
	push r6

	loadn r4, #2048

	loadn r2, #' '
	outchar r2, r0
	inc r0

	outchar r2, r0
	inc r0

	outchar r2, r0
	inc r0

	loadn r2, #'_'
	add r2, r2, r4
	loadn r1, #34
	render_game_top:
		outchar r2, r0
		inc r0
		dec r1
		jnz render_game_top

	loadn r2, #' '
	outchar r2, r0
	inc r0

	outchar r2, r0
	inc r0

	outchar r2, r0

	loadn r1, #3
	add r0, r0, r1

	loadn r2, #'{'
	add r2, r2, r4

	loadn r3, #'}'
	add r3, r3, r4

	loadn r1, #26
	loadn r5, #35
	loadn r6, #5
	render_game_middle:
		outchar r2, r0
		add r0, r0, r5
		outchar r3, r0
		add r0, r0, r6
		dec r1
		jnz render_game_middle

	loadn r1, #159
	sub r0, r0, r1

	loadn r2, #'_'
	add r2, r2, r4
	loadn r1, #34
	render_game_bottom:
		outchar r2, r0
		inc r0
		dec r1
		jnz render_game_bottom

	loadn r2, #'{'
	add r2, r2, r4

	loadn r1, #14
	add r0, r0, r1
	outchar r2, r0

	loadn r1, #11
	add r0, r0, r1
	outchar r2, r0

	loadn r1, #29
	add r0, r0, r1
	outchar r2, r0

	loadn r1, #11
	add r0, r0, r1
	outchar r2, r0

	loadn r2, #'_'
	add r2, r2, r4

	loadn r1, #21
	add r0, r0, r1

	loadn r1, #34
	render_game_last_line:
		outchar r2, r0
		inc r0
		dec r1
		jnz render_game_last_line

	loadn r2, #'|'
	add r2, r2, r4

	loadn r1, #15
	sub r0, r0, r1
	outchar r2, r0

	loadn r1, #11
	sub r0, r0, r1
	outchar r2, r0

	loadn r1, #47
	sub r0, r0, r1

	loadn r2, #'L'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'i'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'f'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'e'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #':'
	add r2, r2, r4
	outchar r2, r0

	loadn r1, #5
	add r0, r0, r1

	loadn r2, #'T'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'i'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'m'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'e'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #':'
	add r2, r2, r4
	outchar r2, r0

	loadn r1, #7
	add r0, r0, r1

	loadn r2, #'S'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'c'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'o'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'r'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'e'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #':'
	add r2, r2, r4
	outchar r2, r0

	pop r6
	pop r5
	pop r4
	pop r3
	pop r2
	pop r1
	pop r0
	rts

render_records:
	push r0
	push r1
	push r2
	push r4

	loadn r4, #2048

	loadn r1, #11
	add r0, r0, r1

	loadn r2, #'N'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'a'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'m'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'e'
	add r2, r2, r4
	outchar r2, r0

	loadn r1, #5
	add r0, r0, r1

	loadn r2, #'S'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'c'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'o'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'r'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'e'
	add r2, r2, r4
	outchar r2, r0

	loadn r1, #5
	add r0, r0, r1

	loadn r2, #'L'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'i'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'f'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'e'
	add r2, r2, r4
	outchar r2, r0

	loadn r1, #93
	add r0, r0, r1

	loadn r2, #'1'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'s'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'t'
	add r2, r2, r4
	outchar r2, r0

	loadn r1, #78
	add r0, r0, r1

	loadn r2, #'2'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'n'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'d'
	add r2, r2, r4
	outchar r2, r0

	add r0, r0, r1

	loadn r2, #'3'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'r'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'d'
	add r2, r2, r4
	outchar r2, r0

	add r0, r0, r1

	loadn r2, #'4'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'t'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'h'
	add r2, r2, r4
	outchar r2, r0

	add r0, r0, r1

	loadn r2, #'5'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'t'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'h'
	add r2, r2, r4
	outchar r2, r0

	add r0, r0, r1

	loadn r2, #'6'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'t'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'h'
	add r2, r2, r4
	outchar r2, r0

	add r0, r0, r1

	loadn r2, #'7'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'t'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'h'
	add r2, r2, r4
	outchar r2, r0

	add r0, r0, r1

	loadn r2, #'8'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'t'
	add r2, r2, r4
	outchar r2, r0
	inc r0

	loadn r2, #'h'
	add r2, r2, r4
	outchar r2, r0

	loadn r1, #642
	sub r0, r0, r1

	loadn r2, #'_'
	add r2, r2, r4
	loadn r1, #32
	render_records_border_1:
		outchar r2, r0
		inc r0
		dec r1
		jnz render_records_border_1

	loadn r1, #11
	add r0, r0, r1

	loadn r2, #'{'
	add r2, r2, r4
	loadn r4, #40
	loadn r1, #16
	render_records_border_2:
		outchar r2, r0
		add r0, r0, r4
		dec r1
		jnz render_records_border_2

	pop r4
	pop r2
	pop r1
	pop r0
	rts

render_booster:
	push r0
	push r1
	push r2

	loadn r2, #2304
	loadn r1, #'F'
	add r1, r1, r2
	outchar r1, r0
	dec r0

	loadn r2, #2048
	loadn r1, #'"'
	add r1, r1, r2
	outchar r1, r0

	pop r2
	pop r1
	pop r0
	rts

render_timer:
	push r0
	push r1
	push r2

	loadn r2, #3072
	loadn r1, #'T'
	add r1, r1, r2
	outchar r1, r0
	dec r0

	loadn r2, #2048
	loadn r1, #'"'
	add r1, r1, r2
	outchar r1, r0

	pop r2
	pop r1
	pop r0
	rts

render_score_life:
	push r0
	push r1
	push r2

	loadn r2, #2048
	loadn r1, #805

	loadn r0, #'S'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r0, #'c'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r0, #'o'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r0, #'r'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r0, #'e'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	loadn r0, #':'
	outchar r0, r1

	loadn r1, #885

	loadn r0, #'L'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r0, #'i'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r0, #'f'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	loadn r0, #'e'
	add r0, r0, r2
	outchar r0, r1
	inc r1

	inc r1
	inc r1
	loadn r0, #':'
	outchar r0, r1

	load  r0, score
	loadn r1, #813
	call printd

	load  r0, life
	loadn r1, #893
	call printd

	pop r2
	pop r1
	pop r0
	rts

; ===================================================================
;                           Blank functions
; ===================================================================
blank_tank:
	push r0
	push r1
	push r2

	mov r1, r0
	loadn r0, #' '

	loadn r2, #80
	sub r1, r1, r2

	outchar r0, r1

	loadn r2, #40
	add r1, r1, r2

	outchar r0, r1
	dec r1

	add r1, r1, r2

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1

	loadn r2, #38
	add r1, r1, r2

	outchar r0, r1
	inc r1

	outchar r0, r1
	inc r1

	outchar r0, r1

	pop r2
	pop r1
	pop r0
	rts

blank_shot:
	push r1

	loadn r1, #' '
	outchar r1, r0

	pop r1
	rts

blank_mob:
	push r0
	push r1
	push r2

	loadn r1, #' '
	loadn r2, #38
	dec r0

	outchar r1, r0
	inc r0

	outchar r1, r0
	inc r0

	outchar r1, r0

	add r0, r0, r2

	outchar r1, r0
	inc r0

	outchar r1, r0
	inc r0

	outchar r1, r0

	pop r2
	pop r1
	pop r0
	rts

blank_booster:
	push r0
	push r1

	loadn r1, #' '

	outchar r1, r0

	dec r0
	outchar r1, r0

	pop r1
	pop r0
	rts

blank_timer:
	push r0
	push r1

	loadn r1, #' '

	outchar r1, r0

	dec r0
	outchar r1, r0

	pop r1
	pop r0
	rts

; ---------------------------------------------------------------
; ===============================================================
