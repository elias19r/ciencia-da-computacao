/**
 * projeto04
 * 2012/11/05
 * Elias Italiano Rodrigues, 7987251, AED1, BCC-A, ICMC-USP
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Estrutura de lista ligada para o número grande
typedef struct No {
	int digito;
	struct No *ant;
	struct No *prox;
} TipoNo;

typedef struct No *Apontador;
typedef struct {
	Apontador MS; // apontador para o dígito mais significativo
	Apontador LS; // apontador para o dígito menos significativo
	int base;     // número da base
	char sinal;   // -1: negativo; 1: positivo
	int contador; // quantidade de dígitos
} TipoLista;

/**
 * Faz a inicialização da lista
 */
void inicializarLista(TipoLista **L) {
	*L = (TipoLista *) malloc(sizeof(TipoLista));
	(*L)->MS = NULL;
	(*L)->LS = NULL;
	(*L)->base = 10;
	(*L)->sinal = 1;
	(*L)->contador = 0;
}

/**
 * Insere um dígito na lista
 */
void inserirLista(TipoLista *L, int D) {
	Apontador p;
	
	p = (Apontador) malloc(sizeof(TipoNo));
	p->digito = D;
	p->prox = NULL;
	
	if (L->MS == NULL) {
		p->ant = NULL;
		L->MS = p;
		L->LS = p;
	} else {
		p->ant = L->LS;
		L->LS->prox = p;
		L->LS = p;
	}
	
	L->contador++;
}

/**
 * Insere um dígito na lista, mas de maneira inversa
 */
void inserirListaInvertido(TipoLista *L, int D) {
	Apontador p;
	
	p = (Apontador) malloc(sizeof(TipoNo));
	p->digito = D;
	p->ant = NULL;

	if (L->LS == NULL) {
		p->prox = NULL;
		L->MS = p;
		L->LS = p;
	} else {
		p->prox = L->MS;
		L->MS->ant = p;
		L->MS = p;
	}
	
	L->contador++;
}

/**
 * Exclui uma lista desalocando da memória os seus nós
 */
void excluirLista(TipoLista *L) {
	int i = L->contador;
	Apontador p = L->MS;
	Apontador q = L->MS->prox;
	
	while (i != 0) {
		free(p);
		if (q != NULL) {
			q = q->prox;
			p = q;
		}
		i--;
	}
	
	free(L);
}

/**
 * Imprime a lista de dígitos
 */
void imprimirLista(TipoLista *L, char *separador) {
	int i = L->contador;
	Apontador p = L->MS;
	
	if (L->sinal == -1)
		printf("-");
	while (i != 0) {
		printf("%d", p->digito);
		p = p->prox;
		i--;
		if (L->base > 10 && i != 0)
			printf("%s", separador);
	}
	printf("\n");
}

/**
 * Converte um dígito em base qualquer para um nova base
 */
int converterDigito(int digito, int base) {
	char strNum[15] = "", strDig[5] = "", strRes[15] = "";
	int mod = digito % base, res = digito / base, i, j, tmp;

	while (res > 0) {
		sprintf(strDig, "%d", mod);
		strcat(strNum, strDig);
		tmp = res;
		res = res / base;
		mod = tmp % base;
	}
	
	sprintf(strDig, "%d", mod);
	strcat(strNum, strDig);
	
	for (j = 0, i = strlen(strNum)-1; i >= 0; i--, j++) {
		strRes[j] = strNum[i];
	}
	res = atoi(strRes);
	
	return res;
}

/**
 * Converte um número grande em base qualquer para uma nova base
 */
TipoLista *converterNumero(TipoLista *L, int base) {
	TipoLista *R = NULL;
	Apontador p;
	int i, resultado = 0;
	char *str;
	
	inicializarLista(&R);
	R->base = base;
	
	p = L->MS;
	i = L->contador;
	while (i != 0) {
		resultado = resultado * converterDigito(L->base, base) + converterDigito(p->digito, base);
		p = p->prox;
		i--;
	}
	
	str = (char *) malloc(sizeof(char) * (10000));
	sprintf(str, "%d", resultado);
	i = 0;
	while (str[i] != '\0') {
		inserirLista(R, str[i] - '0');
		i++;
	}
	free(str);
	excluirLista(L);
	
	return R;
}

/**
 * Efetua a soma de dois números grandes
 */
void somar(TipoLista *arg1, TipoLista *arg2, TipoLista *resultado) {
	if (arg1->sinal == arg2->sinal) {
		Apontador p1, p2;
		int vaiUm = 0, res, contador1, contador2;
		
		resultado->sinal = arg1->sinal;
		p1 = arg1->LS;
		p2 = arg2->LS;
		contador1 = arg1->contador;
		contador2 = arg2->contador;

		while (contador1 != 0 || contador2 != 0) {
			if (contador1 != 0 && contador2 != 0) {
				res = p1->digito + p2->digito + vaiUm;
				p1 = p1->ant;
				p2 = p2->ant;
				contador1--;
				contador2--;
			} else if (contador1 != 0) {
				res = p1->digito + vaiUm;
				p1 = p1->ant;
				contador1--;
			} else {
				res = p2->digito + vaiUm;
				p2 = p2->ant;
				contador2--;
			}
			vaiUm = res / arg1->base;
			res = res % arg1->base;
			inserirListaInvertido(resultado, res);
		}
		
	}
}

int main(int argc, char *argv[]) {
	char strArg1[1002], strArg2[1002], op[2], separador[101];
	int base, i;
	TipoLista *arg1 = NULL, *arg2 = NULL, *resultado = NULL;

	// Leitura das entradas	
	scanf("%s", strArg1);
	scanf("%s", op);
	scanf("%s", strArg2);
	scanf("%d", &base);
	scanf("%s", separador);
	
	// Inicialização das listas
	inicializarLista(&arg1);
	inicializarLista(&arg2);
	inicializarLista(&resultado);
	
	// Inserindo o argumento 1 na lista
	i = 0;
	if (strArg1[i] == '-') {
		arg1->sinal = -1;
		i++;
	}
	while (strArg1[i] != '\0') {
		inserirLista(arg1, strArg1[i] - '0');
		i++;
	}
	
	// Inserindo o argumento 2 na lista
	i = 0;
	if (strArg2[i] == '-') {
		arg2->sinal = -1;
		i++;
	}
	while (strArg2[i] != '\0') {
		inserirLista(arg2, strArg2[i] - '0');
		i++;
	}
	
	// Selecionando e executando a operação
	switch (op[0]) {
		case '+':
			somar(arg1, arg2, resultado);
		break;
		
		default:;
	}
	
	// Imprimindo o resultado
	resultado = converterNumero(resultado, base);
	imprimirLista(resultado, separador);
	
	// Liberação de memória
	excluirLista(arg1);
	excluirLista(arg2);

	return 0;
}
