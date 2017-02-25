all:
	flex -o ./src/extract.c ./src/extract.l
	gcc ./src/extract.c -o ./bin/extract -lfl

clean:
	find -name "*~" | xargs rm -rf
	rm -f ./bin/*

