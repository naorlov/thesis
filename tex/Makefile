all: proposal diploma 

.ONESHELL:
proposal:
	mkdir -p build 
	cp proposal.tex bibliography.bib build
	cd build
	pdflatex -interaction=nonstopmode proposal.tex 1> pdflatex_1.logs
	bibtex proposal 1> bibtex.logs
	pdflatex -interaction=nonstopmode proposal.tex 1> pdflatex_2.logs
	pdflatex -interaction=nonstopmode proposal.tex 1> pdflatex_3.logs
	cd ..
	cp build/proposal.pdf .

.ONESHELL:
diploma:
	mkdir -p build
	cp -r diploma.tex bibliography.bib header.tex sections build
	cd build
	pdflatex -interaction=nonstopmode diploma.tex 1> pdflatex_1.logs
	bibtex diploma 1> bibtex.logs
	pdflatex -interaction=nonstopmode diploma.tex 1> pdflatex_2.logs
	pdflatex -interaction=nonstopmode diploma.tex 1> pdflatex_3.logs
	cd ..
	cp build/diploma.pdf .

clear:
	rm -rf build

logs:
	less build/pdflatex_1.logs build/bibtex.logs build/pdflatex_2.logs build/pdflatex_3.logs