#include "../../include/core/Messenger.hh"

int main(int argc, char *argv[]) {
	Messenger *msgr = new Messenger();
	msgr->execute();
	
	delete msgr;
	return 0;
}

