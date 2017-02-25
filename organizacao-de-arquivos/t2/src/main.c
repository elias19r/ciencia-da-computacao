/**
 * Trabalho 02: t2
 * 2013/04/20
 * Elias Italiano Rodrigues, 7987251, Arquivos, BCC, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <myfunc.h>

// Inclui headers dos TADs
#include <pgm_p2.h>
#include <txt.h>

/**
 * Função principal
 */
int main(int argc, char *argv[]) {
	// Strings para leitura dos comandos
	char *cmd;
	char **cmdargv;
	int cmdargc;
	
	while (1) {
		// Recebe o comando e separa os argumentos
		cmd = scanln();
		cmdargv = getargs(cmd, &cmdargc);
		
		if (cmdargc > 0) {
			// Comando "sair"
			if (strcmp(cmdargv[0], "sair") == 0) {
				// Libera o comando e quebra o laço
				free(cmd);
				while (cmdargc != 0)
					free(cmdargv[--cmdargc]);
				free(cmdargv);
				break;
			}

		}
		
		if (cmdargc > 2) {
			if (strcmp(cmdargv[0], "compactar") == 0) {

				// Comando "compactar run-length <filename>"
				if (strcmp(cmdargv[1], "run-length") == 0) {
					FILE *fp = fopen(cmdargv[2], "rb");

					if (fp != NULL) {
						pgm_p2_data *img = (pgm_p2_data *) malloc(sizeof(pgm_p2_data));
						pgm_p2_new(img);

						// Faz a leitura do arquivo .pgm
						pgm_p2_read(img, fp);
						fclose(fp);
						
						// Compacta e imprime o compactado na tela
						pgm_p2_compress(img);
						pgm_p2_print_rl(img);

						// Libera memória
						pgm_p2_delete(img);
						free(img);
					}
					
				// Comando "compactar huffman <filename>"
				} else if (strcmp(cmdargv[1], "huffman") == 0) {
					FILE *fp = fopen(cmdargv[2], "rb");

					if (fp != NULL) {
						txt_data *txt = (txt_data *) malloc(sizeof(txt_data));
						txt_new(txt);

						// Faz a leitura do arquivo .txt
						txt_read(txt, fp);
						fclose(fp);
						
						// Compacta e imprime o compactado na tela
						txt_compress(txt);
						txt_print_huff(txt);

						// Libera memória
						txt_delete(txt);
						free(txt);
					}
				}
				
			} else if (strcmp(cmdargv[0], "descompactar") == 0) {

				// Comando "descompactar run-length <filename>"
				if (strcmp(cmdargv[1], "run-length") == 0) {
					FILE *fp = fopen(cmdargv[2], "rb");
					if (fp != NULL) {
						pgm_p2_data *img = (pgm_p2_data *) malloc(sizeof(pgm_p2_data));
						pgm_p2_new(img);
						
						// Faz a leitura do arquivo .rl
						pgm_p2_read_rl(img, fp);
						fclose(fp);
						
						// Descompacta e imprime na tela
						pgm_p2_decompress(img);
						pgm_p2_print(img);

						// Libera memória
						pgm_p2_delete(img);	
						free(img);
					}
					
				// Comando "descompactar huffman <filename>"
				} else if (strcmp(cmdargv[1], "huffman") == 0) {
					FILE *fp = fopen(cmdargv[2], "rb");

					if (fp != NULL) {
						txt_data *txt = (txt_data *) malloc(sizeof(txt_data));
						txt_new(txt);

						// Faz a leitura do arquivo .huff
						txt_read_huff(txt, fp);
						fclose(fp);
						
						// Descompacta e imprime o conteúdo descomprimido na tela
						txt_decompress(txt);
						txt_print(txt);

						// Libera memória
						txt_delete(txt);
						free(txt);
					}
				}

			} else if (strcmp(cmdargv[0], "dump") == 0) {

				// Comando "dump tree <filename>"
				if (strcmp(cmdargv[1], "tree") == 0) {
					FILE *fp = fopen(cmdargv[2], "rb");

					if (fp != NULL) {
						txt_data *txt = (txt_data *) malloc(sizeof(txt_data));
						txt_new(txt);

						// Faz a leitura do arquivo .txt
						txt_read(txt, fp);
						fclose(fp);
						
						// Compacta e imprime a árvore de huff na tela
						txt_compress(txt);
						txt_print_huff_tree(txt);

						// Libera memória
						txt_delete(txt);
						free(txt);
					}
				}
			}
		}

		// Libera o comando
		free(cmd);
		while (cmdargc != 0)
			free(cmdargv[--cmdargc]);
		free(cmdargv);
	}

	return 0;
}
