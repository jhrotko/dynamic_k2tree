#!/usr/bin/env gnuplot

set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'neigh_time_dmgen.pdf'
set title "Neighbor Iterator (dmgen)"
set grid
set xlabel "√m"
set ylabel "t (µs)"
#set xrange [:6000]
#set yrange [:16]
plot "data-time-dmgen" using 1:($2*1000000) with linespoints pt 8 notitle


set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'neigh_mem_dmgen.pdf'
set title "Neighbor Iterator (dmgen)"
set grid
set xlabel "n+m (10⁶)"
set ylabel "memory (MB)"
set yrange [:150]
plot "data-memory-dmgen" using ($1/1000000):($2/1000) with linespoints pt 8 notitle


set terminal pdfcairo
set datafile separator whitespace
set output 'neigh_time_webgraph.pdf'
set title "Neighbor Iterator (webgraph)"
set grid
set xlabel "√m"
set ylabel "t (µs)"
plot "data-time-webgraph" using 1:($2*1000000) with linespoints pt 8 notitle

set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'neigh_mem_webgraph.pdf'
set title "Neighbor Iterator (webgraph)"
set grid
set xlabel "n+m"
set ylabel "memory (MB)"
plot "data-memory-webgraph" using 1:($2/1000) with linespoints pt 8 notitle