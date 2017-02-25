#include "../../include/core/Entity.hh"

Entity::Entity() {
	this->id = new String();
}

Entity::Entity(char *id) {
	this->id = new String(id);
}

Entity::Entity(String *id) {
	this->id = new String(id);
}

Entity::~Entity() {
	delete this->id;
}

void Entity::setId(String *id) {
	this->id->set(id);
}

String *Entity::getId() {
	return this->id->clone();
}

void Entity::sendMessage(String *msg) {
	
}

void Entity::sendMessage(Image *img) {
	
}

void Entity::sendMessage(unsigned char *audio) {
	
}

