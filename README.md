SPECTRA - Spectral analysis with Fourier transform
==============

Spectra is a window-optimized spectrum analyzer with discrete Fourier transform.

Downloading and building
=============

To download you can click upon or type the following in the Terminal:

	git clone git://github.com/EmptyStackExn/spectra.git

The directory `./spectra/` is created. You need to compile:

	cd spectra
	make

You can now execute the program with the following arguments:

	./spectra [INPUT FILE] [OUTPUT FILE]

You can also produce a 3D plot in the current directory with [R](http://www.r-project.org/) with:

	make graph SPECTRUM=[SPRECTRUM FILE PREVIOUSLY PRODUCED]


Usage
=============

This unstable version not (yet) user-friendly. Parameters for FT computings need to be changed in `spectra.c` file at the following lines:

        // BEGIN physics
        int n_samples = 0;                                              // (unit)

        ...

        float frequency_components[number_frequency_components];        // (set of Hz)
        // END physics


Specifications for input and output files
---------------------------

The input file is a sequence of lines consisting of ASCII decimal numbers seperated by tab spaces `\t`. A line stands for an acquisition at a time t and yet several fields in a line represents the different indexed channels of the ADC. This specification is compatible with comma-delimited [LabVIEW Measurement filetype](http://www.ni.com/white-paper/4139/en). Pratham filetype specifications will soon get available in the next versions.

The output file is a [comma-seperated values (CSV)](http://supercsv.sourceforge.net/csvSpecification.html) file.


Troubleshooting 
---------------

You can report bugs at <nguyenva@informatique.univ-paris-diderot.fr>


Minimum system requirements
---------------------------

- [gcc](http://cran.r-project.org/web/packages/scatterplot3d/index.html)
- [R](http://www.r-project.org/) with [scatterplot3d library](http://cran.r-project.org/web/packages/scatterplot3d/index.html) (for graphics)

LICENSE
=======

The copyright to this code is held by Institut de Physique du Globe de Paris. All rights reserved. This file is distributed under the license CeCILL Free Software License Agreement.

THIS FILE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Hai Nguyen Van <nguyenva@informatique.univ-paris-diderot.fr>
