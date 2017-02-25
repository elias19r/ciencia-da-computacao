# Compila o PDF da monografia 
# ===========================


# ------------ CONFIGURACAO --------------
#
# Nome final do PDF
NAME=tcc1-monografia_7987251
#
# Diretorio para guardar arquivos compilados
BUILD=./build
#
# Arquivo LaTeX principal (nao colocar o ".tex" aqui)
MAIN=main
#
# Arquivo de bibliografia (coloque o ".bib" aqui)
REFS=referencias.bib
#
# ----------------------------------------


# Compiladores
TEX=pdflatex
BIB=bibtex


# Default: compila apenas o PDF sem (re)fazer referencias, sumario etc
fast:
	mkdir -p $(BUILD)
	$(TEX) -output-directory $(BUILD) $(MAIN).tex
	mv $(BUILD)/$(MAIN).pdf ./$(NAME).pdf


# Compila TUDO
all:
	mkdir -p $(BUILD)
	
	# Compila o LaTeX
	$(TEX) -output-directory $(BUILD) $(MAIN).tex

	# Compilar as referencias
	BSTINPUT="$(BUILD):" TEXMFOUTPUT="$(BUILD):" bibtex $(BUILD)/$(MAIN)
	
	# Recompila o LaTeX para refazer referencias e sumario
	$(TEX) -output-directory $(BUILD) $(MAIN).tex
	$(TEX) -output-directory $(BUILD) $(MAIN).tex

	# Move o PDF compilado para o diretorio atual
	mv $(BUILD)/$(MAIN).pdf ./$(NAME).pdf


# Limpa o diretorio de build
clean:
	rm -rf $(BUILD)/*


# Atalho para clean & all
x: clean all
