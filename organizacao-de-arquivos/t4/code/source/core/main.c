#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../include/lib/mylib.h"
#include "../../include/config/commands.h"
#include "../../include/controller/athlete.h"

int main(int argc, char *argv[]) {

	// Chars para leitura dos comandos
	char *cmd_line, **cmd_argv;
	int cmd_argc;
	
	while (1) {
		// Recebe o comando e separa os argumentos
		cmd_line = scanln();
		cmd_argv = getargs(cmd_line, &cmd_argc);
		
		if (cmd_argc == 1) {

			// Comando "sair"
			if (strcmp(cmd_argv[0], CMD_EXIT) == 0) {
				// Libera o comando e quebra o loop
				free(cmd_line);
				while (cmd_argc != 0)
					free(cmd_argv[--cmd_argc]);
				free(cmd_argv);
				break;

			// Comando "cadastrar"
			} else if (strcmp(cmd_argv[0], CMD_INSERT) == 0)
				athlete_action_insert();
					

		} else if (cmd_argc == 2) {

			// Comando "remover <cpf>"
			if (strcmp(cmd_argv[0], CMD_DELETE) == 0)
				athlete_action_delete(cmd_argv[1]);

			// Comando "dump <arquivo>"
			else if (strcmp(cmd_argv[0], CMD_DUMP) == 0)
				athlete_action_dump(cmd_argv[1]);

		} else if (cmd_argc == 4) {
			// Comando "buscar <campo> = <valor>"
			if (strcmp(cmd_argv[0], CMD_SEARCH) == 0)
				athlete_action_search(cmd_argv[1], cmd_argv[3]);
			
		} else if (cmd_argc == 8) {
			// Comando "buscar <campo> = <valor> <e|ou> <campo> = <valor>"
			if (strcmp(cmd_argv[0], CMD_SEARCH) == 0)
				athlete_action_search_combined(cmd_argv[1], cmd_argv[3], cmd_argv[4], cmd_argv[5], cmd_argv[7]);
		}

		// Libera o comando
		free(cmd_line);
		while (cmd_argc != 0)
			free(cmd_argv[--cmd_argc]);
		free(cmd_argv);
	}

	return 0;
}
