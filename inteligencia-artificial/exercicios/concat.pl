concat([], L, L).
concat([E|Cauda], L, [E|Cauda1]) :- concat(Cauda, L, Cauda1).