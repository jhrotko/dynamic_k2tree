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
plot "runs-data-time-dmgen" using 1:($3*1000000) with linespoints t "add edge parallel",\
   "runs-data-time-dmgen" using 1:($6*1000000) with linespoints t "add edge wait",\
   "runs-data-time-dmgen" using 1:($2*1000000) with linespoints t "add edge",\
   "runs-data-time-dmgen" using 1:($4*1000000) with linespoints t "add edge delay",\
   "runs-data-time-dmgen" using 1:($5*1000000) with linespoints t "add edge munro"

set terminal pdfcairo
set datafile separator whitespace
set output 'add_time_webgraph.pdf'
set title "Add Edge (webgraph)"
set grid
set xlabel "log_k(n)log(m)"
set ylabel "t (µs)"
set key left
plot "runs-data-time-webgraph" using 1:($3*1000000) with linespoints t "add edge parallel",\
   "runs-data-time-webgraph" using 1:($6*1000000) with linespoints t "add edge wait",\
   "runs-data-time-webgraph" using 1:($2*1000000) with linespoints t "add edge",\
   "runs-data-time-webgraph" using 1:($4*1000000) with linespoints t "add edge delay",\
   "runs-data-time-webgraph" using 1:($5*1000000) with linespoints t "add edge munro"


set terminal pdfcairo
set datafile separator whitespace
set output 'add_mem_dmgen.pdf'
set title "Add Edge (dmgen)"
set grid
set rmargin at screen 0.90
set xlabel "n+m (10^6)"
set ylabel "memory (MB)"
set key left
plot "runs-data-memory-dmgen" using ($1/1000000):($2/1000) with linespoints t "add edge parallel",\
    "runs-data-memory-dmgen" using ($1/1000000):($6/1000) with linespoints t "add edge wait",\
   "runs-data-memory-dmgen" using ($1/1000000):($3/1000) with linespoints t "add edge",\
   "runs-data-memory-dmgen" using ($1/1000000):($4/1000) with linespoints t "add edge delay",\
   "runs-data-memory-dmgen" using ($1/1000000):($5/1000) with linespoints t "add edge munro"

set terminal pdfcairo
set datafile separator whitespace
set output 'add_mem_webgraph.pdf'
set title "Add Edge (webgraph)"
set grid
set rmargin at screen 0.90
set xlabel "n+m (10^6)"
set ylabel "memory (MB)"
set key left
plot "runs-data-memory-webgraph" using ($1/1000000):($2/1000) with linespoints t "add edge parallel",\
         "runs-data-memory-webgraph" using ($1/1000000):($6/1000) with linespoints t "add edge wait",\
        "runs-data-memory-webgraph" using ($1/1000000):($3/1000) with linespoints t "add edge",\
        "runs-data-memory-webgraph" using ($1/1000000):($4/1000) with linespoints t "add edge delay",\
        "runs-data-memory-webgraph" using ($1/1000000):($5/1000) with linespoints t "add edge munro"