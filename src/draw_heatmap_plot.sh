#! /bin/bash

function gnuplot_header {
    echo "unset key
set view map
set cblabel \"Amplitude (dB)\"
set palette rgbformulae 21,22,23
set title \"Signal spectrum of $1\"
set term pdf enhanced
set output \"$1.pdf\"
set xlabel \"Time (s)\"
set ylabel \"Frequency (Hz)\"
set xrange [0:]
set yrange [0:]
plot '-' using 2:1:3 with image"
}

(gnuplot_header $1 ; (cat $1 | sed -e 's/,/ /g')) | gnuplot
