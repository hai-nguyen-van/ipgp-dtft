/*
 * SPECTRA - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.
 * File funtion.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "function.h"

// need to be checked!
int int_of_string (char *s){
  int n[1];
  sscanf (s, "%d", n);
  return n[0];
}

// returns if c is a beginning of a number
int is_number (char c){
  return ((c == '-') || (c == '0') || (c == '1') || (c == '2') || (c == '3') || (c == '4') || (c == '5') || (c == '6') || (c == '7') || (c == '8') || (c == '9'));
}

// counts the number of samples of a channel in a file
int number_of_samples_of_file (char *file_name){
  FILE *input_file = fopen(file_name, "r");
  char buffer[MAX_FILE_LINE_LENGTH];
  int n = 0;
  while (!feof (input_file)){
    fscanf (input_file, "%[^\n]\n", buffer); // focus on a specific line
    if (is_number (buffer[0])){
      // sscanf (buffer, format_as_lvm, temp);
      // printf ("counted value : %f\n", temp[0]); // uncomment for bug solving
      // printf("\r%d", n); // comment for better performance
      n++;
    }
  }
  fclose(input_file);
  return n;
}

// makes a format string that matches number values in a LabVIEW Measurement file
void format_string_field_selection_as_lvm (char *s, int n){
  int i;
  strcpy (s, "");
  for (i = 0 ; i < n ; i++)
    strcat (s, "%*f\t");
  strcat (s, "%f");
}

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
  float tau;
  float D;
  float alpha;
  switch (window_type){
  case 0: // rectangular function (uniform)
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
      window[t] = window[t] * (0.5 - 0.5 * (cos (2.0 * PI * (((float) t) / ((float) (samp_window_length - 1))))));
    break;
	
  case 3: // Hamming window
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (0.54 - 0.46 * (cos ((2 * PI * t) / (samp_window_length - 1))));
    break;

  case 4: // Kaiser-Bessel window
    M = ((float) samp_window_length) - 1.0;
    float beta = 3 * PI; // warning:has to be changed with respect to parameter alpha : 'b = 'a * PI
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (modified_bessel_0 (beta * sqrt (1 - pow ((((2 * ((float) t)) / M) - 1), 2))));
    break;

  case 5: // flat top window
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (1.0
			       - 1.93  * cos ((2 * PI * t) / (samp_window_length - 1))
			       + 1.29  * cos ((4 * PI * t) / (samp_window_length - 1))
			       - 0.388 * cos ((6 * PI * t) / (samp_window_length - 1))
			       + 0.032 * cos ((8 * PI * t) / (samp_window_length - 1)));
    break;

  case 6: // Poisson window (exponential)
    D = 60; // warning:parameter D is the targeted decay
    tau = (8.69 * samp_window_length) / (2.0 * D);
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * exp (((-1.0) * abs (t - ((samp_window_length - 1) / 2.0))) / tau);
    break;

  case 7: // Blackman window
    alpha = 0.16; //warning: parameter 'a 
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (((1.0 - alpha) / 2.0)
			       - 0.5          * cos ((2 * PI * t) / (samp_window_length - 1))
			       + (alpha / 2)  * cos ((4 * PI * t) / (samp_window_length - 1)));
    break;

  case 8: // Exact Blackman window
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * ((7938.0 / 18608.0)
			       - (9240.0 / 18608.0) * cos ((2 * PI * t) / (samp_window_length - 1))
			       + (1430.0 / 18608.0) * cos ((4 * PI * t) / (samp_window_length - 1)));
    break;

  case 9: // Blackman-Harris window
    for (t = 0 ; t < samp_window_length ; t++)
      window[t] = window[t] * (0.35875
			       - 0.48829 * cos ((2 * PI * t) / (samp_window_length - 1))
			       + 0.14128 * cos ((4 * PI * t) / (samp_window_length - 1))
			       - 0.01168 * cos ((6 * PI * t) / (samp_window_length - 1)));
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
    return (samples[samp_number - 1] * cos ((2 * freq_comp * PI * n) / samp_freq));
  }
  else{
    return (samples[n] * cos ((2 * freq_comp * PI * n) / samp_freq)) + somme_fourier_1 ((n + 1), samples, samp_number, freq_comp, samp_freq);
  }
}

// in discrete Fourier transform, returns the second sum
float somme_fourier_2 (int n, float samples[], int samp_number, float freq_comp, float samp_freq){
  if (n == (samp_number - 1)){
    return samples[samp_number - 1] * cos ((2 * freq_comp * PI * n) / samp_freq);
  }
  else{
    return (samples[n] * sin ((2 * freq_comp * PI * n) / samp_freq)) + somme_fourier_1 ((n + 1), samples, samp_number, freq_comp, samp_freq);
  }
}

char *window_name_of_window_id (int window_id){
  switch (window_id){
  case 0:  return "rectangular window (uniform)"; break;
  case 1:  return "triangular window (Barlett)"; break;
  case 2:  return "Hann window"; break;
  case 3:  return "Hamming window"; break;
  case 4:  return "Kaiser-Bessel window"; break;
  case 5:  return "flat top window"; break;
  case 6:  return "Poisson window (exponential)"; break;
  case 7:  return "Blackman window"; break;
  case 8:  return "Exact Blackman window"; break;
  case 9:  return "Blackman-Harris window"; break;
  default: exit (EXIT_FAILURE); break;
  }
}

