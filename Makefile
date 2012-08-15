all: spectra.c
	gcc -Wall spectra.c -o spectra -lm

plot:
	R CMD BATCH --no-save --no-restore '--args input="${SPECTRUM}" output="${SPECTRUM}.png"' draw_3d_plot.R 
#	evince ${SPECTRUM}.pdf &

clean: 
	rm -f *~
	rm -f *.Rout

clean_output:
	rm -f *.pdf
	rm -f *.png