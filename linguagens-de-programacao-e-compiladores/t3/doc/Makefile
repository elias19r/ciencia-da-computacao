DOC = RELATORIO

all:
	pdflatex -jobname "$(DOC)" -output-directory ./ ./main.tex

redo:
	pdflatex -jobname "$(DOC)" -output-directory ./ ./main.tex

move:
	mv $(DOC).pdf ../

clean:
	rm -rf ./*~
	rm -rf ./input/*~
	rm -rf ./*.aux
	rm -rf ./*.log
	rm -rf ./*.toc
	rm -rf ./*.out

done: all redo move clean
