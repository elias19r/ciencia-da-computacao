DOC = apresentacao
TEX = apresentacao

all:
	pdflatex -jobname "$(DOC)" -output-directory ./ ./$(TEX).tex

redo:
	pdflatex -jobname "$(DOC)" -output-directory ./ ./$(TEX).tex

move:
	mv $(DOC).pdf ../

clean:
	rm -rf ./*~
	rm -rf ./*.aux
	rm -rf ./*.log
	rm -rf ./*.toc
	rm -rf ./*.out
	rm -rf ./*.nav
	rm -rf ./*.snm
	rm -rf ./*.vrb

done: all redo move clean
