#ifndef _MESSENGER_H_
#define _MESSENGER_H_

// Define nomes dos comandos do messenger
#define CMD_INSERT_USER         "inserirusuario"
#define CMD_DELETE_USER         "removerusuario"
#define CMD_INSERT_RELATIONSHIP "inserirrelacionamento"
#define CMD_BINARY_SEARCH       "buscaBinUsuario"
#define CMD_SEQUENTIAL_SEARCH   "buscaSeqUsuario"
#define CMD_EXIT                "quit"

// Define nome do arquivo temporário
#define TMP_FILE "file/.tmp"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <Integer.h>
#include <String.h>
#include <Vector.h>
#include <Date.h>
#include <Pattern.h>
#include <Scanner.h>
#include <Index.h>
#include <Relationship.h>
#include <User.h>

/**
 * Classe Messenger (Comunicador)
 */
class Messenger {
	public:
		/**
		 * Construtor e Destrutor
		 */
		Messenger() { }
		~Messenger() { }

		/**
		 * Insere um usuário no arquivo de dados
		 */
		bool insertUser(User *user) {
			bool flag = false;
			Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
			index->load(USERS_INDEX);
			index->binarySearch(user->getEmail()->toCharArray());
		
			// Se não encontrou: adiciona
			if (index->found() == 0) {
			
				// Adiciona no arquivo de dados
				FILE *fpDAT = fopen(USERS_DATA, "ab+");
				if (fpDAT != NULL) {	
					user->write(fpDAT);
					fclose(fpDAT);

					// Atualiza o arquivo .idx
					index->create(USERS_DATA, USER_RECORD_SIZE, USER_EMAIL_OFFSET);
					index->make(USERS_INDEX);

					flag = true;
				}
			}
			delete index;
			return flag;
		}

		/**
		 * Remove um usuário do arquivo de dados
		 */
		bool deleteUser(String *email) {
			bool flag = false;
			Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
			index->load(USERS_INDEX);
			index->binarySearch(email->toCharArray());

			// Se o usuário existe
			if (index->found() == 1) {
				// Remove do arquivo de dados
				FILE *fpDAT = fopen(USERS_DATA, "rb+");
				if (fpDAT != NULL) {
					User *user = new User();
					user->setOffset(-USER_RECORD_SIZE);
					user->read(fpDAT);
					user->setOffset(index->match(0));
					user->write(fpDAT);
					delete user;
				
					// Trunca o último registro do arquivo
					fseek(fpDAT, 0, SEEK_END);
					long newFileSize = ftell(fpDAT) - USER_RECORD_SIZE;
					fclose(fpDAT);
					if (newFileSize >= 0)
						truncate(USERS_DATA, newFileSize);

					// Atualiza o arquivo .idx
					index->create(USERS_DATA, USER_RECORD_SIZE, USER_EMAIL_OFFSET);
					index->make(USERS_INDEX);

					// Remove todos os relacionamentos quem envolvem este usuário				
					fpDAT = fopen(RELATIONSHIPS_DATA, "rb");
					if (fpDAT != NULL) {
						FILE *fpTMP = fopen(TMP_FILE, "wb+");
						Relationship *rel = new Relationship(USER_EMAIL_SIZE);
				
						while (!feof(fpDAT)) {
							rel->read(fpDAT);
							if (!feof(fpDAT)) {
								if (!email->equals(rel->user()) && !email->equals(rel->contact())) {
									index->binarySearch(rel->contact());
									rel->offset(index->match(0));
									rel->write(fpTMP);
								}
							}
						}
						fclose(fpDAT);
						fclose(fpTMP);
						delete rel;
				
						remove(RELATIONSHIPS_DATA);
						rename(TMP_FILE, RELATIONSHIPS_DATA);
					}
					flag = true;
				}
			}
			delete index;
			return flag;
		}
	
		/**
		 * Faz a busca binária por um usuário, passado o seu e-mail
		 */
		User *binarySearch(String *email) {
			User *user = NULL;
			Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
			index->load(USERS_INDEX);
			index->binarySearch(email->toCharArray());
			
			// Se encontrou
			if (index->found() == 1) {
				FILE *fpDAT = fopen(USERS_DATA, "rb");
				if (fpDAT != NULL) {
					user = new User();
					user->setOffset(index->match(0));
					user->read(fpDAT);
					user->loadContacts();
					fclose(fpDAT);
				}
			}
			delete index;
			return user;
		}
		
		/**
		 * Faz a busca sequencial por um usuário, passado o seu e-mail
		 */		
		User *sequentialSearch(String *email) {
			User *user = NULL;
			Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
			index->load(USERS_INDEX);
			index->sequentialSearch(email->toCharArray());
			
			// Se encontrou
			if (index->found() == 1) {
				FILE *fpDAT = fopen(USERS_DATA, "rb");
				if (fpDAT != NULL) {
					user = new User();
					user->setOffset(index->match(0));
					user->read(fpDAT);
					user->loadContacts();
					fclose(fpDAT);
				}
			}
			delete index;
			return user;
		}
		
		/**
		 * Executa o comunicador que espera receber comandos
		 */
		void execute() {
			Scanner *keyboard = new Scanner();
			String *cmd;
	
			// Faz leitura do teclado
			while (keyboard->hasNext()) {

				// Pega uma string e confere qual comando
				cmd = keyboard->nextString();

				// Comando "inserirusuario"
				if (cmd->equals(CMD_INSERT_USER)) {
					User *user = new User();
					String *string;
			
					// Recebe e-mail
					while (keyboard->hasNext()) {
						string = keyboard->nextString();
						if (user->setEmail(string)) {
							delete string;
							break;
						} else
							printf("Email invalido\n");
						delete string;
					}
			
					// Recebe senha
					while (keyboard->hasNext()) {
						string = keyboard->nextString();
						if (user->setPassword(string)) {
							delete string;
							break;
						} else
							printf("Quantidade de caracteres da senha invalida\n");
						delete string;
					}
			
					// Recebe nome completo
					keyboard->hasNext();
					string = keyboard->nextString();
					user->setFullName(string);
					delete string;
			
					// Recebe apelido
					keyboard->hasNext();
					string = keyboard->nextString();
					user->setNickName(string);
					delete string;
			
					// Recebe cidade
					keyboard->hasNext();
					string = keyboard->nextString();
					user->setCity(string);
					delete string;
			
					// Recebe estado
					keyboard->hasNext();
					string = keyboard->nextString();
					user->setState(string);
					delete string;
			
					// Recebe país
					keyboard->hasNext();
					string = keyboard->nextString();
					user->setCountry(string);
					delete string;

					// Recebe data de nascimento
					while (keyboard->hasNext()) {
						string = keyboard->nextString();
						if (user->setBirthday(string)) {
							delete string;
							break;
						} else
							printf("Data de nascimento invalida\n");
						delete string;
					}

					// Terminou de ler as informações
					//     agora tenta inserir o usuário no arquivo .dat
					if (this->insertUser(user))
						printf("Usuario inserido com sucesso\n");
					else
						printf("Falha ao inserir usuario\n");
			
					delete user;
			
				// Comando "removerusuario"
				} else if (cmd->equals(CMD_DELETE_USER)) {
					// Recebe e-mail
					keyboard->hasNext();
					String *email = keyboard->nextString();

					// Tenta remover o usuário no arquivo .dat
					if (this->deleteUser(email))
						printf("Usuario removido com sucesso\n");
					else
						printf("Problemas ao remover usuario\n");

					delete email;
			
				// Comando "buscaBinUsuario"
				} else if (cmd->equals(CMD_BINARY_SEARCH)) {
					// Recebe e-mail
					keyboard->hasNext();
					String *email = keyboard->nextString();

					// Procura usuário
					User *user = this->binarySearch(email);
			
					// Imprime informações
					if (user != NULL) {
						user->printInfo();
						user->printContacts();
						delete user;
					}
					delete email;
			
				// Comando "buscaSeqUsuario"
				} else if (cmd->equals(CMD_SEQUENTIAL_SEARCH)) {
					// Recebe e-mail
					keyboard->hasNext();
					String *email = keyboard->nextString();

					// Procura usuário
					User *user = this->sequentialSearch(email);
			
					// Imprime informações
					if (user != NULL) {
						user->printInfo();
						user->printContacts();
						delete user;
					}
					delete email;
			
				// Comando "inserirrelacionamento"
				} else if (cmd->equals(CMD_INSERT_RELATIONSHIP)) {
					// Recebe e-mails
					keyboard->hasNext();
					String *email1 = keyboard->nextString();
					keyboard->hasNext();
					String *email2 = keyboard->nextString();

					// Procura usuários
					User *user1 = this->binarySearch(email1);
					User *user2 = this->binarySearch(email2);

					// Insere relacionamentos
					if (user1 != NULL && user2 != NULL) {
						user1->insertRelationship(user2);
						user2->insertRelationship(user1);
						printf("Relacionamento criado com sucesso\n");
					} else {
						printf("Relacionamento nao pode ser criado\n");
					}

					if (user1 != NULL) delete user1;
					if (user2 != NULL) delete user2;
					delete email1;
					delete email2;

				// Comando "quit"
				} else if (cmd->equals(CMD_EXIT)) {
					delete cmd;
					break;
				}
		
				delete cmd;
			}
			delete keyboard;
		}
};

#endif
