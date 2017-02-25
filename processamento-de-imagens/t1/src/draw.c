/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * SCC0251 Processamento de Imagens
 * Trabalho 1: Draw
 * 
 * Elias Italiano Rodrigues 7987251
 * 2015-03-03
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

/**
 * Funcao principal
 */
int main(int argc, char **argv)
{
	FILE *fp;
	int c;
	char *name = NULL, *type = NULL, *figure = NULL;
	int do_output = 1, show_help = 0, show_version = 0, do_fill = 0, draw_line = 0;
	int height = 257, width = 257, size = 60; // valores default
	int center_h, center_w, x, y;
	int x0 = -1, y0 = -1, xend = -1, yend = -1, dx, dy, steps;
	double xinc, yinc, vx, vy;
	int i, j, k;
	unsigned char **img;
	
	// Opcoes do programa com getopt
	struct option long_options[] = {
		{"name",      required_argument, NULL,         'n'},
		{"type",      required_argument, NULL,         't'},
		{"width",     required_argument, NULL,         'w'},
		{"height",    required_argument, NULL,         'h'},
		{"figure",    required_argument, NULL,         'f'},
		{"size",      required_argument, NULL,         's'},
		{"fill",      no_argument,       &do_fill,      1},
		{"no-output", no_argument,       &do_output,    0},
		{"help",      no_argument,       &show_help,    1},
		{"version",   no_argument,       &show_version, 1},
		{NULL, 0, NULL, 0}
	};

	// Tratamento dos argumentos usando getopt
	while ((c = getopt_long(argc, argv, "n:t:w:h:f:s:l::::", long_options, NULL)) != -1) {
		switch (c) {
			case 'n':
				name = optarg;
				break;
			case 't':
				if (strcmp(optarg, "P2") != 0 && strcmp(optarg, "P5") != 0) {
					fprintf(stderr, "Unkown argument for option -t, --type\nCheck --help for information\n");
					return EXIT_FAILURE;
				}
				type = optarg;
				break;
			case 'w':
				width = atoi(optarg);
				if (width < 2) {
					fprintf(stderr, "Invalid argument for option -w, --width\n");
					return EXIT_FAILURE;
				}
				break;
			case 'h':
				height = atoi(optarg);
				if (height < 2) {
					fprintf(stderr, "Invalid argument for option -h, --height\n");
					return EXIT_FAILURE;
				}
				break;
			case 'f':
				if (strcmp(optarg, "circle") != 0 && strcmp(optarg, "square") != 0) {
					fprintf(stderr, "Unkown argument for option -f, --figure\nCheck --help for information\n");
					return EXIT_FAILURE;
				}
				figure = optarg;
				break;
			case 's':
				size = atoi(optarg);
				if (size < 1) {
					fprintf(stderr, "Invalid argument for option -s, --size\n");
					return EXIT_FAILURE;
				}
				break;
			case 'l':
				if (argv[optind] != NULL) {
					x0 = atoi(argv[optind]);
					if (argv[optind+1] != NULL) {
						y0 = atoi(argv[optind+1]);
						if (argv[optind+2] != NULL) {
							xend = atoi(argv[optind+2]);
							if (argv[optind+3] != NULL) {
								yend = atoi(argv[optind+3]);
							}
						}
					}
				}
				draw_line = 1;
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
printf("This program creates a PGM file with a circle or square in the middle\n\
and an optional line segment\n\
List of options and arguments for this program\n\
    -n, --name <path>             set <path> as the file name for the output\n\
    -t, --type <P2|P5>            set the PGM type for the image\n\
    -w, --width <number>          set <number> as the width of the image\n\
    -h, --height <number>         set <number> as the height of the image\n\
    -f, --figure <circle|square>  set figure to draw on the image\n\
    -s, --size   <number>         make the figure considering <number> as radius\n\
                                  for circle or as side for square\n\
    -l <x0 y0 xend yend>          set a line to be drawn on the image\n\
    --fill                        fill the figure\n\
    --no-output                   do not create the output file\n\
    --help                        display this help and exit\n\
    --version                     output version information and exit\n");
		return EXIT_SUCCESS;
	}

	if (show_version) {
printf("draw v0.1\nElias Rodrigues <elias.rodrigues@usp.br>\n2015-03-03\n");
		return EXIT_SUCCESS;
	}
	
	// Validacao para o tamanho do desenho caber no canvas
	if (width <= 2 * size || height <= 2 * size) {
		fprintf(stderr, "Invalid value for option -s, --size\n");
		return EXIT_FAILURE;
	}
	if (draw_line && (
			x0 < 0 || xend >= width || y0 < 0 || yend >= height ||
			x0 >= width || xend < 0 || y0 >= height || yend < 0)
		) {
		fprintf(stderr, "Invalid values for option -l\n");
		return EXIT_FAILURE;		
	}

	// Alocacao de memoria para a imagem
	img = (unsigned char **)malloc(sizeof(char *) * height);
	for (i = 0; i < height; i++) {
		img[i] = (unsigned char *)malloc(sizeof(char) * width);
		for (j = 0; j < width; j++) {
			img[i][j] = 1;
		}
	}
	
	// Calculo do centro da imagem
	center_h = height / 2;
	center_w = width / 2;

	// Desenha o quadrado
	if (figure != NULL && strcmp(figure, "square") == 0) {
		for (x = center_w - size; x <= center_w + size; x++) {
			// desenha pintando dentro
			if (do_fill) {
				for (i = size; i >= 0; i--) {
					img[center_h + i][x] = 0;
					img[center_h - i][x] = 0;					
				}
			// desenha so a borda
			} else {
				img[center_h + size][x] = 0;
				img[center_h - size][x] = 0;
			}
		}
		for (y = center_h - size; y <= center_h + size; y++) {
			img[y][center_w + size] = 0;
			img[y][center_w - size] = 0;
		}		
	// Desenha o circulo
	// Obs: desenha duas vezes (uma em funcao de x, outra em funcao de y) para
	// 	corrigir pontos falhos devido ao truncamento de double para int
	} else {
		for (x = 0; x <= size; x++) {
			y = (int)sqrt((double)(size * size - x * x));
			// desenha pintando dentro
			if (do_fill) {
				for (i = 0; i < y; i++) {
					img[center_h + i][center_w + x] = 0;
					img[center_h + i][center_w - x] = 0;
					img[center_h - i][center_w + x] = 0;
					img[center_h - i][center_w - x] = 0;
				}
			// desenha so a borda
			} else {
				img[center_h + y][center_w + x] = 0;
				img[center_h + y][center_w - x] = 0;
				img[center_h - y][center_w + x] = 0;
				img[center_h - y][center_w - x] = 0;
			}
		}
		for (y = 0; y <= size; y++) {
			x = (int)sqrt((double)(size * size - y * y));
			// desenha pintando dentro
			if (do_fill) {
				for (j = 0; j < x; j++) {
					img[center_h + y][center_w + j] = 0;
					img[center_h + y][center_w - j] = 0;
					img[center_h - y][center_w + j] = 0;
					img[center_h - y][center_w - j] = 0;
				}
			// desenha so a borda
			} else {
				img[center_h + y][center_w + x] = 0;
				img[center_h + y][center_w - x] = 0;
				img[center_h - y][center_w + x] = 0;
				img[center_h - y][center_w - x] = 0;				
			}
		}
	}
	
	// Desenha linha usando algoritmo DDA
	if (draw_line) {
		dy = yend - y0;
		dx = xend - x0;
		vy = (double)y0;
		vx = (double)x0;
		
		if (fabs((double)dx) > fabs((double)dy)) {
			steps = (int)fabs(dx);
		} else {
			steps = (int)fabs(dy);
		}
		
		yinc = (double)dy / (double)steps;
		xinc = (double)dx / (double)steps;
		
		img[(int)round(vy)][(int)round(vx)] = 0;
		for (k = 0; k < steps; k++) {
			vy += yinc;
			vx += xinc;
			img[(int)round(vy)][(int)round(vx)] = 0;
		}
	}

	// Cria arquivo da imagem
	if (do_output) {		
		if (type != NULL && strcmp(type, "P5") == 0) {
			if (name != NULL) {
				fp = fopen(name, "wb");
			} else {
				fp = fopen("out.pgm", "wb");
			}
			fprintf(fp, "P5\n%d\n%d\n%d\n", width, height, 1);
			for (i = 0; i < height; i++) {
				fwrite(img[i], sizeof(unsigned char), width, fp);
			}
			fclose(fp);
		} else {
			if (name != NULL) {
				fp = fopen(name, "w");
			} else {
				fp = fopen("out.pgm", "w");
			}
			fprintf(fp, "P2\n%d\n%d\n%d\n", width, height, 1);
			for (i = 0; i < height; i++) {
				for (j = 0; j < width; j++) {
					fprintf(fp, "%hhu ", img[i][j]);
				}
			}
			fclose(fp);
		}
	}
	
	// Liberacao de memoria
	for (i = 0; i < height; i++) {
		free(img[i]);
	}
	free(img);
	
	return EXIT_SUCCESS;
}

