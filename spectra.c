/*
 * Spectrum analyzing with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// factorial function
// be careful of recursive calls !
int factorial (int n){
  if (n == 0)
    return 1;
  else
    return n * factorial (n - 1);
}

// calculates the Modified Bessel function at nth order of the 1st kind
float modified_bessel_0 (float x){
  int k;
  int N = 12; //supposed to be infinite! for sure do further calculations of error to get good value with respect to precision : too high number causes division by 0
  float res = 0;
  for (k = 0 ; k < N ; k++){
    res = res + (pow ((0.25 * pow (x, 2.0)), ((float) k))) / pow (((float) (factorial (k))), 2.0);
  }
  return res;
}

// calculates and affects window function on a restricted part of the signal
void apply_window_function (float window[], int samp_window_length, int window_type){
  int t;
  float M;
  switch (window_type){
  case 0: // rectangular function
    return;
    break;

  case 1: // triangular function, Barlett
    for (t = 0 ; t < samp_window_length ; t++)
      if ((t >= 0) && (t < (samp_window_length / 2)))
	window[t] = window[t] * ((2.0 * t) / samp_window_length);
      else
	window[t] = window[t] * ((2.0 * (samp_window_length - t)) / samp_window_length);
    break;

  case 2: // Hann window
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (0.5 - 0.5 * (cos (2.0 * 3.14 * (((float) t) / ((float) (samp_window_length - 1))))));
    break;
	
  case 3: // Hamming window
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (0.54 - 0.46 * (cos ((2 * 3.14 * t) / (samp_window_length - 1))));
    break;

  case 4: // Kaiser-Bessel window
    M = ((float) samp_window_length) - 1.0;
    float beta = 3 * 3.14; // warning:has to be changed with respect to parameter alpha : 'b = 'a * PI
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (modified_bessel_0 (beta * sqrt (1 - pow ((((2 * ((float) t)) / M) - 1), 2))));
    break;
    
  default:
    exit(EXIT_FAILURE);
    break;
  }
}

// prints on stdout an array t as a set of float
void print_float_array (int length, float t[]){
  if (length < 7){
    int i;
    printf("{ ");
    for (i = 0 ; i < length ; i++)
      printf ("%f ; ", t[i]);
    printf(" }");
    return;
  }
  else{
    printf("{ %f, %f, ..., %f }", t[0], t[1], t[length - 1]);
  }
}

// initialize an array of frequencies wrt central frequency, span and bandwidth resolution
void frequency_components_array_init (float central_frequency, float span, float bandwidth_res, float frequency_components[]){
  int number_frequency_components = (int) (span / bandwidth_res);
  int i;
  for (i = 0 ; i < number_frequency_components ; i++){
    frequency_components[i] = (central_frequency - (span / 2)) + i * bandwidth_res;
  }
}

// replace a substring by another
char* replace(char *st, char *orig, char *repl) {
  static char buffer[4096];
  char *ch;
  if (!(ch = strstr(st, orig)))
   return st;
  strncpy(buffer, st, ch-st);  
  buffer[ch-st] = 0;
  sprintf(buffer+(ch-st), "%s%s", repl, ch+strlen(orig));
  return buffer;
}

// modify input, replace , with .
char* string2float(char* buf){
  return replace(buf,",",".");
}

// in discrete Fourier transform, returns the first sum
// recursive function needs a number of access to stack equals to number of samples to sum
float somme_fourier_1 (int n, float samples[], int samp_number, float freq_comp, float samp_freq){
  if (n == (samp_number - 1)){
    return (samples[samp_number - 1] * cos ((2 * freq_comp * 3.14 * n) / samp_freq));
  }
  else{
    return (samples[n] * cos ((2 * freq_comp * 3.14 * n) / samp_freq)) + somme_fourier_1 ((n + 1), samples, samp_number, freq_comp, samp_freq);
  }
}

// in discrete Fourier transform, returns the second sum
float somme_fourier_2 (int n, float samples[], int samp_number, float freq_comp, float samp_freq){
  if (n == (samp_number - 1)){
    return samples[samp_number - 1] * cos ((2 * freq_comp * 3.14 * n) / samp_freq);
  }
  else{
    return (samples[n] * sin ((2 * freq_comp * 3.14 * n) / samp_freq)) + somme_fourier_1 ((n + 1), samples, samp_number, freq_comp, samp_freq);
  }
}

// main function 
int main (int argc, char **argv){
  int j;
  int success = 0;
  char buf[256];
  float temp1;
  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  // BEGIN physics
  int n_samples = 0;                                     // (unit)
  int samp_freq = 10000;                                 // (Hz)
  int bit_rate  = 2;                                     // (Hz = bps)
  float window_length = 1.0 / bit_rate;                  // (s)
  int samp_window_length = (int) (samp_freq / bit_rate); // (unit) //attention inexact
  
  // manual choice for FT calc
  /*
    int number_frequency_components = 4;
    float frequency_components[] = { 20.0 , 21.0 , 22.0, 23.0 };
  */

  // pseudo continuous FT calc
  float central_frequency = 200000;                               // (Hz)
  float span = 300000;                                            // (Hz)
  float bandwidth_res = 1000;                                     // (Hz)
  int number_frequency_components = (int) (span / bandwidth_res); // (unit)
  float frequency_components[number_frequency_components];        // (set of Hz)
  // END physics

  frequency_components_array_init (central_frequency, span, bandwidth_res, frequency_components);

  printf("Starting spectrum analyzing of recorded signal...\n\n");
  printf("Preliminary hypothesis : \n");
  printf("Fs (sampling frequency : Hz) = %d\n", samp_freq);
  printf("T (window length : s) = %f\n", window_length);

  float window[samp_window_length];

  while(fgets(buf, 100, input_file) != NULL){
    success = sscanf(string2float(buf), "\t%f", &temp1);
    if (success > 0){
      success = 0;
      //      printf("\r%d", n_samples); // comment for better performance
      n_samples++;
    }
  }
  printf("\rN (number of samples : unit) = %d\n\n", n_samples);
  printf("Fourier transform computings on :\n");
  printf("Window function = rectangular window\n"); //a moduler
  printf("Samples for one window = %f\n", (samp_freq * window_length));
  printf("Number of windows to be computed on recorded signal (bits) = %f\n", (n_samples / (samp_freq * window_length)));

  int number_windows_in_signal = (int) (n_samples / (samp_freq * window_length));
  
  printf("Searching frequency components = ");
  print_float_array (number_frequency_components, frequency_components);
  printf("\n\n");

  printf("Computing... \n");

  // file processing
  input_file = fopen(argv[1], "r");
  success = 0;
  int i = 0;
  int k = 0;

  float time;
  float frequency;
  float amplitude;

  fprintf (output_file, "Frequency,Time,Amplitude\n");
  while(fgets(buf, 100, input_file) != NULL){
    success = sscanf(string2float(buf), "\t%f", &temp1);
    if (success > 0){
      if (i == (samp_window_length - 1)){
	i = 0; // put index to 0 to get a new computation window afterwards
	
	//apply a window function
	apply_window_function (window, samp_window_length, 4); // last number is window type, see upon for further functions

	//processing FT
	for (j = 0 ; j < number_frequency_components ; j++){
	  time = k * window_length;
	  frequency = frequency_components[j];
	  amplitude = sqrt(
		     pow ((somme_fourier_1 (0, window, samp_window_length, frequency_components[j], samp_freq)), 2)
		     + pow ((somme_fourier_2 (0, window, samp_window_length, frequency_components[j], samp_freq)), 2)
		     );
	  fprintf (output_file, "%f,%f,%f\n", frequency, time, amplitude);
	}
	k++;
	printf ("\r[%d] Calculating at %dth window...\t", ((int) (((k * 100) / number_windows_in_signal))), k); // comment for better performance
      }
      else{
	// fill the computation window 
	window[i] = temp1;
	i++;
      }
    }
  }


  // ending process
  printf("\nSpectrum analysis has ended.\n");
  fclose(input_file);
  fclose(output_file);
  return 0;
  
}
