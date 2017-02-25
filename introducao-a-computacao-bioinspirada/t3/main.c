#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <libgen.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>

// App attributes
#define APP_NAME    "aco-tsp"
#define APP_VERSION "1.0"
#define APP_DATE    "2015-07-03"
#define APP_AUTHORS "Elias Rodrigues <elias.rodrigues@usp.br> &\nVinicius Biondo <vinicius.biondo@usp.br>"

// Default values
#define DEFAULT_IN         "./data/tsp_euc-2d/a280.tsp"
#define DEFAULT_EXPECTED   "./data/optimal.result"
#define DEFAULT_THREADS    1
#define DEFAULT_ANTS       10
#define DEFAULT_ALPHA      1.0
#define DEFAULT_BETA       1.0
#define DEFAULT_RHO        0.1
#define DEFAULT_PHEROMONE  1.0
#define DEFAULT_WEIGHT     1.0
#define DEFAULT_TIMES      1
#define UNDEFINED_NODE    -1
#define INVALID_PATH      -1

/**
 * Structure to hold a expected result for a given filename
 */
struct result_s
{
	char filename[100];
	int  value;
};

/**
 * Structure to store arguments for threads
 */
struct args_s
{
	int a; // interval [a,b]
	int b;
	int status; // if thread succeeded
};

// Graph with node-to-node distances
int dimension;
double **node = NULL;
int **distance = NULL;

// Pheromone for each node-to-node
double **pheromone = NULL;

// Expected results from option -e, --expected
FILE *fp_expected = NULL;
char *expected_results = NULL; // file path
struct result_s *expected = NULL;
int nexpected; // amount of expected results loaded

// Path and filename for input
FILE *fp_in = NULL;
char *in = NULL, *in_filename = NULL;

// Parameters for program and algorithm
double **ant_prob = NULL; // cumulative probabilities
int *ant_distance = NULL; // distances for each ant
int **ant_position = NULL;
int **ant_path = NULL;    // paths for each ant
pthread_t *threads = NULL;
int nthreads = DEFAULT_THREADS;
struct args_s *args = NULL;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int ants     = DEFAULT_ANTS;
double alpha = DEFAULT_ALPHA;
double beta  = DEFAULT_BETA;
double rho   = DEFAULT_RHO;
double initial_pheromone = DEFAULT_PHEROMONE;
double weight = DEFAULT_WEIGHT;
int times = DEFAULT_TIMES;
int ants_result = INT_MAX;
int *ants_solution = NULL;
int ants_index = 0;

/**
 * Build a given path transforming it into a vector
 * Check this path is correct and return its distance
 */
int build_path(int k)
{
	int i, j, n, total_distance = 0, value;
	int *buffer_path = (int *)malloc(sizeof(int) * dimension);

	// Create a vector solution
	for (i = 0; i < dimension; i++) {
		for (n = 0; n < dimension; n++) {
			if (ant_path[k][n] == i) {
				buffer_path[i] = n;
				break;
			}
		}
	}

	// Check if it is a valid path
	for (i = 0; i < dimension; i++) {
		value = buffer_path[i];
		for (n = 0; n < dimension; n++) {
			if (i != n) {
				if (value == buffer_path[n]) {
					return INVALID_PATH;
				}
			}
		}
	}

	// Copy the vector path into original one and calcule its distance
	for (n = 0; n < dimension-1; n++) {
		ant_path[k][n] = buffer_path[n];

		i = buffer_path[n];
		j = buffer_path[n+1];
		total_distance += distance[i][j];
	}
	ant_path[k][dimension-1] = buffer_path[dimension-1];

	free(buffer_path);

	return total_distance;
}

/**
 * Launch a set of ants from [a,b]
 */
void *launch_ants(void *args)
{
	int i, j, k, pace, goto_node;
	double rand01;
	int a = ((struct args_s *)args)->a;
	int b = ((struct args_s *)args)->b;
	((struct args_s *)args)->status = EXIT_SUCCESS;

	for (k = a; k <= b; k++) {

		// Reset ant's path
		for (j = 0; j < dimension; j++) {
			ant_path[k][j] = UNDEFINED_NODE;
		}
		ant_path[k][ant_position[k][0]] = 0; // initial node of ant k, so pace equals 0
		ant_position[k][1] = ant_position[k][0]; // current node of ant k

		// Build a path for ant k
		for (pace = 1; pace < dimension; pace++) {

			// Starting from its current position i
			i = ant_position[k][1];

			if (i == UNDEFINED_NODE) {
				fprintf(stderr, "error: invalid node %d for ant %d\n", i, k);
				((struct args_s *)args)->status = EXIT_FAILURE;
				pthread_exit(NULL);
			}

			// Calculate acumulative probabilities
			ant_prob[k][0] = pow(pheromone[i][0], alpha) * pow(1.0/(double)distance[i][0], beta);
			for (j = 1; j < dimension; j++) {
				if (ant_path[k][j] == UNDEFINED_NODE) {
					ant_prob[k][j] = ant_prob[k][j-1] + pow(pheromone[i][j], alpha) * pow(1.0/(double)distance[i][j], beta);
				} else {
					ant_prob[k][j] = 0.0;
				}
			}

			// Choose which node to go
			rand01 = (double)rand() / (double)RAND_MAX;

			goto_node = UNDEFINED_NODE;

			for (j = 0; j < dimension; j++) {
				if (ant_path[k][j] == UNDEFINED_NODE) {
					if (rand01 <= ant_prob[k][j]/ant_prob[k][dimension-1]) {
						goto_node = j;
						break;
					}
				}
			}

			if (goto_node != UNDEFINED_NODE) {
				ant_path[k][goto_node] = pace;  // go to node
				ant_position[k][1] = goto_node; // save current position of ant k
			} else {
				fprintf(stderr, "error: ant %d couldn't find a node to go at pace %d\n", k, pace);
				((struct args_s *)args)->status = EXIT_FAILURE;
				pthread_exit(NULL);
			}
		}

		// Check if the path built is OK and calcute its distance
		ant_distance[k] = build_path(k);
		if (ant_distance[k] == INVALID_PATH) {
			fprintf(stderr, "error: invalid path built for ant %d\n", k);
			((struct args_s *)args)->status = EXIT_FAILURE;
			pthread_exit(NULL);

		// Deposit pheromone on the path
		} else {
			for (pace = 0; pace < dimension-1; pace++) {
				i = ant_path[k][pace];
				j = ant_path[k][pace+1];

				// Update pheromone: CRITICAL SECTION
				pthread_mutex_lock(&mutex);
				pheromone[i][j] += weight/(double)ant_distance[k];
				pheromone[i][j] *= 1.0 - rho;
				pthread_mutex_unlock(&mutex);
			}
		}
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	// Temporary variables
	char buffer[100], *pchar;
	double coord_x, coord_y;
	double delta_x, delta_y;
	int i, j, k, t, id, value;
	int ants_per_thread;
	struct timespec starttime, endtime;
	double elapsed;

	// Flags to show help or version
	int show_help = 0, show_version = 0, do_print = 0;

	// Define arguments with getopt
	char c;
	struct option long_options[] = {
		{"in",         required_argument, NULL,        'i'},
		{"expected",   required_argument, NULL,        'e'},
		{"threads",    required_argument, NULL,        't'},
		{"ants",       required_argument, NULL,        'k'},
		{"alpha",      required_argument, NULL,        'a'},
		{"beta",       required_argument, NULL,        'b'},
		{"rho",        required_argument, NULL,        'r'},
		{"pheromone",  required_argument, NULL,        'p'},
		{"weight",     required_argument, NULL,        'w'},
		{"times",      required_argument, NULL,        'n'},
		{"print",      no_argument,       &do_print,     1},
		{"help",       no_argument,       &show_help,    1},
		{"version",    no_argument,       &show_version, 1},
		{NULL, 0, NULL, 0}
	};

	// Handle the arguments with getopt
	while ((c = getopt_long(argc, argv, "i:e:t:k:a:b:r:p:w:n:", long_options, NULL)) != -1) {
		switch (c) {
			case 'i':
				in = strdup(optarg);
				in_filename = basename(optarg);
				break;
			case 'e':
				expected_results = strdup(optarg);
				break;
			case 't':
				nthreads = atoi(optarg);
				if (nthreads < 1) {
					fprintf(stderr, "error: invalid number of threads\n");
					return EXIT_FAILURE;
				}
				break;
			case 'k':
				ants = atoi(optarg);
				if (ants < 1) {
					fprintf(stderr, "error: invalid number of ants\n");
					return EXIT_FAILURE;
				}
				break;
			case 'a':
				alpha = atof(optarg);
				if (alpha < 1.0) {
					fprintf(stderr, "error: invalid alpha constant\n");
					return EXIT_FAILURE;
				}
				break;
			case 'b':
				beta = atof(optarg);
				if (beta < 1.0) {
					fprintf(stderr, "error: invalid beta constant\n");
					return EXIT_FAILURE;
				}
				break;
			case 'r':
				rho = atof(optarg);
				if (rho < 0.0 || rho > 1.0) {
					fprintf(stderr, "error: invalid rho constant\n");
					return EXIT_FAILURE;
				}
				break;
			case 'p':
				initial_pheromone = atof(optarg);
				if (initial_pheromone < 0.0) {
					fprintf(stderr, "error: invalid initial pheromone value\n");
					return EXIT_FAILURE;
				}
				break;
			case 'w':
				weight = atof(optarg);
				if (weight < 0.0) {
					fprintf(stderr, "error: invalid weight constant\n");
					return EXIT_FAILURE;
				}
				break;
			case 'n':
				times = atoi(optarg);
				if (times < 1) {
					fprintf(stderr, "error: invalid times value\n");
					return EXIT_FAILURE;
				}
				break;
			case '?':
				fprintf(stderr, "error: invalid options or arguments: please check --help for info\n");
				return EXIT_FAILURE;
			case 0:
			default:
				break;
		}
	}

	// Print help info
	if (show_help) {
		printf("\
This program applies ACO algorithm to solve a TSP using\n\
data form TSPLIB of type EUC_2D.\n\
\n\
List of options and arguments for this program:\n\
  -i, --in           input .tsp EUC_2D file from TSPLIB\n\
                     (default %s)\n\
  -e, --expected     expected results (default %s)\n\
  -t, --threads      number of threads to be used (default %d)\n\
  -k, --ants         number of ants (default %d)\n\
  -a, --alpha        alpha constant (default %lf)\n\
  -b, --beta         beta constant (default %lf)\n\
  -r, --rho          rho constant (default %lf)\n\
  -p, --pheromone    initial pheromone value (default %lf)\n\
  -w, --weight       weight for depositing pheromone (default %lf)\n\
  -n, --times        number of runs (default %d)\n\
  --print            print progress of best distances for each iteration\n\
  --help             print this help information\n\
  --version          print version number\n",
  DEFAULT_IN, DEFAULT_EXPECTED, DEFAULT_THREADS,
  DEFAULT_ANTS, DEFAULT_ALPHA, DEFAULT_BETA, DEFAULT_RHO,
  DEFAULT_PHEROMONE, DEFAULT_WEIGHT, DEFAULT_TIMES);
		return EXIT_SUCCESS;
	}

	// Print version info
	if (show_version) {
		printf("\
%s %s\n\
%s\n\
%s\n",
APP_NAME, APP_VERSION, APP_AUTHORS, APP_DATE);
		return EXIT_SUCCESS;
	}

	// Check if number of threads and ants are valid
	if (nthreads > ants) {
		fprintf(stderr, "error: number of threads is greater than number of ants\n");
		return EXIT_FAILURE;
	}

	// Check input path and filename
	if (in == NULL) {
		in = (char *)malloc(sizeof(char) * 100);
		strcpy(in, DEFAULT_IN);

		in_filename = basename(in);
	}

	// Check result path
	if (expected_results == NULL) {
		expected_results = (char *)malloc(sizeof(char) * 100);
		strcpy(expected_results, DEFAULT_EXPECTED);
	}

	// Open input file
	fp_in = fopen(in, "r");
	if (fp_in == NULL) {
		fprintf(stderr, "error: couldn't read file %s\n", in);
		return EXIT_FAILURE;
	}

	// Try to open result file
	fp_expected = fopen(expected_results, "r");
	if (fp_expected != NULL) {
		// Read expected results
		nexpected = 0;
		while (fscanf(fp_expected, "%s", buffer) != EOF && fscanf(fp_expected, "%d", &value) != EOF) {
			expected = realloc(expected, sizeof(struct result_s) * (nexpected + 1));
			
			strcpy(expected[nexpected].filename, buffer);
			expected[nexpected].value = value;
			nexpected++;
		}
	} else {
		free(expected_results);
		expected_results = NULL;
	}

	// First integer of input file tells us the dimension
	if (fscanf(fp_in, "%d", &dimension) < 1) {
		fprintf(stderr, "error: couldn't read dimension value from file\n");
		return EXIT_FAILURE;
	}
	
	// Allocate memory for distances (which is all integers)
	distance = (int **)malloc(sizeof(int *) * dimension);
	for (i = 0; i < dimension; i++) {
		distance[i] = (int *)malloc(sizeof(int) * dimension);
	}

	// Allocate memory for pheromone
	pheromone = (double **)malloc(sizeof(double *) * dimension);
	for (i = 0; i < dimension; i++) {
		pheromone[i] = (double *)malloc(sizeof(double) * dimension);
		for (j = 0; j < dimension; j++) {
			pheromone[i][j] = initial_pheromone;
		}
	}

	// Allocate memory for nodes (each node has two coordenates (x, y))
	node = (double **)malloc(sizeof(double *) * dimension);
	for (i = 0; i < dimension; i++) {
		node[i] = (double *)malloc(sizeof(double) * 2);
	}

	// Read nodes coordenates from input file
	for (i = 0; i < dimension; i++) {
		if (fscanf(fp_in, "%d",  &id)      == EOF ||
		    fscanf(fp_in, "%lf", &coord_x) == EOF ||
		    fscanf(fp_in, "%lf", &coord_y) == EOF
		) fprintf(stderr, "warning: unexpected end of file in %s\n", in);

		if (id != i + 1) {
			fprintf(stderr, "warning: in file %s, node %d is out of order\n", in, id);
		}
		node[id-1][0] = coord_x;
		node[id-1][1] = coord_y;
	}

	// Calculate the integer Euclidean distance (EUC_2D)
	for (i = 0; i < dimension; i++) {
		for (j = 0; j < dimension; j++) {
			if (i == j) {
				distance[i][j] = INT_MAX;
				distance[j][i] = INT_MAX;
			} else {
				delta_x  = node[i][0] - node[j][0];
				delta_y  = node[i][1] - node[j][1];

				// All distances are rounded and symmetric in TSPLIB EUC_2D files
				distance[i][j] = (int)(sqrt(delta_x*delta_x + delta_y*delta_y) + 0.5);
				if (distance[i][j] < 1) {
					distance[i][j] = 1;
				}
				distance[j][i] = distance[i][j];
			}
		}
	}

	// Initialize ants
	srand(time(NULL));
	ants_solution = (int *)malloc(sizeof(int) * dimension);
	ant_distance = (int *)malloc(sizeof(int) * ants);
	ant_position = (int **)malloc(sizeof(int *) * ants);
	ant_path = (int **)malloc(sizeof(int *) * ants);
	ant_prob = (double **)malloc(sizeof(double *) * ants);
	
	for (k = 0; k < ants; k++) {
		ant_prob[k] = (double *)malloc(sizeof(double) * dimension);
		ant_path[k] = (int *)malloc(sizeof(int) * dimension);

		// At index 0 is stored the ant's initial position (node)
		// At index 1 is stored the ant's current position (node)
		ant_position[k] = (int *)malloc(sizeof(int) * 2);
		ant_position[k][0] = rand() % dimension;
	}

	// Allocate threads
	threads = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
	args = (struct args_s *)malloc(sizeof(struct args_s) * nthreads);

	// Print gathered information
	printf("\
input path            : %s\n\
input filename        : %s\n\
input dimension       : %d\n\
expected results file : %s\n\
\n\
threads   : %d\n\
ants      : %d\n\
alpha     : %lf\n\
beta      : %lf\n\
rho       : %lf\n\
pheromone : %lf\n\
weight    : %lf\n\
times     : %d\n\
\n",
in, in_filename, dimension, expected_results,
nthreads, ants, alpha, beta, rho, initial_pheromone, weight, times);

	// Distribution of work: calculute intervals [a,b] for each thread
	ants_per_thread = ants/nthreads;
	for (i = 0; i < nthreads; i++) {
		args[i].a = i * ants_per_thread;
		if (i != nthreads - 1) {
			args[i].b = (i + 1) * ants_per_thread - 1;
		} else {
			args[i].b = ants-1;
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &starttime);

	// Run the algorithm
	for (t = 0; t < times; t++) {

		// Create threads
		for (i = 0; i < nthreads; i++) {
			if (pthread_create(threads+i, NULL, launch_ants, args+i) != 0) {
				fprintf(stderr, "error: couldn't create thread %d\n", i);
				return EXIT_FAILURE;
			}
		}

		// Join threads
		for (i = 0; i < nthreads; i++) {
			pthread_join(threads[i], NULL);
			if (args[i].status == EXIT_FAILURE) {
				fprintf(stderr, "error: thread %d failed\n", i);
				return EXIT_FAILURE;
			}
		}

		// Get the best solution
		for (k = 0; k < ants; k++) {
			if (ant_distance[k] < ants_result) {
				ants_result = ant_distance[k];
				ants_index  = k;
				memcpy(ants_solution, ant_path[k], sizeof(int) * dimension);
			}
		}
		
		// Print result to show the progress
		if (do_print) {
			printf("progress : %d\n", ants_result);
			if (t == times-1) {
				printf("\n");
			}
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &endtime);

	elapsed = (endtime.tv_sec - starttime.tv_sec);
	elapsed += (endtime.tv_nsec - starttime.tv_nsec) / 1000000000.0;

	printf("time (s)        : %lf\n", elapsed);
	printf("ants result     : %d\n", ants_result);

	// Compare ants_result with a expected result and print info
	if (fp_expected != NULL) {
		pchar = strrchr(in_filename, '.');
		if (pchar != NULL) {
			*pchar = '\0';
		}
		for (i = 0; i < nexpected; i++) {
			if (strcmp(expected[i].filename, in_filename) == 0) {
				printf("expected result : %d\n", expected[i].value);
				printf("error           : %lf\n", (double)ants_result / (double)expected[i].value - 1.0);
			}
		}
	}

	// Print path of solution
	printf("\nants path       :\n");
	for (i = 0; i < dimension; i++) {
		printf("%d ", ants_solution[i]);
	}
	printf("\n");

	// Free allocated memory
	if (fp_in != NULL)
		fclose(fp_in);
	if (fp_expected != NULL)
		fclose(fp_expected);
	if (in != NULL)
		free(in);
	if (expected_results != NULL)
		free(expected_results);
	if (expected != NULL)
		free(expected);
	if (distance != NULL) {
		for (i = 0; i < dimension; i++)
			if (distance[i] != NULL)
				free(distance[i]);
		free(distance);
	}
	if (node != NULL) {
		for (i = 0; i < dimension; i++)
			if (node[i] != NULL)
				free(node[i]);
		free(node);
	}
	if (threads != NULL)
		free(threads);
	if (args != NULL)
		free(args);
	if (ant_prob != NULL) {
		for (k = 0; k < ants; k++)
			if (ant_prob[k] != NULL)
				free(ant_prob[k]);
		free(ant_prob);
	}
	if (ant_position != NULL) {
		for (k = 0; k < ants; k++)
			if (ant_position[k] != NULL)
				free(ant_position[k]);
		free(ant_position);
	}
	if (ants_solution != NULL)
		free(ants_solution);
	if (ant_distance != NULL)
		free(ant_distance);
	if (ant_path != NULL) {
		for (k = 0; k < ants; k++)
			if (ant_path[k] != NULL)
				free(ant_path[k]);
		free(ant_path);
	}
	if (pheromone != NULL) {
		for (i = 0; i < dimension; i++) {
			if (pheromone[i] != NULL)
				free(pheromone[i]);
		}
		free(pheromone);
	}

	return EXIT_SUCCESS;
}
