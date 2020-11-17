#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
RUNS=5
RUNS_FILE_BACKGROUND="runs_time_background.txt"
RUNS_FILE="runs_time.txt"
RUNS_FILE_DELAY="runs_time_delay.txt"
RUNS_FILE_DELAY_MUNRO="runs_time_delay_munro.txt"
RUNS_DATA="runs.data"

TYPE="dmgen"
declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host" "eu-2015-host")
declare -a WEBGRAPH_NODES=(100000 1382908 4769354 11264052)

if [[ $1 != "-webgraph" && ($1 != "-plot" || $2 != "-plot") && $1 != "-dmgen" ]]; then
  echo "Usage: ./run_test.sh [OPTIONAL: -plot]"
  echo "  -plot: just plot the current data without having to run everything again."
  exit
elif [[ $1 == "-webgraph" ]]; then
  TYPE="webgraph"
  DATASETDIR="../../datasets/webgraph"
fi

eval_time_mean() {
  echo "$DATASETDIR/$1/$1.tsv $2"
  echo "first"
  ./add_mean "$DATASETDIR/$1/$1.tsv" "$2" $RUNS 1 >>"$1/$RUNS_FILE"
  echo "second"
  ./add_mean "$DATASETDIR/$1/$1.tsv" "$2" $RUNS 2 >>"$1/$RUNS_FILE_BACKGROUND"
  echo "third"
  ./add_mean "$DATASETDIR/$1/$1.tsv" "$2" $RUNS 3 >>"$1/$RUNS_FILE_DELAY"
  echo "fourth"
  ./add_mean "$DATASETDIR/$1/$1.tsv" "$2" $RUNS 4 >>"$1/$RUNS_FILE_DELAY_MUNRO"
}

eval_memory() {
  #  $1 - number of vertices of the current test file
  /usr/bin/time -v --output="$1/mem_add.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 1
  /usr/bin/time -v --output="$1/mem_add_background.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 2
  /usr/bin/time -v --output="$1/mem_add_delay.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 3
  /usr/bin/time -v --output="$1/mem_add_delay_munro.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 4
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
  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      edges=$(wc -l "$DATASETDIR/$dataset/$dataset.tsv" | awk '{ print $1 }')

      X_time+=("$(echo "l(${WEBGRAPH_NODES[${i}]})/l(2)*l($edges)" | bc -l)")
      X_mem+=("$(echo "${WEBGRAPH_NODES[${i}]} + $edges" | bc)")

      TIME+=("$(cat "$dataset/$RUNS_FILE")")
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add.txt")")

      TIME_BACKGROUND+=("$(cat "$dataset/$RUNS_FILE_BACKGROUND")")
      MEMORY_BACKGROUND+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add_background.txt")")

      TIME_DELAY+=("$(cat "$dataset/$RUNS_FILE_DELAY")")
      MEMORY_DELAY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add_delay.txt")")

      TIME_DELAY_MUNRO+=("$(cat "$dataset/$RUNS_FILE_DELAY_MUNRO")")
      MEMORY_DELAY_MUNRO+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add_delay_munro.txt")")
      i=$((i + 1))
    done

  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      edges=$(wc -l "$DATASETDIR/$vertices/$vertices.tsv" | awk '{ print $1 }')

      X_time+=("$(echo "l($vertices)/l(2)*l($edges)" | bc -l)")
      X_mem+=("$(echo "$vertices + $edges" | bc)")

      TIME+=("$(cat "$vertices/$RUNS_FILE")")
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add.txt")")

      TIME_BACKGROUND+=("$(cat "$vertices/$RUNS_FILE_BACKGROUND")")
      MEMORY_BACKGROUND+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_background.txt")")

      TIME_DELAY+=("$(cat "$vertices/$RUNS_FILE_DELAY")")
      MEMORY_DELAY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_delay.txt")")

      TIME_DELAY_MUNRO+=("$(cat "$vertices/$RUNS_FILE_DELAY_MUNRO")")
      MEMORY_DELAY_MUNRO+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_delay_munro.txt")")
      i=$((i + 1))
    done
  fi

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${X_time[${item}]} ${TIME_BACKGROUND[${item}]} ${MEMORY_BACKGROUND[${item}]} ${TIME[${item}]} ${MEMORY[${item}]} ${TIME_DELAY[${item}]} ${MEMORY_DELAY[${item}]} ${TIME_DELAY_MUNRO[${item}]} ${MEMORY_DELAY_MUNRO[${item}]} ${X_mem[${item}]}" >>$RUNS_DATA
  done
  i=$((i + 1))
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal pngcairo
  set datafile separator whitespace
  set output 'add_time_$TYPE.png'
  set xlabel "log_k(n)log(m)"
  set ylabel "Time (s)"
  set key left
  plot "$RUNS_DATA" using 1:2 with linespoints t "add edge parallel",\
       "$RUNS_DATA" using 1:4 with linespoints t "add edge",\
       "$RUNS_DATA" using 1:6 with linespoints t "add edge delay",\
       "$RUNS_DATA" using 1:8 with linespoints t "add edge munro"
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal pngcairo
  set datafile separator whitespace
  set output 'add_mem_$TYPE.png'
  set rmargin at screen 0.90
  set xlabel "n + m"
  set ylabel "Memory (kbytes)"
  set key left
  plot "$RUNS_DATA" using 10:3 with linespoints t "add edge parallel",\
       "$RUNS_DATA" using 10:5 with linespoints t "add edge",\
       "$RUNS_DATA" using 10:7 with linespoints t "add edge delay",\
       "$RUNS_DATA" using 10:9 with linespoints t "add edge munro"
EOF
}

if [[ $2 != "-plot" ]]; then
  echo "Compiling and cleaning..."
  make --keep-going clean all

  if [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      rm -r "$vertices"
    done

    echo "Evaluating..."
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      mkdir -p $vertices

      echo "running for $vertices"
      eval_time_mean $vertices $vertices
      eval_memory $vertices $vertices
    done
  fi

  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      echo "deleting $dataset..."
      rm -r $dataset
    done

    echo "Evaluating..."
    k=0
    for dataset in "${WEBGRAPH[@]}"; do
      mkdir -p "$dataset"

      echo "running for $dataset ${WEBGRAPH_NODES[${k}]}"
      eval_time_mean "$dataset" "${WEBGRAPH_NODES[${k}]}"
      eval_memory "$dataset" "${WEBGRAPH_NODES[${k}]}"
      k=$((k + 1))
    done
  fi
fi

echo "Preparing data..."
prepared_data

echo "Ploting time..."
plot_data_time

echo "Ploting memory..."
plot_data_mem

echo "Cleaning up..."
rm $RUNS_DATA
echo "Done!"
