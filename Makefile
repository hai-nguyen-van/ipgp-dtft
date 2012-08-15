all: spectra.c
	gcc -Wall spectra.c -o spectra -lm

plot:
	R CMD BATCH --no-save --no-restore '--args fichier="${SPECTRUM}"' draw_3d_plot.R 
	evince Rplots.pdf

clean: 
	rm -f *~
	rm -f *.Rout