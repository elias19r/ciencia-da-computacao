/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * SCC0251 Processamento de Imagens
 * Trabalho 1: Flood-fill
 * 
 * Elias Italiano Rodrigues 7987251
 * 2015-03-17
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "../inc/pgm.h"

/**
 * Implementacao de uma pilha
 */
typedef struct {
	int count;
	int **items;
} stack_t;

void stack_init(stack_t *s)
{
	s->count = 0;
	s->items = NULL;
}

void stack_destroy(stack_t *s)
{
	int i;
	for (i = 0; i < s->count; i++) {
		free(s->items[i]);
	}
	free(s->items);
}

void stack_add(stack_t *s, int *p)
{
	s->items = (int **)realloc(s->items, sizeof(int *) * (s->count + 1));
	s->items[s->count] = (int *)malloc(sizeof(int) * 2);
	s->items[s->count][0] = p[0];
	s->items[s->count][1] = p[1];
	s->count++;
}

void stack_remove(stack_t *s)
{
	free(s->items[s->count-1]);
	s->items = (int **)realloc(s->items, sizeof(int *) * (s->count - 1));
	s->count--;
}

int *stack_top(stack_t *s)
{
	return s->items[s->count-1];
}

int stack_empty(stack_t *s)
{
	if (s->count > 0) {
		return 0;
	}
	return 1;
}

/**
 * Preenche uma regiao da imagem que envolve o pixel de coordenadas p=(px,py)
 * Onde px e' a coluna e py e' a linha
 */
void flood_fill(pgm_t *img, int *p, int color)
{
	// Os quatros vizinhos de p=(px,py)
	int v[4][2];

	// Pilha
	stack_t s;
	int *t, i;
	
	// Cor do pixel inicial
	int p_color = img->data[p[1]][p[0]];
	
	stack_init(&s);
	stack_add(&s, p);
	
	while (!stack_empty(&s)) {
		t = stack_top(&s);

		// Pinta o pixel		
		img->data[t[1]][t[0]] = color;

		// Encontra os 4 vizinhos
		v[0][0] = t[0];
		v[0][1] = t[1] - 1;

		v[1][0] = t[0] + 1;
		v[1][1] = t[1];

		v[2][0] = t[0];
		v[2][1] = t[1] + 1;

		v[3][0] = t[0] - 1;
		v[3][1] = t[1];
		
		// Remove atual da pilha
		stack_remove(&s);
		
		// Empilha os vizinhos que nao foram ainda pintados
		for (i = 0; i < 4; i++) {
			if (valid_pixel(img, v[i]) && img->data[v[i][1]][v[i][0]] == p_color) {
				// Confere se nao esta' empilhando novamente o pixel inicial
				// Isso pode acontecer quando o pixel dado estiver na borda
				// 	da area e nao dentro dela
				if (v[i][0] == p[0] && v[i][1] == p[1]) {
					stack_destroy(&s);
					return;
				} else {
					stack_add(&s, v[i]);
				}
			}
		}
	}
	stack_destroy(&s);
}

/**
 * Funcao principal
 */
int main(int argc, char **argv)
{
	// Variaveis de opcoes do programa
	int c;
	char *input = NULL, *output = NULL;
	int do_output = 1, show_help = 0, show_version = 0;
	int p[2];
	int color;

	// Variaveis para tratar a imagem
	pgm_t *img = NULL;
	int i, ret;
	
	// Opcoes do programa com getopt
	struct option long_options[] = {
		{"input",     required_argument, NULL,         'i'},
		{"output",    required_argument, NULL,         'o'},
		{"color",     required_argument, NULL,         'c'},
		{"no-output", no_argument,       &do_output,    0},
		{"help",      no_argument,       &show_help,    1},
		{"version",   no_argument,       &show_version, 1},
		{NULL, 0, NULL, 0}
	};

	// Tratamento dos argumentos usando getopt
	while ((c = getopt_long(argc, argv, "i:o:c:x:y:", long_options, NULL)) != -1) {
		switch (c) {
			case 'i':
				input = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			case 'x':
				p[0] = atoi(optarg);
				if (p[0] < 0) {
					fprintf(stderr, "Invalid argument for option -x\n");
					return EXIT_FAILURE;
				}
				break;
			case 'y':
				p[1] = atoi(optarg);
				if (p[1] < 0) {
					fprintf(stderr, "Invalid argument for option -y\n");
					return EXIT_FAILURE;
				}
				break;
			case 'c':
				color = atoi(optarg);
				if (color < 0 || color > 255) {
					fprintf(stderr, "Invalid argument for option -c, --color\n");
					return EXIT_FAILURE;
				}
				break;
			case '?':
				fprintf(stderr, "An error has occurred while parsing the options and arguments\n");
				return EXIT_FAILURE;
			case 0:
			default:
				break;
		}
	}

	if (show_help) {
printf("This program creates a PGM file with a filled area defined by a pixel\n\
List of options and arguments for this program\n\
    -i, --input <path>        set <path> as the input file\n\
    -o, --output <path>       set <path> as the output file\n\
    -x <number>               coordenates of the chosen pixel\n\
    -y <number>               where x is its columm and y its line\n\
    -c, --color <number>      the color used to fill the area\n\
    --no-output               do not create the output file\n\
    --help                    display this help and exit\n\
    --version                 output version information and exit\n");
		return EXIT_SUCCESS;
	}

	if (show_version) {
printf("flood-fill v0.1\nElias Rodrigues <elias.rodrigues@usp.br>\n2015-03-17\n");
		return EXIT_SUCCESS;
	}

	// Carrega imagem PGM para a memoria	
	if (input == NULL) {
		img = pgm_read("in.pgm"); // nome default de arquivo
	} else {
		img = pgm_read(input);
	}
	if (img == NULL) {
		return EXIT_FAILURE;
	}

	// Valida valores de p
	if (!valid_pixel(img, p)) {
		fprintf(stderr, "Invalid argument for option -p\n");

		// Liberacao de memoria
		for (i = 0; i < img->image_height; i++) {
			free(img->data[i]);
		}
		free(img->data);
		free(img);
		
		return EXIT_FAILURE;	
	}
	
	// Pinta a regiao fechado onde se encontra o pixel p=(px,py)
	flood_fill(img, p, color);

	// Cria arquivo da imagem
	if (do_output) {
		if (output == NULL) {
			ret = pgm_write("out.pgm", img); // nome default de arquivo
		} else {
			ret = pgm_write(output, img);
		}
		if (!ret) {
			// Liberacao de memoria
			for (i = 0; i < img->image_height; i++) {
				free(img->data[i]);
			}
			free(img->data);
			free(img);
			
			return EXIT_FAILURE;
		}
	}
	
	// Liberacao de memoria
	for (i = 0; i < img->image_height; i++) {
		free(img->data[i]);
	}
	free(img->data);
	free(img);
	
	return EXIT_SUCCESS;
}

