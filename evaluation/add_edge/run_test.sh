#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
RUNS=1
RUNS_FILE_BACKGROUND="runs_time_background.txt"
RUNS_FILE="runs_time.txt"
RUNS_FILE_DELAY="runs_time_delay.txt"
RUNS_FILE_DELAY_MUNRO="runs_time_delay_munro.txt"
RUNS_DATA="runs.data"
TYPE=""

if [[ $1 != "-mean" && $1 != "-deviation" ]]; then
  echo "Usage: ./run_test.sh -[mean | deviation] [OPTIONAL: -plot]"
  echo "  -plot: just plot the current data without having to run everything again."
  exit
else
  TYPE="${1:1}" #remove '-' from first argument
fi

eval_time_mean() {
    ./add_mean $DATASETDIR/$1/$1.tsv $1 $RUNS 1 >>"$1/mean_$RUNS_FILE"
    ./add_mean $DATASETDIR/$1/$1.tsv $1 $RUNS 2 >>"$1/mean_$RUNS_FILE_BACKGROUND"
    ./add_mean $DATASETDIR/$1/$1.tsv $1 $RUNS 3 >>"$1/mean_$RUNS_FILE_DELAY"
    ./add_mean $DATASETDIR/$1/$1.tsv $1 $RUNS 4 >>"$1/mean_$RUNS_FILE_DELAY_MUNRO"
}
eval_time_deviation() {
    ./add_deviation $DATASETDIR/$1/$1.tsv $1 $RUNS 1 >>"$1/deviation_$RUNS_FILE"
    ./add_deviation $DATASETDIR/$1/$1.tsv $1 $RUNS 2 >>"$1/deviation_$RUNS_FILE_BACKGROUND"
    ./add_deviation $DATASETDIR/$1/$1.tsv $1 $RUNS 3 >>"$1/deviation_$RUNS_FILE_DELAY"
    ./add_deviation $DATASETDIR/$1/$1.tsv $1 $RUNS 4 >>"$1/deviation_$RUNS_FILE_DELAY_MUNRO"
}

eval_memory() {
  #  $1 - number of vertices of the current test file
  /usr/bin/time -v --output="$1/mem_add_$1.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $1 1 1
  /usr/bin/time -v --output="$1/mem_add_background_$1.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $1 1 2
  /usr/bin/time -v --output="$1/mem_add_delay_$1.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $1 1 3
  /usr/bin/time -v --output="$1/mem_add_delay_munro_$1.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $1 1 4
}

declare -a X_time=()             #n+m
declare -a X_mem=()              #n+m
declare -a TIME_BACKGROUND=()    #time add_edge
declare -a MEMORY_BACKGROUND=()  #memory add_edge
declare -a TIME=()               #time add_edge
declare -a MEMORY=()             #memory add_edge
declare -a TIME_DELAY=()         #time add_edge
declare -a MEMORY_DELAY=()       #memory add_edge
declare -a TIME_DELAY_MUNRO=()   #time add_edge
declare -a MEMORY_DELAY_MUNRO=() #memory add_edge
i=0
prepared_data() {
  for vertices in $(ls $DATASETDIR | sort --version-sort); do
    edges=$(wc -l $DATASETDIR/$vertices/$vertices.tsv | awk '{ print $1 }')
    echo "l($vertices)/l(2)*l($edges)"
    X_time+=("$(echo "l($vertices)/l(2)*l($edges)" | bc -l)")
    X_mem+=("$(echo "$vertices + $edges" | bc)")

    TIME+=("$(cat "$vertices/$1_$RUNS_FILE")")
    MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_$vertices.txt")")

    TIME_BACKGROUND+=("$(cat "$vertices/$1_$RUNS_FILE_BACKGROUND")")
    MEMORY_BACKGROUND+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_background_$vertices.txt")")

    TIME_DELAY+=("$(cat "$vertices/$1_$RUNS_FILE_DELAY")")
    MEMORY_DELAY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_delay_$vertices.txt")")

    TIME_DELAY_MUNRO+=("$(cat "$vertices/$1_$RUNS_FILE_DELAY_MUNRO")")
    MEMORY_DELAY_MUNRO+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_delay_munro_$vertices.txt")")
    i=$((i + 1))
  done

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${X_time[${item}]} ${TIME_BACKGROUND[${item}]} ${MEMORY_BACKGROUND[${item}]} ${TIME[${item}]} ${MEMORY[${item}]} ${TIME_DELAY[${item}]} ${MEMORY_DELAY[${item}]} ${TIME_DELAY_MUNRO[${item}]} ${MEMORY_DELAY_MUNRO[${item}]} ${X_mem[${item}]}" >>$RUNS_DATA
  done
  i=$((i + 1))
}

plot_data_time() {
  MIN=${TIME_BACKGROUND[0]}
  if [[ $TYPE == "mean" ]]; then
    MIN=0.000001
  fi
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'add_($TYPE)_time.png'
  set title "Add edge $TYPE" font ",14" textcolor rgbcolor "royalblue"
  set xrange [${X_time[0]}:${X_time[${i}]}]
  set xlabel "log_k(n)log(m)"
  set yrange [$MIN:${TIME_DELAY_MUNRO[${i}]}]
  set ylabel "time"
  set key left
  plot "$RUNS_DATA" using 1:2 with linespoints t "add edge parallel",\
       "$RUNS_DATA" using 1:4 with linespoints t "add edge",\
       "$RUNS_DATA" using 1:6 with linespoints t "add edge delay",\
       "$RUNS_DATA" using 1:8 with linespoints t "add edge munro"
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'add_($TYPE)_mem.png'
  set rmargin at screen 0.90
  set title "Add edge Memory" font ",14" textcolor rgbcolor "royalblue"
  set xrange [0:${X_mem[${i}]}]
  set xlabel "n + m"
  set yrange [4300:${MEMORY[${i}]}]
  set ylabel "memory"
  set key left
  plot "$RUNS_DATA" using 10:3 with linespoints t "add edge parallel",\
       "$RUNS_DATA" using 10:5 with linespoints t "add edge",\
       "$RUNS_DATA" using 10:7 with linespoints t "add edge delay",\
       "$RUNS_DATA" using 10:9 with linespoints t "add edge munro"
EOF
}

if [[ $2 != "-plot" ]]; then
  echo "Compiling and cleaning..."
  make clean all
  for vertices in $(ls $DATASETDIR | sort --version-sort); do
    rm -r "$vertices"
  done

  echo "Evaluating..."
  for vertices in $(ls $DATASETDIR | sort --version-sort); do
    mkdir -p $vertices

    echo "running for $vertices"
    if [[ $1 == "-mean" ]]; then
      eval_time_mean $vertices
    fi

    if [[ $1 == "-deviation" ]]; then
      eval_time_deviation $vertices
    fi
    eval_memory $vertices
  done
fi

echo "Preparing data..."
prepared_data $TYPE

echo "Ploting time..."
plot_data_time

echo "Ploting memory..."
plot_data_mem

echo "Cleaning up..."
rm $RUNS_DATA
echo "Done!"
