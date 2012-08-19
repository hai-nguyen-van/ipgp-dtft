/*
 * SPECTRA - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "function.h"

// main function 
int main (int argc, char **argv){

  // BEGIN physics -------------------------------------------------------------------------------
  int n_samples;       // DO NOT FILL HERE!              // (unit)
  float samp_freq = 10000;                              // (Hz)
  int bit_rate = 1;                                      // (Hz = bps)
  float window_length = 1.0 / bit_rate;                  // (s)
  int samp_window_length = (int) (samp_freq / bit_rate); // (unit) //attention inexact
  
  // manual choice for FT calc
  /*
    int number_frequency_components = 4;
    float frequency_components[] = { 20.0 , 21.0 , 22.0, 23.0 };
  */

  // pseudo continuous FT calc
  float central_frequency = 100000;                               // (Hz)
  float span = 200000;                                            // (Hz)
  float bandwidth_res = 100;                                       // (Hz)
  int number_frequency_components = (int) (span / bandwidth_res); // (unit)
  float frequency_components[number_frequency_components];        // (set of Hz)
  int window_function_id = 0;                                     // see upon for associated id window function

  int field = 0;                                                  // select field in file seperated by delimiter
  // END physics --------------------------------------------------------------------------------

  frequency_components_array_init (central_frequency, span, bandwidth_res, frequency_components); // comment if manual choice for FT computings parameters

  printf("Starting spectrum analyzing of recorded signal...\n\n");
  printf("Preliminary hypothesis : \n");
  printf("Fs (sampling frequency) = %f Hz\n", samp_freq);
  printf("T (window length) = %f s\n", window_length);
  float window[samp_window_length];

  char format_as_lvm [field * 5 + 3];
  format_string_field_selection_as_lvm (format_as_lvm, field);

  n_samples = number_of_samples_of_file (argv[1]);

  printf("\rN (number of samples) = %d unit\n", n_samples);
  printf("Dt (signal length) = %f s = %f min\n\n", n_samples / samp_freq, (n_samples / samp_freq) / 60);

  printf("Fourier transform computings on :\n");
  printf("Window function = %s\n", window_name_of_window_id (window_function_id)); //a moduler
  printf("Samples for one window = %f\n", (samp_freq * window_length));
  printf("Number of windows to be computed on recorded signal (bits) = %f\n", (n_samples / (samp_freq * window_length)));

  int number_windows_in_signal = (int) (n_samples / (samp_freq * window_length));
  
  printf("Searching frequency components = ");
  print_float_array (number_frequency_components, frequency_components);
  printf("\n\n");

  printf("Computing... \n");

  int j; // iterator
  char buffer[MAX_FILE_LINE_LENGTH];
  float temp [1];
  
  // file processing
  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");
  int i = 0;
  int k = 0;

  float time;
  float frequency;
  float amplitude;

  fprintf (output_file, "Frequency,Time,Amplitude\n");

  while (!feof (input_file)){
    fscanf (input_file, "%[^\n]\n", buffer); // focus on a specific line
    if (is_number (buffer[0])){
      sscanf  (buffer, format_as_lvm, temp);
      // printf("counted value : %f\n", temp[0]); // uncomment for bug solving
      
      if (i == (samp_window_length - 1)){
	i = 0; // put index to 0 to get a new computation window afterwards
	
	//apply a window function
	apply_window_function (window, samp_window_length, window_function_id);

	//processing Fourier transform
	for (j = 0 ; j < number_frequency_components ; j++){
	  time = k * window_length;
	  frequency = frequency_components[j];
	  amplitude = sqrt(
		     pow ((somme_fourier_1 (0, window, samp_window_length, frequency_components[j], samp_freq)), 2)
		     + pow ((somme_fourier_2 (0, window, samp_window_length, frequency_components[j], samp_freq)), 2)
		     );
	  //	  printf ("%f,%f,%f\n", frequency, time, amplitude);
	  fprintf (output_file, "%f,%f,%f\n", frequency, time, amplitude);
	}
	k++;
	printf ("\r[%d] Calculating at %dth window...\t", ((int) (((k * 100) / number_windows_in_signal))), k); // comment for better performance
      }
      else{
	// fill the computation window until window gets full for FT computation
	window[i] = temp[0];
	i++;
      }
    }
  }
  fclose(input_file);


  // ending process
  printf("\nSpectrum analysis has ended.\n");
  fclose(input_file);
  fclose(output_file);

  return EXIT_SUCCESS;
}
