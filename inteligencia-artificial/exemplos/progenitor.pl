% ===================================================

progenitor(solange, naiara).
progenitor(solange, vitor).
progenitor(zilda,   solange).
progenitor(amador,  solange).

masc(vitor).
masc(amador).
fem(solange).
fem(naiara).
fem(zilda).

temfilhos(X):-
	progenitor(X, _).

pai(X):-
	progenitor(X, _),
	masc(X).

temfilho(X):-
	progenitor(X, Y),
	masc(Y).

avos(X, Z):-
	progenitor(X, Y),
	progenitor(Y, Z).

irmaos(I1, I2):-
	progenitor(X, I1),
	progenitor(X, I2),
	I1 \= I2.

netos(X, Z):-
	avos(Z, X).

soma(C, A, B):-
	C is A + B.

ad-tres-e-dobra(X, Y):-
	Y is (X + 3) * 2.


