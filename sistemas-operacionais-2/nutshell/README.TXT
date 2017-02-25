Universidade de Sao Paulo
Instituto de Ciencias Matematicas e de Computacao
SSC0141 Sistemas Operacionais 2

---------------------------------------------------
 nutshell - Entrega final - 2014/07/07
---------------------------------------------------

Elias Italiano Rodrigues  7987251
Rafael Andreatta Martins  7564019
Rodolfo Megiato de Lima   7987286
Vinicius Katata Biondo    6783972

---------------------------------------------------
 Instrucoes
---------------------------------------------------
- Estando no diretorio do nutshell, execute:
	- Para compilar: make
	- Para executar: make run
- Importante: executar export LD_LIBRARY_PATH=. se deseja que a lib seja buscada no diretorio atual (do nutshell)

---------------------------------------------------
 Funcoes implementadas
---------------------------------------------------
- Execucao de comandos internos 'quit' ou 'exit' e 'cd'
- Execucao de comandos externos via fork() e exec() utilizando o caminho de busca (PATH) herdado do shell padrao (bash)
- Os comandos externos aceitam uma lista variavel de argumentos
- Redirecionamento de entrada: leitura (<)
- Redirecionamento de saida: sobrescrita (>), concatenacao (>>)
- O comando 'exit' ou 'quit' causam o termino do programa com return EXIT_SUCCESS
- Execucao de processos via pipeline
