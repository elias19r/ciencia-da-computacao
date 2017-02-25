all:
	javac -cp ./build:./lib:. -d ./build src/messenger/*.java

clean:
	rm -f t3.zip
	rm -rf ./build
	mkdir build
	find -name "*~" | xargs rm -rf

server:
	java -cp ./build:./lib:. messenger.Server 1234 messengerdb messenger QWERTY
	
client:
	java -cp ./build:. messenger.Client localhost 1234

x: clean all server

zip: clean
	zip -r t3.zip *
