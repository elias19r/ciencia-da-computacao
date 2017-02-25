/**
 * trabalho04
 * 2012/10/02
 * Elias Italiano Rodrigues, 7987251, ICC2, BCC-B, ICMC-USP
 */

/**
 * Código dado pelo enunciado do trabalho
 */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

char *getpage(char *ip, int port, char *page) {
	int sockfd = 0, n = 0, count = 0;
	char *recvBuff = NULL, *sendBuff;
	struct sockaddr_in serv_addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return NULL;
	}
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	if(inet_pton(AF_INET, ip, &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return NULL;
	}
	if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return NULL;
	}
	// requesting the page
	// building the request string
	sendBuff = (char *) malloc(sizeof(char) * (strlen(page) + 7));
	snprintf(sendBuff, sizeof(char) * (strlen(page) + 7), "GET %s\r\n", page);
	// writing in the socket the request
	write(sockfd, sendBuff, strlen(sendBuff));
	// reading the page
//	int i = 0;
	count = 1;
	recvBuff = (char *) realloc(recvBuff, sizeof(char));
	// read each character of the page
	while ( (n = read(sockfd, &recvBuff[count-1], 1)) > 0)
	{
		//printf("%c", recvBuff[count]);
		recvBuff = (char *) realloc(recvBuff, sizeof(char) * (count+1));
		count++;
	}
	recvBuff[count-1] = '\0';
	free(sendBuff);
	
	return (recvBuff);
}

/**
 * Estruturas compondo uma lista dinâmica para armazenar os títulos
 * das notícias e a quantidade de palavras de cada uma.
 * Cada item é um título encontrado no arquivo XML.
 *
 * A lista contém também dois contadores: uma para o total de itens e
 * outro para o total de itens com descrição (quantidade de palavras diferente de zero).
 */
typedef struct {
	char titulo[300];
	int qtdePalavras;
} TipoItem;

typedef struct {
	TipoItem *itens;
	int totalItens;
	int totalItensComDescricao;
} TipoLista;

/**
 * Função para inicializar a lista, definindo nulo para os valores.
 */
void inicializarLista(TipoLista *lista) {
	lista->itens = NULL;
	lista->totalItens = 0;
	lista->totalItensComDescricao = 0;
}

/**
 * Pesquisa um item na lista pelo seu título.
 * @return -1 para não encontrado, ID do item para encontrado
 */
int pesquisarLista(TipoLista *lista, TipoItem item) {
	int i;
	
	for (i = 0; i < lista->totalItens; i++)
		if (strcmp(item.titulo, lista->itens[i].titulo) == 0)
			return i;
			
	return -1;
}

/**
 * Insere um item na lista. Caso o item já exista, não insere um novo, mas soma
 * a quantidade de palavras de sua descrição.
 */
void inserirListaItem(TipoLista *lista, TipoItem item) {
	// Pesquisa se item já existe na lista
	int id = pesquisarLista(lista, item);
	
	if (id == -1) {
		lista->itens = (TipoItem *) realloc(lista->itens, sizeof(TipoItem) * (lista->totalItens + 1));
		lista->itens[lista->totalItens] = item;
		if (lista->itens[lista->totalItens].qtdePalavras != 0) // confere quantidade de palavras e incrementa contador
			lista->totalItensComDescricao++;
		lista->totalItens++; // incrementa contador
	} else {
		// Soma a quantidade de palavras caso já exista na lista
		lista->itens[id].qtdePalavras += item.qtdePalavras;
	}

}

/**
 * Ordena a lista por ordem alfabética usando o algoritmo Insertion Sort.
 * Para isso, verifica o retorna da função strcmp.
 * http://www.cplusplus.com/reference/clibrary/cstring/strcmp/
 */
void ordenarLista(TipoLista *lista) {
	int i, j;
	TipoItem tmp;
	
	for (i = 1; i < lista->totalItens; i++) {
		j = i-1;
		
		while (j >= 0 && strcmp(lista->itens[j+1].titulo, lista->itens[j].titulo) < 0) {
			tmp = lista->itens[j];
			lista->itens[j] = lista->itens[j+1];
			lista->itens[j+1] = tmp;
			j--;
		}
	}
}

/**
 * Função que escaneia o conteúdo do arquivo XML baixado pela função getpage e
 * pega os títulos e suas respectivas descrições. Faz a contagem de palavras na
 * descrição e insere um novo item na lista.
 */
void escanearDados(char *stream, TipoLista *lista) {
	int count;
	// blocoComeco e blocoFim: delimitam todo o conteúdo de um <title> até o próximo <title>
	// strComeco e strFim: delimitam o conteúdo da string a ser copiada.
	char *blocoComeco, *blocoFim, *strComeco, *strFim, *palavra;
	TipoItem item;

	blocoComeco = strstr(stream, "<title>"); // encontra o primeiro <title>

	while (blocoComeco != NULL) {
		blocoFim = strstr(blocoComeco+7, "<title>"); // encontra o <title> (ou recebe o fim do conteúdo)
		if (blocoFim == NULL)
			blocoFim = blocoComeco + strlen(blocoComeco);
		
		// Decrementa blocoFim para que ele aponte o caracter imediatamente
		//     anterior ao <title> seguinte (ou para o fim do conteúdo)
		blocoFim--;
		
		// Coloca um \0. Pronto, agora temos todo bloco que precisamos delimitados por
		//     blocoComeco e blocoFim
		blocoFim[0] = '\0';
		
		// Demilita a string do título e salva o título
		strComeco = blocoComeco+7;
		strFim = strstr(strComeco, "</title>");
		strFim[0] = '\0';
		strcpy(item.titulo, strComeco);
		
		// Demilita a string da descrição e conta suas palavras
		strComeco = strstr(strFim+1, "<description>");
		if (strComeco != NULL) {
			strComeco += 13;
			strFim = strstr(strComeco, "</description>");
			strFim[0] = '\0';
			
			// Contando palavras com a função strtok
			count = 0;
			palavra = strtok(strComeco, " ,<>");
			while (palavra != NULL) {
				count++;
				palavra = strtok(NULL, " ,<>");
			}
			item.qtdePalavras = count;
		} else {
			item.qtdePalavras = 0;
		}
		
		// Insere o item na lista e redefine a variável item
		inserirListaItem(lista, item);
		item.titulo[0] = '\0';
		item.qtdePalavras = 0;
		
		// "Anda" o blocoComeco para o próximo <title>
		blocoComeco = strstr(blocoFim+1, "<title>");
	}
}

int main(int argc, char **argv) {
	char ip[100], endereco[300], *dados;
	int porta, i;
	TipoLista lista;
	
	// Leitura das entradas
	scanf("%s", ip);
	scanf("%d", &porta);
	scanf("%s", endereco);

	// Baixando a página com a função dada pelo enunciado do trabalho
	dados = getpage(ip, porta, endereco);
	
	// Tratando os dados
	inicializarLista(&lista);
	escanearDados(dados, &lista);
	ordenarLista(&lista);
	
	// Saídas e Liberação de Memória
	printf("%d\n%d\n", lista.totalItens, lista.totalItensComDescricao);
	for (i = 0; i < lista.totalItens; i++) {
		if (lista.itens[i].qtdePalavras)
			printf("%s : %d\n", lista.itens[i].titulo, lista.itens[i].qtdePalavras);
	}
	free(lista.itens);
	free(dados);
	
	return 0;
}
