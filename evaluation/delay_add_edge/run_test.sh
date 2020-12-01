#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
RUNS=3
RUNS_FILE_BACKGROUND="runs_time_background.txt"
RUNS_FILE="runs_time.txt"
RUNS_FILE_DELAY="runs_time_delay.txt"
RUNS_FILE_DELAY_MUNRO="runs_time_delay_munro.txt"
RUNS_DATA="runs.data"
vertices=100000
TYPE="dmgen"

PLOT=0
if [[ $2 != "-plot" && $1 != "-webgraph" && $1 != "-dmgen" ]]; then
  echo "Usage: ./run_test.sh [-dmgen/webgraph] [OPTIONAL: -plot]"
  echo "  -plot: just plot the current data without having to run everything again."
  exit
elif [[ $2 == "-plot" ]]; then
  PLOT=1
fi

if [[ $1 == "-webgraph" ]]; then
  TYPE="webgraph"
  DATASETDIR="../../datasets/webgraph"
fi

eval_time() {
  ./add $DATASETDIR/$1/$1.tsv $2 $RUNS 1 >>"$RUNS_FILE-$TYPE"
  ./add $DATASETDIR/$1/$1.tsv $2 $RUNS 2 >>"$RUNS_FILE_BACKGROUND-$TYPE"
  ./add $DATASETDIR/$1/$1.tsv $2 $RUNS 3 >>"$RUNS_FILE_DELAY-$TYPE"
  ./add $DATASETDIR/$1/$1.tsv $2 $RUNS 4 >>"$RUNS_FILE_DELAY_MUNRO-$TYPE"
}

declare -a X_time=()
declare -a X_time_BACKGROUND=()
declare -a X_time_DELAY=()
declare -a X_time_MUNRO=()
i=0
prepared_data() {
  while read line; do
    X_time+=("$line")
    i=$((i + 1))
  done <"$RUNS_FILE-$TYPE"

  while read line; do
    X_time_BACKGROUND+=("$line")
  done <"$RUNS_FILE_BACKGROUND-$TYPE"

  while read line; do
    X_time_DELAY+=("$line")
  done <"$RUNS_FILE_DELAY-$TYPE"

  while read line; do
    X_time_MUNRO+=("$line")
  done <"$RUNS_FILE_DELAY_MUNRO-$TYPE"

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "$item ${X_time[${item}]} ${X_time_BACKGROUND[${item}]} ${X_time_DELAY[${item}]}  ${X_time_MUNRO[${item}]}" >>"$RUNS_DATA-$TYPE"
  done
  i=$((i + 1))
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal pdfcairo mono font "sans, 16"
  set output 'add_per_edge_time_$TYPE.pdf'
  set title "Add time per edge ($TYPE)"
  set grid
  set xlabel "m"
  set ylabel "t(s)"
    set style line 1 \
    linecolor rgb '#00000' \
    linetype 1 linewidth 2 \
    pointtype 8 pointsize 1
  plot "$RUNS_DATA-$TYPE" using 1:2 w p ls 8 pt 6 t "add edge",\
       "$RUNS_DATA-$TYPE" using 1:4 w p ls 6 pt 9 t "add edge delay",\
       "$RUNS_DATA-$TYPE" using 1:5 w p ls 3 pt 9 t "add edge munro",\
       "$RUNS_DATA-$TYPE" using 1:3 w p ls 4 pt 11 t "add edge parallel"
EOF
}

if [[ $PLOT == 0 ]]; then
  echo "Compiling..."
  make --keep-going clean add

  echo "Cleaning ..."
  rm "$RUNS_FILE-$TYPE"
  rm "$RUNS_FILE_BACKGROUND-$TYPE"
  rm "$RUNS_FILE_DELAY-$TYPE"
  rm "$RUNS_FILE_DELAY_MUNRO-$TYPE"
  rm "$RUNS_DATA-$TYPE"

  echo "Evaluating..."
  if [[ $TYPE == "dmgen" ]]; then
    echo "running for $vertices"
    eval_time $vertices $vertices
  else
    eval_time "uk-2007-05@100000" 100000
  fi
  echo "Preparing data..."
  prepared_data
fi

echo "Ploting time..."
plot_data_time

echo "Done!"
