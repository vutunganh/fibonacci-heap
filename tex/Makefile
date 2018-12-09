TEX := pdfcsplain

vu-tung-anh.pdf: vu-tung-anh.tex style.tex
	$(TEX) $<
	bibtex vu-tung-anh.aux
	$(TEX) $<
	$(TEX) $<

.PHONY: clean

clean:
	rm *.log *.ref *.aux *.bbl *.blg

