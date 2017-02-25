#!/bin/bash
# Executa testes para os algoritmos Monte Carlo

echo "Inicializando teste de Monte Carlo"

echo "Criando arquivo ./test/mc_resultados ..."
printf "%-22s %-8s %s\n\n" "programa" "threads" "tempo" > ./test/mc_resultados

echo "Executando monte-carlo ..."

/usr/bin/time -o ./test/monte-carlo/tempo -f "%e" ./bin/monte-carlo 1000000000 > ./test/monte-carlo/pi
printf "%-22s %-8s " "monte-carlo" "-" >> ./test/mc_resultados
cat ./test/monte-carlo/tempo >> ./test/mc_resultados

for thr in 2 4 8
do
	echo "Executando monte-carlo_pthread com $thr threads ..."

	/usr/bin/time -o ./test/monte-carlo_pthread/tempo_thr-$thr -f "%e" ./bin/monte-carlo_pthread 1000000000 $thr > ./test/monte-carlo_pthread/pi_thr-$thr
	printf "%-22s %-8d " "monte-carlo_pthread" $thr >> ./test/mc_resultados
	cat ./test/monte-carlo_pthread/tempo_thr-$thr >> ./test/mc_resultados
done

# Saidas

echo "Escrevendo saidas em ./test/mc_resultados ..."

printf "\n\nsaidas pi\n" >> ./test/mc_resultados

printf "\nmonte-carlo  -\n" >> ./test/mc_resultados
cat ./test/monte-carlo/pi >> ./test/mc_resultados

for thr in 2 4 8
do
	printf "\nmonte-carlo_pthread  $thr\n" >> ./test/mc_resultados
	cat ./test/monte-carlo_pthread/pi_thr-$thr >> ./test/mc_resultados
done

echo "Teste completo!"

echo "Imprimindo resultados ..."

