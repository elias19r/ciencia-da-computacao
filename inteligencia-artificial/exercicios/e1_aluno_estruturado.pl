% e1_aluno.pl
% SCC0230 Inteligencia Artificial
% Elias Rodrigues, 7987251
% 2014-09-02

% aluno(nome, cidade, nasc, notas, frequencia)
aluno(nome('Elias', 'Rodrigues'), cidade('São Carlos', 'SP'),     nasc(19, 12, 1990), [10.0, 7.3], 0.90).
aluno(nome('Joao', 'Silva'),      cidade('São Paulo', 'SP'),      nasc(1,  2,  1991), [ 4.9, 4.0], 1.00).
aluno(nome('Maria', 'Selva'),     cidade('Rio de Janeiro', 'RJ'), nasc(2,  3,  1992), [ 7.8, 7.0], 0.65).
aluno(nome('José', 'Jota'),       cidade('Belo Horizonte', 'MG'), nasc(4,  5,  1993), [ 7.1, 6.0], 0.80).

% soma(+Lista, -Soma)
soma([], 0).
soma([Cabeca | Cauda], Soma) :-
	soma(Cauda, SubSoma),
	Soma is Cabeca + SubSoma.

% tamanho(+Lista, -Tamanho)
tamanho([], 0).
tamanho([_ | Cauda], Tamanho) :-
	tamanho(Cauda, SubTamanho),
	Tamanho is 1 + SubTamanho.

% media(+Lista, -Media)
media(Lista, Media) :-
	soma(Lista, Soma),
	tamanho(Lista, Tamanho),
	Media is Soma / Tamanho.

% aprovado(?Nome)
aprovado(Nome) :-
	aluno(Nome, _, _, Notas, Frequencia),
	Frequencia >= 0.7,
	media(Notas, Media),
	Media >= 5.


