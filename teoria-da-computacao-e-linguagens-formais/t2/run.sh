#!/bin/bash
# Executa o extrator de informacao para todas as entradas da pasta './data'

for ((i = 1; i <= 13; i++))
do
	echo "Running for input $i..."
	./bin/extract < ./data/input${i} > ./data/output${i}
	echo "File data/output${i} created."
done

echo "Done."
