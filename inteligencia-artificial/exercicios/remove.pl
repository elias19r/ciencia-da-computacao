remove(E, [E|Cauda], Cauda).
remove(E, [E1|Cauda], [E1|Cauda1]) :- remove(E, Cauda, Cauda1).