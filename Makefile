all: spectra.c function.o
	gcc -Wall function.o spectra.c -o spectra -lm

function.o: function.c
	gcc -Wall -c function.c -o function.o 

plot:
	R CMD BATCH --no-save --no-restore '--args input="${SPECTRUM}" output="${SPECTRUM}.png"' draw_3d_plot.R 
#	evince ${SPECTRUM}.pdf &

clean: 
	rm -f *~
	rm -f *.Rout
	rm -f *.o

clean_output:
	rm -f *.pdf
	rm -f *.png
