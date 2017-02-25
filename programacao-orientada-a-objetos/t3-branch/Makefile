PACKAGE = src/msgr
PROGRAM = t3-msgr
ZIP     = $(PROGRAM).zip

BUILD = ./build
LIB   = ./lib
CLASS_PATH = $(BUILD):$(LIB):.

SERVER = msgr.app.Server
CLIENT = msgr.app.Client

SERVER_ARGS = 1234 1000
CLIENT_ARGS = localhost 1234

all:
	javac -cp $(CLASS_PATH) -d $(BUILD) $(PACKAGE)/**/*.java

clean:
	rm -rf $(BUILD)
	mkdir $(BUILD)
	find -name "*~" | xargs rm -rf
	rm -f $(ZIP)

server:
	java -cp $(CLASS_PATH) $(SERVER) $(SERVER_ARGS)
	
client:
	java -cp $(CLASS_PATH) $(CLIENT) $(CLIENT_ARGS)

x: clean all server

zip: clean
	zip -r $(ZIP) *

