/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * SCC0261 Multimidia
 *
 * Projeto de Multimidia: Compressao de Imagem Digital - Parte 3
 * Grupo 2
 * 	Elias Italiano Rodrigues        7987251
 * 	(*)Ricardo Issamu Fukuda Gunzi  7986729
 * 	Rodolfo Megiato de Lima         7987286
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Codigos de sucesso e erros
#define SUCCESS              0
#define ERR_INVALID_PATH     1
#define ERR_INVALID_BM       2
#define ERR_INVALID_ZBM      3
#define ERR_INVALID_SIZE     4
#define ERR_CANNOT_WRITE     5
#define ERR_HUFFMAN_OVERFLOW 6
#define ERR_UNKNOWN          7

// Tipos de formatos BMP para '.bmp' e ZBM para '.bmpz' (a imagem comprimida)
#define BM  0
#define ZBM 1

// Tamanhos usados no programa
#define NCOLORS        3 // numero de cores
#define FRAMEROWS      8 // 8 pixels por linha
#define FRAMECOLS      8 // 8 pixels por coluna
#define FRAMEROWSIZE  24 // 8 * 3 bytes por linha
#define FRAMESIZE    192 // 8 * 8 * 3 bytes em um frame
#define BLOCKSIZE     64 // 64 bytes em bloco de uma cor R/G/B
#define MINLENGTH      2 // qtde minima de repeticao para comprimir no runlength
#define SKIPSIZE      64 // tamanho maximo para o contador de repeticao no runlength

// Define os nomes das cores base
#define R 0
#define G 1
#define B 2

// Valores para corrigir o type cast da DCT inversa
#define UPPERBOUND 250
#define LOWERBOUND  10

// Tipo de dados byte
typedef unsigned char byte;

// Estrutura para os dados do cabecalho da imagem
typedef struct {
	unsigned short int fileformat;
	unsigned int filesize;
	unsigned short int reserved1;
	unsigned short int reserved2;
	unsigned int pxarrayoffset;

	unsigned int headersize;
	unsigned int width;
	unsigned int height;
	unsigned char other[28]; // demais infos do DIB que nao serao usadas
} HEADER;

// Estrutura de dados para um no' da arvore de Huffman
typedef struct s_node {
	byte occurr;
	byte sss;
	int freq;
	struct s_node *left;
	struct s_node *right;
} NODE;

// Estrutura de dados para um codigo de Huffman
typedef struct {
	unsigned int code;
	byte length;
	byte occurr;
	byte sss;
} CODE;

// Matriz para quantizacao
const double quantmatrix[FRAMEROWS][FRAMECOLS] = {
	{16.0, 11.0, 10.0, 16.0, 24.0,  40.0,  51.0,  61.0 },
	{12.0, 12.0, 14.0, 19.0, 26.0,  58.0,  60.0,  55.0 },
	{14.0, 13.0, 16.0, 24.0, 40.0,  57.0,  69.0,  56.0 },
	{14.0, 17.0, 22.0, 29.0, 51.0,  87.0,  80.0,  62.0 },
	{18.0, 22.0, 37.0, 56.0, 68.0,  109.0, 103.0, 77.0 },
	{24.0, 35.0, 55.0, 64.0, 81.0,  104.0, 113.0, 92.0 },
	{49.0, 64.0, 78.0, 87.0, 103.0, 121.0, 120.0, 101.0},
	{72.0, 92.0, 95.0, 98.0, 112.0, 100.0, 103.0, 99.0 }
};

// Valores pre-calculados para os cossenos das funcoes DCT
const double _cos[FRAMEROWS][FRAMECOLS] = {
	{1.000,  0.981,  0.924,  0.831,  0.707,  0.556,  0.383,  0.195},
	{1.000,  0.831,  0.383, -0.195, -0.707, -0.981, -0.924, -0.556},
	{1.000,  0.556, -0.383, -0.981, -0.707,  0.195,  0.924,  0.831},
	{1.000,  0.195, -0.924, -0.556,  0.707,  0.831, -0.383, -0.981},
	{1.000, -0.195, -0.924,  0.556,  0.707, -0.831, -0.383,  0.981},
	{1.000, -0.556, -0.383,  0.981, -0.707, -0.195,  0.924, -0.831},
	{1.000, -0.831,  0.383,  0.195, -0.707,  0.981, -0.924,  0.556},
	{1.000, -0.981,  0.924, -0.831,  0.707, -0.556,  0.383, -0.195}
};

/**
 * Aloca e retorna um novo no' da arvore de Huffman
 */
NODE *newnode(byte occurr, byte sss, int freq)
{
	NODE *n = (NODE *)malloc(sizeof(NODE));
	n->occurr = occurr;
	n->sss = sss;
	n->freq = freq;
	n->left = NULL;
	n->right = NULL;

	return n;
}

/**
 * Concatena dois no's e retorna um novo no'
 */
NODE *nodeconcat(NODE *left, NODE *right)
{
	if (left == NULL || right == NULL)
		return NULL;

	NODE *n = newnode(0, 0, left->freq + right->freq);
	n->left = left;
	n->right = right;

	return n;
} 

/**
 * Deleta recursivamente a arvore de Huffman, liberando a memoria utilizada
 */
void deletetree(NODE *root)
{
	if (root == NULL)
		return;

	if (root->right != NULL)
		deletetree(root->right);

	if (root->left != NULL)
		deletetree(root->left);

	free(root);
}

/**
 * Insercao ordenado de um novo no' no vetor de no's
 */
void insertionsort(NODE ***vectornodes, int length, NODE *newnode)
{
	int i = 0, inserted = 0;
	*vectornodes = (NODE **)realloc(*vectornodes, sizeof(NODE *) * (length + 1));

	while (i < length) {
		if (((*vectornodes)[i])->freq <= newnode->freq) {
			int j = i;
			NODE *a1 = newnode;
			NODE *a2;

			while (j < length) {
				a2 = ((*vectornodes)[j]);
				((*vectornodes)[j]) = a1;
				a1 = a2;
				j++;
			}

			((*vectornodes)[j]) = a1;
			inserted = 1;
			break;
		}
		i++;
	}
	if (inserted == 0)
		(*vectornodes)[length] = newnode;
}

/**
 * Retorna a quantidade de bits necessarios para representar um valor inteiro
 */
byte getsss(int number)
{
	int i;
	byte amount = 0;

	// Vai "shiftando" o numero para direita ate se obter o valor zero
	// A quantidade de vezes que foi "shiftado" representa os bits necessarios
	//	para escrever esse numero
	for (i = 0; i < 32; i++) {
		number >>= 1;
		amount++;

		if (number == 0)
			break;
	}

	return amount;
}

/**
 * Retorna em uma matriz a frenquencia de ocorrencia de cada par <occur, sss>
 */
int **getfrequence(int nblocks, int **rl[3], int *nrl[3], int **skips[3], int *nskips[3])
{
	int color, n, i, j, occurr;
	byte sss;

	int **freqmatrix = (int **)malloc(65 * sizeof(int *));
	for (i = 0; i < 65; i++)
		freqmatrix[i] = (int *)calloc(33, sizeof(int));

	// Itera para cada cor R, G, B
	for (color = 0; color < NCOLORS; color++) {
		// Itera para cada vetor rl de color
		for (n = 0; n < nblocks; n++) {
			for (i = j = 0; i < nrl[color][n]; i++) {
				if (j < nskips[color][n] && skips[color][n][j] == i) {
					occurr = rl[color][n][i++];
					j++;
				} else {
					occurr = 1;
				}

				sss = getsss(rl[color][n][i]);
				freqmatrix[occurr][sss]++;
			}
		}
	}
	return freqmatrix;
}

/**
 * Constroi a arvore de Huffman a partir da matriz de frequencia dos pares <occurr, sss>
 */
NODE* buildtree(int **freqmatrix)
{
	NODE *tree = NULL;
	NODE **vectornodes = NULL;
	
	int i, j, count = 0;
	
	for (i = 1; i < 65; i++) {
		for (j = 1; j < 33; j++) {
			if (freqmatrix[i][j] > 0) {
				insertionsort(&vectornodes, count, newnode(i, j, freqmatrix[i][j]));
				count++;
			}
		}
	}
	
	// Concatena os nos de forma iterativa e no final obtem-se um arvore de Huffman
	while (count-- > 1) {
		NODE *auxnode = nodeconcat(vectornodes[count], vectornodes[count-1]);
		insertionsort(&vectornodes, count, auxnode);
	}
	
	tree = vectornodes[0];
	if (vectornodes != NULL)
		free(vectornodes);

	return tree;
}

/**
 * Percorre a arvore de Huffman criada e gera os codigos de Huffman
 * Retorna o vetor de CODEs jah ordenados de acordo com o length do codigo (equivale a frequencia)
 */
void generatecodes(NODE *tree, unsigned int path, byte pathlength, CODE **out_codes, int *out_ncodes)
{
	if (tree->right != NULL) {
		unsigned int path2 = path;
		path2 <<= 1;
		path2 += 1;

		generatecodes(tree->right, path2, pathlength + 1, out_codes, out_ncodes);
	}

	if (tree->left != NULL) {
		unsigned int path2 = path;
		path2 <<= 1;

		generatecodes(tree->left, path2, pathlength + 1, out_codes, out_ncodes);
	}
	
	if (tree->right == NULL && tree->left == NULL) {
		CODE newcode;
		newcode.occurr = tree->occurr;
		newcode.sss = tree->sss;
		newcode.length = pathlength;
		newcode.code = path;
		*out_codes = realloc(*out_codes, ((*out_ncodes) + 1) * sizeof(CODE));
		
		// Insercao ordenada dos codes
		int i, j, insert = 0;
		CODE aux, aux2;
		for (i = 0; i < *out_ncodes; i++) {
			if (newcode.length < (*out_codes)[i].length) {
				aux = newcode;
				for (j = i; j < *out_ncodes; j++) {
					aux2 = (*out_codes)[j];
					(*out_codes)[j] = aux;
					aux = aux2;
				}
				(*out_codes)[*out_ncodes] = aux;
				insert = 1;
				break;
			}
		}
		if (insert == 0)
			(*out_codes)[*out_ncodes] = newcode;
		(*out_ncodes)++;
	}
}

/**
 * Criacao dos codigos de Huffman sobre os vetores codificados por carreira
 */
CODE *huffman(int nblocks, int **rl[3], int *nrl[3], int **skips[3], int *nskips[3], int *ncodes)
{
	int i;
	int **freqmatrix = getfrequence(nblocks, rl, nrl, skips, nskips);
	NODE *tree = buildtree(freqmatrix);
	
	for (i = 0; i < 65; i++)
		free(freqmatrix[i]);
	free(freqmatrix);
	
	unsigned int path = 0;
	byte pathlength = 0;
	CODE *codes = NULL;
	*ncodes = 0;
	generatecodes(tree, path, pathlength, &codes, ncodes);
	
	deletetree(tree);

	return codes;
}

/**
 * Confere se a imagem .bmp ou .bmpz em path e' valida
 */
int valid(const char *path, int format)
{
	FILE *fp = fopen(path, "rb");

	// Verifica se a imagem existe
	if (fp == NULL)
		return ERR_INVALID_PATH;

	// Verifica no header da imagem se e' do formato BM ou ZBM
	char fileformat[10];
	if (format == BM) {
		fread(fileformat, 2, 1, fp);
		fileformat[2] = '\0';

		if (strcmp(fileformat, "BM") != 0) {
			fclose(fp);
			return ERR_INVALID_BM;
		}

	} else if (format == ZBM) {
		fread(fileformat, 3, 1, fp);
		fileformat[3] = '\0';

		if (strcmp(fileformat, "ZBM") != 0) {
			fclose(fp);
			return ERR_INVALID_ZBM;
		} else {
			fclose(fp);
			return SUCCESS;
		}
	}

	// Verifica no header da imagem se altura e largura sao multiplos de 8
	int size[2];
	fseek(fp, 18, SEEK_SET);
	fread(size, 8, 1, fp);

	if (size[0] % 8 != 0 || size[1] % 8 != 0) {
		fclose(fp);
		return ERR_INVALID_SIZE;
	}

	fclose(fp);
	return SUCCESS;
}

/**
 * Le e retorna o cabecalho da imagem .bmp ou .bmpz
 */
int prepare(const char *path, HEADER *header, int format)
{
	FILE *fp = fopen(path, "rb");

	if (fp == NULL)
		return ERR_INVALID_PATH;

	// Se for imagem comprimida, pula o byte que indica 'Z'
	if (format == ZBM)
		fseek(fp, 1, SEEK_SET);

	// bmp header
	fread(&header->fileformat,    2, 1, fp);
	fread(&header->filesize,      4, 1, fp);
	fread(&header->reserved1,     2, 1, fp);
	fread(&header->reserved2,     2, 1, fp);
	fread(&header->pxarrayoffset, 4, 1, fp);

	// dib header
	fread(&header->headersize,  4, 1, fp);
	fread(&header->width,       4, 1, fp);
	fread(&header->height,      4, 1, fp);
	fread(header->other,       28, 1, fp);

	fclose(fp);

	return SUCCESS;
}

/**
 * Retorna vetores de matrizes 8x8 com todos os blocos de uma cor
 */
int makeblocks(const char *path, HEADER *header, int nblocks, byte ***blocks[NCOLORS])
{
	FILE *fp = fopen(path, "rb");
	if (fp == NULL)
		return ERR_UNKNOWN;

	int n, i, j, x, y, z; // variaveis de iteracao
	byte frameline[FRAMEROWSIZE]; // buffer para guardar uma linha de frame

	// Quantidade de linhas e colunas
	int nrows = header->height / FRAMEROWS;
	int ncols = header->width  / FRAMECOLS;

	// Aloca toda a memoria necessaria para armazenar todos os blocos
	blocks[R] = (byte ***)malloc(nblocks * sizeof(byte **));
	blocks[G] = (byte ***)malloc(nblocks * sizeof(byte **));
	blocks[B] = (byte ***)malloc(nblocks * sizeof(byte **));
	for (n = 0; n < nblocks; n++) {
		blocks[R][n] = (byte **)malloc(FRAMEROWS * sizeof(byte *));
		blocks[G][n] = (byte **)malloc(FRAMEROWS * sizeof(byte *));
		blocks[B][n] = (byte **)malloc(FRAMEROWS * sizeof(byte *));
		for (i = 0; i < FRAMECOLS; i++) {
			blocks[R][n][i] = (byte *)malloc(FRAMECOLS);
			blocks[G][n][i] = (byte *)malloc(FRAMECOLS);
			blocks[B][n][i] = (byte *)malloc(FRAMECOLS);
		}
	}

	// Le cada frame da imagem
	for (n = i = 0; i < nrows; i++) {

		for (j = 0; j < ncols; j++, n++) {

			// Posiciona no comeco de um frame n
			fseek(fp, header->pxarrayoffset + i * ncols * FRAMESIZE + j * FRAMEROWSIZE, SEEK_SET);

			for (x = 0; x < FRAMEROWS; x++) {
				// Le uma linha x do frame n
				fread(frameline, FRAMEROWSIZE, 1, fp);

				// Pega desta linha somente os pixels da color especificada
				// e atribui ao bloco n na linha x
				for (y = z = 0; y < FRAMECOLS; y++) {
					// Lembrando que um pixel tem 3 bytes na ordem BGR
					blocks[B][n][x][y] = frameline[z++];
					blocks[G][n][x][y] = frameline[z++];
					blocks[R][n][x][y] = frameline[z++];
				}

				// Pula para a linha seguinte do frame n
				fseek(fp, NCOLORS * header->width - FRAMEROWSIZE, SEEK_CUR);
			}
		}
	}

	fclose(fp);

	return SUCCESS;
}

/**
 * Aplica a formula da transformada DCT em n blocos
 */
void dct(byte ***blocks[NCOLORS], int nblocks, double ***dctblocks[NCOLORS])
{
	int n, i, j, x, y;
	double a = 0.125;
	double b = 0.25;
	double c = 0.176777;
	double constant; // a, b, e sao os valores possiveis para constant
	double sum[NCOLORS] = {0.0, 0.0, 0.0};
	double cosines;

	// Aloca memoria para todos os blocos dct
	dctblocks[R] = (double ***)malloc(sizeof(double **) * nblocks);
	dctblocks[G] = (double ***)malloc(sizeof(double **) * nblocks);
	dctblocks[B] = (double ***)malloc(sizeof(double **) * nblocks);
	for (n = 0; n < nblocks; n++) {
		dctblocks[R][n] = (double **)malloc(sizeof(double *) * FRAMEROWS);
		dctblocks[G][n] = (double **)malloc(sizeof(double *) * FRAMEROWS);
		dctblocks[B][n] = (double **)malloc(sizeof(double *) * FRAMEROWS);
		for (i = 0; i < FRAMEROWS; i++) {
			dctblocks[R][n][i] = (double *)malloc(FRAMECOLS * sizeof(double));
			dctblocks[G][n][i] = (double *)malloc(FRAMECOLS * sizeof(double));
			dctblocks[B][n][i] = (double *)malloc(FRAMECOLS * sizeof(double));
		}
	}
	
	// Aplica a formula aos n blocos
	for (n = 0; n < nblocks; n++) {
		for (i = 0; i < FRAMEROWS; i++) {
			for (j = 0; j < FRAMECOLS; j++) {

				// Define valor da constante
				if (i == 0 && j == 0)
					constant = a;
				else if (i != 0 && j != 0)
					constant = b;
				else
					constant = c;

				// Itera as somas da formula
				for (x = 0; x < FRAMEROWS; x++) {
					for (y = 0; y < FRAMECOLS; y++) {
						cosines = _cos[x][i] * _cos[y][j];
						sum[R] += blocks[R][n][x][y] * cosines;
						sum[G] += blocks[G][n][x][y] * cosines;
						sum[B] += blocks[B][n][x][y] * cosines;
					}
				}
				sum[R] *= constant;
				sum[G] *= constant;
				sum[B] *= constant;

				// Faz a quantizacao do bloco que ja' foi transformado pela DCT
				sum[R] /= quantmatrix[i][j];
				sum[G] /= quantmatrix[i][j];
				sum[B] /= quantmatrix[i][j];

				dctblocks[R][n][i][j] = sum[R];
				dctblocks[G][n][i][j] = sum[G];
				dctblocks[B][n][i][j] = sum[B];
				sum[R] = 0.0;
				sum[G] = 0.0;
				sum[B] = 0.0;
			}
		}
	}
}

/**
 * Aplica a formula inversa da transformada DCT em n blocos
 */
void dct_inverse(double ***dctblocks[NCOLORS], int nblocks, byte ***blocks[NCOLORS])
{	
	int n, x, y, i, j;
	double a = 0.125;
	double b = 0.25;
	double c = 0.176777;
	double constant; // a, b, e sao os valores possiveis para constant
	double dequantization[NCOLORS];
	double sum[NCOLORS] = {0.0, 0.0, 0.0};
	double cosines;
	
	// Aloca memoria para os blocos
	blocks[R] = (byte ***)malloc(sizeof(byte **) * nblocks);
	blocks[G] = (byte ***)malloc(sizeof(byte **) * nblocks);
	blocks[B] = (byte ***)malloc(sizeof(byte **) * nblocks);
	for (n = 0; n < nblocks; n++) {
		blocks[R][n] = (byte **)malloc(sizeof(byte *) * FRAMEROWS);
		blocks[G][n] = (byte **)malloc(sizeof(byte *) * FRAMEROWS);
		blocks[B][n] = (byte **)malloc(sizeof(byte *) * FRAMEROWS);
		for (x = 0; x < FRAMEROWS; x++) {
			blocks[R][n][x] = (byte *)malloc(FRAMECOLS * sizeof(byte));
			blocks[G][n][x] = (byte *)malloc(FRAMECOLS * sizeof(byte));
			blocks[B][n][x] = (byte *)malloc(FRAMECOLS * sizeof(byte));
		}
	}
	
	// Aplica a formula aos n blocos
	for (n = 0; n < nblocks; n++) {
		for (x = 0; x < FRAMEROWS; x++) {
			for (y = 0; y < FRAMECOLS; y++) {

				// Itera as somas das formulas
				for (i = 0; i < FRAMEROWS; i++) {
					for (j = 0; j < FRAMECOLS; j++) {

						// Define valor da constante
						if (i == 0 && j == 0)
							constant = a;
						else if (i != 0 && j != 0)
							constant = b;
						else
							constant = c;
						
						// Desquantizacao do valor
						dequantization[R] = dctblocks[R][n][i][j] * quantmatrix[i][j];
						dequantization[G] = dctblocks[G][n][i][j] * quantmatrix[i][j];
						dequantization[B] = dctblocks[B][n][i][j] * quantmatrix[i][j];
						
						cosines = _cos[x][i] * _cos[y][j];
						sum[R] += constant * dequantization[R] * cosines;
						sum[G] += constant * dequantization[G] * cosines;
						sum[B] += constant * dequantization[B] * cosines;
					}
				}

				// Correcao para o cast type do valor retornado pela DCT inversa
				//	caso seja maior que 255, atribui um valor proximo: 250
				//	caso seja menor que 0, atribui um valor proximo: 10
				if (sum[R] > 255.0)
					blocks[R][n][x][y] = UPPERBOUND;
				else if (sum[R] < 0.0)
					blocks[R][n][x][y] = LOWERBOUND;
				else
					blocks[R][n][x][y] = sum[R];

				if (sum[G] > 255.0)
					blocks[G][n][x][y] = UPPERBOUND;
				else if (sum[G] < 0.0)
					blocks[G][n][x][y] = LOWERBOUND;
				else
					blocks[G][n][x][y] = sum[G];

				if (sum[B] > 255.0)
					blocks[B][n][x][y] = UPPERBOUND;
				else if (sum[B] < 0.0)
					blocks[B][n][x][y] = LOWERBOUND;
				else
					blocks[B][n][x][y] = sum[B];

				sum[R] = 0.0;
				sum[G] = 0.0;
				sum[B] = 0.0;
			}
		}
	}
}

/**
 * Vetoriza em "zig-zag" n blocos da DCT
 */
int **vectoring(double ***dctblocks, int nblocks)
{
	int n;

	// Aloca memoria para o vetor
	int **vecblocks = (int **)malloc(nblocks * sizeof(int *));
	for (n = 0; n < nblocks; n++)
		vecblocks[n] = (int *)malloc(BLOCKSIZE * sizeof(int));
	
	for (n = 0; n < nblocks; n++) {
		vecblocks[n][63] = dctblocks[n][0][0];
		vecblocks[n][62] = dctblocks[n][0][1];
		vecblocks[n][61] = dctblocks[n][1][0];
		vecblocks[n][60] = dctblocks[n][2][0];
		vecblocks[n][59] = dctblocks[n][1][1];
		vecblocks[n][58] = dctblocks[n][0][2];
		vecblocks[n][57] = dctblocks[n][0][3];
		vecblocks[n][56] = dctblocks[n][1][2];
		vecblocks[n][55] = dctblocks[n][2][1];
		vecblocks[n][54] = dctblocks[n][3][0];
		vecblocks[n][53] = dctblocks[n][4][0];
		vecblocks[n][52] = dctblocks[n][3][1];
		vecblocks[n][51] = dctblocks[n][2][2];
		vecblocks[n][50] = dctblocks[n][1][3];
		vecblocks[n][49] = dctblocks[n][0][4];
		vecblocks[n][48] = dctblocks[n][0][5];
		vecblocks[n][47] = dctblocks[n][1][4];
		vecblocks[n][46] = dctblocks[n][2][3];
		vecblocks[n][45] = dctblocks[n][3][2];
		vecblocks[n][44] = dctblocks[n][4][1];
		vecblocks[n][43] = dctblocks[n][5][0];
		vecblocks[n][42] = dctblocks[n][6][0];
		vecblocks[n][41] = dctblocks[n][5][1];
		vecblocks[n][40] = dctblocks[n][4][2];
		vecblocks[n][39] = dctblocks[n][3][3];
		vecblocks[n][38] = dctblocks[n][2][4];
		vecblocks[n][37] = dctblocks[n][1][5];
		vecblocks[n][36] = dctblocks[n][0][6];
		vecblocks[n][35] = dctblocks[n][0][7];
		vecblocks[n][34] = dctblocks[n][1][6];
		vecblocks[n][33] = dctblocks[n][2][5];
		vecblocks[n][32] = dctblocks[n][3][4];
		vecblocks[n][31] = dctblocks[n][4][3];
		vecblocks[n][30] = dctblocks[n][5][2];
		vecblocks[n][29] = dctblocks[n][6][1];
		vecblocks[n][28] = dctblocks[n][7][0];
		vecblocks[n][27] = dctblocks[n][7][1];
		vecblocks[n][26] = dctblocks[n][6][2];
		vecblocks[n][25] = dctblocks[n][5][3];
		vecblocks[n][24] = dctblocks[n][4][4];
		vecblocks[n][23] = dctblocks[n][3][5];
		vecblocks[n][22] = dctblocks[n][2][6];
		vecblocks[n][21] = dctblocks[n][1][7];
		vecblocks[n][20] = dctblocks[n][2][7];
		vecblocks[n][19] = dctblocks[n][3][6];
		vecblocks[n][18] = dctblocks[n][4][5];
		vecblocks[n][17] = dctblocks[n][5][4];
		vecblocks[n][16] = dctblocks[n][6][3];
		vecblocks[n][15] = dctblocks[n][7][2];
		vecblocks[n][14] = dctblocks[n][7][3];
		vecblocks[n][13] = dctblocks[n][6][4];
		vecblocks[n][12] = dctblocks[n][5][5];
		vecblocks[n][11] = dctblocks[n][4][6];
		vecblocks[n][10] = dctblocks[n][3][7];
		vecblocks[n][9]  = dctblocks[n][4][7];
		vecblocks[n][8]  = dctblocks[n][5][6];
		vecblocks[n][7]  = dctblocks[n][6][5];
		vecblocks[n][6]  = dctblocks[n][7][4];
		vecblocks[n][5]  = dctblocks[n][7][5];
		vecblocks[n][4]  = dctblocks[n][6][6];
		vecblocks[n][3]  = dctblocks[n][5][7];
		vecblocks[n][2]  = dctblocks[n][6][7];
		vecblocks[n][1]  = dctblocks[n][7][6];
		vecblocks[n][0]  = dctblocks[n][7][7];
	}

	return vecblocks;
}

/**
 * Codificacao por carreira de n blocos vetorizados
 * Em rl sao retornados os vetores codificados e os comprimentos de cada vetor em nrl
 * Em skips sao retornados quais os indices de rl representam uma repeticao
 * e os comprimentos de cada vetor skip estao em nskips
 */
void runlength(int **vecblocks, int nblocks, int ***rl, int **nrl, int ***skips, int **nskips)
{
	int i, k;
	int value, occurr = 1;

	// Aloca memoria para os vetores comprimidos com runlength
	*nrl = (int *)calloc(nblocks, sizeof(int));
	*rl = (int **)malloc(sizeof(byte *) * nblocks);
	for (i = 0; i < nblocks; i++)
		(*rl)[i] = NULL; 
	
	// Aloca memoria para os vetores que guardam os indices de skips
	*nskips = (int *)calloc(nblocks, sizeof(int));
	*skips = (int **)malloc(sizeof(int *) * nblocks);
	for (i = 0; i < nblocks; i++)
		(*skips)[i] = NULL;

	// Para cada bloco vetorizado k
	for (k = 0; k < nblocks; k++) {

		// Pega o primeiro valor
		value = vecblocks[k][0];

		// Percorre os valores procurando por repeticoes
		for (i = 1; i < BLOCKSIZE; i++) {

			// Confere se algum dos valores seguintes sao iguais a este
			if (vecblocks[k][i] == value && occurr < SKIPSIZE) {
				occurr++;

			// Se ocorreu repeticao minima de valores iguais entao marca skip
			} else if (occurr >= MINLENGTH) {
				(*skips)[k] = (int *)realloc((*skips)[k], sizeof(int) * ((*nskips)[k] + 1));
				(*rl)[k] = (int *)realloc((*rl)[k], sizeof(int) * ((*nrl)[k] + 2));
				(*skips)[k][(*nskips)[k]++] = (*nrl)[k];
				(*rl)[k][(*nrl)[k]++] = occurr;
				(*rl)[k][(*nrl)[k]++] = value;
				occurr = 1;
				value = vecblocks[k][i];

			// Se _nao_ ocorreu repeticao minima de valores, simplesmente os copia
			} else {
				(*rl)[k] = (int *)realloc((*rl)[k], sizeof(int) * ((*nrl)[k] + occurr));
				while (occurr >= 1) {
					(*rl)[k][(*nrl)[k]++] = value;
					occurr--;
				}
				occurr = 1;
				value = vecblocks[k][i];
			}
		}

		// Insere os valores restantes no comprimido
		if (occurr >= MINLENGTH) {
			(*skips)[k] = (int *)realloc((*skips)[k], sizeof(int) * ((*nskips)[k] + 1));
			(*rl)[k] = (int*)realloc((*rl)[k], sizeof(int) * ((*nrl)[k] + 2));
			(*skips)[k][(*nskips)[k]++] = (*nrl)[k];
			(*rl)[k][(*nrl)[k]++] = occurr;
			(*rl)[k][(*nrl)[k]++] = value;
			occurr = 1;
		} else {
			(*rl)[k] = (int *)realloc((*rl)[k], sizeof(int) * ((*nrl)[k] + occurr));
			while (occurr >= 1) {
				((*rl)[k])[((*nrl)[k])++] = value;
				occurr--;
			}
			occurr = 1;
		}
	}
}

/**
 * Escreve bits num arquivo
 * Retorna a quantidade de bytes que foram escritos
 * Se fp == NULL, retorna o bit atual do byte que esta' sendo usado
 */
int writebits(FILE *fp, int value, byte sss, int reset)
{
	static byte buffer = 0; // byte a ser escrito no arquivo
	static int shift = 7;  // marca o bit atual a ser usado no byte buffer
	
	if (reset) {
		buffer = 0;
		shift = 7;
		return 0;
	}

	if (fp == NULL)
		return shift;

	int written = 0; // qtde de bytes escritos
	int i = sss - 1; // shift para o i-esimo bit de value

	while (i >= 0) {

		// Copia o i-esimo bit de value para buffer
		if (value & (1 << i))
			buffer += 1 << shift;
		i--;
		shift--;

		// Terminou de preencher um byte, entao
		// 	escreve no arquivo, reseta buffer e conta byte escrito
		if (shift < 0) {
			fwrite(&buffer, 1, 1, fp);
			buffer = 0;
			shift = 7;
			written++;
		}
	}

	return written;
}

/**
 * Escreve o arquivo comprimido ZBM
 */
int writezbm(const char *path, int nblocks, HEADER *header, CODE *codes, int ncodes, int **rl[NCOLORS], int *nrl[NCOLORS], int **skips[NCOLORS], int *nskips[NCOLORS])
{
	char fileformat[] = "ZBM";
	char compr[205];
	strcpy(compr, path);
	strcat(compr, "z");

	FILE *fp = fopen(compr, "wb+");
	if (fp == NULL)
		return ERR_CANNOT_WRITE;

	// bmp header
	fwrite(fileformat,             3, 1, fp);
	fwrite(&header->filesize,      4, 1, fp);
	fwrite(&header->reserved1,     2, 1, fp);
	fwrite(&header->reserved2,     2, 1, fp);
	fwrite(&header->pxarrayoffset, 4, 1, fp);

	// dib header
	fwrite(&header->headersize,  4, 1, fp);
	fwrite(&header->width,       4, 1, fp);
	fwrite(&header->height,      4, 1, fp);
	fwrite(header->other,      28, 1, fp);

	int i, j, k, n, value, color;
	byte occurr, sss;

	int datasize = 0; // qtde de bytes de dados escritos
	writebits(NULL, 0, 0, 1); // reseta os valores estaticos da funcao

	// Para cada rl de R/G/B, escreve os dados codificados com Huffman,
	// ou seja, somente com os bits necessarios para cada par <huffcode, value>
	for (color = 0; color < 3; color++) {

		for (n = 0; n < nblocks; n++) {

			for (i = j = 0; i < nrl[color][n]; i++) {

				if (j < nskips[color][n] && i == skips[color][n][j]) {
					// E' um par <occurr, value>
					occurr = rl[color][n][i++];
					value  = rl[color][n][i];
					j++;
				} else {
					// E' apenas um <value>, portanto occurr = 1
					occurr = 1;
					value  = rl[color][n][i];
				}

				sss = getsss(value);

				// Pesquisa o codigo referente ao par <occurr, sss>
				// na tabela dos codigos de Huffman
				for (k = 0; k < ncodes; k++) {
					if (codes[k].occurr == occurr && codes[k].sss == sss) {
						// Escreve no arquivo os valores <huffcode, value>
						// usando somente os bits necessarios

						datasize += writebits(fp, codes[k].code, codes[k].length, 0);
						datasize += writebits(fp, value, sss, 0);

						break;
					}
				}
			}
		}
	}

	// Confere quantos bits foram utilizados no ultimo byte de dados escrito
	int datalastbyte = writebits(NULL, 0, 0, 0);
	if (datalastbyte != 7) {
		// Termina de escrever o ultimo byte com zeros
		datasize += writebits(fp, 0, datalastbyte + 1, 0);
	}

	int huffsize = 0; // qtde de bytes escritos

	// Escrevendo os codigos de Huffman
	huffsize += ncodes * 3;
	for (n = 0; n < ncodes; n++) {
		fwrite(&codes[n].length, 1, 1, fp);
		fwrite(&codes[n].occurr, 1, 1, fp);
		fwrite(&codes[n].sss,    1, 1, fp);
	}
	for (n = 0; n < ncodes; n++) {
		huffsize += writebits(fp, codes[n].code, codes[n].length, 0);
	}

	// Confere quantos bits foram utilizados no ultimo byte do huffman escrito
	int hufflastbyte = writebits(NULL, 0, 0, 0);
	if (hufflastbyte != 7) {
		// Termina de escrever o ultimo byte com zeros
		huffsize += writebits(fp, 0, hufflastbyte + 1, 0);
	}

	// Escrevendo os valores de metadados

	fwrite(&datasize, sizeof(int), 1, fp);
	fwrite(&huffsize, sizeof(int), 1, fp);

	fwrite(&ncodes, sizeof(int), 1, fp);
	fwrite(&nblocks, sizeof(int), 1, fp);

	fclose(fp);

	return SUCCESS;
}

/**
 * Escreve o arquivo descomprimido BM
 */
int writebm(const char *path, HEADER *header, byte ***blocks[NCOLORS], int nblocks)
{
	char decompr[215];
	strcpy(decompr, path);
	decompr[strlen(decompr)-5] = '\0';
	strcat(decompr, "_restored.bmp"); // anexa "_restored" no nome para diferenciar do arquivo original

	FILE *fp = fopen(decompr, "wb+");
	if (fp == NULL)
		return ERR_CANNOT_WRITE;

	// bmp header
	fwrite(&header->fileformat,    2, 1, fp);
	fwrite(&header->filesize,      4, 1, fp);
	fwrite(&header->reserved1,     2, 1, fp);
	fwrite(&header->reserved2,     2, 1, fp);
	fwrite(&header->pxarrayoffset, 4, 1, fp);

	// dib header
	fwrite(&header->headersize,  4, 1, fp);
	fwrite(&header->width,       4, 1, fp);
	fwrite(&header->height,      4, 1, fp);
	fwrite(header->other,      28, 1, fp);

	int n, i, j, x, y; // variaveis de iteracao

	// Quantidade de linhas e colunas
	int nrows = header->height / FRAMEROWS;
	int ncols = header->width  / FRAMECOLS;

	// Le cada frame n da imagem
	for (n = i = 0; i < nrows; i++) {

		for (j = 0; j < ncols; j++, n++) {

			// Posiciona no comeco de um frame n
			fseek(fp, header->pxarrayoffset + i * ncols * FRAMESIZE + j * FRAMEROWSIZE, SEEK_SET);

			// Vai escrevendo pixel a pixel cada linha do bloco n
			for (x = 0; x < FRAMEROWS; x++) {
				for (y = 0; y < FRAMECOLS; y++) {
					// Ordem BGR
					fwrite(&blocks[B][n][x][y], 1, 1, fp);
					fwrite(&blocks[G][n][x][y], 1, 1, fp);
					fwrite(&blocks[R][n][x][y], 1, 1, fp);
				}
				// Pula para a linha seguinte do frame n no arquivo
				fseek(fp, NCOLORS * header->width - FRAMEROWSIZE, SEEK_CUR);
			}
		}
	}

	fclose(fp);

	return SUCCESS;
}

/**
 * Le e retorna somente os ultimos inteiros de um arquivo comprimido que sao os metadados
 */
int getmeta(const char *path, int *datasize, int *huffsize, int *ncodes, int *nblocks)
{
	FILE *fp = fopen(path, "rb");

	if (fp == NULL)
		return ERR_INVALID_PATH;

	fseek(fp, -4 * sizeof(int), SEEK_END);

	fread(datasize, sizeof(int), 1, fp);
	fread(huffsize, sizeof(int), 1, fp);
	fread(ncodes, sizeof(int), 1, fp);
	fread(nblocks, sizeof(int), 1, fp);

	fclose(fp);

	return SUCCESS;
}

/**
 * Le os bytes de dados de uma arquivo comprimido ZBM
 */
byte *getdata(const char *path, int offset, int datasize)
{
	FILE *fp = fopen(path, "rb");

	if (fp == NULL)
		return NULL;

	// Posiciona onde comecam os dados
	fseek(fp, offset, SEEK_SET);

	// Aloca toda memoria necessaria e le os dados
	byte *data = (byte *)malloc(datasize);
	fread(data, datasize, 1, fp);

	fclose(fp);

	return data;
}

/**
 * Processo de desvetorizacao de um bloco, ou seja,
 * dado um vetor de comprimento 64 ele recompoem o bloco 8x8 correspondente
 */
void unvectoring(double **block, double *vector)
{
	block[0][0] = vector[63];
	block[0][1] = vector[62];
	block[1][0] = vector[61];
	block[2][0] = vector[60];
	block[1][1] = vector[59];
	block[0][2] = vector[58];
	block[0][3] = vector[57];
	block[1][2] = vector[56];
	block[2][1] = vector[55];
	block[3][0] = vector[54];
	block[4][0] = vector[53];
	block[3][1] = vector[52];
	block[2][2] = vector[51];
	block[1][3] = vector[50];
	block[0][4] = vector[49];
	block[0][5] = vector[48];
	block[1][4] = vector[47];
	block[2][3] = vector[46];
	block[3][2] = vector[45];
	block[4][1] = vector[44];
	block[5][0] = vector[43];
	block[6][0] = vector[42];
	block[5][1] = vector[41];
	block[4][2] = vector[40];
	block[3][3] = vector[39];
	block[2][4] = vector[38];
	block[1][5] = vector[37];
	block[0][6] = vector[36];
	block[0][7] = vector[35];
	block[1][6] = vector[34];
	block[2][5] = vector[33];
	block[3][4] = vector[32];
	block[4][3] = vector[31];
	block[5][2] = vector[30];
	block[6][1] = vector[29];
	block[7][0] = vector[28];
	block[7][1] = vector[27];
	block[6][2] = vector[26];
	block[5][3] = vector[25];
	block[4][4] = vector[24];
	block[3][5] = vector[23];
	block[2][6] = vector[22];
	block[1][7] = vector[21];
	block[2][7] = vector[20];
	block[3][6] = vector[19];
	block[4][5] = vector[18];
	block[5][4] = vector[17];
	block[6][3] = vector[16];
	block[7][2] = vector[15];
	block[7][3] = vector[14];
	block[6][4] = vector[13];
	block[5][5] = vector[12];
	block[4][6] = vector[11];
	block[3][7] = vector[10];
	block[4][7] = vector[9];
	block[5][6] = vector[8];
	block[6][5] = vector[7];
	block[7][4] = vector[6];
	block[7][5] = vector[5];
	block[6][6] = vector[4];
	block[5][7] = vector[3];
	block[6][7] = vector[2];
	block[7][6] = vector[1];
	block[7][7] = vector[0];
}

/**
 * Recupera os blocos de uma cor a partir de um conjunto de dados comprimidos lidos de um arquivo ZBM
 * Para isso, efetua a decodificacao dos dados usando os codigos de Huffman
 *
 * A cada chamada da funcao, ela recupera os blocos de uma cor
 * Na primeira chamada recupera da cor R, na segunda da G e na terceira da B
 */
double ***getblocks(int nblocks, CODE *codes, int ncodes, byte *data, int datasize, int *err, int reset)
{
	static int k = 0; // posicao do byte atual sendo lido do conjunto de dados
	static int i = 7; // marca o bit sendo usado do byte atual no conjunto de dados
	
	if (reset) {
		k = 0;
		i = 7;
		return NULL;
	}

	int j, n, v, s, decode; // variaveis auxiliares e de iteracao
	byte length;
	unsigned int code;

	// Variavel de erro, caso encontre um codigo de huffman com mais de 32 bits
	*err = SUCCESS;

	// buffers
	int buffer;
	double *vecbuffer = (double *)malloc(BLOCKSIZE * sizeof(double));

	// Alocando toda memoria necessaria para os n blocos
	double ***blocks = (double ***)malloc(nblocks * sizeof(double **));
	for (n = 0; n < nblocks; n++) {
		blocks[n] = (double **)malloc(FRAMEROWS * sizeof(double *));
		for (j = 0; j < FRAMEROWS; j++) {
			blocks[n][j] = (double *)malloc(FRAMECOLS * sizeof(double));
		}
	}

	buffer = 0;
	length = 0;
	code = 0;
	v = 0;

	// Para cada bloco, ele pega os dados e preenche num vetor
	// Apos isso, e' feita a desvetorizacao e entao compoem-se um bloco
	for (n = 0; n < nblocks && k < datasize; ) {

		// Copia o i-esimo bit de data[k] para code
		code <<= 1;
		if (data[k] & (1 << i))
			code += 1;
		i--;
		length++;

		if (i < 0) {
			// Terminou de ler um byte de data[]
			i = 7;
			k++;
		}
		if (length > 32) {
			// Existe um codigo de huffman com mais de 32 bits!
			*err = ERR_HUFFMAN_OVERFLOW;
			length = 0;
			code = 0;
		}

		// Pesquisa se o que ja foi lido e' um codigo de huffman
		for (decode = j = 0; j < ncodes; j++) {
			if (codes[j].length == length && codes[j].code == code) {
				decode = 1;
				break;
			}
		}

		// Decodificacao do codigo de Huffman
		if (decode) {
			buffer = 0;
			for (s = 0; s < codes[j].sss; s++) {

				// Copia o i-esimo bit de data[k] para buffer
				buffer <<= 1;
				if (data[k] & (1 << i))
					buffer += 1;

				i--;
				if (i < 0) {
					// Terminou de ler um byte de data[]
					i = 7;
					k++;
				}
			}
			// Escreve no vetorbuffer occurr vezes o valor lido
			for (s = 0; s < codes[j].occurr; s++) {
				vecbuffer[v++] = (double)buffer;
				if (v > 63) {
					// Completou-se um vetor, entao
					// 	faz a desvetorizacao e guarda um bloco
					unvectoring(blocks[n++], vecbuffer);
					v = 0;
				}
			}
			code = 0;
			length = 0;
		}
	}

	free(vecbuffer);

	return blocks;
}

/**
 * Le e retorna a tabela de Huffman de um arquivo comprimido ZBM
 */
CODE *getcodes(const char *path, int offset, int huffsize, int ncodes)
{
	FILE *fp = fopen(path, "rb");

	if (fp == NULL)
		return NULL;

	// Posiciona onde comecam os dados do codigo de huffman
	fseek(fp, offset, SEEK_SET);

	// Aloca toda memoria necessaria e le os dados do codigo
	byte *huffdata = (byte *)malloc(huffsize);
	fread(huffdata, huffsize, 1, fp);

	fclose(fp);

	// Criar estruturas CODE a partir dos dados lidos
	CODE *codes = (CODE *)malloc(ncodes * sizeof(CODE));

	int n, k = 0;
	for (n = 0; n < ncodes; n++) {
		codes[n].length = huffdata[k++];
		codes[n].occurr = huffdata[k++];
		codes[n].sss    = huffdata[k++];
	}

	unsigned int code;
	int i = 7;
	byte length;

	// Recupera os codigos de huffman lendo bits
	for (n = 0; n < ncodes && k < huffsize; n++) {
		code = 0;
		for (length = 0; length < codes[n].length; length++) {
			code <<= 1;

			// Copia o i-esimo bit de huffdata[k] para code
			if (huffdata[k] & (1 << i))
				code += 1;

			i--;
			if (i < 0) {
				i = 7;
				k++;
			}
		}
		codes[n].code = code; // guarda o code lido
	}

	free(huffdata);

	return codes;
}

/**
 * Comprime uma imagem no formato .bmp
 */
int compress(const char *path)
{
	// Variaveis auxiliares
	int n, i, ret;

	/* ------------ PREPARACAO DA IMAGEM -------------------------------- */

	// Validacao da imagem
	ret = valid(path, BM);
	if (ret != SUCCESS)
		return ret;

	// Leitura dos dados do cabecalho
	HEADER *header = (HEADER *)malloc(sizeof(HEADER));
	ret = prepare(path, header, BM);
	if (ret != SUCCESS)
		return ret;

	/* ------------ PREPARACAO DOS BLOCOS ------------------------------- */

	// Calcula a quantidade frames 8x8 que contem na imagem
	int nblocks = (header->height / FRAMEROWS) * (header->width  / FRAMECOLS);

	// Separacao dos blocos R, G, B
	byte ***blocks[NCOLORS];
	ret = makeblocks(path, header, nblocks, blocks);
	if (ret != SUCCESS)
		return ret;

	/* ---- APLICACAO DA TRANSFORMADA DCT NOS BLOCOS E QUANTIZACAO ------ */

	double ***dctblocks[NCOLORS];
	dct(blocks, nblocks, dctblocks);

	// Liberacao de memoria dos blocks
	for (n = 0; n < nblocks; n++) {
		for (i = 0; i < FRAMEROWS; i++) {
			free(blocks[R][n][i]);
			free(blocks[G][n][i]);
			free(blocks[B][n][i]);
		}
		free(blocks[R][n]);
		free(blocks[G][n]);
		free(blocks[B][n]);
	}
	free(blocks[R]);
	free(blocks[G]);
	free(blocks[B]);

	/* ------------ VETORIZACAO DOS BLOCOS DCT -------------------------- */

	int **vecblocks[NCOLORS];
	vecblocks[R] = vectoring(dctblocks[R], nblocks);
	vecblocks[G] = vectoring(dctblocks[G], nblocks);
	vecblocks[B] = vectoring(dctblocks[B], nblocks);

	// Liberacao de memoria dos dctblocks
	for (n = 0; n < nblocks; n++) {
		for (i = 0; i < FRAMEROWS; i++) {
			free(dctblocks[R][n][i]);
			free(dctblocks[G][n][i]);
			free(dctblocks[B][n][i]);
		}
		free(dctblocks[R][n]);
		free(dctblocks[G][n]);
		free(dctblocks[B][n]);
	}
	free(dctblocks[R]);
	free(dctblocks[G]);
	free(dctblocks[B]);

	/* ------------ RUN-LENGTH ------------------------------------------ */

	int **rl[NCOLORS];
	int *nrl[NCOLORS];
	int **skips[NCOLORS];
	int *nskips[NCOLORS];

	runlength(vecblocks[R], nblocks, &rl[R], &nrl[R], &skips[R], &nskips[R]);
	runlength(vecblocks[G], nblocks, &rl[G], &nrl[G], &skips[G], &nskips[G]);
	runlength(vecblocks[B], nblocks, &rl[B], &nrl[B], &skips[B], &nskips[B]);

	// Liberacao de memoria dos vecblocks
	for (n = 0; n < nblocks; n++) {
		free(vecblocks[R][n]);
		free(vecblocks[G][n]);
		free(vecblocks[B][n]);
	}
	free(vecblocks[R]);
	free(vecblocks[G]);
	free(vecblocks[B]);

	/* ------------ HUFFMAN --------------------------------------------- */

	CODE *codes;
	int ncodes;
	codes = huffman(nblocks, rl, nrl, skips, nskips, &ncodes);

	/* ------------ GRAVA BITS ------------------------------------------ */

	ret = writezbm(path, nblocks, header, codes, ncodes, rl, nrl, skips, nskips);

	// Liberacao de memoria dos rl, nrl, skips, nskips
	for (n = 0; n < nblocks; n++) {
		free(rl[R][n]);
		free(rl[G][n]);
		free(rl[B][n]);
		free(skips[R][n]);
		free(skips[G][n]);
		free(skips[B][n]);
	}
	free(rl[R]);
	free(rl[G]);
	free(rl[B]);
	free(nrl[R]);
	free(nrl[G]);
	free(nrl[B]);
	free(skips[R]);
	free(skips[G]);
	free(skips[B]);
	free(nskips[R]);
	free(nskips[G]);
	free(nskips[B]);

	// Liberacao de memoria do header e codes
	free(header);
	free(codes);

	if (ret != SUCCESS)
		return ret;

	return SUCCESS;
}

/**
 * Descomprime uma imagem comprimida no formato .bmpz
 */
int decompress(const char *path)
{
	// Variaveis auxiliares
	int n, i, ret;

	/* ------------ PREPARACAO DA IMAGEM -------------------------------- */

	// Validacao da imagem
	ret = valid(path, ZBM);
	if (ret != SUCCESS)
		return ret;

	// Leitura dos dados do cabecalho
	HEADER *header = (HEADER *)malloc(sizeof(HEADER));
	ret = prepare(path, header, ZBM);
	if (ret != SUCCESS)
		return ret;

	/* ------------ LEITURA DOS METADADOS ------------------------------- */

	int datasize, huffsize;
	int ncodes, nblocks;
	getmeta(path, &datasize, &huffsize, &ncodes, &nblocks);

	/* ------------ LEITURA DOS DADOS ----------------------------------- */

	byte *data  = getdata(path, header->pxarrayoffset + 1, datasize);
	CODE *codes = getcodes(path, header->pxarrayoffset + 1 + datasize, huffsize, ncodes);
	if (data == NULL || codes == NULL)
		return ERR_UNKNOWN;

	/* ------------ RECUPERACAO DOS BLOCOS DCT R, G, B ------------------- */

	double ***dctblocks[NCOLORS];
	int err[NCOLORS];
	getblocks(0, NULL, 0, NULL, 0, NULL, 1); // resetando os valores estaticos da funcao
	dctblocks[R] = getblocks(nblocks, codes, ncodes, data, datasize, &err[R], 0);
	dctblocks[G] = getblocks(nblocks, codes, ncodes, data, datasize, &err[G], 0);
	dctblocks[B] = getblocks(nblocks, codes, ncodes, data, datasize, &err[B], 0);

	if (err[R] != SUCCESS)
		return err[R];
	if (err[G] != SUCCESS)
		return err[G];
	if (err[B] != SUCCESS)
		return err[B];

	// Liberacao de memoria de data e codes
	free(data);
	free(codes);

	/* --- APLICACAO DA TRANSFORMADA DCT INVERSA E DESQUANTIZACAO DOS BLOCOS --- */

	byte ***blocks[NCOLORS];
	dct_inverse(dctblocks, nblocks, blocks);

	// Liberacao de memoria dos dctblocks
	for (n = 0; n < nblocks; n++) {
		for (i = 0; i < FRAMEROWS; i++) {
			free(dctblocks[R][n][i]);
			free(dctblocks[G][n][i]);
			free(dctblocks[B][n][i]);
		}
		free(dctblocks[R][n]);
		free(dctblocks[G][n]);
		free(dctblocks[B][n]);
	}
	free(dctblocks[R]);
	free(dctblocks[G]);
	free(dctblocks[B]);

	/* ------------ GRAVA DADOS E RECONSTROI O ARQUIVO ORIGINAL---------- */

	ret = writebm(path, header, blocks, nblocks);

	// Liberacao de memoria dos blocks
	for (n = 0; n < nblocks; n++) {
		for (i = 0; i < FRAMEROWS; i++) {
			free(blocks[R][n][i]);
			free(blocks[G][n][i]);
			free(blocks[B][n][i]);
		}
		free(blocks[R][n]);
		free(blocks[G][n]);
		free(blocks[B][n]);
	}
	free(blocks[R]);
	free(blocks[G]);
	free(blocks[B]);

	// Liberacao de header
	free(header);

	if (ret != SUCCESS)
		return ret;

	return SUCCESS;
}

// Variaveis com o conteudo que imprime para cada tela do programa

const char ui_home[] = "\
------------------------------\n\
 BMP 24-bit                   \n\
------------------------------\n\
\n\
    1. Compress\n\
    2. Decompress\n\
\n\
    0. Exit\n\
\n\
\n\
    > Inform option: \
";

const char ui_compress[] = "\
------------------------------\n\
 BMP 24-bit > Compress        \n\
------------------------------\n\
\n\
    1. Select image\n\
    2. Apply compression\n\
\n\
    0. Back\n\
\n\
\n\
    > Inform option: \
";

const char ui_compress_select[] = "\
--------------------------------------\n\
 BMP 24-bit > Compress > Select image \n\
--------------------------------------\n\
\n\
    > Inform image path (or 0 to cancel): \
";

const char ui_compress_apply[] = "\
-------------------------------------------\n\
 BMP 24-bit > Compress > Apply compression \n\
-------------------------------------------\n\
\n\
";

const char ui_decompress[] = "\
------------------------------\n\
 BMP 24-bit > Decompress      \n\
------------------------------\n\
\n\
    1. Select image\n\
    2. Apply decompression\n\
\n\
    0. Back\n\
\n\
\n\
    > Inform option: \
";

const char ui_decompress_select[] = "\
----------------------------------------\n\
 BMP 24-bit > Decompress > Select image \n\
----------------------------------------\n\
\n\
    > Inform image path (or 0 to cancel): \
";

const char ui_decompress_apply[] = "\
-----------------------------------------------\n\
 BMP 24-bit > Decompress > Apply decompression \n\
-----------------------------------------------\n\
\n\
";

/**
 * Mostra uma tela de menus
 */
void screen(const char *str)
{
	system("cls"); // FIXME "cls" para Windows; "clear" para Linux
	printf("%s", str);
}

/**
 * Funcao principal do programa
 */
int main(int argc, char **argv)
{
	int option;

	do {
		screen(ui_home);
		scanf("%d", &option);

		// Compress
		if (option == 1) {
			int option;
			char path[200];

			do {
				screen(ui_compress);
				scanf("%d", &option);

				// Compress > Select image
				if (option == 1) {
					screen(ui_compress_select);
					scanf("%s", path);
				}

				// Compress > Apply compression
				if (option == 2) {
					int ret;

					screen(ui_compress_apply);
					ret = compress(path);

					if (ret == SUCCESS)
						printf("\n\tImage successfully compressed!\n\n\tFile '%sz' created.\n", path);

					else if (ret == ERR_INVALID_PATH)
						printf("\n\tERROR: invalid image path\n");

					else if (ret == ERR_INVALID_BM)
						printf("\n\tERROR: image format not supported\n");

					else if (ret == ERR_INVALID_SIZE)
						printf("\n\tERROR: width and height must be a multiple of 8\n");

					else if (ret == ERR_CANNOT_WRITE)
						printf("\n\tERROR: output file could not be written\n");

					else if (ret == ERR_UNKNOWN)
						printf("\n\tERROR: unknown\n");

					printf("\n\tPress enter to continue\n");
					getchar();
					getchar();
				}

			} while (option != 0);
		}

		// Decompress
		if (option == 2) {
			int option;
			char path[200];

			do {
				screen(ui_decompress);
				scanf("%d", &option);

				// Decompress > Select image
				if (option == 1) {
					screen(ui_decompress_select);
					scanf("%s", path);
				}

				// Decompress > Apply decompression
				if (option == 2) {
					int ret;

					screen(ui_decompress_apply);
					ret = decompress(path);

					if (ret == SUCCESS) {
						path[strlen(path)-5] = '\0';
						printf("\n\tImage successfully decompressed!\n\n\tFile '%s_restored.bmp' created.\n", path);
					}
					else if (ret == ERR_INVALID_PATH)
						printf("\n\tERROR: invalid image path\n");

					else if (ret == ERR_INVALID_ZBM)
						printf("\n\tERROR: image compressed format not supported or corrupted\n");

					else if (ret == ERR_CANNOT_WRITE)
						printf("\n\tERROR: output file could not be written\n");

					else if (ret == ERR_HUFFMAN_OVERFLOW)
						printf("\n\tERROR: image contains Huffman codes larger than 32 bits\n");

					else if (ret == ERR_UNKNOWN)
						printf("\n\tERROR: unknown\n");

					printf("\n\tPress enter to continue\n");
					getchar();
					getchar();
				}

			} while (option != 0);
		}

	} while (option != 0);

	return EXIT_SUCCESS;
}

