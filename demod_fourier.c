/*
 * FM demodulation with Fourier transform with a rectangular window
 * Hai Nguyen Van <hai.ngv@etu.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * Licensed under CeCILL Free Software License Agreement
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

void frequency_components_array_init (float central_frequency, float span, float bandwidth_res, float frequency_components[]){
  int number_frequency_components = (int) (span / bandwidth_res);
  int i;
  for (i = 0 ; i < number_frequency_components ; i++){
    frequency_components[i] = (central_frequency - (span / 2)) + i * bandwidth_res;
  }
}

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

// recursive function needs a number of access to stack equals to number of samples to sum
float somme_fourier_1 (int n, float samples[], int samp_number, float freq_comp, float samp_freq){
  if (n == (samp_number - 1)){
    return (samples[samp_number - 1] * cos ((2 * freq_comp * 3.14 * n) / samp_freq));
  }
  else{
    return (samples[n] * cos ((2 * freq_comp * 3.14 * n) / samp_freq)) + somme_fourier_1 ((n + 1), samples, samp_number, freq_comp, samp_freq);
  }
}

float somme_fourier_2 (int n, float samples[], int samp_number, float freq_comp, float samp_freq){
  if (n == (samp_number - 1)){
    return samples[samp_number - 1] * cos ((2 * freq_comp * 3.14 * n) / samp_freq);
  }
  else{
    return (samples[n] * sin ((2 * freq_comp * 3.14 * n) / samp_freq)) + somme_fourier_1 ((n + 1), samples, samp_number, freq_comp, samp_freq);
  }
}


int main (int argc, char **argv){
  int j;
  int success = 0;
  char buf[256];
  float temp1;
  FILE *input_file = fopen(argv[1], "r");
  FILE *output_file = fopen(argv[2], "w");

  // BEGIN physics
  int n_samples = 0;       // (1)
  int samp_freq = 10000;   // (Hz)
  int bit_rate  = 2;    // (Hz = bps)
  float window_length = 1.0 / bit_rate; // (s)
  int samp_window_length = (int) (samp_freq / bit_rate); //attention inexact
  
  // manual choice for FT
  /*
    int number_frequency_components = 4;
    float frequency_components[] = { 20.0 , 21.0 , 22.0, 23.0 };
  */

  // pseudo continuous FT calc
  float central_frequency = 200000;
  float span = 300000;
  float bandwidth_res = 5000;
  int number_frequency_components = (int) (span / bandwidth_res);
  float frequency_components[number_frequency_components];
  frequency_components_array_init (central_frequency, span, bandwidth_res, frequency_components);
  // END physics

  printf("Starting FM demodulation of recorded signal...\n\n");
  printf("Preliminary hypothesis : \n");
  printf("Fs (sampling frequency : Hz) = %d\n", samp_freq);
  printf("T (window length) = %f\n", window_length);

  float window[samp_window_length];

  while(fgets(buf, 100, input_file) != NULL){
    success = sscanf(string2float(buf), "\t%f", &temp1);
    if (success > 0){
      success = 0;
      //      printf("\r%d", n_samples); // comment for better performance
      n_samples++;
    }
  }
  printf("\rN (number of samples) = %d\n\n", n_samples);
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
	i = 0;
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
  printf("\nFT demodulation has ended.\n");
  fclose(input_file);
  fclose(output_file);
  return 0;
  
}
