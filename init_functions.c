/**
 * SPECTRA - a window-optimized spectrum analyzer with Fourier transform
 * Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
 * Institut de Physique du Globe de Paris, Université Paris Diderot
 * The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.

 * File init_functions.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "init_functions.h"

// initializes a window array with a signal
void init_array_window (float window[], float signal[], int samp_window_length, int index){
  int n;
  for (n = 0 ; n < samp_window_length ; n++)
    window [n] = signal [index * samp_window_length + n];
  return;
}

// initilizes a signal array with a file name
void init_array_signal (float signal[], int signal_size, char format_string[], char file_name[]){
  FILE *input_file = fopen(file_name, "r");
  char buffer[MAX_FILE_LINE_LENGTH];
  int n = 0;
  float temp[1];
  while (!feof (input_file)){
    fscanf (input_file, "%[^\n]\n", buffer); // focus on a specific line
    if (is_number (buffer[0])){
      sscanf (buffer, format_string, temp);
      signal [n] = temp[0];
      if (n * 100 % signal_size == 0){
        printf ("\r%d%%", (int) ((n * 100) / signal_size));
        fflush(stdout);
      }
      n++;
    }
  }
  fclose(input_file);
  return;
}

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


