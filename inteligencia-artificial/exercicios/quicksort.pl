maior(X, Y) :- X > Y.

concat([], L, L).
concat([E|Cauda], L, [E|Cauda1]) :- concat(Cauda, L, Cauda1).

part(_, [], [], []).
part(X, [Y|Cauda], [Y|Men], Mai) :- maior(X, Y), !, part(X, Cauda, Men, Mai).
part(X, [Y|Cauda], Men, [Y|Mai]) :- part(X, Cauda, Men, Mai).

quicksort([], []).
quicksort([E|Cauda], ListaOrd) :- part(E, Cauda, Men, Mai), quicksort(Men, MenOrd), quicksort(Mai, MaiOrd), concat(MenOrd, [E|MaiOrd], ListaOrd).