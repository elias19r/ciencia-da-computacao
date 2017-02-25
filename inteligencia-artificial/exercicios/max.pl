max([E], E).
max([X, Y|Cauda], Max) :- X >= Y, !, max([X|Cauda], Max).
max([X, Y|Cauda], Max) :- max([Y|Cauda], Max).