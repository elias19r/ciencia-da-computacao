#!/bin/bash

# Define o nome do arquivo, por exemplo: ./data/1mes_pre.apr.data
input=$1

# Ocorrencia minima dos produtos
nfreq=$2

# Faz um copia
echo "Making a copy of .data file and converting its encode to UTF-8..."
iconv -f ISO-8859-1 -t UTF-8 < ./${input} > ./${input}.copy

# Remove caracteres ,%" do arquivo original
echo "Removing invalid characters from copied .data file..."
sed -i 's/[,]/\_/gI' ./${input}.copy
sed -i 's/[%"`]//gI'  ./${input}.copy

# Corrige os erros encontrados nos nomes dos produtos
echo "Fixing names..."

sed -i 's/AZEITE_QUINTA_DOS_$/AZEITE_QUINTA_DOS_OLIVAIS/gI' ./${input}.copy 

sed -i 's/ADOCANTE_STEVIA/ADOCANTE_STEVITA/gI' ./${input}.copy 

sed -i 's/ADES_LIGHT_LARANJA_ALIMLIQSOJA/ADES_LIGHT_LARANJA_ALIMLIQDE_SOJA/gI' ./${input}.copy

sed -i 's/ARROZ_KIARROZ_PARBOILIZADO/ARROZ_KIARROZ_PARBOLIZADO/gI' ./${input}.copy

sed -i 's/ARROZ_UNCLE_BEN_S/ARROZ_UNCLE_BENS/gI' ./${input}.copy

sed -i 's/BISCNESLTE/BISCNESTLE/gI' ./${input}.copy

sed -i 's/BUBBALLO/BUBBALOO/gI' ./${input}.copy

sed -i 's/CIGARRO_GALAXI/CIGARRO_GALAXY/gI' ./${input}.copy

sed -i 's/DELICIAS_DE_CARN /DELICIAS_DE_CARNE /gI' ./${input}.copy

sed -i 's/DELICIAS_DE_CARN$/DELICIAS_DE_CARNE/gI' ./${input}.copy

sed -i 's/Ç/C/gI' ./${input}.copy
sed -i 's/[É|È]/E/gI' ./${input}.copy

sed -i 's/FILME_PCV_ROLO_FILMITTO/FILME_PVC_ROLO_FILMITTO/gI' ./${input}.copy

sed -i 's/PAO_WICKBOLD_HAMBURGER/PAO_WICKBOLD_HAMBUGUER/gI' ./${input}.copy

sed -i 's/PENEIRA_JOLY/PENEIRA_JOLLY/gI' ./${input}.copy

sed -i 's/PIPE_RIGATE_GRANZAN /PIPE_RIGATE_GRANZANI /gI' ./${input}.copy

sed -i 's/PIPE_RIGATE_GRANZAN$/PIPE_RIGATE_GRANZANI/gI' ./${input}.copy

sed -i 's/STEVIA/STEVITA/gI' ./${input}.copy

sed -i 's/VIDREX_CRISTA /VIDREX_CRISTAL /gI' ./${input}.copy

sed -i 's/VIDREX_CRISTA$/VIDREX_CRISTAL/gI' ./${input}.copy

sed -i 's/X_14_MULT_USO/X_14_MULTI_USO/gI' ./${input}.copy

# Compila e executa o pre-processador para transformar o .data em .arff
echo "Compiling and creating .arff file..."
gcc -o ./bin/pre-processor ./src/pre-processor.c -g -Wall
./bin/pre-processor ${nfreq} < ./${input}.copy > ./${input}.arff

echo "Done. File \"./${input}.arff\" created."
