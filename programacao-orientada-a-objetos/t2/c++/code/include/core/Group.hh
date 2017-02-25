#ifndef _GROUP_HH_
#define _GROUP_HH_

#define GROUP_RECORD_SIZE  (160 * sizeof(char))

#define GROUP_NAME_SIZE    (80  * sizeof(char))
#define GROUP_EMAIL_SIZE   (80  * sizeof(char))

#define GROUP_NAME_OFFSET   0
#define GROUP_EMAIL_OFFSET (80  * sizeof(char))

#include "../core/Entity.hh"
#include "../core/User.hh"
#include "../util/Index.hh"
#include "../util/Vector.hh"
#include "../type/String.hh"
#include "../type/Image.hh"

#define GROUP_NAME_TYPE  CCHAR
#define GROUP_EMAIL_TYPE CCHAR

class Group : public Entity {
	private:
		Vector<User *> *userList;
	public:
		Group();
		Group(String *id);
		virtual ~Group();
		void sendMessage(String *msg);
		void sendMessage(Image *img);
		void sendMessage(unsigned char *audio);
};

#endif
