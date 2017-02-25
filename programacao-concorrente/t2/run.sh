#!/bin/bash

# $1: algoritmo (0 dijkstra, 1 floyd-warshall)
# $2: numero de threads
# $3: arquivo de entrada
# $4: numero de vezes para executar

# Executa os mesmos testes variando o programa: sequential, parallel
for program in sequential parallel
do
	# nome do arquivo final
	report_name=''
	if [ "$program" == 'parallel' ] ; then
		report_name=${program}_${1}_${2}_${3}_${4}
	else
		report_name=${program}_${1}_-_${3}_${4}
	fi

	echo "Initiating $program tests with $3"
	
	# Reseta arquivos temporarios com dados
	printf "" > ./test/tmp_outputs
	printf "" > ./test/tmp_results

	# Executa n vezes o teste para a mesma entrada e
	# salva os tempos e outputs de cada execucao
	for ((n = 1; n <= $4; n++))
	do
		echo "Running test $n..."

		/usr/bin/time -o ./test/tmp_x -f "%e" ./bin/$program $1 $2 < ./$3 >> ./test/tmp_outputs
		cat ./test/tmp_x >> ./test/tmp_results
	done

	# Calcula estatisticas e cria um arquivo final
	# que contem um relatorio de todos os testes executados
	echo "Calculating statistics for $program tests..."
	printf "" > test/${report_name}

	if [ $1 == 0 ] ; then
		printf "Algorithm  : Dijkstra\n" >> test/${report_name}
	else
		printf "Algorithm  : Floyd-Warshall\n" >> test/${report_name}
	fi
	if [ "$program" == 'parallel' ] ; then
		printf "Threads    : $2\n" >> test/${report_name}
	fi
	printf "Input file : $3\n" >> test/${report_name}
	printf "Run times  : $4\n" >> test/${report_name}

	printf "\nStatistics (s)\n" >> test/${report_name}
	printf "$4\n" > ./test/tmp_stats
	cat ./test/tmp_results >> ./test/tmp_stats
	./bin/stats < test/tmp_stats >> test/${report_name}

	printf "\nList of times (s) for each run\n" >> test/${report_name}
	cat ./test/tmp_results >> test/${report_name}

	printf "\nList of outputs for each run\n" >> test/${report_name}
	cat ./test/tmp_outputs >> test/${report_name}

	echo "Finished $program tests."
	echo "> FILE CREATED: ./test/${report_name}"
done

# Limpa arquivos temporarios criados "tmp_" e termina
echo "Cleaning temporary files..."
rm -f ./test/tmp_*

echo "Done."

