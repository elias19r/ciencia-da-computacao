#include "../../include/core/Messenger.hh"

Messenger::Messenger() {
	this->userList = new Vector<User *>();
}

Messenger::~Messenger() {
	for (int i = 0, count = this->userList->size(); i < count; ++i)
		delete this->userList->get(i);

	delete this->userList;
}

bool Messenger::insertUser(User *user) {
	bool flag = false;
	String *stringBuffer = user->getEmail();
	char *buffer = stringBuffer->toCharArray();
	
	Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
	index->load(USERS_INDEX);
	index->binarySearch(buffer);

	if (index->found() == 0) {
		User *userCopy = user->clone();
		this->userList->add(userCopy);
	
		FILE *fpDAT = fopen(USERS_DATA, "ab+");
		if (fpDAT != NULL) {	
			user->write(fpDAT);
			fclose(fpDAT);

			index->create(USERS_DATA, USER_RECORD_SIZE, USER_EMAIL_OFFSET);
			index->make(USERS_INDEX);

			flag = true;
			printf("Usuario inserido com sucesso\n");
		} else
			printf("Falha ao inserir usuario\n");

	} else
		printf("Falha ao inserir usuario\n");
	
	delete index;
	delete stringBuffer;
	free(buffer);
	return flag;
}

bool Messenger::deleteUser(String *email) {
	bool flag = false;
	char *buffer = email->toCharArray();
	
	Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
	index->load(USERS_INDEX);
	index->binarySearch(buffer);

	if (index->found() == 1) {
		FILE *fpDAT = fopen(USERS_DATA, "rb+");
		if (fpDAT != NULL) {

			User *user = new User();
			user->setOffset(-USER_RECORD_SIZE);
			user->read(fpDAT);
			user->setOffset(index->match(0));
			user->write(fpDAT);
			delete user;
		
			fseek(fpDAT, 0, SEEK_END);
			long newFileSize = ftell(fpDAT) - USER_RECORD_SIZE;
			fclose(fpDAT);
			if (newFileSize >= 0)
				truncate(USERS_DATA, newFileSize);

			index->create(USERS_DATA, USER_RECORD_SIZE, USER_EMAIL_OFFSET);
			index->make(USERS_INDEX);

			printf("Usuario removido com sucesso\n");
			flag = true;
		} else
			printf("Problemas ao remover usuario\n");

	} else
		printf("Problemas ao remover usuario\n");

	delete index;
	free(buffer);
	return flag;
}

User *Messenger::binarySearch(String *email) {
	char *buffer = email->toCharArray();
	User *user = NULL;
	
	Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
	index->load(USERS_INDEX);
	index->binarySearch(buffer);
	
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
	free(buffer);
	return user;
}

User *Messenger::sequentialSearch(String *email) {
	char *buffer = email->toCharArray();
	User *user = NULL;
	
	Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
	index->load(USERS_INDEX);
	index->sequentialSearch(buffer);
	
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
	free(buffer);
	return user;
}

void Messenger::execute() {
	Scanner *keyboard = new Scanner();
	String *cmd;

	while (keyboard->next()) {

		// Le comando
		cmd = keyboard->hasNextString();

		// Comando "enviarmensagem"
		if (cmd->equals(CMD_SEND_MESSAGE)) {
			String *email;
			String *entityName;
			String *message;
			String *userEmail;
			bool flag = false;
			int i, count;
			
			// Le email do usuario
			keyboard->next();
			email = keyboard->hasNextString();

			// Le nome da entidade (destino)
			keyboard->next();
			entityName = keyboard->hasNextString();

			// Le a mensagem
			keyboard->next();
			message = keyboard->hasNextString();
			
			// Procura usuario na vetor de usuarios deste comunicador
			for (i = 0, count = this->userList->size(); i < count; ++i) {
				userEmail = this->userList->get(i)->getEmail();
				if (userEmail->equals(email)) {
					delete userEmail;
					flag = true;
					break;
				}
				delete userEmail;
			}

			// Se encontrou, procura entityName na lista de contatos (entidades) do usuario
			if (flag) {
				User *user = this->userList->get(i);
				Vector<Entity *> *entityList = user->getContacts();
				String *entityId;
				flag = false;
				
				for (i = 0, count = entityList->size(); i < count; ++i) {
					entityId = entityList->get(i)->getId();
					if (entityId->equals(entityName)) {
						delete entityId;
						flag = true;
						break;
					}
					delete entityId;
				}
				delete entityList;
				
				// Se encontrou a entidade, envia a mensagem
				if (flag) {
					char *buffer;
					
					printf("Mensagem\n");
					
					buffer = email->toCharArray();
					printf("De: %s\n", buffer);
					free(buffer);
					
					buffer = entityName->toCharArray();
					printf("Para: %s\n", buffer);
					free(buffer);
					
					buffer = message->toCharArray();
					printf("Texto: %s\n", buffer);
					free(buffer);
	
				} else
					printf("Problemas ao enviar mensagem\n");					
			
			} else
				printf("Problemas ao enviar mensagem\n");
			
			delete email;
			delete entityName;
			delete message;
			

		} else if (cmd->equals(CMD_LOGIN_USER)) {
			User *user = new User();
			String *email, *password;
			
			keyboard->next();
			email = keyboard->hasNextString();
	
			keyboard->next();
			password = keyboard->hasNextString();
			
			user->login(email, password);
			
			delete email;
			delete password;
			
			
		} else if (cmd->equals(CMD_INSERT_USER)) {
			User *user = new User();
			String *buffer;
	
			while (keyboard->next()) {
				buffer = keyboard->hasNextString();
				if (user->setEmail(buffer)) {
					delete buffer;
					break;
				}
				delete buffer;
			}
	
			while (keyboard->next()) {
				buffer = keyboard->hasNextString();
				if (user->setPassword(buffer)) {
					delete buffer;
					break;
				}
				delete buffer;
			}
	
			keyboard->next();
			buffer = keyboard->hasNextString();
			user->setFullName(buffer);
			delete buffer;
	
			keyboard->next();
			buffer = keyboard->hasNextString();
			user->setNickName(buffer);
			delete buffer;
	
			keyboard->next();
			buffer = keyboard->hasNextString();
			user->setCity(buffer);
			delete buffer;
	
			keyboard->next();
			buffer = keyboard->hasNextString();
			user->setState(buffer);
			delete buffer;
	
			keyboard->next();
			buffer = keyboard->hasNextString();
			user->setCountry(buffer);
			delete buffer;

			while (keyboard->next()) {
				buffer = keyboard->hasNextString();
				if (user->setBirthday(buffer)) {
					delete buffer;
					break;
				}
				delete buffer;
			}

			this->insertUser(user);
			delete user;
	
		} else if (cmd->equals(CMD_DELETE_USER)) {
			keyboard->next();
			String *email = keyboard->hasNextString();

			this->deleteUser(email);
			delete email;
	
		} else if (cmd->equals(CMD_BINARY_SEARCH)) {
			keyboard->next();
			String *email = keyboard->hasNextString();

			User *user = this->binarySearch(email);
	
			if (user != NULL) {
				user->printInfo();
				user->printContacts();
				delete user;
			}
			delete email;
	
		} else if (cmd->equals(CMD_SEQUENTIAL_SEARCH)) {
			keyboard->next();
			String *email = keyboard->hasNextString();

			User *user = this->sequentialSearch(email);
	
			if (user != NULL) {
				user->printInfo();
				user->printContacts();
				delete user;
			}
			delete email;
	
		// Comando "inserirrelacionamento"
		} else if (cmd->equals(CMD_INSERT_RELATIONSHIP)) {
			String *id1;
			String *id2;
			User *user1;
			User *user2;
			
			// Le o primeiro id
			keyboard->next();
			id1 = keyboard->hasNextString();
			
			// Le o segundo id
			keyboard->next();
			id2 = keyboard->hasNextString();

			// Procura por usuarios com estas id
			user1 = this->binarySearch(id1);
			user2 = this->binarySearch(id2);

			// NOTE: nunca ocorrera "Erro ao inserir relacionamento",
			// 	pois no caso de os dois usuarios nao existirem, sera criado
			// 	um relacionamento de grupo com grupo...
			
			// usuario - usuario
			if (user1 != NULL && user2 != NULL) {
//				if (user1->existsContact(user2))
//					printf("Relacionamento existente\n");
//				else {
//					user1->insertRelationship(user2);
//					printf("Relacionamento inserido com sucesso\n");
//				}
//				delete user1;
//				delete user2;
				
			// usuario - grupo
			} else if (user1 != NULL) {
//				Group *group = this->binarySearchGroup(id2);
//				if (group == NULL) {
//					group = new Group(id2);
//					this->insertGroup(group);
//				}
//				
//				if (user1->existsContact(group))
//					printf("Relacionamento existente\n");
//				else
//					user1->insertRelationship(group);
// 
//				delete user1;
//				delete group;
			
			// grupo - usuario
			} else if (user2 != NULL) {
//				Group *group = this->binarySearchGroup(id1);
//				if (group == NULL) {
//					group = new Group(id1);
//					this->insertGroup(group);					
//				}
//				
//				if (group->existsContact(user2))
//					printf("Relacionamento existente\n");
//				else
//					group->insertRelationship(user2);
//				
//				delete user2;
//				delete group;
			
			// grupo - grupo
			} else {
//				Group *group1 = this->binarySearchGroup(id1);
//				Group *group2 = this->binarySearchGroup(id2);
//				
//				if (group1 == NULL) {
//					group1 = new Group(id1);
//					this->insertGroup(group1);
//				}
//				
//				if (group2 == NULL) {
//					group2 = new Group(id2);
//					this->insertGroup(group2);
//				}
// 
//				if (group1->existsContact(group2))
//					printf("Relacionamento existente\n");
//				else {
//					if (group2->existsContact(group1))
//						printf("Relacionamento existente e reentrante\n");
//				
//					group1->insertRelationship(group2);
//				}
//				
//				delete group1;
//				delete group2;
			}
			
			delete id1;
			delete id2;

		} else if (cmd->equals(CMD_EXIT)) {
			delete cmd;
			break;
		}

		delete cmd;
	}
	delete keyboard;
}

