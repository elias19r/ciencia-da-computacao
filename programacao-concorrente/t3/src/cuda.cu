/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 3: Smooth
 * Algoritmo usando CUDA
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/time.h>
#include "../inc/image.h"
#include "../inc/ppm_p3.h"
#include "../inc/smooth.h"

/**
 * Le uma imagem PPM P3 da entrada padrao (vetorizada)
 */
pixel_t *ppm_p3_vscan(ppm_header_t *ppm)
{
	int i, npixels;
	pixel_t *img;

	// Le o cabecalho
	scanf("%s", ppm->format);
	scanf("%d", &ppm->width);
	scanf("%d", &ppm->height);
	scanf("%d", &ppm->max_value);

	npixels = ppm->width * ppm->height;
	
	// Aloca vetor de pixels para guardar a imagem inteira
	img = (pixel_t *)malloc(sizeof(pixel_t) * npixels);
	if (img == NULL) {
		fprintf(stderr, "ERROR: could not allocate memory for image\n");
		exit(EXIT_FAILURE);
	}

	// Le e carrega a imagem para a memoria
	for (i = 0; i < npixels; i++) {
		scanf("%hhu", &img[i][R]);
		scanf("%hhu", &img[i][G]);
		scanf("%hhu", &img[i][B]);
	}
	
	return img;	
}

/**
 * Imprime uma imagem (vetorizada) PPM P3 na saida padrao
 */
void ppm_p3_vprint(pixel_t *img, ppm_header_t *ppm)
{
	int i, npixels = ppm->width * ppm->height;

	printf("%s\n", ppm->format);
	printf("%d %d\n", ppm->width, ppm->height);
	printf("%d\n", ppm->max_value);

	for (i = 0; i < npixels; i++) {
		printf("%hhu ", img[i][R]);
		printf("%hhu ", img[i][G]);
		printf("%hhu ", img[i][B]);
	}
}

/**
 * Funcao que aplica o algoritmo smooth para remocao de ruidos em uma
 * imagem vetorizada usando CUDA
 */
__global__ 
void cudaSmooth(pixel_t *img_in, pixel_t *img_out, int width, int height)
{
	int i = blockIdx.y * blockDim.y + threadIdx.y;
	int j = blockIdx.x * blockDim.x + threadIdx.x;
	int a, b, k, l;
	int mean[3];

	// Valida posicao
	if (i >= height || j >= width)
		return;
	
	mean[R] = 0;
	mean[G] = 0;
	mean[B] = 0;

	// Itera na regiao do smooth
	for (a = 0, k = i - BLOCK_OFFSET; a < BLOCK_SIZE; a++, k++) {
		for (b = 0, l = j - BLOCK_OFFSET; b < BLOCK_SIZE; b++, l++) {
			if (k < 0 || l < 0 || k >= height || l >= width) {
				// Quando nao houver pixel valido (problema da borda)
				//	coloca o pixel padrao
				mean[R] += DEFAULT_R;
				mean[G] += DEFAULT_G;
				mean[B] += DEFAULT_B;
			} else {
				mean[R] += (int)img_in[k * width + l][R];
				mean[G] += (int)img_in[k * width + l][G];
				mean[B] += (int)img_in[k * width + l][B];
			}
		}
	}

	// Substitui na imagem resultado
	img_out[i * width + j][R] = (unsigned char)(mean[R] / BLOCK_NPIXELS);
	img_out[i * width + j][G] = (unsigned char)(mean[G] / BLOCK_NPIXELS);
	img_out[i * width + j][B] = (unsigned char)(mean[B] / BLOCK_NPIXELS);
}

/**
 * Verifica retorno de um cudaError_t
 */
void cudaFatal(cudaError_t error)
{
	if (error != cudaSuccess) {
		fprintf(stderr,"ERROR: %s\n", cudaGetErrorString(error));
		exit(EXIT_FAILURE);
	}
}

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	ppm_header_t ppm; // cabecalho do arquivo PPM P3
	pixel_t *img; // imagem inteira (vetorizada) do host
	pixel_t *img_in, *img_out; // imagem inteira (vetorizada) do device
	int npixels;
	
	struct timeval start, end;

	// Opcoes do programa
	int c;
	int do_output = 1, do_time = 0;
	struct option long_options[] = {
		{"no-output", no_argument,  &do_output, 0 },
		{"time",      no_argument,  &do_time,   1 },
		{0, 0, 0, 0}
	};

	// Tratamento dos argumentos usando optget
	while ((c = getopt_long(argc, argv, ":nt", long_options, NULL)) != -1) {
		switch (c) {
			case 'n':
				do_output = 0;
				break;
			case 't':
				do_time = 1;
				break;
			case 0:
			case '?':
			case ':':
			default:
				break;
		}
	}
	
	// Le imagem da entrada padrao
	img = ppm_p3_vscan(&ppm);
	npixels = ppm.width * ppm.height;

	gettimeofday(&start, NULL);

	// Aloca memoria na device
	cudaFatal(cudaMalloc(&img_in,  sizeof(pixel_t) * npixels));
	cudaFatal(cudaMalloc(&img_out, sizeof(pixel_t) * npixels));
	
	// Copia imagem para a device
	cudaFatal(cudaMemcpy(img_in, img, sizeof(pixel_t) * npixels, cudaMemcpyHostToDevice));

	// 32*32 = 1024, o valor maximo de quantidade de threads por bloco :D
	dim3 threadsPerBlock(32, 32);
	dim3 numBlocks((ppm.width + threadsPerBlock.x - 1) / threadsPerBlock.x, (ppm.height + threadsPerBlock.y - 1 ) / threadsPerBlock.y);

	cudaSmooth<<<numBlocks, threadsPerBlock>>>(img_in, img_out, ppm.width, ppm.height);
	cudaFatal(cudaThreadSynchronize());

	// Copia resultado de volta para o host
	cudaFatal(cudaMemcpy(img, img_out, sizeof(pixel_t) * npixels, cudaMemcpyDeviceToHost));

	gettimeofday(&end, NULL);

	// Imprime tempo de processamento
	if (do_time) {
		printf("%.2lf\n", (double)(end.tv_sec - start.tv_sec) + (double)(end.tv_usec - start.tv_usec) / 1000000.0 );
	}

	// Imprime imagem apos aplicado o smooth
	if (do_output) {
		ppm_p3_vprint(img, &ppm);
	}

	// Libera memoria
	free(img);
	cudaFree(img_in);
	cudaFree(img_out);
	
	return EXIT_SUCCESS;
}
