all: presentation proposal diploma thesis

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

ONESHELL:
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


.ONESHELL:
presentation:
	mkdir -p build 
	cp proposal_presentation.tex build
	cd build
	pdflatex -interaction=nonstopmode proposal_presentation.tex 1> pdflatex_1.logs
	bibtex proposal 1> bibtex.logs
	pdflatex -interaction=nonstopmode proposal_presentation.tex 1> pdflatex_2.logs
	pdflatex -interaction=nonstopmode proposal_presentation.tex 1> pdflatex_3.logs
	cd ..
	cp build/proposal_presentation.pdf .


.ONESHELL:
thesis:
	mkdir -p build 
	cp -r thesis.tex bibliography.bib  sections build
	cd build
	pdflatex -interaction=nonstopmode -shell-escape thesis.tex 1> pdflatex_1.logs
	bibtex thesis 1> bibtex.logs
	pdflatex -interaction=nonstopmode -shell-escape thesis.tex 1> pdflatex_2.logs
	pdflatex -interaction=nonstopmode -shell-escape thesis.tex 1> pdflatex_3.logs
	cd ..
	cp build/thesis.pdf .



clear:
	rm -rf build

logs:
	less build/pdflatex_1.logs build/bibtex.logs build/pdflatex_2.logs build/pdflatex_3.logs
