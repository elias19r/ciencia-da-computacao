#ifndef _MESSENGER_HH_
#define _MESSENGER_HH_

#define CMD_SEND_MESSAGE        "enviarmensagem"
#define CMD_LOGIN_USER          "autenticarusuario"
#define CMD_INSERT_USER         "inserirusuario"
#define CMD_DELETE_USER         "removerusuario"
#define CMD_INSERT_RELATIONSHIP "inserirrelacionamento"
#define CMD_DELETE_RELATIONSHIP "removerrelacionamento"
#define CMD_SEARCH_RELATIONSHIP "buscarrelacionamento"
#define CMD_BINARY_SEARCH       "buscaBinUsuario"
#define CMD_SEQUENTIAL_SEARCH   "buscaSeqUsuario"
#define CMD_EXIT                "quit"

#define TMP_FILE "data/.tmp"

#include <unistd.h>
#include "../core/User.hh"
#include "../core/Group.hh"
#include "../util/Date.hh"
#include "../util/Index.hh"
#include "../util/Pattern.hh"
#include "../util/Scanner.hh"
#include "../util/Vector.hh"
#include "../type/String.hh"
#include "../type/Integer.hh"

class Messenger {
	private:
		Vector<User *> *userList;
	public:
		Messenger();
		virtual ~Messenger();
		bool insertUser(User *user);
		bool deleteUser(String *email);
		User *binarySearch(String *email);
		User *sequentialSearch(String *email);
		void execute();
};

#endif
