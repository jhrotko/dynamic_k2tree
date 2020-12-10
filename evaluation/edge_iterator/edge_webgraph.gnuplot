#!/usr/bin/env gnuplot

set terminal pdfcairo
set datafile separator whitespace
set output 'edge_time_webgraph.pdf'
set title "Edge Iterator (webgraph)"
set grid
set xlabel "log_k n"
set ylabel "t (µs)"
plot "data-time-webgraph" using 1:($2*1000000) with linespoints pt 8 notitle

set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'edge_mem_webgraph.pdf'
set title "Edge Iterator (webgraph)"
set grid
set xlabel "n+m"
set ylabel "memory (MB)"
plot "data-memory-webgraph" using 1:($2/1000) with linespoints pt 8 notitle