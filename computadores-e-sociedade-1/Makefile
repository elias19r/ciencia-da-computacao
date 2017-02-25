all:
	pdflatex -jobname "monografia" -output-directory ./ ./main.tex

done: all all clean

clean:
	rm -rf ./*~
	rm -rf ./input/*~
	rm -rf ./*.aux
	rm -rf ./*.log
	rm -rf ./*.toc
	rm -rf ./*.out

