#ifndef _USER_H_
#define _USER_H_

// Define constantes para nomes de arquivos
#define USERS_DATA         "file/usuarios.dat"
#define USERS_INDEX        "file/usuarios.idx"
#define RELATIONSHIPS_DATA "file/relacionamentos.dat"

// Define constantes para os registros em arquivo

#define RELATIONSHIP_RECORD_SIZE (164 * sizeof(char))

#define USER_RECORD_SIZE   (338 * sizeof(char))

#define USER_EMAIL_SIZE    (80 * sizeof(char))
#define USER_PASSWORD_SIZE (30 * sizeof(char))
#define USER_FULLNAME_SIZE (80 * sizeof(char))
#define USER_NICKNAME_SIZE (30 * sizeof(char))
#define USER_CITY_SIZE     (30 * sizeof(char))
#define USER_STATE_SIZE    (30 * sizeof(char))
#define USER_COUNTRY_SIZE  (50 * sizeof(char))
#define USER_BIRTHDAY_SIZE (8  * sizeof(char))

#define USER_EMAIL_OFFSET    0
#define USER_PASSWORD_OFFSET (80  * sizeof(char))
#define USER_FULLNAME_OFFSET (110 * sizeof(char))
#define USER_NICKNAME_OFFSET (190 * sizeof(char))
#define USER_CITY_OFFSET     (220 * sizeof(char))
#define USER_STATE_OFFSET    (250 * sizeof(char))
#define USER_COUNTRY_OFFSET  (280 * sizeof(char))
#define USER_BIRTHDAY_OFFSET (330 * sizeof(char))

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <String.h>
#include <Date.h>
#include <Pattern.h>
#include <Relationship.h>
#include <Index.h>

#define USER_EMAIL_TYPE    CCHAR
#define USER_PASSWORD_TYPE CCHAR
#define USER_FULLNAME_TYPE CCHAR
#define USER_NICKNAME_TYPE CCHAR
#define USER_CITY_TYPE     CCHAR
#define USER_STATE_TYPE    CCHAR
#define USER_COUNTRY_TYPE  CCHAR
#define USER_BIRTHDAY_TYPE CCHAR

/**
 * Classe User
 * Contém e manipula as informações de um usuário
 */
class User {
	private:
		// Informações sobre o usuário
		String *email;
		String *password;
		String *fullName;
		String *nickName;
		String *city;
		String *state;
		String *country;
		Date *birthday;
		Vector<User *> *contacts;
		int offset;   // offset no arquivo de dados
		bool loggedIn; // se usuário está autenticado (ainda não está em uso)
		bool seek;     // define se os métodos User::read e User::write fazem seek no arquivo

	public:
		/**
		 * Construtor
		 */
		User() {
			this->email    = NULL;
			this->password = NULL;
			this->fullName = NULL;
			this->nickName = NULL;
			this->city     = NULL;
			this->state    = NULL;
			this->country  = NULL;
			this->birthday = NULL;
			this->contacts = NULL;
			this->offset   = -1;
			this->loggedIn = false;
			this->seek     = false;
		}

		/**
		 * Destrutor
		 */
		~User() {
			if (this->email    != NULL) delete this->email;
			if (this->password != NULL) delete this->password;
			if (this->fullName != NULL) delete this->fullName;
			if (this->nickName != NULL) delete this->nickName;
			if (this->city     != NULL) delete this->city;
			if (this->state    != NULL) delete this->state;
			if (this->country  != NULL) delete this->country;
			if (this->birthday != NULL) delete this->birthday;
			if (this->contacts != NULL) {
				for (int i = 0, size = this->contacts->size(); i < size; i++) {
					delete this->contacts->get(i);
				}
				delete this->contacts;
			}
		}
		
		/**
		 * Escreve um usuário no arquivo de dados
		 */
		void write(FILE *fpDAT) {
			if (this->seek) {
				if (this->offset < 0)
					fseek(fpDAT, this->offset, SEEK_END);
				else
					fseek(fpDAT, this->offset, SEEK_SET);
			}

			char *email    = (char *) calloc(1, USER_EMAIL_SIZE    + sizeof(char));
			char *password = (char *) calloc(1, USER_PASSWORD_SIZE + sizeof(char));
			char *fullName = (char *) calloc(1, USER_FULLNAME_SIZE + sizeof(char));
			char *nickName = (char *) calloc(1, USER_NICKNAME_SIZE + sizeof(char));
			char *city     = (char *) calloc(1, USER_CITY_SIZE     + sizeof(char));
			char *state    = (char *) calloc(1, USER_STATE_SIZE    + sizeof(char));
			char *country  = (char *) calloc(1, USER_COUNTRY_SIZE  + sizeof(char));
			char *birthday = (char *) calloc(1, USER_BIRTHDAY_SIZE + sizeof(char));
			
			memcpy(email,    this->email->toCharArray(),    this->email->length());
			memcpy(password, this->password->toCharArray(), this->password->length());
			memcpy(fullName, this->fullName->toCharArray(), this->fullName->length());
			memcpy(nickName, this->nickName->toCharArray(), this->nickName->length());
			memcpy(city,     this->city->toCharArray(),     this->city->length());
			memcpy(state,    this->state->toCharArray(),    this->state->length());
			memcpy(country,  this->country->toCharArray(),  this->country->length());
			memcpy(birthday, this->birthday->toCharArray(), USER_BIRTHDAY_SIZE);

			fwrite(email,    USER_EMAIL_SIZE,    1, fpDAT);
			fwrite(password, USER_PASSWORD_SIZE, 1, fpDAT);
			fwrite(fullName, USER_FULLNAME_SIZE, 1, fpDAT);
			fwrite(nickName, USER_NICKNAME_SIZE, 1, fpDAT);
			fwrite(city,     USER_CITY_SIZE,     1, fpDAT);
			fwrite(state,    USER_STATE_SIZE,    1, fpDAT);
			fwrite(country,  USER_COUNTRY_SIZE,  1, fpDAT);
			fwrite(birthday, USER_BIRTHDAY_SIZE, 1, fpDAT);
			
			free(email);
			free(password);
			free(fullName);
			free(nickName);
			free(city);
			free(state);
			free(country);
			free(birthday);
		}
		
		/**
		 * Carrega variáveis membro deste usuário a partir de um arquivo de dados
		 */
		void read(FILE *fpDAT) {
			char *email    = (char *) calloc(1, USER_EMAIL_SIZE    + sizeof(char));
			char *password = (char *) calloc(1, USER_PASSWORD_SIZE + sizeof(char));
			char *fullName = (char *) calloc(1, USER_FULLNAME_SIZE + sizeof(char));
			char *nickName = (char *) calloc(1, USER_NICKNAME_SIZE + sizeof(char));
			char *city     = (char *) calloc(1, USER_CITY_SIZE     + sizeof(char));
			char *state    = (char *) calloc(1, USER_STATE_SIZE    + sizeof(char));
			char *country  = (char *) calloc(1, USER_COUNTRY_SIZE  + sizeof(char));
			char *birthday = (char *) calloc(1, USER_BIRTHDAY_SIZE + sizeof(char));
			
			if (this->seek) {
				if (this->offset < 0)
					fseek(fpDAT, this->offset, SEEK_END);
				else
					fseek(fpDAT, this->offset, SEEK_SET);
			}

			fread(email, USER_EMAIL_SIZE, 1, fpDAT);
			email[USER_EMAIL_SIZE] = '\0';
			if (this->email != NULL)
				this->email->set(email);
			else
				this->email = new String(email);
			
			fread(password, USER_PASSWORD_SIZE, 1, fpDAT);
			password[USER_PASSWORD_SIZE] = '\0';
			if (this->password != NULL)
				this->password->set(password);
			else
				this->password = new String(password);
			
			fread(fullName, USER_FULLNAME_SIZE, 1, fpDAT);
			fullName[USER_FULLNAME_SIZE] = '\0';
			if (this->fullName != NULL)
				this->fullName->set(fullName);
			else
				this->fullName = new String(fullName);
			
			fread(nickName, USER_NICKNAME_SIZE, 1, fpDAT);
			nickName[USER_NICKNAME_SIZE] = '\0';
			if (this->nickName != NULL)
				this->nickName->set(nickName);
			else
				this->nickName = new String(nickName);
			
			fread(city, USER_CITY_SIZE, 1, fpDAT);
			city[USER_CITY_SIZE] = '\0';
			if (this->city != NULL)
				this->city->set(city);
			else
				this->city = new String(city);
			
			fread(state, USER_STATE_SIZE, 1, fpDAT);
			state[USER_STATE_SIZE] = '\0';
			if (this->state != NULL)
				this->state->set(state);
			else
				this->state = new String(state);
			
			fread(country, USER_COUNTRY_SIZE, 1, fpDAT);
			country[USER_COUNTRY_SIZE] = '\0';
			if (this->country != NULL)
				this->country->set(country);
			else
				this->country = new String(country);
			
			fread(birthday, USER_BIRTHDAY_SIZE, 1, fpDAT);
			birthday[USER_BIRTHDAY_SIZE] = '\0';
			if (this->birthday != NULL)
				this->birthday->set(birthday);
			else
				this->birthday = new Date(birthday);
			
			free(email);
			free(password);
			free(fullName);
			free(nickName);
			free(city);
			free(state);
			free(country);
			free(birthday);			
		}

		/**
		 * Carrega variáveis membro deste usuário a partir de um arquivo de dados
		 * Porém são carregados apenas informações de contato (e-mail e apelido)
		 */		
		void readAsContact(FILE *fpDAT) {
			char *email = (char *) calloc(1, USER_EMAIL_SIZE + sizeof(char));
			char *nickName = (char *) calloc(1, USER_NICKNAME_SIZE + sizeof(char));
			
			if (this->seek) {
				if (this->offset < 0)
					fseek(fpDAT, this->offset, SEEK_END);					
				else
					fseek(fpDAT, this->offset, SEEK_SET);
			}

			fread(email, USER_EMAIL_SIZE, 1, fpDAT);
			email[USER_EMAIL_SIZE] = '\0';
			if (this->email != NULL)
				this->email->set(email);
			else
				this->email = new String(email);
			
			fseek(fpDAT, USER_PASSWORD_SIZE + USER_FULLNAME_SIZE, SEEK_CUR);
			
			fread(nickName, USER_NICKNAME_SIZE, 1, fpDAT);
			nickName[USER_NICKNAME_SIZE] = '\0';
			if (this->nickName != NULL)
				this->nickName->set(nickName);
			else
				this->nickName = new String(nickName);
			
			free(email);
			free(nickName);
		}
		 
		/**
		 * Passado um offset, lê o arquivo de dados e retorna um contato como objeto User
		 */
		User *pickContact(int offset) {
			User *contact = NULL;
			FILE *fpDAT = fopen(USERS_DATA, "rb");
			if (fpDAT != NULL) {
				contact = new User();
				contact->setOffset(offset);
				contact->readAsContact(fpDAT);
				fclose(fpDAT);
			}		
			return contact;
		}
		 
		 /**
		  * Carrega os contatos deste usuário a partir do arquivo de dados
		  */
		 void loadContacts() {
		 	FILE *fpDAT = fopen(RELATIONSHIPS_DATA, "rb");
		 	if (fpDAT != NULL) {
		 		Relationship *rel = new Relationship(USER_EMAIL_SIZE);
			 	bool flag = false;
			 	
			 	if (this->contacts != NULL)
			 		flag = true;
			 	else
			 		this->contacts = new Vector<User *>();

			 	while (!feof(fpDAT)) {
			 		rel->read(fpDAT);
			 		if (!feof(fpDAT)) {
						if (this->email->equals(rel->user())) {
							flag = true;
							this->contacts->add(this->pickContact(rel->offset()));
						}
			 		}
			 	}
			 	fclose(fpDAT);
			 	delete rel;

			 	if (!flag) {
			 		delete this->contacts;
			 		this->contacts = NULL;
			 	}
		 	}
		 }

		/**
		 * Métodos SETTERS que definem as informações do usuário
		 */

		bool setEmail(String *string) {
			if (Pattern::email(string)) {
				if (this->email != NULL)
					this->email->set(string);
				else
					this->email = new String(string);
					
				return true;
			} else
				return false;
		}

		bool setPassword(String *string) {
			if (Pattern::password(string, 2, USER_PASSWORD_SIZE)) {
				if (this->password != NULL)
					this->password->set(string);
				else
					this->password = new String(string);
					
				return true;
			} else
				return false;
		}

		void setFullName(String *string) {
			if (this->fullName != NULL)
				this->fullName->set(string);
			else
				this->fullName = new String(string);
		}

		void setNickName(String *string) {
			if (this->nickName != NULL)
				this->nickName->set(string);
			else
				this->nickName = new String(string);
		}

		void setCity(String *string) {
			if (this->city != NULL)
				this->city->set(string);
			else
				this->city = new String(string);
		}

		void setState(String *string) {
			if (this->state != NULL)
				this->state->set(string);
			else
				this->state = new String(string);
		}

		void setCountry(String *string) {
			if (this->country != NULL)
				this->country->set(string);
			else
				this->country = new String(string);
		}

		bool setBirthday(String *string) {
			if (Pattern::date(string)) {
				if (this->birthday == NULL)
					this->birthday->set(string);
				else
					this->birthday = new Date(string);

				return true;
			} else
				return false;
		}
		
		void setOffset(int value) {
			this->offset = value;
			this->seek = true;
		}
		
		void dontSeek() {
			this->seek = false;
		}

		/**
		 * Métodos GETTERS que retornam as informações do usuário
		 */

		String *getEmail()    { return this->email; }
		String *getPassword() { return this->password; }
		String *getFullName() { return this->fullName; }
		String *getNickName() { return this->nickName; }
		String *getCity()     { return this->city; }
		String *getState()    { return this->state; }
		String *getCountry()  { return this->country; }
		Date *getBirthday()   { return this->birthday; }
		Vector<User *> *getContacts() { return this->contacts; }
		int getOffset() { return this->offset; }

		/**
		 * Insere um relacionamento
		 */
		void insertRelationship(User *user) {

			// Adiciona na lista de contatos do objeto
			User *contact = pickContact(user->getOffset());
			if (this->contacts == NULL)
				this->contacts = new Vector<User *>();
			this->contacts->add(contact);

			// Adiciona no arquivo de dados dos relacionamentos
			FILE *fpDAT = fopen(RELATIONSHIPS_DATA, "ab+");
			if (fpDAT != NULL) {
				Relationship *rel = new Relationship(USER_EMAIL_SIZE);
				
				rel->user(this->email->toCharArray());
				rel->contact(user->email->toCharArray());
				rel->offset(user->getOffset());
				
				rel->write(fpDAT);

				fclose(fpDAT);
				delete rel;
			}
		}

		/**
		 * Remove um relacionamento.
		 */
		void deleteContact(User *user) {

			// Remove da lista de contatos deste objeto
			if (this->contacts != NULL) {
				for (int i = 0, size = this->contacts->size(); i < size; i++) {
					if (this->contacts->get(i)->email->equals(user->getEmail())) {
						this->contacts->removeElementAt(i);
					}
				}
			}

			// Remove do arquivo de dados
			FILE *fpDAT = fopen(RELATIONSHIPS_DATA, "rb+");
			if (fpDAT != NULL) {			
				Relationship *rel = new Relationship(USER_EMAIL_SIZE);
				int i = 0;
				bool flag = false;

				while (!feof(fpDAT)) {
					rel->read(fpDAT);
					if (!feof(fpDAT)) {
						if (this->email->equals(rel->user()) && user->getEmail()->equals(rel->contact())) {	
							// Copia o último registro do arquivo
							rel->at(-RELATIONSHIP_RECORD_SIZE);
							rel->read(fpDAT);
							
							// Sobrescreve o que queremos excluir
							rel->at(i * RELATIONSHIP_RECORD_SIZE);
							rel->write(fpDAT);
							
							flag = true;
							delete rel;
							break;
						}
					}
					i++;
				}
				if (flag) {
					// Trunca o final do arquivo
					fseek(fpDAT, 0, SEEK_END);
					long newFileSize = ftell(fpDAT) - RELATIONSHIP_RECORD_SIZE;
					fclose(fpDAT);
					if (newFileSize >= 0)
						truncate(RELATIONSHIPS_DATA, newFileSize);
				} else
					fclose(fpDAT);
			}
		}

		/**
		 * Autentica o usuário
		 */
		bool login(String *email, String *password) {
			bool flag = false;
			Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
			index->load(USERS_INDEX);
			index->binarySearch(email->toCharArray());
			
			if (index->found() == 1) {
				FILE *fpDAT = fopen(USERS_DATA, "rb");
				if (fpDAT != NULL) {
					this->setOffset(index->match(0));
					this->read(fpDAT);
					fclose(fpDAT);

					if (this->password->equals(password)) {
						flag = true;
						this->loggedIn = true;
						this->loadContacts();
					}
				}
			}
			delete index;
			return flag;
		}

		/**
		 * Imprime na tela as informações do usuário
		 */
		void printInfo() {
			if (this->email    != NULL) printf("%s\n", this->email->toCharArray());
			if (this->fullName != NULL) printf("%s\n", this->fullName->toCharArray());
			if (this->contacts != NULL)
				printf("%d\n", this->contacts->size());
			else
				printf("0\n");
		}

		/**
		 * Imprime a lista de contatos do usuário
		 */
		void printContacts() {
			if (this->contacts != NULL) {
				for (int i = 0, size = this->contacts->size(); i < size; i++)
					printf("%s\t%s\n", this->contacts->get(i)->getEmail()->toCharArray(), this->contacts->get(i)->getNickName()->toCharArray());
			}
		}
};

#endif
