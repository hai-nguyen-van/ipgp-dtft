SPECTRA - Spectral analysis with Fourier transform
==============

Spectra is a spectrum analyzer on a variable window function with discrete Fourier transform.

Downloading and building
=============

To download you can click upon or type the following in the Terminal:

	git clone git://github.com/EmptyStackExn/spectra.git

The directory `./spectra/` is created:

	cd spectra
	make

You can execute the program with the following arguments:

	./spectra [INPUT FILE] [OUTPUT FILE]

You can also draw a 3D plot with [R](http://www.r-project.org/):

	make graph SPECTRUM=[SPRECTRUM FILE PREVIOUSLY PRODUCED]


Usage
=============

This unstable version not (yet) user-friendly. Parameters for FT computings need to be changed in `spectra.c` file:

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
        float bandwidth_res = 5000;                                     // (Hz)
        int number_frequency_components = (int) (span / bandwidth_res); // (unit)
        float frequency_components[number_frequency_components];        // (set of Hz)
        // END physics

The window function must be specified at the line:

        apply_window_function (window, samp_window_length, 1); // window function calculation

Troubleshooting 
---------------

You can report bugs at <nguyenva@informatique.univ-paris-diderot.fr>


Minimum system requirements
---------------------------

- gcc
- R with scatterplot3d library (for graphics)

LICENSE
=======

The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.

THIS FILE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
