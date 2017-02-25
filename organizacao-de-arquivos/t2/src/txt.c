#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <myfunc.h>

#include <txt.h>

/**
 * Inicializa a estrutura de dados
 */
void txt_new(txt_data *txt) {
	txt->data = NULL;
	txt->data_huff = NULL;
	txt->header_huff = NULL;
	txt->size = 0;
	txt->size_huff = 0;
	txt->size_header_huff = 0;
	txt->huff_tree = NULL;
	txt->symbols = NULL;
	txt->paths = NULL;
	txt->count = 0;
}

/**
 * Função auxiliar da txt_huff_quicksort para trocar os valores do vetor
 */
static void txt_huff_sort_swap(node **v, int i, int j) {
	node *tmp = v[i];
	v[i] = v[j];
	v[j] = tmp;
}

/**
 * Ordena um vetor ponteiros de nós de árvore de Huffman
 * Algoritmo do quicksort
 */
static void txt_huff_sort(node **v, int left, int right, int both) {
	if (left < right) {
		int i, j;
		
		i = left;
		for (j = left + 1; j <= right; j++) {
			if (both) {
				// Ordena pela frequência
				if (v[j]->frequence < v[left]->frequence) {
					i++;
					txt_huff_sort_swap(v, i, j);
			
				// Se frequência for igual, ordena pelo valor do símbolo
				} else if (v[j]->frequence == v[left]->frequence) {
					if (strcmp(v[j]->term, v[left]->term) < 0) {
						i++;
						txt_huff_sort_swap(v, i, j);					
					}
				}
			} else {
				if (strcmp(v[j]->term, v[left]->term) < 0) {
					i++;
					txt_huff_sort_swap(v, i, j);					
				}				
			}
		}
		txt_huff_sort_swap(v, left, i);
		
		// Chama a ordenção recursivamente pro dois lados
		txt_huff_sort(v, left, i - 1, both);
		txt_huff_sort(v, i + 1, right, both);
	}
}

/**
 * Recebe os nós folhas com as frequência calculadas e constrói a árvore de Huffman
 */
static void txt_build_huff_tree(txt_data *txt, node **vector, int size) {
	char buffer[255];
	int start, end, bufferlen;
	node *new;
	
	// Define as posições de começo e fim do vetor
	start = 0;
	end = size-1;
	
	// Enquanto o vetor tiver pelo menos dois elementos:
	// 	ordena e cria novo nó juntando os dois primeiros nós
	while (start < end) {

		// Ordena por frequência e por valor numérico do símbolo
		txt_huff_sort(vector, start, end, 1);
		
		sprintf(buffer, "%s%s", vector[start]->term, vector[start+1]->term);
		bufferlen = strlen(buffer);
	
		// Cria um novo nó que é junção dos dois primeiros nós do vetor
		new = (node *) malloc(sizeof(node));
		new->frequence = vector[start]->frequence + vector[start+1]->frequence;
		new->term = (char *) malloc(sizeof(char) * (bufferlen + 1));
		memcpy(new->term, buffer, bufferlen);
		new->term[bufferlen] = '\0';
		
		// Define os ponteiros left e right deste novo nó
		new->left = vector[start];
		new->right = vector[start+1];
		
		// Adiciona este novo nó no vetor
		vector[start+1] = new;

		// Diminui o vetor (incrementando a posição relativa ao início dele)
		start++;
	}

	// Define a raiz da árvore
	txt->huff_tree = vector[start];
}

/**
 * Remove os símbolos com frequência igual a zero
 * e retorna o novo tamanho do vetor de folhas
 */
static unsigned int txt_trim_huff_leafs(node ***vector) {
	int i, size = 0;
	node **new = NULL;
	
	for (i = 0; i < 63; i++) {
		if ((*vector)[i]->frequence != 0) {
			new = (node **) realloc(new, sizeof(node *) * (size + 1));
			new[size++] = (*vector)[i];
		} else {
			free((*vector)[i]->term);
			free((*vector)[i]);
		}
	}
	
	free(*vector);
	*vector = new;
	
	return size;
}

/**
 * Função recursiva que retorna uma string representativa do caminho para um símbolo
 * de acordo com o nó inicial de uma árvore de Huffman
 */
static void txt_search_huff_rec(node *tree, char *term, char **path, int *count) {
	if (tree != NULL) {
		// Encontrou
		if (strcmp(term, tree->term) == 0) {
			*path = (char *) realloc(*path, sizeof(char) * (*count + 1));
			(*path)[*count] = '\0';
		
		// Vai para esquerda '0'
		} else if (tree->left != NULL && strstr(tree->left->term, term) != NULL) {
			*path = (char *) realloc(*path, sizeof(char) * (*count + 1));
			(*path)[(*count)++] = '0';
			txt_search_huff_rec(tree->left, term, path, count);
			
		// Vai para a direita '1'
		} else if (tree->right != NULL && strstr(tree->right->term, term) != NULL) {
			*path = (char *) realloc(*path, sizeof(char) * (*count + 1));
			(*path)[(*count)++] = '1';
			txt_search_huff_rec(tree->right, term, path, count);
		}
	} else {
		*path = (char *) realloc(*path, sizeof(char) * (*count + 1));
		(*path)[*count] = '\0';		
	}
}

/**
 * Retorna uma string representativa do caminho para um símbolo
 * de acordo com a árvore de Huffman da estrutura de dados
 */
char *txt_search_huff(txt_data *txt, char *term) {
	char *path = NULL;
	int count = 0;
	txt_search_huff_rec(txt->huff_tree, term, &path, &count);
	return path;
}

/**
 * Cria a string header_huff com base nos nós folhas com as frenquências
 */
static void txt_make_header_huff(txt_data *txt, node **vector, int size) {
	int i, bufferlen;
	char buffer[255], *path;
	
	for (i = 0; i < size; i++) {
		path = txt_search_huff(txt, vector[i]->term);
		sprintf(buffer, "%s - %s", vector[i]->term, path);
		free(path);
		bufferlen = strlen(buffer);
		txt->header_huff = (char *) realloc(txt->header_huff, sizeof(char) * (txt->size_header_huff + bufferlen + 1));
		memcpy(txt->header_huff + txt->size_header_huff, buffer, bufferlen);
		txt->size_header_huff += bufferlen + 1;
		txt->header_huff[txt->size_header_huff-1] = '\n';
	}
	
	// Salva o símbolo limitador e finaliza a string do cabeçalho
	strcpy(buffer, "-");
	bufferlen = strlen(buffer);
	txt->header_huff = (char *) realloc(txt->header_huff, sizeof(char) * (txt->size_header_huff + bufferlen + 2));
	memcpy(txt->header_huff + txt->size_header_huff, buffer, bufferlen);
	txt->size_header_huff += bufferlen + 1;
	txt->header_huff[txt->size_header_huff-1] = '\n';
	txt->header_huff[txt->size_header_huff] = '\0';
}

/**
 * Inicializa as folhas da árvore de Huffman
 */
static node **txt_new_huff_leafs() {
	int i;
	node **vector = (node **) malloc(sizeof(node *) * 63);

	for (i = 0; i < 63; i++) {
		vector[i] = (node *) malloc(sizeof(node));
		vector[i]->term = (char *) malloc(sizeof(char) * 2);
		vector[i]->frequence = 0;
		vector[i]->left = NULL;
		vector[i]->right = NULL;
		
		// Colocando termos (letras) segundo o código da tabela ASCII
		// [ ]
		if (i < 1) {
			vector[i]->term[0] = 32;
		// [0-9]
		} else if (i < 11) {
			vector[i]->term[0] = 47 + i;
		// [A-Z]
		} else if (i < 37) {
			vector[i]->term[0] = 54 + i;
		// [a-z]
		} else if (i < 63) {
			vector[i]->term[0] = 60 + i;
		}
		vector[i]->term[1] = '\0';
	}
	
	return vector;
}

/**
 * Calcula as frequências dos símbolos no texto não comprimido
 * e manda construir a árvore de Huffman
 * Somente caracteres [ 0-9A-Za-z]
 */
static void txt_build_huff_tree_from_data(txt_data *txt) {
	int i, j, size;
	node **vector_bk;
	
	// Inicialização dos nós folhas
	node **vector = txt_new_huff_leafs();
	
	// Calculando a frequência de cada nó folha nos dados do texto
	i = 0;
	while (txt->data[i] != '\0') {

		// Se é [ ]
		j = txt->data[i] - 32;
		if (j == 0) {
			vector[j]->frequence++;
		} else {
			// Se é [0-9]
			j = txt->data[i] - 47;
			if (j >= 1 && j < 11) {
				vector[j]->frequence++;
			} else {
				// Se é [A-Z]
				j = txt->data[i] - 54;
				if (j >= 11 && j < 37) {
					vector[j]->frequence++;
				} else {
					// Se é [a-z]
					j = txt->data[i] - 60;
					if (j >= 37 && j < 63) {
						vector[j]->frequence++;
					}
				}
			}
		}
		i++;
	}
	
	// Compacta o vetor, removendo símbolos com frequência igual a zero
	size = txt_trim_huff_leafs(&vector);
	
	if (size > 0) {
		// Faz backup do vetor, pois o algortimo de construção da árvore irá alterá-lo
		vector_bk = (node **) malloc(sizeof(node *) * size);
		memcpy(vector_bk, vector, sizeof(node *) * size);
		
		// Manda construir a árvore de Huffman
		txt_build_huff_tree(txt, vector, size);
		
		// Cria o header_huff
		txt_huff_sort(vector_bk, 0, size-1, 0);
		txt_make_header_huff(txt, vector_bk, size);
		free(vector_bk);
		free(vector);
	}
}

/**
 * Lê o texto de um arquivo .txt
 */
void txt_read(txt_data *txt, FILE *fp) {
	char c;
	
	// Percorre o arquivo lendo os bytes
	while (!feof(fp)) {
		fread(&c, sizeof(char), 1, fp);
		if (!feof(fp)) {
			txt->data = (char *) realloc(txt->data, sizeof(char) * (txt->size + 1));
			txt->data[txt->size++] = c;
		}
	}
	txt->data = (char *) realloc(txt->data, sizeof(char) * (txt->size + 1));
	txt->data[txt->size] = '\0';
}

/**
 * Lê o conteúdo um arquivo .huff
 */
void txt_read_huff(txt_data *txt, FILE *fp) {
	char c;
	
	// Lê o cabeçalho do arquivo separadamente
	txt_read_header_huff(txt, fp);
	
	// Percorre o arquivo lendo o restante dos bytes
	while (!feof(fp)) {
		fread(&c, sizeof(char), 1, fp);
		if (!feof(fp)) {
			txt->data_huff = (unsigned char *) realloc(txt->data_huff, sizeof(unsigned char) * (txt->size_huff + 1));
			txt->data_huff[txt->size_huff++] = c;
		}
	}
	txt->data_huff = (unsigned char *) realloc(txt->data_huff, sizeof(unsigned char) * (txt->size_huff + 1));
	txt->data_huff[txt->size_huff] = '\0';
}

/**
 * Lê somente o cabeçalho de uma arquivo .huff
 */
void txt_read_header_huff(txt_data *txt, FILE *fp) {
	char *buffer;
	int bufferlen;

	// Lê um linha do arquivo	
	buffer = fscanln(fp);
	bufferlen = strlen(buffer);
	
	// Enquanto não encontrar o símbolo limitador,
	// 	vai lendo as linhas do arquivo e salvando no cabeçalho
	while (*buffer != '-') {
		txt->header_huff = (char *) realloc(txt->header_huff, sizeof(char) * (txt->size_header_huff + bufferlen + 1));
		memcpy(txt->header_huff + txt->size_header_huff, buffer, bufferlen);
		txt->size_header_huff += bufferlen + 1;
		txt->header_huff[txt->size_header_huff-1] = '\n';
		free(buffer);
		buffer = fscanln(fp);
		bufferlen = strlen(buffer);
	}
	
	// Salva o símbolo limitador e finaliza a string do cabeçalho
	txt->header_huff = (char *) realloc(txt->header_huff, sizeof(char) * (txt->size_header_huff + bufferlen + 2));
	memcpy(txt->header_huff + txt->size_header_huff, buffer, bufferlen);
	txt->size_header_huff += bufferlen + 1;
	txt->header_huff[txt->size_header_huff-1] = '\n';
	txt->header_huff[txt->size_header_huff] = '\0';
	free(buffer);
}

/**
 * Cria a tabela de símbolos
 */
static void txt_huff_make_symbols(txt_data *txt) {
	char *line, *word;
	
	if (txt->size_header_huff > 0) {
	// Enquanto não encontrar o símbolo limitador,
	// 	vai lendo as linhas do cabeçalho e montando a tabela de símbolos
		line = nextln(txt->header_huff);
		while (*line != '-') {
			txt->symbols = (char *) realloc(txt->symbols, sizeof(char) * (txt->count + 1));
			txt->symbols[txt->count] = *line;
		
			// Pega o hífen e descarta.
			word = nextw(line+1);
			free(word);

			// Pega o path e atribui
			word = nextw(NULL);
			txt->paths = (char **) realloc(txt->paths, sizeof(char *) * (txt->count + 1));
			txt->paths[txt->count++] = word;

			free(line);
			line = nextln(NULL);
		}
		free(line);
	}
}

/**
 * Recebe uma string representativa de um byte,
 * 	converte e escreve o byte em data_huff
 */
static void txt_huff_data_write(txt_data *txt, char *str, int start, int end) {
	int value = 0, i, j;
	char buffer[2] = {'\0', '\0'};
	
	if (start - end == 7) {
		for (i = start, j = 7; i <= end; i++, j--) {
			buffer[0] = str[i];
			value += atoi(buffer) * ((int) pow(2.0, (double) j));
		}
		
		txt->data_huff = (unsigned char *) realloc(txt->data_huff, sizeof(unsigned char) * (txt->size_huff + 1));
		txt->data_huff[txt->size_huff++] = (unsigned char) value;
	}
}

/**
 * Recebe o valor do último byte para escrever em data_huff
 */
static void txt_huff_data_write_last_byte(txt_data *txt, int value) {
	txt->data_huff = (unsigned char *) realloc(txt->data_huff, sizeof(unsigned char) * (txt->size_huff + 1));
	txt->data_huff[txt->size_huff++] = (unsigned char) value;
}

/**
 * Compacta o texto usando o algoritmo de Huffman
 */
void txt_compress(txt_data *txt) {
	if (txt->size > 0) {
		int i, start, end, central, flag, bufferlen;
		char buffer[33];
		
		// Constrói a árvore de Huffman para esses dados
		txt_build_huff_tree_from_data(txt);
	
		// Pega a tabela de símbolos a partir do cabeçalho
		txt_huff_make_symbols(txt);

		// Aplica o algoritmo de compactação
		i = 0;
		while (txt->data[i] != '\0') {
			flag = 0;
			start = 0;
			end = txt->count-1;
	
			// Procura caractere na tabela de símbolos					
			while (start <= end) {
				central = (start + end) / 2;
				
				if (txt->data[i] > txt->symbols[central]) {
					start = central + 1;
				} else if (txt->data[i] < txt->symbols[central]) {
					end = central - 1;
				} else {
					flag = 1;
					break;
				}
			}
			// Escreve em buffer a string representativa do caractere
			if (flag) {
				sprintf(buffer, "%s", txt->paths[central]);
				bufferlen = strlen(buffer);
				// Quando tiver mais de 8, já dá pra formar um byte
				// 	então escreve no data_huff
				if (bufferlen >= 8) {
					txt_huff_data_write(txt, buffer, 0, 7);
				}
			}
			
			i++;
		}
		
		// Confere o tamanho do último byte escrito
		bufferlen = strlen(buffer);
		
		// Se menor que 8, preenche a string com zeros e manda escrever o byte
		if (bufferlen > 0 && bufferlen < 8) {
			char zeros[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
			sprintf(buffer, "%s", zeros);
			txt_huff_data_write(txt, buffer, 0, 7);
			
			// Escreve o último byte
			txt_huff_data_write_last_byte(txt, bufferlen);

		} else {
			// Escreve o último byte
			txt_huff_data_write_last_byte(txt, 8);
		}
		
	}
}

/**
 * Recebe um byte e gera uma string representativa com zeros e uns
 */
static char *txt_huff_byte_to_str(unsigned char byte) {
	int size = 0, i, j, len;
	char *tmp = NULL, *str = NULL;

	unsigned char res = byte / 2;
	unsigned char mod = byte % 2;
	tmp = (char *) realloc(tmp, sizeof(char) * (size + 1));
	if (mod == 0) tmp[size++] = '0'; else tmp[size++] = '1';

	// Faz divisões sucessivas e pega o resto colocando na string
	while (res != 0) {
		mod = res % 2;
		res = res / 2;
		tmp = (char *) realloc(tmp, sizeof(char) * (size + 1));
		if (mod == 0) tmp[size++] = '0'; else tmp[size++] = '1';
	}
	tmp = (char *) realloc(tmp, sizeof(char) * (size + 1));
	tmp[size] = '\0';
	
	// Inverte a string (pois a string representativa do binário está ao contrário)
	len = strlen(tmp);
	str = (char *) calloc(len + 1, sizeof(char));
	for (i = 0, j = len-1; i < len; i++, j--) {
		str[j] = tmp[i];
	}
	str[i] = '\0';
	free(tmp);
	
	return str;
}

/**
 * Descompacta o texto comprimido com base na árvore de Huffman
 * TODO
 */
void txt_decompress(txt_data *txt) {
	if (txt->size_huff > 0) {
		int i, j, k, flag, start, end, central;
		char *byte_str, *buffer;
		
		// Pega a tabela de símbolos a partir do cabeçalho
		txt_huff_make_symbols(txt);

		// Aplica o algoritmo de descompactação
		// Lê bytes; gera a string representativa deles; pesquisa nos symbols; pega o símbolo e escreve na str do data

		// TODO
		// Terminar a descompactação...
	}
}

/**
 * Imprime na tela o texto não-comprimido
 */
void txt_print(txt_data *txt) {
	printf("%s", txt->data);
}

/**
 * Imprime na tela o texto compactado com o algoritmo de Huffman
 */
void txt_print_huff(txt_data *txt) {
	printf("%s", txt->header_huff);
// FIXME
// A linha abaixo imprime o conteudo binário compactado com Huffman.
// O Sistema de Submissão de Programas (SSP) não consegue validar essa impressão,
// 	por isso a linha está comentada.

//	printf("%s", txt->data_huff);
}

/**
 * Percorre a árvore de Huffman in-order imprimindo os nós
 */
static void txt_print_huff_tree_rec(node *tree) {
	if (tree != NULL) {
		txt_print_huff_tree_rec(tree->left);
		printf("%6d - %s\n", tree->frequence, tree->term);
		txt_print_huff_tree_rec(tree->right);
	}
}

/**
 * Imprime na tela a árvore de Huffman percorrida em ordem
 */
void txt_print_huff_tree(txt_data *txt) {
	if (txt->huff_tree != NULL) {
		txt_print_huff_tree_rec(txt->huff_tree);
	}
}

/**
 * Libera toda a memória alocada na estrutura de dados e reinicializa
 */
void txt_delete(txt_data *txt) {
	txt_free(txt);
	txt_free_huff(txt);
	txt_free_header_huff(txt);
	txt_free_huff_symbols(txt);
	txt_free_huff_tree(txt);
	
	txt_new(txt);
}

/**
 * Libera somente a memória alocada pelo dados do texto não-comprimido
 */
void txt_free(txt_data *txt) {
	if (txt->data != NULL) {
		free(txt->data);
		txt->data = NULL;
	}
	
	txt->size = 0;
}

/**
 * Libera somente a memória alocada pelos dados da compactação de Huffman
 */
void txt_free_huff(txt_data *txt) {
	if (txt->data_huff != NULL) {
		free(txt->data_huff);
		txt->data_huff = NULL;
	}

	txt->size_huff = 0;
	
	txt_free_huff_tree(txt);
}

/**
 * Libera somente a memória alocada pelo cabeçalho da compactação de Huffman
 */
void txt_free_header_huff(txt_data *txt) {
	if (txt->header_huff != NULL) {
		free(txt->header_huff);
		txt->data_huff = NULL;
	}
	
	txt->size_header_huff = 0;
}

/**
 * Libera somente a memória alocada para a tabela de símbolos de Huffman
 */
void txt_free_huff_symbols(txt_data *txt) {
	if (txt->symbols != NULL) {
		free(txt->symbols);
		txt->symbols = NULL;
	}
	if (txt->paths != NULL) {
		while (txt->count != 0) {
			free(txt->paths[--txt->count]);
		}
		free(txt->paths);
	}
}

/**
 * Percorre a árvore de Huffman em pós-ordem liberando os nós
 */
static void txt_free_huff_tree_rec(node *tree) {
	if (tree->left != NULL) {
		txt_free_huff_tree_rec(tree->left);
	}
	
	if (tree->right != NULL) {
		txt_free_huff_tree_rec(tree->right);
	}

	free(tree->term);
	free(tree);
}

/**
 * Libera somente a árvore de Huffman gerada pela compactação
 */
void txt_free_huff_tree(txt_data *txt) {
	if (txt->huff_tree != NULL) {
		txt_free_huff_tree_rec(txt->huff_tree);
		txt->huff_tree = NULL;
	}
}

