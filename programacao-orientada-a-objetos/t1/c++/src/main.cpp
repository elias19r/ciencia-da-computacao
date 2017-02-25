/**
 * Trabalho 01: t1
 * 2013/03/31
 * Elias Italiano Rodrigues, 7987251, POO, BCC-B, ICMC-USP
 */

// Inclui o header do comunicador
#include <Messenger.h>

/**
 * Função principal
 */
int main(int argc, char *argv[]) {
	// Cria instância do comunicador e executa
	Messenger *msgr = new Messenger();
	msgr->execute();
	
	delete msgr;
	return 0;
}

