#!/usr/bin/env gnuplot

set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'edge_time_dmgen.pdf'
set title "Edge Iterator (dmgen)"
set grid
set xlabel "log_k n"
set ylabel "t (µs)"
#set xrange [:6000]
#set yrange [:16]
plot "data-time-dmgen" using 1:($2*1000000) with linespoints pt 8 notitle


set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'edge_mem_dmgen.pdf'
set title "Edge Iterator (dmgen)"
set grid
set xlabel "n+m (10⁶)"
set ylabel "memory (MB)"
set yrange [:150]
plot "data-memory-dmgen" using ($1/1000000):($2/1000) with linespoints pt 8 notitle