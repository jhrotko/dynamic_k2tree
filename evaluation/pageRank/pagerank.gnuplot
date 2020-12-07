#!/usr/bin/env gnuplot

set terminal pdfcairo mono font "sans, 16"
set datafile separator whitespace
set output 'pagerank_time_dmgen.pdf'
set grid
set title "PageRank (dmgen)"
set xlabel "n √m + m (10⁹)"
set ylabel "t (s)"
set style data linespoints
plot "time-data-dmgen" using ($1/1000000000):($2) pt 4 notitle

set terminal pdfcairo mono font "sans, 16"
set datafile separator whitespace
set output 'pagerank_mem_dmgen.pdf'
set grid
set title "PageRank (dmgen)"
set xlabel "n+m (10⁶)"
set ylabel "memory (MB)"
set style data linespoints
plot "time-data-dmgen" using ($3/1000000):($4/1000) pt 4 notitle