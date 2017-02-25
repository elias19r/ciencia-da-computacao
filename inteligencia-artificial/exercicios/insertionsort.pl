maior(X, Y) :- X > Y.

inserir(X, [Y|Cauda1], [Y|Cauda2]) :- maior(X, Y), !, inserir(X, Cauda1, Cauda2).
inserir(X, L, [X|L]).

insertionsort([], []).
insertionsort([Cab|Cauda], ListaOrd) :- insertionsort(Cauda, CaudaOrd), inserir(Cab, CaudaOrd, ListaOrd).