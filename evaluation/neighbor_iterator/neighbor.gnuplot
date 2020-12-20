#!/usr/bin/env gnuplot

set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'neigh_time_dmgen.pdf'
set title "Neighbor Iterator (dmgen)"
set grid
set xlabel "√m"
set ylabel "t (µs)"
set yrange [:800]
set key left
plot "data-time-dmgen" using 1:($2*1000000) with linespoints pt 8 t "iterator" ,\
       "data-time-dmgen" using 1:($3*1000000) with linespoints pt 8 t "list"

set terminal pdfcairo mono font "sans, 12"
set datafile separator whitespace
set output 'neigh_mem_dmgen.pdf'
set title "Neighbor Iterator (dmgen)"
set grid
set xlabel "n+m (10⁶)"
set ylabel "memory (MB)"
set yrange [:130]
set key left
plot "data-memory-dmgen" using ($1/1000000):($2/1000) with linespoints pt 8 t "iterator",\
       "data-memory-dmgen" using ($1/1000000):($3/1000) with linespoints pt 8 t "list"