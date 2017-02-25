/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * SCC0251 Processamento de Imagens
 * Trabalho 1: Contrast
 * 
 * Elias Italiano Rodrigues 7987251
 * 2015-03-10
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include "../inc/pgm.h"

/**
 * Funcao principal
 */
int main(int argc, char **argv)
{
	// Fator usado para escolher um 'min' e 'max' do histograma que tenha 
	//  	ocorrencia de pelo menos: (value) * (total de pixels da imagem)
	double value = 0.001, lower_bound;

	// Variaveis de opcoes do programa
	int c;
	char *input = NULL, *output = NULL;
	int do_output = 1, show_help = 0, show_version = 0;

	// Variaveis para tratar a imagem
	pgm_t *img = NULL;
	int histogram[256], min, max;
	int ret = 0, i, j, k;
	double tmp = 0.0;
	
	// Opcoes do programa com getopt
	struct option long_options[] = {
		{"input",     required_argument, NULL,         'i'},
		{"output",    required_argument, NULL,         'o'},
		{"value",     required_argument, NULL,         'v'},
		{"no-output", no_argument,       &do_output,    0},
		{"help",      no_argument,       &show_help,    1},
		{"version",   no_argument,       &show_version, 1},
		{NULL, 0, NULL, 0}
	};

	// Tratamento dos argumentos usando getopt
	while ((c = getopt_long(argc, argv, "i:o:v:", long_options, NULL)) != -1) {
		switch (c) {
			case 'i':
				input = optarg;
				break;
			case 'o':
				output = optarg;
				break;
			case 'v':
				value = atof(optarg);
				if (value < 0.0 || value > 1.0) {
					fprintf(stderr, "Invalid argument for option -v, --value\n");
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
printf("This program creates a PGM file with enhanced constrast\n\
List of options and arguments for this program\n\
    -i, --input <path>        set <path> as the input file\n\
    -o, --output <path>       set <path> as the output file\n\
    -v, --value <number>      ajust the contrast at <number> factor; it's\n\
                              0.001 by default\n\
    --no-output               do not create the output file\n\
    --help                    display this help and exit\n\
    --version                 output version information and exit\n");
		return EXIT_SUCCESS;
	}

	if (show_version) {
printf("contrast v0.1\nElias Rodrigues <elias.rodrigues@usp.br>\n2015-03-10\n");
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

	// Calcular histograma da imagem
	for (k = 0; k <= img->max_grey; k++) {
		histogram[k] = 0;
	}
	for (i = 0; i < img->image_height; i++) {
		for (j = 0; j < img->image_width; j++) {
			histogram[img->data[i][j]]++;
		}
	}

	// Encontra os valores minimo e maximo do histograma

	// Valores default de 'min' e 'max'
	min = 0;
	max = 255;

	// Valores de 'min' e 'max' sem considerar 'value'
	for (k = 0; k <= img->max_grey; k++) {
		if (histogram[k] > 0) {
			min = k;
			break;
		}
	}
	for (k = img->max_grey; k >= 0; k--) {
		if (histogram[k] > 0) {
			max = k;
			break;
		}
	}
	
	// Valores de 'min' e 'max' considerando 'value' (preferencialmente usa estes)
	lower_bound = value * (double)img->image_width * (double)img->image_height;
	for (k = min; k <= max; k++) {
		if (histogram[k] > 0 && (double)histogram[k] >= lower_bound) {
			min = k;
			break;
		}
	}
	for (k = max; k >= min; k--) {
		if (histogram[k] > 0 && (double)histogram[k] >= lower_bound) {
			max = k;
			break;
		}
	}
	
	// "Alonga" o histograma e redefine valores da imagem
	img->max_grey = 255;
	for (i = 0; i < img->image_height; i++) {
		for (j = 0; j < img->image_width; j++) {
			tmp = ((double)img->max_grey * (double)((int)img->data[i][j] - min)) / (double)(max - min);
			if (tmp > 255.0) {
				img->data[i][j] = 255;
			} else if (tmp < 0.0) {
				img->data[i][j] = 0;
			} else {
				img->data[i][j] = (byte)tmp;
			}
		}
	}

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

