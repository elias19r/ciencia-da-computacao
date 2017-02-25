maior(X, Y) :- X > Y.

swap([X, Y|Cauda], [Y, X|Cauda]) :- maior(X, Y).
swap([Z|Cauda], [Z|Cauda1]) :- swap(Cauda, Cauda1).

bubblesort(Lista, ListaOrd) :- swap(Lista, Lista1), !, bubblesort(Lista1, ListaOrd).
bubblesort(ListaOrd, ListaOrd).