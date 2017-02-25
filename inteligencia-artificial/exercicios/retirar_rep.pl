retirar_todas(_, [], []) :- !.
retirar_todas(E, [E|Cauda], L) :- retirar_todas(E, Cauda, L), !.
retirar_todas(E, [E1|Cauda], [E1|Cauda1]) :- retirar_todas(E, Cauda, Cauda1).

retirar_rep([], []) :- !.
retirar_rep([E|Cauda], [E|Cauda1]) :- retirar_todas(E, Cauda, L), retirar_rep(L, Cauda1).