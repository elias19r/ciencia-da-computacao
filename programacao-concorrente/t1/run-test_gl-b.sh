#!/bin/bash
# Executa testes para os algoritmos Gauss-Legendre e Borwein (1984)

echo "Inicializando teste de Gauss-Legendre vs Borwein (1984)"

echo "Criando arquivo ./test/gl-b_resultados ..."
printf "%-24s %-9s %s\n\n" "programa" "precisao" "tempo" > ./test/gl-b_resultados

for number in 10000 100000 1000000 10000000
do
	for prog in gauss-legendre gauss-legendre_pthread borwein borwein_pthread
	do
		echo "Executando $prog $number ..."

		/usr/bin/time -o ./test/$prog/tempo_d-$number -f "%e" ./bin/$prog $number > ./test/$prog/pi_d-$number
		printf "%-24s %-9d " "$prog" $number >> ./test/gl-b_resultados
		cat ./test/$prog/tempo_d-$number >> ./test/gl-b_resultados
	done
	printf "\n" >> ./test/gl-b_resultados
done

echo "Conferindo md5sum das saidas ..."

cd ./test
printf "\nmd5sum checks\n" >> ./gl-b_resultados
for prog in gauss-legendre gauss-legendre_pthread borwein borwein_pthread
do
	cd ./$prog/
	printf "\n%s\n" "$prog" >> ../gl-b_resultados
	md5sum -c checagem.md5sum >> ../gl-b_resultados
	cd ../
done

echo "Teste completo!"

echo "Imprimindo resultados ..."

