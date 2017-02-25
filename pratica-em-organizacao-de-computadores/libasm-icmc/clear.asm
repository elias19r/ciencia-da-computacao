; Este programa apaga a tela

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

; Comeco do programa
jmp main

main:
	call clear
	halt

; -------------------------------------------------------------------------
; clear: Apaga a tela imprimindo o caractere espaco em todas as posicoes
; -------------------------------------------------------------------------
clear:
	push fr
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
	pop fr
	rts

