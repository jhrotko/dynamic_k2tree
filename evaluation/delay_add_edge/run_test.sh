#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
RUNS=3
RUNS_FILE_BACKGROUND="runs_time_background.txt"
RUNS_FILE="runs_time.txt"
RUNS_FILE_DELAY="runs_time_delay.txt"
RUNS_FILE_DELAY_MUNRO="runs_time_delay_munro.txt"
RUNS_DATA="runs.data"
vertices=1000000

PLOT=0
if [[ $1 != "-plot" && $1 != "" ]]; then
  echo "Usage: ./run_test.sh [OPTIONAL: -plot]"
  echo "  -plot: just plot the current data without having to run everything again."
  exit
elif [[ $1 == "-plot" ]]; then
  PLOT=1
fi

eval_time() {
  ./add $DATASETDIR/$1/$1.tsv $1 $RUNS 1 >>"$RUNS_FILE"
  ./add $DATASETDIR/$1/$1.tsv $1 $RUNS 2 >>"$RUNS_FILE_BACKGROUND"
  ./add $DATASETDIR/$1/$1.tsv $1 $RUNS 3 >>"$RUNS_FILE_DELAY"
  ./add $DATASETDIR/$1/$1.tsv $1 $RUNS 4 >>"$RUNS_FILE_DELAY_MUNRO"
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
  done <$RUNS_FILE

  while read line; do
    X_time_BACKGROUND+=("$line")
  done <$RUNS_FILE_BACKGROUND

  while read line; do
    X_time_DELAY+=("$line")
  done <$RUNS_FILE_DELAY

  while read line; do
    X_time_MUNRO+=("$line")
  done <$RUNS_FILE_DELAY_MUNRO

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "$item ${X_time[${item}]} ${X_time_BACKGROUND[${item}]} ${X_time_DELAY[${item}]}  ${X_time_MUNRO[${item}]}" >>$RUNS_DATA
  done
  i=$((i + 1))
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal pngcairo size 1024,768
  set datafile separator whitespace
  set output 'add_per_edge_time.png'
  set xrange [0:203257]
  set xlabel "m"
  set ylabel "Time (s)"
    set style line 1 \
    linecolor rgb '#00000' \
    linetype 1 linewidth 2 \
    pointtype 8 pointsize 1
  plot "$RUNS_DATA" using 1:2 w p ls 8 pt 6 t "add edge",\
       "$RUNS_DATA" using 1:4 w p ls 6 pt 9 t "add edge delay",\
       "$RUNS_DATA" using 1:5 w p ls 3 pt 9 t "add edge munro",\
       "$RUNS_DATA" using 1:3 w p ls 4 pt 11 t "add edge parallel"
EOF
}

if [[ $PLOT == 0 ]]; then
  echo "Compiling..."
  make --keep-going clean add

  echo "Cleaning ..."
  rm $RUNS_FILE
  rm $RUNS_FILE_BACKGROUND
  rm $RUNS_FILE_DELAY
  rm $RUNS_FILE_DELAY_MUNRO
  rm $RUNS_DATA

  echo "Evaluating..."
  echo "running for $vertices"
  eval_time $vertices

  echo "Preparing data..."
  prepared_data
fi

echo "Ploting time..."
plot_data_time

echo "Done!"
