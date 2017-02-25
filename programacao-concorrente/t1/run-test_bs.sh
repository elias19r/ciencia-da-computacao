#!/bin/bash
# Executa testes para os algoritmos Black-Scholes

echo "Inicializando teste de Black-Scholes"

echo "Criando arquivo ./test/bs_resultados ..."
printf "%-22s %-8s %s\n\n" "programa" "threads" "tempo" > ./test/bs_resultados

echo "Executando black-scholes ..."

/usr/bin/time -o ./test/black-scholes/tempo -f "%e" ./bin/black-scholes < ./entrada_blackscholes.txt > ./test/black-scholes/intconf
printf "%-22s %-8s " "black-scholes" "-" >> ./test/bs_resultados
cat ./test/black-scholes/tempo >> ./test/bs_resultados

for thr in 2 4 8
do
	echo "Executando black-scholes_pthread com $thr threads ..."

	/usr/bin/time -o ./test/black-scholes_pthread/tempo_thr-$thr -f "%e" ./bin/black-scholes_pthread $thr < ./entrada_blackscholes.txt > ./test/black-scholes_pthread/intconf_thr-$thr
	printf "%-22s %-8d " "black-scholes_pthread" $thr >> ./test/bs_resultados
	cat ./test/black-scholes_pthread/tempo_thr-$thr >> ./test/bs_resultados
done

# Saidas

echo "Escrevendo saidas em ./test/bs_resultados ..."

printf "\n\nsaidas\n" >> ./test/bs_resultados

printf "\nblack-scholes  -\n" >> ./test/bs_resultados
cat ./test/black-scholes/intconf >> ./test/bs_resultados

for thr in 2 4 8
do
	printf "\nblack-scholes_pthread  $thr\n" >> ./test/bs_resultados
	cat ./test/black-scholes_pthread/intconf_thr-$thr >> ./test/bs_resultados
done

echo "Teste completo!"

echo "Imprimindo resultados ..."

