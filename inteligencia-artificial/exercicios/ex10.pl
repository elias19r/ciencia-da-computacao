insert(E, [], [E]) :- !.
insert(E, L, [E|L]).

remove(E, [E|Cauda], Cauda) :- !.
remove(E, [E1|Cauda], [E1|Cauda2]) :- remove(E, Cauda, Cauda2).

remove_all(_, [], []) :- !.
remove_all(E, [E|Cauda], L) :- remove_all(E, Cauda, L), !.
remove_all(E, [E1|Cauda], [E1|Cauda2]) :- remove_all(E, Cauda, Cauda2).