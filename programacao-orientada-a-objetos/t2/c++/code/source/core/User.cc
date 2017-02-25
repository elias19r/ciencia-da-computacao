#include "../../include/core/User.hh"

User::User() {
	this->email      = new String();
	this->password   = new String();
	this->fullName   = new String();
	this->nickName   = new String();
	this->city       = new String();
	this->state      = new String();
	this->country    = new String();
	this->birthday   = new Date();
	this->entityList = new Vector<Entity *>();
	this->offset     = -1;
	this->loggedIn   = false;
	this->seek       = false;
}

User::User(User *user) {
	this->id         = user->id->clone();
	this->email      = user->email->clone();
	this->password   = user->password->clone();
	this->fullName   = user->fullName->clone();
	this->nickName   = user->nickName->clone();
	this->city       = user->city->clone();
	this->state      = user->state->clone();
	this->country    = user->country->clone();
	this->birthday   = user->birthday->clone();
	this->entityList = user->entityList->clone();
	this->offset     = user->offset;
	this->loggedIn   = user->loggedIn;
	this->seek       = user->seek;
}

User::~User() {
	delete this->email;
	delete this->password;
	delete this->fullName;
	delete this->nickName;
	delete this->city;
	delete this->state;
	delete this->country;
	delete this->birthday;
	for (int i = 0, count = this->entityList->size(); i < count; ++i)
		delete this->entityList->get(i);

	delete this->entityList;
}

void User::sendMessage(String *msg) {
	
}

void User::write(FILE *fpDAT) {
	if (this->seek) {
		if (this->offset < 0)
			fseek(fpDAT, this->offset, SEEK_END);
		else
			fseek(fpDAT, this->offset, SEEK_SET);
	}

	char *buffer   = NULL;
	char *email    = (char *) calloc(1, USER_EMAIL_SIZE    + sizeof(char));
	char *password = (char *) calloc(1, USER_PASSWORD_SIZE + sizeof(char));
	char *fullName = (char *) calloc(1, USER_FULLNAME_SIZE + sizeof(char));
	char *nickName = (char *) calloc(1, USER_NICKNAME_SIZE + sizeof(char));
	char *city     = (char *) calloc(1, USER_CITY_SIZE     + sizeof(char));
	char *state    = (char *) calloc(1, USER_STATE_SIZE    + sizeof(char));
	char *country  = (char *) calloc(1, USER_COUNTRY_SIZE  + sizeof(char));
	char *birthday = (char *) calloc(1, USER_BIRTHDAY_SIZE + sizeof(char));
	
	buffer = this->email->toCharArray();    memcpy(email,    buffer, strlen(buffer)); free(buffer);
	buffer = this->password->toCharArray(); memcpy(password, buffer, strlen(buffer)); free(buffer);
	buffer = this->fullName->toCharArray(); memcpy(fullName, buffer, strlen(buffer)); free(buffer);
	buffer = this->nickName->toCharArray(); memcpy(nickName, buffer, strlen(buffer)); free(buffer);
	buffer = this->city->toCharArray();     memcpy(city,     buffer, strlen(buffer)); free(buffer);
	buffer = this->state->toCharArray();    memcpy(state,    buffer, strlen(buffer)); free(buffer);
	buffer = this->country->toCharArray();  memcpy(country,  buffer, strlen(buffer)); free(buffer);
	buffer = this->birthday->toCharArray(); memcpy(birthday, buffer, strlen(buffer)); free(buffer);

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

void User::read(FILE *fpDAT) {
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

	fread(email, USER_EMAIL_SIZE, 1, fpDAT);
	email[USER_EMAIL_SIZE] = '\0';
	this->email->set(email);
	
	fread(password, USER_PASSWORD_SIZE, 1, fpDAT);
	password[USER_PASSWORD_SIZE] = '\0';
	this->password->set(password);
	
	fread(fullName, USER_FULLNAME_SIZE, 1, fpDAT);
	fullName[USER_FULLNAME_SIZE] = '\0';
	this->fullName->set(fullName);
	
	fread(nickName, USER_NICKNAME_SIZE, 1, fpDAT);
	nickName[USER_NICKNAME_SIZE] = '\0';
	this->nickName->set(nickName);
	
	fread(city, USER_CITY_SIZE, 1, fpDAT);
	city[USER_CITY_SIZE] = '\0';
	this->city->set(city);
	
	fread(state, USER_STATE_SIZE, 1, fpDAT);
	state[USER_STATE_SIZE] = '\0';
	this->state->set(state);
	
	fread(country, USER_COUNTRY_SIZE, 1, fpDAT);
	country[USER_COUNTRY_SIZE] = '\0';
	this->country->set(country);
	
	fread(birthday, USER_BIRTHDAY_SIZE, 1, fpDAT);
	birthday[USER_BIRTHDAY_SIZE] = '\0';
	this->birthday->set(birthday);
	
	free(email);
	free(password);
	free(fullName);
	free(nickName);
	free(city);
	free(state);
	free(country);
	free(birthday);			
}

bool User::setEmail(String *email) {
	bool flag = false;
	Pattern *pattern = new Pattern();
	
	if (pattern->email(email)) {
		this->email->set(email);
		this->setId(email);
		flag = true;
	} else
		printf("Email invalido\n");

	delete pattern;
	return flag;
}

bool User::setPassword(String *password) {
	bool flag = false;
	Pattern *pattern = new Pattern();
	
	if (pattern->password(password, 2, USER_PASSWORD_SIZE)) {
		this->password->set(password);
		flag = true;
	} else
		printf("Quantidade de caracteres da senha invalida\n");

	delete pattern;
	return flag;
}

void User::setFullName(String *fullName) {
	this->fullName->set(fullName);
}

void User::setNickName(String *nickName) {
	this->nickName->set(nickName);
}

void User::setCity(String *city) {
	this->city->set(city);
}

void User::setState(String *state) {
	this->state->set(state);
}

void User::setCountry(String *country) {
	this->country->set(country);
}

bool User::setBirthday(String *date) {
	bool flag = false;
	Pattern *pattern = new Pattern();
	
	if (pattern->date(date)) {
		this->birthday->set(date);
		flag = true;
	} else
		printf("Data de nascimento invalida\n");

	delete pattern;
	return flag;
}

void User::setOffset(int value) {
	this->offset = value;
	this->seek = true;
}

void User::dontSeek() {
	this->seek = false;
}

String *User::getEmail() {
	return this->email->clone();
}

String *User::getPassword() {
	return this->password->clone();
}

String *User::getFullName() {
	return this->fullName->clone();
}

String *User::getNickName() {
	return this->nickName->clone();
}

String *User::getCity() {
	return this->city->clone();
}

String *User::getState() {
	return this->state->clone();
}

String *User::getCountry() {
	return this->country->clone();
}

Date *User::getBirthday() {
	return this->birthday->clone();
}

int User::getOffset() {
	return this->offset;
}

Vector<Entity *> *User::getContacts() {
	return this->entityList->clone();
}

void User::loadContacts() {
	
}

void User::deleteContact(User *user) {

}

bool User::login(String *email, String *password) {
	bool flag = false;
	
	// Cria objeto index e carrega o usuarios.idx
	Index *index = new Index(USER_EMAIL_TYPE, USER_EMAIL_SIZE);
	index->load(USERS_INDEX);
	
	// Faz busca binaria
	index->binarySearch(email);
	
	// Se encontrou
	if (index->found() == 1) {
		printf("Usuario encontrado\n");

		// Abre arquivo de dados e carrega os dados do usuario		
		FILE *fpDAT = fopen(USERS_DATA, "rb");
		if (fpDAT != NULL) {
			this->setOffset(index->match(0));
			this->read(fpDAT);
			fclose(fpDAT);

			// Confere senha.
			// Se igual, define loggedIn e carrega contatos deste usuario
			if (this->password->equals(password)) {
				printf("Senha correta\n");
				this->loggedIn = true;
				this->loadContacts();
				flag = true;
			} else
				printf("Senha incorreta\n");
		}

	// Se nao encontrou
	} else
		printf("Usuario nao encontrado\n");

	delete index;
	return flag;
}

void User::printInfo() {
	char *buffer;
	
	buffer = this->email->toCharArray();
	printf("%s\n", buffer);
	free(buffer);

	buffer = this->fullName->toCharArray();
	printf("%s\n", buffer);
	free(buffer);
	
	printf("%d\n", this->entityList->size());
}

void User::printContacts() {
	String *stringBuffer;
	char *buffer;

	for (int i = 0, count = this->entityList->size(); i < count; ++i) {
		stringBuffer = this->entityList->get(i)->getId();
		buffer = stringBuffer->toCharArray();
		printf("%s\n", buffer);
	}
}

User *User::clone() {
	return new User(this);
}

void User::insertRelationship(String *id) {
	
}

