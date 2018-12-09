GNUPLOT := gnuplot
PDF_FILES := graph-1.pdf graph-2.pdf graph-3.pdf graph-4.pdf \
  standard-uniform-avg-steps.pdf standard-imbalanced-avg-steps.pdf

build: $(PDF_FILES)

%.pdf: %.gpi settings.gpi
	$(GNUPLOT) $<
