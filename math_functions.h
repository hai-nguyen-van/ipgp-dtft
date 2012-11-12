/**
 * IPGP-FFT - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.
 * File function.h
 */

/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
*/
#ifndef _MATH_FUNCTIONS_H_
#define _MATH_FUNCTIONS_H_

#define MAX_FILE_LINE_LENGTH 500
#define PI 3.14159265359

// factorial function
// be careful of recursive calls !
int factorial (int n);

// calculates the Modified Bessel function at nth order of the 1st kind
float modified_bessel_0 (float x);

// calculates and affects window function on a restricted part of the signal
void apply_window_function (float window[], int samp_window_length, int window_type);

// in discrete Fourier transform, returns the first sum
// recursive function needs a number of access to stack equals to number of samples to sum
float somme_fourier_1 (float samples[], int samp_number, float freq_comp, float samp_freq);

// in discrete Fourier transform, returns the second sum
float somme_fourier_2 (float samples[], int samp_number, float freq_comp, float samp_freq);

char *window_name_of_window_id (int window_id);

float modulus_of_fourier_transform_of_signal_window (float window[], int samp_window_length, float frequency_component, float samp_freq);

float modulus_of_fast_fourier_transform_of_signal_window (float window[], int samp_window_length, float frequency_component, float samp_freq);
#endif
