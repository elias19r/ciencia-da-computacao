#include "../../include/core/Group.hh"

Group::Group() {
	this->id = new String();
	this->userList = new Vector<User *>();
}

Group::Group(String *id) {
	this->id = new String(id);
	this->userList = new Vector<User *>();
}

Group::~Group() {
	for (int i = 0, count = this->userList->size(); i < count; ++i)
		delete this->userList->get(i);

	delete this->userList;
}

void Group::sendMessage(String *msg) {
	
}

void Group::sendMessage(Image *img) {
	
}

void Group::sendMessage(unsigned char *audio) {
	
}

