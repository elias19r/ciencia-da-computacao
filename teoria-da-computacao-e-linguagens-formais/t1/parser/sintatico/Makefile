all:
	java -cp javacc.jar jjtree awk.jjt
	java -cp javacc.jar javacc awk.jj
	javac -cp .:javacc.jar *.java

run:
	java AWK

jj:
	java -cp javacc.jar javacc awk.jj
	javac -cp .:javacc.jar *.java

x: all run

clean:
	rm -f *~
	rm -f *.class
