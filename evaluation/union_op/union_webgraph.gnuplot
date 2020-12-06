#!/usr/bin/env gnuplot

set terminal pdfcairo mono font "sans, 16"
set output 'union_time_webgraph_all.pdf'
set title  'Union (webgraph)'

set key left top
set grid
set xlabel 'n + m (10^6)'
set ylabel 't (s)'
set format '%g'
set style data linespoints

##### Black and white version.

#set xtics 0,1.5e12,8e12

plot \
  'time-data-webgraph' using ($4/1000000):1 pt 8 title 'sdk2sdsl', \
  'time-data-sdk-webgraph' using ($4/1000000):1 pt 10 title 'sdk2tree', \


set terminal pdfcairo mono font "sans, 16"
set output 'union_mem_webgraph_all.pdf'
set title  'Union (webgraph)'

set key left top
set grid
set xlabel 'n (10^6)'
set ylabel 'memory (MB)'
set format '%g'
set style data linespoints

##### Black and white version.

#set xtics 0,1.5e12,8e12

plot \
  'time-data-webgraph' using ($3/1000000):($2/1000) pt 8 title 'sdk2sdsl', \
  'time-data-sdk-webgraph' using ($3/1000000):($2/1000) pt 10 title 'sdk2tree', \