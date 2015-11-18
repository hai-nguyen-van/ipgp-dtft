/**
 * IPGP-FFT - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.
 
 * Main file
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "init_functions.h"
#include "math_functions.h"

// main function 
int main (int argc, char **argv){

  // BEGIN physics ----------------------------------------------------------------------
  unsigned int n_samples;             // DO NOT FILL HERE!     // (unit)
  const float samp_freq = 10000;                              // (Hz)
  const float bit_rate = 1;           // DPDS ON NO OF WINDOWS // (Hz = bps)
  const float window_length = 1.0 / bit_rate;                  // (s)
  const int samp_window_length = (int) (samp_freq / bit_rate); // (unit) //attention inexact
  
  // manual choice for FT calc
  /*
    int number_frequency_components = 4;
    float frequency_components[] = { 20.0 , 21.0 , 22.0, 23.0 };
  */

  // pseudo continuous FT calc
  const float central_frequency = 1000;                                // (Hz)
  const float span = 2000;                                                // (Hz)
  const float bandwidth_res = 1;                                         // (Hz)
  const int number_frequency_components = (int) (span / bandwidth_res);  // (unit)
  float frequency_components[number_frequency_components];               // (set of Hz)

  const int window_function_id = int_of_string (argv[3]);                // see upon for associated id window function
  const int field = 0;                                                   // select field in file seperated by delimiter
  // END physics ------------------------------------------------------------------------

  int number_windows_in_signal;

  // initialize format reference string for LabVIEW lvm file format
  char format_as_lvm [field * 5 + 3];
  format_string_field_selection_as_lvm (format_as_lvm, field);

  //TODO
  // initialize format reference string for LabVIEW lvm file format
  // char format_as_raw_pratham [];
  // format_string_field_selection_as_raw_pratham (format_as_raw_pratham, field);

  char *format_ref_string = format_as_lvm;

  frequency_components_array_init (central_frequency, span, bandwidth_res, frequency_components); // comment if manual choice for FT computings parameters

  // start time
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  printf("Starting spectral analyzing of recorded signal on %s at %d-%d-%d %d:%d\n\n", argv[1], 1900 + tm.tm_year, 1 + tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min);
  printf("Preliminary hypothesis: \n");
  printf("Fs (sampling frequency) = %f Hz\n", samp_freq);
  printf("T (window length) = %f s\n", window_length);

  n_samples = number_of_samples_of_file (argv[1]);
  number_windows_in_signal = (int) (n_samples / (samp_freq * window_length));

  printf("\rN (number of samples) = %d unit\n", n_samples);
  printf("Dt (signal length) = %f s = %f min\n\n", n_samples / samp_freq, (n_samples / samp_freq) / 60);

  printf("Fourier transform computings will perform on:\n");
  printf("Window function = %s\n", window_name_of_window_id (window_function_id));
  printf("Samples for one window = %f\n", (samp_freq * window_length));
  printf("Number of windows to be computed on recorded signal (bits) = %f\n", (n_samples / (samp_freq * window_length)));
  printf("Searching frequency components = ");
  print_float_array (number_frequency_components, frequency_components); printf("\n\n");

  printf ("Initializing data into memory...\n");
  
  float *signal = malloc (sizeof(float) * n_samples);
  init_array_signal (signal, n_samples, format_ref_string, argv[1]);

  float *window;
  window = malloc (sizeof (float) * samp_window_length);

  printf("\rComputing... \n");
  
  int i; // iterator on each windows of the signal <-> iteration on time with a step of T
  int j; // iterator on each frequency component in search
  
  float time;      // (s)
  float frequency; // (Hz)
  float amplitude; // (V^2/Hz)

  // file processing
  FILE *output_file = fopen(argv[2], "w");
  fprintf (output_file, "Frequency,Time,Amplitude\n"); // initialize CSV output file header

  // iterating on windows
  for (i = 0 ; i < number_windows_in_signal ; i++){
    // initialize window function
    init_array_window (window, signal, samp_window_length, i);

    //apply a window function
    apply_window_function (window, samp_window_length, window_function_id);
    
    // iterating on each frequency component in search -> processing Fourier transform 
    for (j = 0 ; j < number_frequency_components ; j++){
      time = i * window_length;
      frequency = frequency_components[j];
      amplitude = modulus_of_fourier_transform_of_signal_window (window, samp_window_length, frequency_components[j], samp_freq);
      
      // amplitude = window [j];
      fprintf (output_file, "%f,%f,%f\n", frequency, time, amplitude); // writing on output channel
      printf ("\r%d%%", (int) ((i * j * 100) / ((number_windows_in_signal - 1) * (number_frequency_components - 1)))); // comment for better performance
      // printf ("\r%f,%f,%f\n", frequency, time, amplitude); // uncomment for bug solving
    }

    // printf ("\r[%d] Calculating at %dth window...\t", ((int) (((i * 100) / number_windows_in_signal))), i); // comment for better performance
  }

  // ending process
  printf ("\nDeallocating space in memory...\n");
  free (signal);
  free (window);
  printf ("Spectrum analysis has ended.\n");
  fclose(output_file);

  return EXIT_SUCCESS;
}
