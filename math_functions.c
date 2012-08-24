/**
 * SPECTRA - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.

 * File math_functions.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "math_functions.h"

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

// in discrete Fourier transform, returns the first sum
// recursive function needs a number of access to stack equals to number of samples to sum
float somme_fourier_1 (float samples[], int samp_number, float freq_comp, float samp_freq){
  int n; // iterator
  float result = 0;
  for (n = 0 ; n < samp_number ; n++)
    result = result + (samples[n] * cos ((2 * freq_comp * PI * n) / samp_freq));
  return result;
}

// in discrete Fourier transform, returns the second sum
float somme_fourier_2 (float samples[], int samp_number, float freq_comp, float samp_freq){
  int n; // iterator
  float result = 0;
  for (n = 0 ; n < samp_number ; n++)
    result = result + (samples[n] * sin ((2 * freq_comp * PI * n) / samp_freq));
  return result;
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

