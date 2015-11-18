/**
 * IPGP-FFT - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.
 *
 * File init_functions.h
 */

/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
*/
#ifndef _INIT_FUNCTIONS_H_
#define _INIT_FUNCTIONS_H_

#define MAX_FILE_LINE_LENGTH 500
#define PI 3.14159265359

void init_array_window (float window[], float signal[], int samp_window_length, int index);

void init_array_signal (float signal[], int signal_size, char format_string[], char file_name[]);

// need to be checked!
int int_of_string (char *s);

// returns if c is a beginning of a number
int is_number (char c);

// counts the number of samples of a channel in a file
int number_of_samples_of_file (char *file_name);

// makes a format string that matches number values in a LabVIEW Measurement file
void format_string_field_selection_as_lvm (char *s, int n);

// prints on stdout an array t as a set of float
void print_float_array (int length, float t[]);

// initialize an array of frequencies wrt central frequency, span and bandwidth resolution
void frequency_components_array_init (float central_frequency, float span, float bandwidth_res, float frequency_components[]);

// replace a substring by another
char* replace(char *st, char *orig, char *repl);

// modify input, replace , with .
char* string2float(char* buf);

#endif
