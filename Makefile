LATEX	= latex -shell-escape
BIBTEX	= bibtex
DVIPS	= dvips
DVIPDF  = dvipdft
XDVI	= xdvi -gamma 4
GH		= gv

CC	= gcc
CFLAGS	= -pthread -w
CFILE	= din_phil.c

EXAMPLES = $(wildcard *.h)
SRC	:= $(shell egrep -l '^[^%]*\\begin\{document\}' *.tex)
TRG	= $(SRC:%.tex=%.dvi)
PSF	= $(SRC:%.tex=%.ps)
PDF	= $(SRC:%.tex=%.pdf)

all: pdf aout

aout: $(CFILE)
	$(CC) $(CFLAGS) $(CFILE)

pdf: $(PDF)

ps: $(PSF)

$(TRG): %.dvi: %.tex $(EXAMPLES)
	$(LATEX) $<
	#$(BIBTEX) $(<:%.tex=%)
	$(LATEX) $<
	$(LATEX) $<

$(PSF):%.ps: %.dvi
	$(DVIPS) -R -Poutline -t letter $< -o $@

$(PDF): %.pdf: %.ps
#	$(DVIPDF) -o $@ $<
	ps2pdf $<

show: $(TRG)
	@for i in $(TRG) ; do $(XDVI) $$i & done

showps: $(PSF)
	@for i in $(PSF) ; do $(GH) $$i & done


clean:
	rm -f *.pdf *.ps *.dvi *.out *.log *.aux *.bbl *.blg *.pyg

.PHONY: all show clean ps pdf showps c
