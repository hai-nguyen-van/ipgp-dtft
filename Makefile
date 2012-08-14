all: spectra.c
	gcc -Wall spectra.c -o spectra -lm

graph:
	R CMD BATCH --no-save --no-restore '--args fichier="${SPECTRUM}"' draw_3d_plot.R 

clean: 
	rm -f *~
	rm -f *.Rout