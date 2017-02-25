% Tudo recursivo

ultimo(X, [X]).
ultimo(X, [_ | Tail]):-
	ultimo(X, Tail).


pertence(X, [X | _]):-
	!.
pertence(X, [_ | Tail]):-
	pertence(X, Tail).


soma([], 0).
soma([X | Tail], Soma):-
	soma(Tail, Soma1),
	Soma is Soma1 + X.

