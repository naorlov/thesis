all: proposal diploma 

.ONESHELL:
proposal : proposal.tex
	mkdir -p build 
	cp proposal.tex build
	cd build
	pdflatex proposal.tex 1> pdflatex_1.logs
	bibtex proposal 1> bibtex.logs
	pdflatex proposal.tex 1> pdflatex_2.logs
	pdflatex proposal.tex 1> pdflatex_3.logs
	cd ..
	cp build/proposal.pdf .

diploma:
	echo lal

clear:
	rm -rf build

logs:
	less build/pdflatex_1.logs build/bibtex.logs build/pdflatex_2.logs build/pdflatex_3.logs