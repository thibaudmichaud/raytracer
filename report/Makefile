TEXI2PDFFLAGS += -~

BASE = report
PDF = $(BASE).pdf

$(PDF):
	pdflatex report.tex
	bibtex report
	pdflatex report.tex
	pdflatex report.tex

view: $(PDF)
	xpdf $(PDF)

clean:
	rm -rf report.aux report.log report.pdf
