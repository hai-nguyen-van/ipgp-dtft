all: spectra.c init_functions.o math_functions.o
	gcc -Wall math_functions.o init_functions.o spectra.c -o spectra -lm

init_functions.o: init_functions.c
	gcc -Wall -c init_functions.c -o init_functions.o 

math_functions.o: math_functions.c
	gcc -Wall -c math_functions.c -o math_functions.o 

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
