/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * SCC0251 Processamento de Imagens
 * Trabalho 2: Gaussian, Laplacian, Laplacian of Gaussian
 * 
 * Elias Italiano Rodrigues 7987251
 * 2015-03-31
 */
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>

#include <cv.h>
#include <highgui.h>

#define PIXEL(i,j) (i * width + j)

/**
 * Verifica se um dado pixel e' valido
 */
int valid(int width, int height, int i, int j)
{
	if (i < 0 || j < 0 || i >= height || j >= width) {
		return 0;
	}
	return 1;
}

/**
 * Aplica detecao de zero-crossing
 */
void zero_crossing(IplImage *img, int *data, int *data_tmp, int threshold)
{
	// Variavies auxiliares
	int i, j, k, l;
	int flag, variance, mean;
	
	int width  = img->width;
	int height = img->height;

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			
			// Calcula media local 3x3 centrado em i,j
			mean = 0;
			for (k = i-1; k <= i+1; k++) {
				for (l = j-1; l <= j+1; l++) {
					if (valid(width, height, k, l)) {
						mean += data[PIXEL(k,l)];
					}
				}
			}
			mean /= 9.0;
			
			// Calcula variancia local 3x3 centrado em i,j
			variance = 0;
			for (k = i-1; k <= i+1; k++) {
				for (l = j-1; l <= j+1; l++) {
					if (valid(width, height, k, l)) {
						variance += (data[PIXEL(k,l)] - mean) * (data[PIXEL(k,l)] - mean);
					}
				}
			}
			variance /= 9.0;
			
			// Conta quantos zero-crossing ocorreu nas quatro vizinhancas
			flag = 0;

			if (
				(valid(width, height, i-1, j-1) && valid(width, height, i+1, j+1))
				&& (
					(data_tmp[PIXEL((i-1),(j-1))] > 0 && data_tmp[PIXEL((i+1),(j+1))] < 0) ||
					(data_tmp[PIXEL((i-1),(j-1))] < 0 && data_tmp[PIXEL((i+1),(j+1))] > 0)
				)
			) {
				flag++;
			}
			
			if (
				(valid(width, height, i-1, j) && valid(width, height, i+1, j))
				&& (
					(data_tmp[PIXEL((i-1),j)] > 0 && data_tmp[PIXEL((i+1),j)] < 0) ||
					(data_tmp[PIXEL((i-1),j)] < 0 && data_tmp[PIXEL((i+1),j)] > 0)
				)
			) {
				flag++;
			}
			
			if (
				(valid(width, height, i-1, j+1) && valid(width, height, i+1, j-1))
				&& (
					(data_tmp[PIXEL((i-1),(j+1))] > 0 && data_tmp[PIXEL((i+1),(j-1))] < 0) ||
					(data_tmp[PIXEL((i-1),(j+1))] < 0 && data_tmp[PIXEL((i+1),(j-1))] > 0)
				)
			) {
				flag++;
			}
			
			if (
				(valid(width, height, i, j-1) && valid(width, height, i, j+1))
				&& (
					(data_tmp[PIXEL(i,(j-1))] > 0 && data_tmp[PIXEL(i,(j+1))] < 0) ||
					(data_tmp[PIXEL(i,(j-1))] < 0 && data_tmp[PIXEL(i,(j+1))] > 0)
				)
			) {
				flag++;
			}
			
			// Se ocorreu pelo menos 2 zero-crossing e
			//     a variancia local atingiu o threshold, entao pinta
			if (flag > 1 && variance >= threshold) {
				img->imageData[PIXEL(i,j)] = 0;
			}
		}
	}
}

/**
 * Funcao principal
 */
int main(int argc, char **argv)
{
	// Variaveis de opcoes do programa
	int c;
	char *input = NULL;
	int show_help = 0, show_version = 0;
	int gaussian = 0, laplacian = 0, log = 0;

	// Variaveis para tratar a imagem
	IplImage *img = NULL, *img_g = NULL, *img_l = NULL, *img_log = NULL;
	int height, width;
	int *data = NULL, *data_tmp = NULL;
	int i, j, k, l, p, q;
	int total, sum;
	int threshold = 50; // valor default
	
	// Mascaras
	int mask_g[5][5] = { // Gaussian (soma = 273)
		{1, 4,   7,  4, 1},
		{4, 16, 26, 16, 4},
		{7, 26, 41, 26, 7},
		{4, 16, 26, 16, 4},
		{1, 4,   7,  4, 1}
	};
	int mask_l[3][3] = { // Laplacian
		{0,  1,  0},
		{1, -4,  1},
		{0,  1,  0}
	};
	int mask_log[9][9] = { // LoG
		{0, 1, 1,   2,   2,   2, 1, 1, 0},
		{1, 2, 4,   5,   5,   5, 4, 2, 1},
		{1, 4, 5,   3,   0,   3, 5, 4, 1},
		{2, 5, 3, -12, -24, -12, 3, 5, 2},
		{2, 5, 0, -24, -40, -24, 0, 5, 2},
		{2, 5, 3, -12, -24, -12, 3, 5, 2},
		{1, 4, 5,   3,   0,   3, 5, 4, 1},
		{1, 2, 4,   5,   5,   5, 4, 2, 1},
		{0, 1, 1,   2,   2,   2, 1, 1, 0}
	};
	
	// Opcoes do programa com getopt
	struct option long_options[] = {
		{"input",     required_argument, NULL,        'i'},
		{"gaussian",  no_argument,       &gaussian,     1},
		{"laplacian", no_argument,       &laplacian,    1},
		{"log",       no_argument,       &log,          1},
		{"threshold", required_argument, NULL,        't'},
		{"help",      no_argument,       &show_help,    1},
		{"version",   no_argument,       &show_version, 1},
		{NULL, 0, NULL, 0}
	};

	// Tratamento dos argumentos usando getopt
	while ((c = getopt_long(argc, argv, "i:glt:", long_options, NULL)) != -1) {
		switch (c) {
			case 'i':
				input = optarg;
				break;
			case 'g':
				gaussian = 1;
				break;
			case 'l':
				laplacian = 1;
				break;
			case 't':
				threshold = atoi(optarg);
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
printf("This program applies filters to an image\n\
List of options and arguments for this program\n\
    -i, --input <path>    set <path> as the input file\n\
    -g, --gaussian        apply Gaussian filter\n\
    -l, --laplacian       apply Laplacian filter\n\
    --log                 apply Laplacian of Gaussian (LoG) filter\n\
    -t, --threshold       threshold value to consider for LoG's zero-crossing\n\
    --help                display this help and exit\n\
    --version             output version information and exit\n");
		return EXIT_SUCCESS;
	}

	if (show_version) {
printf("main v0.1\nElias Rodrigues <elias.rodrigues@usp.br>\n2015-03-31\n");
		return EXIT_SUCCESS;
	}

	// Carrega imagem
	if (input == NULL) {
		img = cvLoadImage("lena.jpg", CV_LOAD_IMAGE_GRAYSCALE); // nome padrao
	} else {
		img = cvLoadImage(input, CV_LOAD_IMAGE_GRAYSCALE); // nome informado
	}
	if (!img) {
		fprintf(stderr, "Could not load image file\n");
		return EXIT_FAILURE;
	}
	
	// Le dados da imagem
	width  = img->width;
	height = img->height;
	total = width * height;
	data = (int *)malloc(sizeof(int) * total);
	for (i = 0; i < total; i++) {
		data[i] = (uchar)img->imageData[i];
	}
	
	// Aloca buffer temporario
	data_tmp = (int *)malloc(sizeof(int) * total);

	// Aplica filtro Gaussian
	if (gaussian) {

		// Para cada pixel		
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
			
				// Aplica a mascara Gaussian
				sum = 0;
				for (p = 0, k = i-2; k <= i+2; k++, p++) {
					for (q = 0, l = j-2; l <= j+2; l++, q++) {
						if (valid(width, height, k, l)) {
							sum += mask_g[p][q] * data[PIXEL(k,l)];
						}
					}
				}
				data_tmp[PIXEL(i,j)] = (double)sum / 273.0;
			}
		}
		
		// Cria nova imagem para o filtro Gaussian
		img_g = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
		for (i = 0; i < total; i++) {
			if (data_tmp[i] < 0) {
				img_g->imageData[i] = 0;
			} else if (data_tmp[i] > 255) {
				img_g->imageData[i] = 255;
			} else {
				img_g->imageData[i] = (uchar)data_tmp[i];
			}
		}
		
		// Mostra imagem Gaussian na tela
		cvNamedWindow("Gaussian Image", CV_WINDOW_AUTOSIZE);
		cvShowImage("Gaussian Image", img_g);
	}

	// Aplica filtro Laplacian	
	if (laplacian) {

		// Para cada pixel		
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
			
				// Aplica a mascara Laplacian
				sum = 0;
				for (p = 0, k = i-1; k <= i+1; k++, p++) {
					for (q = 0, l = j-1; l <= j+1; l++, q++) {
						if (valid(width, height, k, l)) {
							sum += mask_l[p][q] * data[PIXEL(k,l)];
						}
					}
				}
				data_tmp[PIXEL(i,j)] = sum;
			}
		}
		
		// Cria nova imagem para o filtro Laplacian
		img_l = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
		for (i = 0; i < total; i++) {
				img_l->imageData[i] = 255;
		}
		
		// Deteta zero-crossing
		zero_crossing(img_l, data, data_tmp, threshold);
		
		// Mostra imagem Laplacian na tela
		cvNamedWindow("Laplacian Image", CV_WINDOW_AUTOSIZE);
		cvShowImage("Laplacian Image", img_l);
	}

	// Aplica filtro LoG
	if (log) {
	
		// Para cada pixel		
		for (i = 0; i < height; i++) {
			for (j = 0; j < width; j++) {
			
				// Aplica a mascara LoG
				sum = 0;
				for (p = 0, k = i-4; k <= i+4; k++, p++) {
					for (q = 0, l = j-4; l <= j+4; l++, q++) {
						if (valid(width, height, k, l)) {
							sum += mask_log[p][q] * data[PIXEL(k,l)];
						}
					}
				}
				data_tmp[PIXEL(i,j)] = sum;
			}
		}
		
		// Cria nova imagem para o filtro LoG
		img_log = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
		for (i = 0; i < total; i++) {
			img_log->imageData[i] = 255;
		}

		// Deteta zero-crossing
		zero_crossing(img_log, data, data_tmp, threshold);

		// Mostra imagem LoG na tela
		cvNamedWindow("LoG Image", CV_WINDOW_AUTOSIZE);
		cvShowImage("LoG Image", img_log);
	}

	// Mostra imagem original na tala
	cvNamedWindow("Original Image", CV_WINDOW_AUTOSIZE);
	cvShowImage("Original Image", img);

	// Espera pressionar uma tecla para fechar janela
	cvWaitKey(0);

	// Libera imagens e dados
	if (img) {
		cvReleaseImage(&img);
	}
	if (img_g) {
		cvReleaseImage(&img_g);
	}
	if (img_l) {
		cvReleaseImage(&img_l);
	}
	if (img_log) {
		cvReleaseImage(&img_log);
	}
	if (data != NULL) {
		free(data);
	}
	if (data_tmp != NULL) {
		free(data_tmp);
	}

	return EXIT_SUCCESS;
}

