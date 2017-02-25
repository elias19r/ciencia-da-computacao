concat([], L, L) :- !.
concat([E|Cauda], L, [E|Cauda1]) :- concat(Cauda, L, Cauda1).

invert([E], [E]) :- !.
invert([E|Cauda], L) :- invert(Cauda, Cauda1), concat(Cauda1, [E], L).

compair([], []).
compair([E|Cauda], [E1|Cauda1]) :- E == E1, compair(Cauda, Cauda1).

palin(L, R) :- invert(L, LI), compair(L, LI), R = 'Sim'.