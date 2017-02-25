#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[]) {
	int n = 0, i;
	double *valores_x = NULL, *valores_y = NULL;
	double x_barra = 0.0, y_barra = 0.0;
	double a = 0.0, b = 0.0;
	double delta_y = 0.0, delta_a = 0.0, delta_b = 0.0;
	double soma1 = 0.0, soma2 = 0.0;
	
	// Título
	printf("\nRegressão linear: y = ax + b\n");
	
	// Recebe a quantidade de entradas
	do {
		printf("\n\nQuantidade de entradas (n): ");
		scanf("%d", &n);
	} while (n < 3);
	

	// Recebe os valores das entradas
	printf("\n\nValores de x e y:\n\n");
	valores_x = (double *) malloc(sizeof(double) * n);
	valores_y = (double *) malloc(sizeof(double) * n);
	for (i = 0; i < n; i++) {
		scanf("%lf", valores_x+i);
		scanf("%lf", valores_y+i);
		printf("\n");
	}
	
	// Média de x (x_barra)
	for (i = 0; i < n; i++) {
		x_barra += valores_x[i];
	}
	x_barra /= (double) n;

	// Média de y (y_barra)
	for (i = 0; i < n; i++) {
		y_barra += valores_y[i];
	}
	y_barra /= (double) n;
	
	// Coeficiente angular (a)
	for (i = 0; i < n; i++) {
		soma1 += (valores_x[i] - x_barra) * valores_y[i];
		soma2 += (valores_x[i] - x_barra) * (valores_x[i] - x_barra);
	}
	a = soma1 / soma2;
	
	// Coeficiente linear (b)
	b = y_barra - (a * x_barra);
	
	// Dispersão média do ajuste (delta_y)
	soma1 = 0.0;
	for (i = 0; i < n; i++) {
		soma1 += (a * valores_x[i] + b - valores_y[i]) * (a * valores_x[i] + b - valores_y[i]);
	}
	delta_y = sqrt(soma1 / ((double) n - 2.0));
	
	// Incerteza do coeficiente angular (delta_a)
	delta_a = delta_y / sqrt(soma2);
	
	// Incerteza do coeficiente linear (delta_b)
	soma1 = 0.0;
	for (i = 0; i < n; i++) {
		soma1 += valores_x[i] * valores_x[i];
	}
	delta_b = sqrt((soma1 * delta_y) / ((double) n * soma2));
	
	printf("\nResultados:\n\n");
	printf("n: %d\n", n);
	printf("a: %lf\n", a);
	printf("b: %lf\n", b);
	printf("delta_y: %lf\n", delta_y);
	printf("delta_a: %lf\n", delta_a);
	printf("delta_b: %lf\n\n", delta_b);
	
	free(valores_x);
	free(valores_y);
	return 0;
}
