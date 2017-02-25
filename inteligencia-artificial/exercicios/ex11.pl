soma([], 0).
soma([E|Cauda], S) :- soma(Cauda, S1), S is S1+E.