CC      = gcc
CFLAGS  = -Wall -g

all: clean draw contrast flood-fill

draw:
	$(CC) -o ./bin/draw ./src/draw.c -lm $(CFLAGS)

contrast:
	$(CC) -o ./bin/contrast ./src/contrast.c ./src/pgm.c $(CFLAGS)

flood-fill:
	$(CC) -o ./bin/flood-fill ./src/flood-fill.c ./src/pgm.c $(CFLAGS)

clean:
	find -name "*~" | xargs rm -rf
	rm -f ./bin/*
	rm -f ./*out.pgm

run:
	./bin/draw -n circle1_out.pgm -t P2 -w 300 -h 200 -f circle -s 70 --fill
	./bin/draw -n circle2_out.pgm -t P5 -w 150 -h 150 -f circle -s 40
	./bin/draw -n square1_out.pgm -f square -s 60 --fill -l 70 20 220 210
	./bin/draw -n square2_out.pgm -f square -s 60 -l 13 57 200 50
	./bin/contrast -i low1.pgm -o low1_out.pgm
	./bin/contrast -i low2.pgm -o low2_out.pgm
	./bin/flood-fill -i borders.pgm -o borders1_out.pgm -c 100 -x 296 -y 304
	./bin/flood-fill -i borders.pgm -o borders2_out.pgm -c 100 -x 62 -y 62

memcheck_draw:
	valgrind --tool=memcheck --leak-check=full ./bin/draw -n circle1_out.pgm -t P2 -w 300 -h 200 -f circle -s 70 --fill
	valgrind --tool=memcheck --leak-check=full ./bin/draw -n circle2_out.pgm -t P5 -w 150 -h 150 -f circle -s 40
	valgrind --tool=memcheck --leak-check=full ./bin/draw -n square1_out.pgm -f square -s 60 --fill -l 70 20 220 210
	valgrind --tool=memcheck --leak-check=full ./bin/draw -n square2_out.pgm -f square -s 60 -l 13 57 200 50

memcheck_contrast:
	valgrind --tool=memcheck --leak-check=full ./bin/contrast -i low1.pgm -o low1_out.pgm
	valgrind --tool=memcheck --leak-check=full ./bin/contrast -i low2.pgm -o low2_out.pgm

memcheck_flood-fill:
	valgrind --tool=memcheck --leak-check=full ./bin/flood-fill -i borders.pgm -o borders1_out.pgm -c 100 -x 296 -y 304
	valgrind --tool=memcheck --leak-check=full ./bin/flood-fill -i borders.pgm -o borders2_out.pgm -c 100 -x 62 -y 62

