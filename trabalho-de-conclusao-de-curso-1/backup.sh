# Cria um .zip de backup do TCC
# =============================
#
# ------- CONFIGURACOES -------
#
BACKUP_DIR="/home/elias19r/Dropbox/backup"
#
# -----------------------------

# Diretorio atual
THIS_PWD=$(pwd)

# Nome do arquivo de backup (Exemplo: tcc1_2016-04-12_10h08min)
BACKUP_NAME=$(basename "$THIS_PWD")"_$(date +%F_%Hh%Mmin)"

# Cria diretorio em /tmp e copia dados com excecao de 'utils' e '.zip'
mkdir -p "/tmp/$BACKUP_NAME"
cp -a $(ls -A | grep -vE "utils|.zip") -t "/tmp/$BACKUP_NAME"

# Vai para /tmp, cria zip e remove o diretorio temporario
cd /tmp/ && zip -q -r "${BACKUP_NAME}.zip" "$BACKUP_NAME" && rm -rf "$BACKUP_NAME"

# Volta para o diretorio atual, move o zip e copia o zip para diretorio de backup
cd "$THIS_PWD" && mv "/tmp/${BACKUP_NAME}.zip" . && cp "${BACKUP_NAME}.zip" "$BACKUP_DIR"
