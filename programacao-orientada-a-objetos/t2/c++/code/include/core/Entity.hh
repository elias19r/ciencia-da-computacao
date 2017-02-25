#ifndef _ENTITY_HH_
#define _ENTITY_HH_

#define USERS_DATA   "data/usuarios.dat"
#define USERS_INDEX  "data/usuarios.idx"
#define GROUPS_INDEX "data/grupos.idx"

#include "../util/Object.hh"
#include "../type/String.hh"
#include "../type/Image.hh"

class Entity : public Object {
	protected:
		String *id;
	public:
		Entity();
		Entity(char *id);
		Entity(String *id);
		virtual ~Entity();
		void setId(String *id);
		String *getId();
		virtual void sendMessage(String *msg);
		virtual void sendMessage(Image *img);
		virtual void sendMessage(unsigned char *audio);
};

#endif
