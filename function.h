/*
 * SPECTRA - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.
 * File funtion.c
 */

/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
*/
#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#define MAX_FILE_LINE_LENGTH 500
#define PI 3.14159265359

// need to be checked!
int int_of_string (char *s);

// returns if c is a beginning of a number
int is_number (char c);

// counts the number of samples of a channel in a file
int number_of_samples_of_file (char *file_name);

// makes a format string that matches number values in a LabVIEW Measurement file
void format_string_field_selection_as_lvm (char *s, int n);

// factorial function
// be careful of recursive calls !
int factorial (int n);

// calculates the Modified Bessel function at nth order of the 1st kind
float modified_bessel_0 (float x);

// calculates and affects window function on a restricted part of the signal
void apply_window_function (float window[], int samp_window_length, int window_type);

// prints on stdout an array t as a set of float
void print_float_array (int length, float t[]);

// initialize an array of frequencies wrt central frequency, span and bandwidth resolution
void frequency_components_array_init (float central_frequency, float span, float bandwidth_res, float frequency_components[]);

// replace a substring by another
char* replace(char *st, char *orig, char *repl);

// modify input, replace , with .
char* string2float(char* buf);

// in discrete Fourier transform, returns the first sum
// recursive function needs a number of access to stack equals to number of samples to sum
float somme_fourier_1 (int n, float samples[], int samp_number, float freq_comp, float samp_freq);

// in discrete Fourier transform, returns the second sum
float somme_fourier_2 (int n, float samples[], int samp_number, float freq_comp, float samp_freq);

char *window_name_of_window_id (int window_id);
#endif
