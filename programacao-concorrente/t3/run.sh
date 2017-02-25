#!/bin/bash

# Argumentos de entrada
# 	$1: programa (sequential, parallel, cuda)
# 	$2: numero de processos (so tem efeito quando o programa e' parallel)
# 	$3: quantidade de processos por host (so tem efeito quando o programa e' parallel)
# 	$4: arquivo de entrada
# 	$5: numero de vezes para executar

# Nome do arquivo final
report_name=''
if [ "$1" == 'parallel' ] ; then
	report_name=${1}_${2}_${3}_${4}_${5}
else
	report_name=${1}_-_-_${4}_${5}
fi

echo "Initiating $1 tests with $4"

# Reseta arquivo com dados dos tempos de execucao
printf "" > ./test/tmp_times

# Executa n vezes o teste para a mesma entrada.
# Omite os outputs e salva todos os tempos de execucao gasto somente com processamento
for ((n = 1; n <= $5; n++))
do
	echo "Running test $n..."
	
	if [ "$1" == 'parallel' ] ; then
		mpirun -hostfile ./hosts -np $2 -npernode $3 ./bin/$1 -n -t < ./$4 > ./test/tmp_x
	else
		./bin/$1 -n -t < ./$4 > ./test/tmp_x
	fi

	cat ./test/tmp_x >> ./test/tmp_times
done

# Calcula estatisticas e cria um arquivo final que contem
# 	um relatorio de todos os testes executados
echo "Calculating statistics for $1 tests..."
printf "" > test/${report_name}

if [ $1 == 'sequential' ] ; then
	printf "Program  : Sequential\n" >> test/${report_name}
elif [ $1 == 'parallel' ] ; then
	printf "Program  : Parallel\n" >> test/${report_name}
else
	printf "Program  : CUDA\n" >> test/${report_name}
fi
if [ "$1" == 'parallel' ] ; then
	printf "# process  : $2\n" >> test/${report_name}
	printf "# per node : $3\n" >> test/${report_name}
fi
printf "Input file : $4\n" >> test/${report_name}
printf "Run times  : $5\n" >> test/${report_name}

printf "\nStatistics (s)\n" >> test/${report_name}
printf "$5\n" > ./test/tmp_stats
cat ./test/tmp_times >> ./test/tmp_stats
./bin/stats < test/tmp_stats >> test/${report_name}

printf "\nList of times (s) for each run\n" >> test/${report_name}
cat ./test/tmp_times >> test/${report_name}

echo "Finished $1 tests."
echo "> FILE CREATED: ./test/${report_name}"

# Limpa arquivos temporarios criados "tmp_" e termina
echo "Cleaning temporary files..."
rm -f ./test/tmp_*

echo "Done."

