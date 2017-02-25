sum([], 0).
sum([E|Cauda], S) :- sum(Cauda, S1), S is S1 + E.