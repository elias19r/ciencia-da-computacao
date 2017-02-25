mae(sofia,joao).
mae(ana,maria).
mae(carla,sofia).

pai(paulo,luis).
pai(paulo,sofia).
pai(luis,pedro).
pai(luis,maria).

progenitor(A,B) :- pai(A,B).
progenitor(A,B) :- mae(A,B).

avo(X,Y) :- progenitor(X,Z), progenitor(Z,Y).
