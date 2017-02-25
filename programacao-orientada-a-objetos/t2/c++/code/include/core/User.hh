#ifndef _USER_HH_
#define _USER_HH_

#define USER_RECORD_SIZE     (338 * sizeof(char))

#define USER_EMAIL_SIZE      (80 * sizeof(char))
#define USER_PASSWORD_SIZE   (30 * sizeof(char))
#define USER_FULLNAME_SIZE   (80 * sizeof(char))
#define USER_NICKNAME_SIZE   (30 * sizeof(char))
#define USER_CITY_SIZE       (30 * sizeof(char))
#define USER_STATE_SIZE      (30 * sizeof(char))
#define USER_COUNTRY_SIZE    (50 * sizeof(char))
#define USER_BIRTHDAY_SIZE   (8  * sizeof(char))

#define USER_EMAIL_OFFSET     0
#define USER_PASSWORD_OFFSET (80  * sizeof(char))
#define USER_FULLNAME_OFFSET (110 * sizeof(char))
#define USER_NICKNAME_OFFSET (190 * sizeof(char))
#define USER_CITY_OFFSET     (220 * sizeof(char))
#define USER_STATE_OFFSET    (250 * sizeof(char))
#define USER_COUNTRY_OFFSET  (280 * sizeof(char))
#define USER_BIRTHDAY_OFFSET (330 * sizeof(char))

#include "../core/Entity.hh"
#include "../util/Date.hh"
#include "../util/Index.hh"
#include "../util/Pattern.hh"
#include "../type/String.hh"

#define USER_EMAIL_TYPE    CCHAR
#define USER_PASSWORD_TYPE CCHAR
#define USER_FULLNAME_TYPE CCHAR
#define USER_NICKNAME_TYPE CCHAR
#define USER_CITY_TYPE     CCHAR
#define USER_STATE_TYPE    CCHAR
#define USER_COUNTRY_TYPE  CCHAR
#define USER_BIRTHDAY_TYPE CCHAR

/**
 * Classe que define e manipula informacoes de um usuario
 */
class User : public Entity {
	private:
		// Informacoes
		String *email;
		String *password;
		String *fullName;
		String *nickName;
		String *city;
		String *state;
		String *country;
		Date *birthday;
		Vector<Entity *> *entityList; // lista de contatos (entidades)
		int offset;    // offset no arquivo de dados
		bool loggedIn; // define se o usuario se autenticou no sistema
		bool seek;     // define se os metodos User::read e User::write devem fazer seek no arquivo de dados
	public:
		User();
		User(User *user);
		virtual ~User();
		void sendMessage(String *msg);
		void write(FILE *fpDAT);
		void read(FILE *fpDAT);
		bool setEmail(String *email);
		bool setPassword(String *password);
		void setFullName(String *fullName);
		void setNickName(String *nickName);
		void setCity(String *city);
		void setState(String *state);
		void setCountry(String *country);
		bool setBirthday(String *date);
		void setOffset(int value);
		void dontSeek();
		String *getEmail();
		String *getPassword();
		String *getFullName();
		String *getNickName();
		String *getCity();
		String *getState();
		String *getCountry();
		Date *getBirthday();
		int getOffset();
		Vector<Entity *> *getContacts();
		void loadContacts();
		void deleteContact(User *user);
		bool login(String *email, String *password);
		void printInfo();
		void printContacts();
		User *clone();
		void insertRelationship(String *id);
};

#endif
