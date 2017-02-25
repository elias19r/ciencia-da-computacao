remove_all(_, [], []) :- !.
remove_all(E, [E|Cauda], L) :- remove_all(E, Cauda, L), !.
remove_all(E, [E1|Cauda], [E1|Cauda1]) :- remove_all(E, Cauda, Cauda1).