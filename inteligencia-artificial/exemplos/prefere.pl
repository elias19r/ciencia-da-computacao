% Fatos

pessoa(raul).
pessoa(maria).
pessoa(bruno).
pessoa(ricardo).
pessoa(julia).

masc(raul).
masc(bruno).
masc(ricardo).

fem(maria).
fem(julia).

% Regras

prefere(raul,  Alguem):- fem(Alguem).
prefere(maria, Alguem):- masc(Alguem).
prefere(julia, Alguem):- fem(Alguem).
prefere(ricardo, Alguem):- pessoa(Alguem), Alguem \= bruno.

possivel(X, Y):-
	prefere(X, Y),
	prefere(Y, X),
	X \= Y.

