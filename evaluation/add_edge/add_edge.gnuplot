#!/usr/bin/env gnuplot

set terminal pdfcairo
set datafile separator whitespace
set output 'add_time_dmgen.pdf'
set title "Add Edge (dmgen)"
set grid
set xlabel "log_k(n)log(m)"
set ylabel "t (µs)"
#set yrange [:30]
set key left
plot "runs-data-dmgen" using 1:($2*1000000) with linespoints t "add edge parallel",\
   "runs-data-dmgen" using 1:($4*1000000) with linespoints t "add edge",\
   "runs-data-dmgen" using 1:($6*1000000) with linespoints t "add edge delay",\
   "runs-data-dmgen" using 1:($8*1000000) with linespoints t "add edge munro"

set terminal pdfcairo
set datafile separator whitespace
set output 'add_time_webgraph.pdf'
set title "Add Edge (webgraph)"
set grid
set xlabel "log_k(n)log(m)"
set ylabel "t (µs)"
set key left
plot "runs-data-webgraph" using 1:($2*1000000) with linespoints t "add edge parallel",\
   "runs-data-webgraph" using 1:($4*1000000) with linespoints t "add edge",\
   "runs-data-webgraph" using 1:($6*1000000) with linespoints t "add edge delay",\
   "runs-data-webgraph" using 1:($8*1000000) with linespoints t "add edge munro"


set terminal pdfcairo
set datafile separator whitespace
set output 'add_mem_dmgen.pdf'
set title "Add Edge (dmgen)"
set grid
set rmargin at screen 0.90
set xlabel "n+m (10^6)"
set ylabel "memory (MB)"
set key left
plot "runs-data-dmgen" using ($10/1000000):($3/1000) with linespoints t "add edge parallel",\
   "runs-data-dmgen" using ($10/1000000):($5/1000) with linespoints t "add edge",\
   "runs-data-dmgen" using ($10/1000000):($7/1000) with linespoints t "add edge delay",\
   "runs-data-dmgen" using ($10/1000000):($9/1000) with linespoints t "add edge munro"

set terminal pdfcairo
set datafile separator whitespace
set output 'add_mem_webgraph.pdf'
set title "Add Edge (webgraph)"
set grid
set rmargin at screen 0.90
set xlabel "n+m (10^6)"
set ylabel "memory (MB)"
set key left
plot "runs-data-webgraph" using ($10/1000000):($3/1000) with linespoints t "add edge parallel",\
   "runs-data-webgraph" using ($10/1000000):($5/1000) with linespoints t "add edge",\
   "runs-data-webgraph" using ($10/1000000):($7/1000) with linespoints t "add edge delay",\
   "runs-data-webgraph" using ($10/1000000):($9/1000) with linespoints t "add edge munro"