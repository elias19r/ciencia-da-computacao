/**
 * Universidade de Sao Paulo
 * Instituto de Ciencias Matematicas e de Computacao
 * 
 * Trabalho 3: Smooth
 * Algoritmo Paralelo usando MPI
 * 
 * SSC0143 Programacao Concorrente
 * 
 * Elias Italiano Rodrigues 7987251
 * Rodolfo Megiato de Lima  7987286
 * Vinicius Katata Biondo   6783972
 */
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <getopt.h>
#include "../inc/image.h"
#include "../inc/ppm_p3.h"
#include "../inc/smooth.h"

// Tags requeridas pelo MPI para marcar envio/recebimento
#define SEND_DATA_TAG   0
#define RETURN_DATA_TAG 1

/**
 * Programa principal
 */
int main(int argc, char **argv)
{
	int my_id, num_procs;
	MPI_Status status;

	// Inicializa MPI
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
      MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	if (num_procs < 2) {
		fprintf(stderr, "ERROR: -np must be greater than 1\n");
		exit(EXIT_FAILURE);
	}

      // Processo MPI Mestre
	if (my_id == 0) {

		ppm_header_t ppm; // cabecalho do arquivo PPM P3
		pixel_t **img; // imagem inteira
		int i, j, n;
		int height, start_row, end_row, avg_rows_per_process, num_rows_to_send;
		
		double starttime, endtime;

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
		img = ppm_p3_scan(&ppm);
		
		if (num_procs > ppm.height) {
			fprintf(stderr, "ERROR: -np must be lesser than image's height\n");
			exit(EXIT_FAILURE);
		}

		starttime = MPI_Wtime();

		// Envia as partes para cada processo
		avg_rows_per_process = ppm.height / num_procs;
		for (n = 1; n < num_procs; n++) {
			
			start_row = n * avg_rows_per_process;

			if (n == num_procs-1)
				end_row = ppm.height - 1;
			else
				end_row = (n + 1) * avg_rows_per_process - 1;

			height = end_row - start_row + 1;
			
			if (n == num_procs-1) {
				start_row -= BLOCK_OFFSET;
				num_rows_to_send = height + BLOCK_OFFSET;
			} else {
				start_row -= BLOCK_OFFSET;
				end_row += BLOCK_OFFSET;
				num_rows_to_send = height + 2*BLOCK_OFFSET;
			}

			if (start_row < 0 || end_row >= ppm.height) {
				fprintf(stderr, "ERROR: invalid image slice for slave process %d\n", n);
				exit(EXIT_FAILURE);
			}

			MPI_Send(&(ppm.width), 1, MPI_INT, n, SEND_DATA_TAG, MPI_COMM_WORLD);
			MPI_Send(&height, 1, MPI_INT, n, SEND_DATA_TAG, MPI_COMM_WORLD);
			MPI_Send(&num_rows_to_send, 1, MPI_INT, n, SEND_DATA_TAG, MPI_COMM_WORLD);

			for (j = start_row, i = 0; i < num_rows_to_send; i++, j++) {
				if (j >= ppm.height) {
					fprintf(stderr, "ERROR: invalid image slice for slave process %d\n", n);
					exit(EXIT_FAILURE);
				}
				MPI_Send(img[j], sizeof(pixel_t) * ppm.width, MPI_BYTE, n, SEND_DATA_TAG, MPI_COMM_WORLD);
			}
		}

		// Mestre faz sua parte
		smooth(img, ppm.width, avg_rows_per_process, 2);

		// Faz o join, recebendo o trabalho de todos os escravos
		for (n = 1; n < num_procs; n++) {

			start_row = n * avg_rows_per_process;

			if (n == num_procs-1)
				end_row = ppm.height - 1;
			else
				end_row = (n + 1) * avg_rows_per_process - 1;

			height = end_row - start_row + 1;

			for (j = start_row, i = 0; i < height; i++, j++) {
				MPI_Recv(img[j], sizeof(pixel_t) * ppm.width, MPI_BYTE, n, RETURN_DATA_TAG, MPI_COMM_WORLD, &status);
			}
		}
		
		endtime = MPI_Wtime();

		// Imprime tempo de processamento
		if (do_time) {
			printf("%.2lf\n", endtime - starttime);
		}
		
		// Imprime imagem
		if (do_output) {
			ppm_p3_print(img, &ppm);
		}

		// Libera memoria
		for (i = 0; i < ppm.height; i++) {
			free(img[i]);
		}
		free(img);

	// Processos MPI Escravos
	} else {

		pixel_t **img_piece;
		int width, height, num_rows_to_recv, flag;
		int i, j;

		// Recebe altura e largura
		MPI_Recv(&width, 1, MPI_INT, 0, SEND_DATA_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(&height, 1, MPI_INT, 0, SEND_DATA_TAG, MPI_COMM_WORLD, &status);
		MPI_Recv(&num_rows_to_recv, 1, MPI_INT, 0, SEND_DATA_TAG, MPI_COMM_WORLD, &status);

		// Aloca memoria para o pedaco do escravo
		img_piece = (pixel_t **)malloc(sizeof(pixel_t *) * num_rows_to_recv);
		if (img_piece == NULL) {
			fprintf(stderr, "ERROR: could not allocate memory for image piece for slave process %d\n", my_id);
			exit(EXIT_FAILURE);
		}
		for (i = 0; i < num_rows_to_recv; i++) {
			img_piece[i] = (pixel_t *)malloc(sizeof(pixel_t) * width);
			if (img_piece[i] == NULL) {
				fprintf(stderr, "ERROR: could not allocate memory for image piece for slave process %d\n", my_id);
				exit(EXIT_FAILURE);
			}
		}

		// Recebe os dados do master
		for (i = 0; i < num_rows_to_recv; i++) {
			MPI_Recv(img_piece[i], sizeof(pixel_t) * width, MPI_BYTE, 0, SEND_DATA_TAG, MPI_COMM_WORLD, &status);
		}

		// Escravo faz sua parte do smooth
		if (my_id == num_procs-1) {
			flag = 1;
		} else {
			flag = 3;
		}
		smooth(img_piece, width, height, flag);

		// Envia dados para o master
		j = BLOCK_OFFSET;
		for (i = 0; i < height; i++, j++) {
			MPI_Send(img_piece[j], sizeof(pixel_t) * width, MPI_BYTE, 0, RETURN_DATA_TAG, MPI_COMM_WORLD);
		}
		for (i = 0; i < num_rows_to_recv; i++) {
			free(img_piece[i]);
		}
		free(img_piece);
	}

	// Finaliza MPI
	MPI_Finalize();

	return EXIT_SUCCESS;
}

