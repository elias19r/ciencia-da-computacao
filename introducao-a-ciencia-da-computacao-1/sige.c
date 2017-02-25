/**
 * SiGE - Sistema para Gerenciamento de Estacionamentos
 *
 * Universidade de São Paulo (USP)
 * Instituto de Ciências Matemáticas e de Computação (ICMC)
 * Bacharelado em Ciências de Computação (BCC)
 *
 * Disciplina: SSC0102 - Laboratório de Introdução a Ciência de Computação I
 * Professor: Seiji Isotani
 *
 * Alunos:
 * Cassiano Zaghi, 7987400
 * Elias Rodrigues, 7987251
 * Rafael Fortunato,
 *
 * Em <mes> de 2012
 *
 */

// Inclui bibliotecas/arquivos
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
//#include <conio.h>

// Estruturas de dados
typedef struct
{
	int primeiro_acesso;
	char nome_estacionamento[201];
	int numero_vagas;
	float preco_primeira_hora;
	float preco_hora_adicional;
	int bonus;
	char cnpj[21];
	char cpf[16];
} Config;

typedef struct
{
	int numero;
	char cpf[16];
	char placa[10];
	int status;
	time_t hora_entrada;
	float preco;
} Vaga;

typedef struct
{
	char cpf[16];
	int tickets;
} Cliente;

typedef struct
{
	int id;
	char nome_estacionamento[201];
	int numero_vagas;
	float preco_primeira_hora;
	float preco_hora_adicional;
	int bonus;
	char cnpj[21];
	char cpf[16];
	int total_vagas;
	int total_vagas_ocupadas;
	int total_clientes;
	int total_entradas;
	int total_saidas;
	int total_fluxos;
	int total_bonus_concedidos;
	int total_tickets_distribuidos;
	float media_veiculos_hora;
	float media_veiculos_dia;
	float media_tempo_permanencia_veiculo;
	float ocupacao_vagas_porcentagens;
	double ocupacao_vagas_tempo;
	float media_ocupacao_vagas_dia;
	float total_arrecadado;
	double total_tempo_permanencia;
	time_t inicio_expediente;
	time_t fim_expediente;
	time_t ultimo_fluxo;
} Relatorio;

// Constantes e Variáveis globais
#define HORA 60.0
#define DIA 1440.0
#define MIN 1.0
#define TOLERANCIA 5.0

Config CONFIG = {0, "", 0, 0, 0, -1, "", ""};
Vaga VAGA;
Cliente CLIENTE;
Relatorio RELATORIO_ATUAL;
Relatorio RELATORIO;

// Protótipos de funções

/* Config */
void definirConfig(int campo);
int salvarConfig();
void exibirConfig();
int lerConfig();
void configuracoes();

/* Estacionamento */
void estacionamento();
void entradaVeiculo();
void saidaVeiculo();
void salvarExpediente();
void fecharExpediente();

/* Utilidades */
void removerLn(char *str);
void limparTeclado();
void imprimirMenu(int menu);
int primeiroAcesso();
void imprimirCabecalho(char titulo[255]);

/* Cliente */
void incrementarTickets();
void adicionarCliente(char *cpf);
int lerCliente(char *cpf);
int salvarCliente();
void tabelaClientes();
void imprimirTabelaClientes();
void imprimirClienteInfo();

/* Vaga */
int lerVaga(int numero);
float totalPagar(double tempo_permanencia);
int vagaOcupada();
time_t desocuparVaga(double tempo_permanencia);
time_t ocuparVaga(char *cpf, char *placa);
int salvarVaga();
void inicializarVagas();
void imprimirTabelaVagas();
double tempoPermanencia();
void imprimirVagaInfo();

/* Relatorio */
void relatorios();
void relatorioAtual();
void relatoriosAnteriores();
int lerRelatorioAtual();
int atualizarRelatorioAtual();
void inicializarRelatorioAtual();
void calcularEstatisticas(time_t agora);
int lerRelatorio(int id);
void imprimirTabelaRelatorios();



/*********************************************************************************/
// MAIN
/*********************************************************************************/

int main()
{
	int opcao;

	// Título da janela do programa
	//system("title SiGE");

	// Lê as configurações do arquivo config.dat para a variável global CONFIG e inicializa RELATORIO_ATUAL
	lerConfig();

	// Confere se é o primeiro acesso. Se sim, mostra a tela de Configurações
	if (primeiroAcesso())
	{
		inicializarRelatorioAtual();
		configuracoes();
	}

	// Lê as informações do arquivo relatorio_atual.dat para a variável global RELATORIO_ATUAL
	lerRelatorioAtual();

	do
	{
		imprimirMenu(0);
		printf("-- O que deseja fazer? --> ");
		scanf("%d", &opcao); limparTeclado();
		switch (opcao)
		{
			case 1:  { configuracoes();    break; }
			case 2:  { estacionamento();   break; }
			case 3:  { relatorios();       break; }

			case 8:  { fecharExpediente(); break; }
			case 9:  { break; }
			default: { opcao = 0; }
		}
	} while (opcao != 9);

	printf("\n\n-- Programa encerrado. Pressione qualquer tecla para fechar.\n\n");
	//getch();
	return 0;
}



/*********************************************************************************/
// CONFIGURAÇÕES
/*********************************************************************************/

/**
 * Define as configurações
 * @param int número do campo que será alterado
 */
void definirConfig(int campo)
{
	system("clear");
	imprimirCabecalho("<SiGE> Configuracoes : Definir");

	switch (campo)
	{
		case 1: // Nome do estacionamento
		{
			printf("-- Informe o nome do estacionamento --> ");
			fgets(CONFIG.nome_estacionamento, 200, stdin); limparTeclado(); removerLn(CONFIG.nome_estacionamento);
			break;
		}
		case 2: // Número de vagas
		{
			printf("-- Informe a quantidade de vagas --> ");
			scanf("%d", &(CONFIG.numero_vagas)); limparTeclado();
			break;
		}
		case 3: // Preço da primeira
		{
			printf("-- Informe o preco da primeira hora --> R$ ");
			scanf("%f", &(CONFIG.preco_primeira_hora)); limparTeclado();
			break;
		}
		case 4: // Preço da hora adicional
		{
			printf("-- Informe o preco da hora adicional --> R$ ");
			scanf("%f", &(CONFIG.preco_hora_adicional)); limparTeclado();
			break;
		}
		case 5: // Bônus
		{
			printf("-- Numero de vezes para Bonus (0 para desativar) --> ");
			scanf("%d", &(CONFIG.bonus)); limparTeclado();
			break;
		}
		case 6: // CNPJ
		{
			printf("-- CNPJ da empresa --> ");
			fgets(CONFIG.cnpj, 20, stdin); limparTeclado(); removerLn(CONFIG.cnpj);
			break;
		}
		case 7: // CPF
		{
			printf("-- CPF do proprietario --> ");
			fgets(CONFIG.cpf, 15, stdin); limparTeclado(); removerLn(CONFIG.cpf);
			break;
		}
	}
}

/**
 * Salva as configurações definidas para o arquivo config.dat
 * @return int 0 erro ao salvar, 1 configurações salvas
 */
int salvarConfig()
{
	int OK = 1;

	// Confere se nenhuma das configurações foi deixada "em branco"
	if ((CONFIG.nome_estacionamento[0] == '\0') || (CONFIG.numero_vagas == 0) || (CONFIG.preco_primeira_hora == 0) || (CONFIG.preco_hora_adicional == 0) || (CONFIG.bonus == -1) || (CONFIG.cnpj[0] == '\0') || (CONFIG.cpf[0] == '\0'))
	{
		OK = 0;
	}

	if (OK)
	{
		FILE *arquivo;
		arquivo = fopen("config.dat", "wb");

		if (arquivo == NULL)
		{
			perror ("\n\n>> Erro ao ler o arquivo \"config.dat\".\n\n");
			//getch();
			fclose(arquivo);
			return 0;
		}

		CONFIG.primeiro_acesso = 1;
		fwrite(&CONFIG, sizeof(Config), 1, arquivo);
		fclose(arquivo);

		// Se o número de vagas foi alterado, reinicializa o arquivo vagas.dat
		if (CONFIG.numero_vagas != RELATORIO_ATUAL.total_vagas)
		{
			inicializarVagas();
		}

		// Se o preço da hora for alterado, fecha o expediente e começa um novo
		if (RELATORIO_ATUAL.preco_primeira_hora != 0)
		{
			if ((CONFIG.preco_primeira_hora != RELATORIO_ATUAL.preco_primeira_hora) || (CONFIG.preco_hora_adicional != RELATORIO_ATUAL.preco_hora_adicional))
			{
				salvarExpediente();
			}
		}

		// Copia as informações do CONFIG para RELATORIO_ATUAL
		strcpy(RELATORIO_ATUAL.nome_estacionamento, CONFIG.nome_estacionamento);
		strcpy(RELATORIO_ATUAL.cnpj,                CONFIG.cnpj);
		strcpy(RELATORIO_ATUAL.cpf,                 CONFIG.cnpj);
		RELATORIO_ATUAL.numero_vagas              = CONFIG.numero_vagas;
		RELATORIO_ATUAL.preco_primeira_hora       = CONFIG.preco_primeira_hora;
		RELATORIO_ATUAL.preco_hora_adicional      = CONFIG.preco_hora_adicional;
		RELATORIO_ATUAL.bonus                     = CONFIG.bonus;
		RELATORIO_ATUAL.total_vagas               = CONFIG.numero_vagas;

		atualizarRelatorioAtual();

		return 1;
	}
	else
	{
		printf("\n\n>> Erro ao salvar: preencha todas as configuracoes.\n\n");
		//getch();
		return 0;
	}
}

/**
 * Exibe os valores das configurações na tela
 */
void exibirConfig()
{
	system("clear");
	imprimirCabecalho("<SiGE> Configuracoes : Exibir");

	printf("Nome do estacionamento: %s\n\n", CONFIG.nome_estacionamento);
	printf("Numero de vagas: %d\n\n", CONFIG.numero_vagas);
	printf("Preco da primeira hora: R$ %.2f\n\n", CONFIG.preco_primeira_hora);
	printf("Preco da hora adicional: R$ %.2f\n\n", CONFIG.preco_hora_adicional);
	printf("Numero de vezes para Bonus: %d\n\n", CONFIG.bonus);
	printf("CNPJ da empresa: %s\n\n", CONFIG.cnpj);
	printf("CPF do proprietario: %s\n\n", CONFIG.cpf);

	printf("\n-- Pressione qualquer tecla para voltar.");
	//getch();
}

/**
 * Lê as configurações
 * Carrega o arquivo config.dat para a variável global CONFIG
 * @return int 0 erro ao ler arquivo, 1 arquivo lido
 */
int lerConfig()
{
	FILE *arquivo;
	arquivo = fopen("config.dat", "rb");

	if (arquivo == NULL)
	{
		perror ("\n\n>> Erro ao ler o arquivo \"config.dat\".\n\n");
		//getch();
		fclose(arquivo);
		return 0;
	}

	fread(&CONFIG, sizeof(Config), 1, arquivo);
	fclose(arquivo);
	return 1;
}

/**
 * Mostra a tela de Configurações
 */
void configuracoes()
{
	int opcao;

	do
	{
		imprimirMenu(1);
		printf("-- O que deseja fazer? --> ");
		scanf("%d", &opcao); limparTeclado();
		switch (opcao)
		{
			case 1: { definirConfig(1); break; }
			case 2: { definirConfig(2); break; }
			case 3: { definirConfig(3); break; }
			case 4: { definirConfig(4); break; }
			case 5: { definirConfig(5); break; }
			case 6: { definirConfig(6); break; }
			case 7: { definirConfig(7); break; }

			case 8: { exibirConfig();   break; }
			case 9:
			{
				if (!salvarConfig())
				{
					opcao = 0;
				}
				break;
			}
			default: { opcao = 0; }
		}
	} while (opcao != 9);
}



/*********************************************************************************/
// ESTACIONAMENTO
/*********************************************************************************/

/**
 * Imprime informações sobre uma vaga lida
 */
void imprimirVagaInfo()
{
	printf("Informacoes sobre a vaga selecionada:\n");

	printf("\nVaga: %d", VAGA.numero);
	if (VAGA.status != 0)
	{
		double tempo_permanencia, h, min;

		printf("\nStatus: Ocupada");
		printf("\nVeiculo: %s", VAGA.placa);
		printf("\nCliente: %s", VAGA.cpf);
		tempo_permanencia = tempoPermanencia();
		h = (int) (tempo_permanencia / HORA);
		min = (tempo_permanencia - (h * HORA)) / MIN;
		printf("\nTempo de permanencia: %dh %dmin\n\n", (int) h, (int) min);
	}
	else
	{
		printf("\nStatus: Livre\n\n");
	}
}

/**
 * Mostra a tela da Tabela de Vagas
 */
void tabelaVagas()
{
	int numero = 0;

	do
	{
		system("clear");
		imprimirCabecalho("<SiGE> Tabela de Vagas");
		imprimirTabelaVagas();

		if (numero != 0)
		{
			if (lerVaga(numero))
			{
				imprimirVagaInfo();
			}
		}

		printf("\n-- Para mais informacoes, informe o numero de uma vaga (0 para voltar) --> "); scanf("%d", &numero); limparTeclado();
	} while (numero != 0);
}

/**
 * Mostra a tela de Estacionamento
 */
void estacionamento()
{
	int opcao;

	do
	{
		imprimirMenu(2);
		printf("-- O que deseja fazer? --> ");
		scanf("%d", &opcao); limparTeclado();
		switch (opcao)
		{
			case 1:  { entradaVeiculo(); break; }
			case 2:  { saidaVeiculo();   break; }
			case 3:  { tabelaVagas();    break; }
			case 4:  { tabelaClientes(); break; }

			case 9:  { break; }
			default: { opcao = 0; }
		}
	} while (opcao != 9);
}

/**
 * Efetua a Entrada de veículo
 */
void entradaVeiculo()
{
	int numero, ocupada;
	char placa[10], cpf[16];
	time_t agora;

	system("clear");
	imprimirCabecalho("<SiGE> Entrada de Veiculo");

	// Imprime a tabela de vagas
	imprimirTabelaVagas();

	while (1)
	{
		printf(">> Para cancelar, informe 0.\n\n");
		printf("\n-- Numero da vaga --> "); scanf("%d", &numero); limparTeclado();
		if (numero == 0)
		{
			break;
		}

		if (lerVaga(numero))
		{
			ocupada = vagaOcupada();
			if (ocupada)
			{
				printf("\n\n>> Erro: esta vaga ja esta ocupada. Desocupe-a primeiro ou escolha outra vaga.\n\n");
				break;
			}
			else
			{
				printf("\n-- Placa do veiculo --> "); fgets(placa, 9, stdin); limparTeclado(); removerLn(placa);
				if (placa[0] == '0')
				{
					break;
				}
				printf("\n-- CPF do cliente --> "); fgets(cpf, 15, stdin); limparTeclado(); removerLn(cpf);
				if (cpf[0] == '0')
				{
					break;
				}

				if (!lerCliente(cpf))
				{
					adicionarCliente(cpf);
				}

				// Marca a vaga selecionada como ocupada e com as informações do cliente
				agora = ocuparVaga(cpf, placa);
				salvarVaga();

				calcularEstatisticas(agora);
				atualizarRelatorioAtual();

				printf("\n\n Entrada registrada!");
				break;
			}
		}
		else
		{
			printf("\n\n>> Erro: vaga inexistente.");
			break;
		}
	}

	printf("\n\n-- Pressione qualquer tecla para voltar.\n\n");
	//getch();
}

/**
 * Efetua a Saída de veículo
 */
void saidaVeiculo()
{
	int numero;
	time_t agora;

	system("clear");
	imprimirCabecalho("<SiGE> Saida de Veiculo");

	// Imprime a tabela de vagas
	imprimirTabelaVagas();

	while (1)
	{
		printf(">> Para cancelar, informe 0.\n\n");
		printf("\n-- Numero da vaga para retirada --> "); scanf("%d", &numero); limparTeclado();
		if (numero == 0)
		{
			break;
		}

		if (lerVaga(numero))
		{
			if (vagaOcupada())
			{
				float total_pagar, recebido, troco;
				double tempo_permanencia, h, min;
				//char cpf[16];

				tempo_permanencia = tempoPermanencia();
				lerCliente(VAGA.cpf);
				agora = desocuparVaga(tempo_permanencia);
				salvarVaga();

				h = (int) (tempo_permanencia / HORA);
				min = (tempo_permanencia - (h * HORA)) / MIN;

				printf("\nTempo de permanencia: %dh %dmin\n", (int) h, (int) min);

				if (CLIENTE.tickets >= CONFIG.bonus)
				{
					CLIENTE.tickets = 0;
					printf("\nCliente tem bonus acumulado!\n");
					RELATORIO_ATUAL.total_bonus_concedidos++;
				}
				else
				{
					incrementarTickets();
					total_pagar = totalPagar(tempo_permanencia);
					printf("\nTotal a pagar: R$ %.2f\n", total_pagar);
					printf("\n-- Recebido: R$ --> "); scanf("%f", &recebido); limparTeclado();
					while (recebido < total_pagar)
					{
						printf("\n>> Erro: valor recebido nao e suficiente.\n");
						printf("\n-- Recebido: R$ --> "); scanf("%f", &recebido); limparTeclado();
					}
					troco = recebido - total_pagar;
					printf("\nTroco: R$ %.2f\n", troco);
				}
				salvarCliente();

				calcularEstatisticas(agora);
				atualizarRelatorioAtual();

				printf("\n\n Saida registrada!");
				break;
			}
			else
			{
				printf("\n\n>> Erro: esta vaga ja esta livre!");
				break;
			}
		}
		else
		{
			printf("\n\n >> Erro: vaga inexistente.\n\n");
			break;
		}
	}

	printf("\n\n-- Pressione qualquer tecla para voltar.\n\n");
	//getch();
}

/**
 * Salva o expediente atual e finaliza o programa,
 * dando início a um novo expediente
 */
void salvarExpediente()
{
	FILE *arquivo;
	Relatorio RELATORIO_LEITURA;

	RELATORIO_ATUAL.fim_expediente = time(NULL);

	arquivo = fopen("relatorios.dat", "a+b");
	fseek(arquivo, -(long) sizeof(Relatorio), SEEK_END);
	fread(&RELATORIO_LEITURA, sizeof(Relatorio), 1, arquivo);
	fseek(arquivo, 0, SEEK_END);
	RELATORIO_ATUAL.id = RELATORIO_LEITURA.id + 1;
	fwrite(&RELATORIO_ATUAL, sizeof(Relatorio), 1, arquivo);
	fclose(arquivo);

	// Reinicializa o RELATORIO_ATUAL
	inicializarRelatorioAtual();
	atualizarRelatorioAtual();

	system("clear");
	imprimirCabecalho(" <SiGE> Iniciando novo expediente...");
	printf("\n\n>> Novo expediente inicializado!\n\n-- Pressione qualquer tecla para continuar.\n\n");
	//getch();
}

/**
 * Mostra a tela de Fechar expediente
 */
void fecharExpediente()
{
	int opcao;

//	do
//	{
		imprimirMenu(4);
		printf("-- O que deseja fazer? --> ");
		scanf("%d", &opcao); limparTeclado();
		switch (opcao)
		{
			case 1:  { salvarExpediente(); break; }
			case 2:  { break; }
			default: { opcao = 0; }
		}
//	} while (opcao != 2);
}



/*********************************************************************************/
// UTILIDADES
/*********************************************************************************/

/**
 * Função para limpar o teclado, resolvendo o problema encontrado
 * quando se usa scanf() antes de gets()
 */
void limparTeclado()
{
	//while (getchar() != '\n');
	fflush(stdin);
}

/**
 * Imprime menus
 * @param int menu é o número do menu a ser impresso
 */
void imprimirMenu(int menu)
{
	switch (menu)
	{
		case 0: // Principal
		{
			system("clear");
			imprimirCabecalho("Sistema Para Gerenciamento de Estacionamentos <SiGE>");
			printf("1. Configuracoes\n\n2. Estacionamento\n\n3. Relatorios\n\n\n8. Fechar expediente\n\n9. Sair\n\n\n");
			break;
		}
		case 1: // Configurações
		{
			system("clear");
			imprimirCabecalho("<SiGE> Configuracoes");
			printf("1. Nome do estacionamento\n\n2. Numero de vagas\n\n3. Preco da hora (primeira)\n\n4. Preco da hora adicional\n\n5. Bonus\n\n6. CNPJ da empresa\n\n7. CPF do proprietario\n\n\n8. Exibir\n\n9. Salvar e voltar\n\n\n");
			break;
		}
		case 2: // Estacionamento
		{
			system("clear");
			imprimirCabecalho("<SiGE> Estacionamento");
			printf("1. Entrada de veiculo\n\n2. Saida de veiculo\n\n3. Tabela de Vagas\n\n4. Clientes\n\n\n9. Voltar\n\n\n");
			break;
		}
		case 3: // Relatórios
		{
			system("clear");
			imprimirCabecalho("<SiGE> Relatorios");
			printf("1. Atual\n\n2. Anteriores\n\n\n9. Voltar\n\n\n");
			break;
		}
		case 4: // Fechar expediente
		{
			system("clear");
			imprimirCabecalho("<SiGE> Fechar expediente");
			printf("Deseja salvar o expediente atual e iniciar um novo?\n\n\n1. Sim\n\n2. Cancelar (voltar)\n\n\n");
			break;
		}
	}
}

/**
 * Função para conferir se este é o primeiro acesso do usuário ao programa
 * @return int 0 se NÃO for o primeiro acesso, 1 se for o primeiro acesso
 */
int primeiroAcesso()
{
	if (CONFIG.primeiro_acesso != 1)
	{
		return 1;
	}

	return 0;
}

/**
 * Remove o '\n' do final de uma string
 * Usado para remover '\n' que o fgets captura quando lê uma string
 */
void removerLn(char *str)
{
	int i = strlen(str) - 1;
	if (str[i] == '\n')
	{
		str[i] = '\0';
	}
}

/**
 * Imprime um cabeçalho
 */
void imprimirCabecalho(char titulo[255])
{
	printf("------------------------------------------------------\n");
	printf(" %s\n", titulo);
	printf("------------------------------------------------------\n\n");
}



/*********************************************************************************/
// CLIENTE
/*********************************************************************************/

/**
 * Incrementa tickets do cliente
 */
void incrementarTickets()
{
	if (CONFIG.bonus != 0)
	{
		CLIENTE.tickets++;
		RELATORIO_ATUAL.total_tickets_distribuidos++;
	}
}

/**
 * Adiciona um cliente em clientes.dat e inicializa seus tickets
 */
void adicionarCliente(char *cpf)
{
	FILE *arquivo;

	arquivo = fopen("clientes.dat", "ab");
	strcpy(CLIENTE.cpf, cpf);
	CLIENTE.tickets = 0;
	fwrite(&CLIENTE, sizeof(Cliente), 1, arquivo);
	fclose(arquivo);

	RELATORIO_ATUAL.total_clientes++;
}

/**
 * Imprime informações sobre um cliente lido
 */
void imprimirClienteInfo()
{
	printf("Informacoes sobre o cliente selecionado:\n");

	printf("\nCPF: %s", CLIENTE.cpf);
	printf("\nTickets: %d\n\n", CLIENTE.tickets);
}

/**
 * Imprime uma tabela de clientes
 */
void imprimirTabelaClientes()
{
	FILE *arquivo;
	int i;
	static int ultimo = 0;

	arquivo = fopen("clientes.dat", "rb");
	fseek(arquivo, ultimo * (long) sizeof(Cliente), SEEK_SET);
	printf("  CPF              |  Tickets\n-------------------------------\n");
	for (i = 0; i < 5; i++)
	{
		fread(&CLIENTE, sizeof(Cliente), 1, arquivo);
		if (feof(arquivo))
		{
			ultimo = 0;
			break;
		}
		printf("%16s      %d\n", CLIENTE.cpf, CLIENTE.tickets);
		ultimo++;
	}
	fclose(arquivo);
}

/**
 * Mostra tela da Tabela de Clientes
 */
void tabelaClientes()
{
	char cpf[16] = {'\n'};

	do
	{
		system("clear");
		imprimirCabecalho("<SiGE> Tabela de Clientes");

		if (cpf[0] != '\n')
		{
			removerLn(cpf);
			if (lerCliente(cpf))
			{
				imprimirClienteInfo();
			}
		}
		else
		{
			imprimirTabelaClientes();
		}

		printf("\n\n>> Para listar mais clientes, aperte ENTER. Para voltar, informe 0.");
		printf("\n\n-- Para mais detalhes, informe o CPF de um cliente --> "); fgets(cpf, 15, stdin); limparTeclado();
	} while (cpf[0] != '0');
}

/**
 * Lê um cliente do arquivo clientes.dat para a variável global CLIENTE
 * @return int 0 para NÃO conseguiu ler, 1 para conseguiu ler
 */
int lerCliente(char *cpf)
{
	FILE *arquivo;

	arquivo = fopen("clientes.dat", "rb");
	while (!feof(arquivo))
	{
		fread(&CLIENTE, sizeof(Cliente), 1, arquivo);
		if (strcmp(CLIENTE.cpf, cpf) == 0)
		{
			fclose(arquivo);
			return 1;
		}
	}
	fclose(arquivo);
	return 0;
}

/**
 * Salva (atualiza - "update") os dados da variável global CLIENTE no arquivo clientes.dat
 * @return int 0 em caso de erro, 1 para salvo
 */
int salvarCliente()
{
	int encontrado = 0;
	FILE *arquivo;
	Cliente CLIENTE_LEITURA;

	arquivo = fopen("clientes.dat", "r+b");
	if (arquivo == NULL)
	{
		perror ("\n\n>> Erro ao ler o arquivo \"clientes.dat\".\n\n");
		//getch();
		fclose(arquivo);
		return 0;
	}
	else
	{
		while (!feof(arquivo))
		{
			fread(&CLIENTE_LEITURA, sizeof(Cliente), 1, arquivo);
			if (strcmp(CLIENTE_LEITURA.cpf, CLIENTE.cpf) == 0)
			{
				fseek(arquivo, -(long) sizeof(Cliente), 1);
				encontrado = 1;
				break;
			}
		}
		if (encontrado)
		{
			fwrite(&CLIENTE, sizeof(Cliente), 1, arquivo);
			fclose(arquivo);
			return 1;
		}
		else
		{
			fclose(arquivo);
			return 0;
		}
	}
}


/*********************************************************************************/
// VAGA
/*********************************************************************************/

/**
 * Lê uma vaga do arquivo vagas.dat para a variável global VAGA
 * @return int 0 para NÃO conseguiu ler, 1 para conseguiu ler
 */
int lerVaga(int numero)
{
	FILE *arquivo;

	arquivo = fopen("vagas.dat", "rb");
	while (!feof(arquivo))
	{
		fread(&VAGA, sizeof(Vaga), 1, arquivo);
		if (VAGA.numero == numero)
		{
			fclose(arquivo);
			return 1;
		}
	}
	fclose(arquivo);
	return 0;
}

/**
 * Calcula o total a pagar com base no tempo de permanencia na vaga
 * e nos valores das horas
 * @param double tempo_permanencia em segundos
 * @return float total_pagar o valor a ser pago
 */
float totalPagar(double tempo_permanencia)
{
	float total_pagar;
	double horas, minutos;

	horas = (int) (tempo_permanencia / HORA);
	minutos = tempo_permanencia - (horas * HORA) / MIN;

	if (minutos > TOLERANCIA)
	{
		horas = horas + 1;
	}

	// Calcula total a pagar
	total_pagar = CONFIG.preco_primeira_hora + ((horas - 1) * CONFIG.preco_hora_adicional);

	RELATORIO_ATUAL.total_arrecadado += total_pagar;
	return total_pagar;
}

/**
 * Verifica se a vaga está ocupada
 * @return int 0 para livre, 1 para ocupada
 */
int vagaOcupada()
{
	if (VAGA.status == 1)
	{
		return 1;
	}

	return 0;
}

/**
 * Desocupa uma vaga previamente lida
 * @param double tempo_permanencia é o tempo de a vaga ficou o ocupada
 * @return time_t a hora em que a vaga foi desocupada
 */
time_t desocuparVaga(double tempo_permanencia)
{
	VAGA.status = 0;
	strcpy(VAGA.cpf, "0");
	strcpy(VAGA.placa, "0");
	VAGA.hora_entrada = 0;

	RELATORIO_ATUAL.total_tempo_permanencia += tempo_permanencia;
	RELATORIO_ATUAL.total_saidas++;
	RELATORIO_ATUAL.total_fluxos++;
	RELATORIO_ATUAL.total_vagas_ocupadas--;

	return time(NULL);
}

/**
 * Retorna o tempo de permanência de um veículo na vaga até o momento
 */
double tempoPermanencia()
{
	double tempo_permanencia = difftime(time(NULL), VAGA.hora_entrada);
	return tempo_permanencia;
}


/**
 * Ocupa uma vaga previamente lida
 * @return time_t a hora em que a vaga foi ocupada
 */
time_t ocuparVaga(char *cpf, char *placa)
{
	strcpy(VAGA.cpf, cpf);
	strcpy(VAGA.placa, placa);
	VAGA.status = 1;
	VAGA.hora_entrada = time(NULL);

	RELATORIO_ATUAL.total_vagas_ocupadas++;
	RELATORIO_ATUAL.total_entradas++;

	return VAGA.hora_entrada;
}

/**
 * Salva os dados da variável global vagas no arquivo vagas.dat
 * @return int 0 para NÃO salvo, 1 para salvo
 */
int salvarVaga()
{
	int encontrado = 0;
	FILE *arquivo;
	Vaga VAGA_LEITURA;

	arquivo = fopen("vagas.dat", "r+b");
	if (arquivo == NULL)
	{
		perror ("\n\n>> Erro ao ler o arquivo \"vagas.dat\".\n\n");
		//getch();
		fclose(arquivo);
		return 0;
	}
	else
	{
		while (!feof(arquivo))
		{
			fread(&VAGA_LEITURA, sizeof(Vaga), 1, arquivo);
			if (VAGA_LEITURA.numero == VAGA.numero)
			{
				fseek(arquivo, -(long) sizeof(Vaga), 1);
				encontrado = 1;
				break;
			}
		}
		if (encontrado)
		{
			fwrite(&VAGA, sizeof(Vaga), 1, arquivo);
			fclose(arquivo);
			return 1;
		}
		else
		{
			fclose(arquivo);
			return 0;
		}
	}
}

/**
 * Inicializa as vagas reescrevendo o arquivo vagas.dat
 */
void inicializarVagas()
{
	int i;
	FILE *arquivo;

	VAGA.cpf[0] = '\0';
	VAGA.placa[0] = '\0';
	VAGA.status = 0;
	VAGA.hora_entrada = 0;
	VAGA.preco = 0;

	arquivo = fopen("vagas.dat", "wb");
	for (i = 0; i < CONFIG.numero_vagas; i++)
	{
		VAGA.numero = i + 1;
		fwrite(&VAGA, sizeof(Vaga), 1, arquivo);
	}
	fclose(arquivo);
}

/**
 * Imprime uma tabela de vagas
 */
void imprimirTabelaVagas()
{
	int i, count = 0, colunas = 3;
	char status[10] = "";

	for (i = 1; i <= CONFIG.numero_vagas; i++)
	{
		lerVaga(i);

		if (VAGA.status == 0)
		{
			strcpy(status, " LIVRE ");
		}
		else
		{
			strcpy(status, VAGA.placa);
		}
		printf("Vaga %3d: %8s  |  ", VAGA.numero, status);
		count++;

		if (count == colunas)
		{
			printf("\n                    |                      |                      |\n");
			count = 0;
		}
	}
	printf("\n\n");
}



/*********************************************************************************/
// RELATORIO
/*********************************************************************************/

/**
 * Mostra a tela de Relatórios
 */
void relatorios()
{
	int opcao;

	do
	{
		imprimirMenu(3);
		printf("-- O que deseja fazer? --> ");
		scanf("%d", &opcao); limparTeclado();
		switch (opcao)
		{
			case 1:  { relatorioAtual();       break; }
			case 2:  { relatoriosAnteriores(); break; }

			case 9:  { break; }
			default: { opcao = 0; }
		}
	} while (opcao != 9);
}

/**
 * Imprime o relatório atual
 */
void relatorioAtual()
{
	system("clear");
	imprimirCabecalho("<SiGE> Relatorio Atual");

	printf("  Estacionamento: %s\n\n", CONFIG.nome_estacionamento);
	printf("  CNPJ/CPF: %s / %s\n\n\n", CONFIG.cnpj, CONFIG.cpf);

	printf("  Total de Vagas: %d  (ocupadas: %d)\n\n", RELATORIO_ATUAL.total_vagas, RELATORIO_ATUAL.total_vagas_ocupadas);
	printf("  Total de Entradas: %d\n\n", RELATORIO_ATUAL.total_entradas);
	printf("  Total de Saidas: %d\n\n", RELATORIO_ATUAL.total_saidas);
	printf("  Total de Fluxos (entradas e saidas completas): %d\n\n", RELATORIO_ATUAL.total_fluxos);
	printf("  Total de Clientes Cadastrados: %d\n\n\n\n", RELATORIO_ATUAL.total_clientes);

	printf("  Total de Arrecadado: R$ %.2f\n\n", RELATORIO_ATUAL.total_arrecadado);
	printf("  Total de Bonus Concedidos: %d\n\n", RELATORIO_ATUAL.total_bonus_concedidos);
	printf("  Total de Tickets Distribuidos: %d\n\n\n\n", RELATORIO_ATUAL.total_tickets_distribuidos);

	printf("  Media veiculos/hora: %.1f\n\n", RELATORIO_ATUAL.media_veiculos_hora);
	printf("  Media veiculos/dia: %.1f\n\n", RELATORIO_ATUAL.media_veiculos_dia);
	printf("  Media de tempo de permanencia por veiculo: %.1f h\n\n", RELATORIO_ATUAL.media_tempo_permanencia_veiculo);
	printf("  Media de ocupacao das vagas por dia: %.1f%% \n\n", RELATORIO_ATUAL.media_ocupacao_vagas_dia);

	printf("\n\n\n-- Pressione qualquer tecla para voltar.");
	//getch();
}

/**
 * Imprime um relatório já lido
 */
void imprimirRelatorio()
{
	struct tm *inicio, *termino;

	system("clear");
	imprimirCabecalho("<SiGE> Relatorio : Impressao");

	printf("  Estacionamento: %s\n\n", RELATORIO.nome_estacionamento);
	printf("  CNPJ/CPF: %s / %s\n\n\n", RELATORIO.cnpj, RELATORIO.cpf);

	inicio = localtime(&RELATORIO.inicio_expediente);
	printf("  Inicio do expediente: %2d/%2d/%4d %dh%d\n\n", inicio->tm_mday, inicio->tm_mon + 1, inicio->tm_year + 1900, inicio->tm_hour, inicio->tm_min);
	termino = localtime(&RELATORIO.fim_expediente);
	printf("  Termino do expediente: %2d/%2d/%4d %dh%d\n\n\n", termino->tm_mday, termino->tm_mon + 1, termino->tm_year + 1900, termino->tm_hour, termino->tm_min);

	printf("  Total de Vagas: %d  (ocupadas: %d)\n\n", RELATORIO.total_vagas, RELATORIO.total_vagas_ocupadas);
	printf("  Total de Entradas: %d\n\n", RELATORIO.total_entradas);
	printf("  Total de Saidas: %d\n\n", RELATORIO.total_saidas);
	printf("  Total de Fluxos (entradas e saidas completas): %d\n\n", RELATORIO.total_fluxos);
	printf("  Total de Clientes Cadastrados: %d\n\n\n\n", RELATORIO.total_clientes);

	printf("  Total de Arrecadado: R$ %.2f\n\n", RELATORIO.total_arrecadado);
	printf("  Total de Bonus Concedidos: %d\n\n", RELATORIO.total_bonus_concedidos);
	printf("  Total de Tickets Distribuidos: %d\n\n\n\n", RELATORIO.total_tickets_distribuidos);

	printf("  Media veiculos/hora: %.1f\n\n", RELATORIO.media_veiculos_hora);
	printf("  Media veiculos/dia: %.1f\n\n", RELATORIO.media_veiculos_dia);
	printf("  Media de tempo de permanencia por veiculo: %.1f h\n\n", RELATORIO.media_tempo_permanencia_veiculo);
	printf("  Media de ocupacao das vagas por dia: %.1f%% \n\n", RELATORIO.media_ocupacao_vagas_dia);

	printf("\n\n\n-- Pressione qualquer tecla para voltar.");
	//getch();
}

/**
 * Mostra uma lista dos relatórios já salvos no
 * computador e permite que o usuário selecione um deles para
 * exibir na tela
 */
void relatoriosAnteriores()
{
	int id = 0;

	do
	{
		system("clear");
		imprimirCabecalho("<SiGE> Relatorios anteriores");

		if (id != 0)
		{
			if (lerRelatorio(id))
			{
				imprimirRelatorio();
			}
		}
		else
		{
			imprimirTabelaRelatorios();
		}

		printf("\n\n>> Para listar mais relatorios, informe 0. Para voltar, informe -1.");
		printf("\n\n-- Para mais detalhes, informe a ID de um relatorio --> "); scanf("%d", &id); limparTeclado();
	} while (id != -1);
}

/**
 * Imprime uma tabela com uma lista de relatórios
 */
void imprimirTabelaRelatorios()
{
	FILE *arquivo;
	int i;
	static int ultimo = 0;

	arquivo = fopen("relatorios.dat", "rb");
	fseek(arquivo, ultimo * (long) sizeof(Relatorio), SEEK_SET);
	printf("  ID        |  Inicio             |  Termino\n-------------------------------------------------------\n");
	for (i = 0; i < 5; i++)
	{
		struct tm *inicio, *termino;

		fread(&RELATORIO, sizeof(Relatorio), 1, arquivo);
		if (feof(arquivo))
		{
			ultimo = 0;
			break;
		}
		printf("%5d          ", RELATORIO.id);
		inicio  = localtime(&RELATORIO.inicio_expediente);
		printf("%2d/%2d/%4d %2dh%2d      ", inicio->tm_mday, inicio->tm_mon + 1, inicio->tm_year + 1900, inicio->tm_hour, inicio->tm_min);
		termino = localtime(&RELATORIO.fim_expediente);
		printf("%2d/%2d/%4d %2dh%2d\n", termino->tm_mday, termino->tm_mon + 1, termino->tm_year + 1900, termino->tm_hour, termino->tm_min);
		ultimo++;
	}
	fclose(arquivo);
}

/**
 * Lê um relatório para a variável global RELATORIO
 * @param int id é a id do relatório
 * @return int 0 para NÃO conseguiu ler, 1 para lido
 */
int lerRelatorio(int id)
{
	FILE *arquivo;

	arquivo = fopen("relatorios.dat", "rb");
	while (!feof(arquivo))
	{
		fread(&RELATORIO, sizeof(Relatorio), 1, arquivo);
		if (RELATORIO.id == id)
		{
			fclose(arquivo);
			return 1;
		}
	}
	fclose(arquivo);
	return 0;
}

/**
 * Lê o relatório atual para a variável global RELATORIO_ATUAL
 * @return int 0 para NÃO conseguiu ler, 1 para lido
 */
int lerRelatorioAtual()
{
	FILE *arquivo;

	arquivo = fopen("relatorio_atual.dat", "rb");
	if (arquivo == NULL)
	{
		perror ("\n\n>> Erro ao ler o arquivo \"relatorio_atual.dat\".\n\n");
		//getch();
		fclose(arquivo);
		return 0;
	}

	fread(&RELATORIO_ATUAL, sizeof(Relatorio), 1, arquivo);
	fclose(arquivo);
	return 1;
}

/**
 * Inicializa o RELATORIO_ATUAL
 */
void inicializarRelatorioAtual()
{
	RELATORIO_ATUAL.id = 0;
//	strcpy(RELATORIO_ATUAL.nome_estacionamento, "");
//	RELATORIO_ATUAL.numero_vagas = 0;
//	RELATORIO_ATUAL.preco_primeira_hora = 0;
//	RELATORIO_ATUAL.preco_hora_adicional = 0;
//	RELATORIO_ATUAL.bonus = -1;
//	strcpy(RELATORIO_ATUAL.cnpj, "");
//	strcpy(RELATORIO_ATUAL.cpf, "");
//	RELATORIO_ATUAL.total_vagas = 0;
//	RELATORIO_ATUAL.total_vagas_ocupadas = 0;
//	RELATORIO_ATUAL.total_clientes = 0;
	RELATORIO_ATUAL.total_entradas = 0;
	RELATORIO_ATUAL.total_saidas = 0;
	RELATORIO_ATUAL.total_fluxos = 0;
	RELATORIO_ATUAL.total_bonus_concedidos = 0;
	RELATORIO_ATUAL.total_tickets_distribuidos = 0;
	RELATORIO_ATUAL.media_veiculos_hora = 0;
	RELATORIO_ATUAL.media_veiculos_dia = 0;
	RELATORIO_ATUAL.media_tempo_permanencia_veiculo = 0;
	RELATORIO_ATUAL.ocupacao_vagas_porcentagens = 0;
	RELATORIO_ATUAL.ocupacao_vagas_tempo = 0;
	RELATORIO_ATUAL.media_ocupacao_vagas_dia = 0;
	RELATORIO_ATUAL.total_arrecadado = 0;
	RELATORIO_ATUAL.total_tempo_permanencia = 0;
	RELATORIO_ATUAL.inicio_expediente = time(NULL);
	RELATORIO_ATUAL.fim_expediente = 0;
	RELATORIO_ATUAL.ultimo_fluxo = 0;
}

/**
 * Atualiza (salva - "update") o arquivo relatorio_atual.dat
 * @return int 0 erro ao atualizar, 1 atualizado
 */
int atualizarRelatorioAtual()
{
	FILE *arquivo;

	arquivo = fopen("relatorio_atual.dat", "wb");
	if (arquivo == NULL)
	{
		perror ("\n\n>> Erro ao ler o arquivo \"relatorio_atual.dat\".\n\n");
		//getch();
		fclose(arquivo);
		return 0;
	}

	fwrite(&RELATORIO_ATUAL, sizeof(Relatorio), 1, arquivo);
	fclose(arquivo);
	return 1;
}

/**
 * Função para calcular as estatisticas sobre o estacionamento
 */
void calcularEstatisticas(time_t agora)
{
	double segundos = difftime(agora, RELATORIO_ATUAL.inicio_expediente);

	if (RELATORIO_ATUAL.total_fluxos != 0)
	{
		double horas = segundos / HORA;
		double dias = segundos / DIA;

		if (horas < 1)
		{
			horas = 1;
		}

		if (dias < 1)
		{
			dias = 1;
		}

		// Media veiculos/hora
		RELATORIO_ATUAL.media_veiculos_hora = (double) RELATORIO_ATUAL.total_fluxos / horas;

		// Media veiculos/dia
		RELATORIO_ATUAL.media_veiculos_dia = (double) RELATORIO_ATUAL.total_fluxos / dias;

		// Media veiculos/tempo de permanência (h)
		RELATORIO_ATUAL.media_tempo_permanencia_veiculo = (RELATORIO_ATUAL.total_tempo_permanencia / HORA) / (double) RELATORIO_ATUAL.total_fluxos;
	}

	// Media de ocupação do estacionamento por dia
	if (RELATORIO_ATUAL.ultimo_fluxo != 0)
	{
		double ocupacao_tempo = difftime(agora, RELATORIO_ATUAL.ultimo_fluxo) / DIA;
		double dias;

		RELATORIO_ATUAL.ocupacao_vagas_porcentagens += (((double) RELATORIO_ATUAL.total_vagas_ocupadas / (double) RELATORIO_ATUAL.total_vagas) * 100.0) * ocupacao_tempo;
		RELATORIO_ATUAL.ocupacao_vagas_tempo =+ ocupacao_tempo;

		dias = RELATORIO_ATUAL.ocupacao_vagas_tempo;
		if (dias < 1)
		{
			dias = 1;
		}

		RELATORIO_ATUAL.media_ocupacao_vagas_dia = RELATORIO_ATUAL.ocupacao_vagas_porcentagens / dias;
	}

	RELATORIO_ATUAL.ultimo_fluxo = agora;
}
