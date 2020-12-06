set terminal pdfcairo mono font "sans, 2"
set output 'add_per_edge_time_dmgen.pdf'
set title "Add time per edge (dmgen)"
set grid
set xlabel "m"
set ylabel "t(s)"
set style line 1 linecolor rgb '#00000' linetype 1 linewidth 2 pointtype 8 pointsize 1
plot "runs.data" using 1:2 w p ls 8 pt 6 t "add edge",\
   "runs.data" using 1:4 w p ls 6 pt 9 t "add edge delay",\
   "runs.data" using 1:5 w p ls 3 pt 9 t "add edge munro",\
   "runs.data" using 1:3 w p ls 4 pt 11 t "add edge parallel"