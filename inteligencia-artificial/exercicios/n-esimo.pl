find(1, [E|_], E).
find(N, [_|Cauda], E) :- find(M, Cauda, E), N is M+1.