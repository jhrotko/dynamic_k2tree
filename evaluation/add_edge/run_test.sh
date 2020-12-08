#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
RUNS=5
RUNS_FILE_BACKGROUND="runs_time_background.txt"
RUNS_FILE_BACKGROUND_WAIT="runs_time_background_wait.txt"
RUNS_FILE="runs_time.txt"
RUNS_FILE_DELAY="runs_time_delay.txt"
RUNS_FILE_DELAY_MUNRO="runs_time_delay_munro.txt"
RUNS_DATA="runs-data"

TYPE="dmgen"
#declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host")
#declare -a WEBGRAPH_NODES=(100000 1382908 4769354)
declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host" "indochina-2004" "eu-2015-host")
declare -a WEBGRAPH_NODES=(100000 1382908 4769354 7414866 11264052)

if [[ $1 != "-webgraph" && ($1 != "-plot" || $2 != "-plot") && $1 != "-dmgen" ]]; then
  echo "Usage: ./run_test.sh [OPTIONAL: -plot]"
  echo "  -plot: just plot the current data without having to run everything again."
  exit
elif [[ $1 == "-webgraph" ]]; then
  TYPE="webgraph"
  DATASETDIR="../../datasets/webgraph"
fi

eval_time() {
  echo "Evaluating time for $1..."

  echo "normal"
  rm "$1/$RUNS_FILE"
  ./add_mean "$DATASETDIR/$1/$1.tsv" $2 $RUNS 1 >>"$1/$RUNS_FILE"
  echo "delay"
  rm "$1/$RUNS_FILE_DELAY"
  ./add_mean "$DATASETDIR/$1/$1.tsv" $2 $RUNS 3 >>"$1/$RUNS_FILE_DELAY"
  echo "delay munro"
  rm "$1/$RUNS_FILE_DELAY_MUNRO"
  ./add_mean "$DATASETDIR/$1/$1.tsv" $2 $RUNS 4 >>"$1/$RUNS_FILE_DELAY_MUNRO"
  echo "background"
  rm "$1/$RUNS_FILE_BACKGROUND"
  ./add_mean "$DATASETDIR/$1/$1.tsv" $2 $RUNS 2 >>"$1/$RUNS_FILE_BACKGROUND"
  sleep 5
  echo "background wait"
  rm "$1/$RUNS_FILE_BACKGROUND_WAIT"
  ./add_mean "$DATASETDIR/$1/$1.tsv" $2 $RUNS 5 >>"$1/$RUNS_FILE_BACKGROUND_WAIT"
  sleep 5
}
eval_memory() {
  echo "Evaluating memory for $1..."

  #  $1 - number of vertices of the current test file
  echo "normal"
  rm "$1/mem_add.txt"
  /usr/bin/time -v --output="$1/mem_add.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 1
  echo "background"
  rm "$1/mem_add_background.txt"
  /usr/bin/time -v --output="$1/mem_add_background.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 2
  echo "delay"
  rm "$1/mem_add_delay.txt"
  /usr/bin/time -v --output="$1/mem_add_delay.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 3
  echo "delay munro"
  rm "$1/mem_add_delay_munro.txt"
  /usr/bin/time -v --output="$1/mem_add_delay_munro.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 4
  echo "background wait"
  rm "$1/mem_add_background_wait.txt"
  /usr/bin/time -v --output="$1/mem_add_background_wait.txt" ./add_mean "$DATASETDIR/$1/$1.tsv" $2 1 5
}

declare -a X_time=()               #n+m
declare -a TIME_BACKGROUND=()      #time add_edge
declare -a TIME_BACKGROUND_WAIT=() #time add_edge
declare -a TIME=()                 #time add_edge
declare -a TIME_DELAY=()           #time add_edge
declare -a TIME_DELAY_MUNRO=()     #time add_edge
i=0
prepared_data_time() {
  rm "$RUNS_DATA-time-$TYPE"
  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      edges=$(wc -l "$DATASETDIR/$dataset/$dataset.tsv" | awk '{ print $1 }')
      echo "${WEBGRAPH_NODES[${i}]} $edges" >>"properties-$TYPE.txt"

      X_time+=("$(echo "l(${WEBGRAPH_NODES[${i}]})/l(2)*l($edges)" | bc -l)")
      TIME+=("$(cat "$dataset/$RUNS_FILE")")
      TIME_BACKGROUND+=("$(cat "$dataset/$RUNS_FILE_BACKGROUND")")
      TIME_BACKGROUND_WAIT+=("$(cat "$dataset/$RUNS_FILE_BACKGROUND_WAIT")")
      TIME_DELAY+=("$(cat "$dataset/$RUNS_FILE_DELAY")")
      TIME_DELAY_MUNRO+=("$(cat "$dataset/$RUNS_FILE_DELAY_MUNRO")")
      i=$((i + 1))
    done

  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      edges=$(wc -l "$DATASETDIR/$vertices/$vertices.tsv" | awk '{ print $1 }')
      X_time+=("$(echo "l($vertices)/l(2)*l($edges)" | bc -l)")
      echo "$vertices $edges" >>"properties-$TYPE.txt"
      TIME+=("$(cat "$vertices/$RUNS_FILE")")
      TIME_BACKGROUND+=("$(cat "$vertices/$RUNS_FILE_BACKGROUND")")
      TIME_BACKGROUND_WAIT+=("$(cat "$vertices/$RUNS_FILE_BACKGROUND_WAIT")")
      TIME_DELAY+=("$(cat "$vertices/$RUNS_FILE_DELAY")")
      TIME_DELAY_MUNRO+=("$(cat "$vertices/$RUNS_FILE_DELAY_MUNRO")")
      i=$((i + 1))
    done
  fi

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${X_time[${item}]} ${TIME[${item}]} ${TIME_BACKGROUND[${item}]} ${TIME_DELAY[${item}]} ${TIME_DELAY_MUNRO[${item}]} ${TIME_BACKGROUND_WAIT[${item}]}" >>"$RUNS_DATA-time-$TYPE"
  done
  i=$((i + 1))
}
declare -a X_mem=()                  #n+m
declare -a MEMORY_BACKGROUND=()      #memory add_edge
declare -a MEMORY_BACKGROUND_WAIT=() #memory add_edge
declare -a MEMORY=()                 #memory add_edge
declare -a MEMORY_DELAY=()           #memory add_edge
declare -a MEMORY_DELAY_MUNRO=()     #memory add_edge

i=0
prepared_data_mem() {
  rm "$RUNS_DATA-memory-$TYPE"
  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      edges=$(wc -l "$DATASETDIR/$dataset/$dataset.tsv" | awk '{ print $1 }')
      X_mem+=("$(echo "${WEBGRAPH_NODES[${i}]} + $edges" | bc)")
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add.txt")")
      MEMORY_BACKGROUND+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add_background.txt")")
      MEMORY_BACKGROUND_WAIT+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add_background_wait.txt")")
      MEMORY_DELAY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add_delay.txt")")
      MEMORY_DELAY_MUNRO+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem_add_delay_munro.txt")")
      i=$((i + 1))
    done

  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      edges=$(wc -l "$DATASETDIR/$vertices/$vertices.tsv" | awk '{ print $1 }')
      X_mem+=("$(echo "$vertices + $edges" | bc)")
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add.txt")")
      MEMORY_BACKGROUND+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_background.txt")")
      MEMORY_BACKGROUND_WAIT+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_background_wait.txt")")
      MEMORY_DELAY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_delay.txt")")
      MEMORY_DELAY_MUNRO+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_delay_munro.txt")")
      i=$((i + 1))
    done
  fi

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${X_mem[${item}]} ${MEMORY_BACKGROUND[${item}]} ${MEMORY[${item}]} ${MEMORY_DELAY[${item}]} ${MEMORY_DELAY_MUNRO[${item}]} ${MEMORY_BACKGROUND_WAIT[${item}]}" >>"$RUNS_DATA-memory-$TYPE"
  done
  i=$((i + 1))
}

plot_data_time() {
  echo "Ploting time..."
  gnuplot -persist <<-EOF
  set terminal pngcairo
  set datafile separator whitespace
  set output 'add_time_$TYPE.png'
  set title "Add Edge ($TYPE)"
  set grid
  set xlabel "log_k(n)log(m)"
  set ylabel "t (µs)"
  set key left
  plot "$RUNS_DATA-time-$TYPE" using 1:2 with linespoints t "add edge",\
       "$RUNS_DATA-time-$TYPE" using 1:3 with linespoints t "add edge parallel",\
       "$RUNS_DATA-time-$TYPE" using 1:6 with linespoints t "add edge parallel wait",\
       "$RUNS_DATA-time-$TYPE" using 1:4 with linespoints t "add edge delay",\
       "$RUNS_DATA-time-$TYPE" using 1:5 with linespoints t "add edge munro"
EOF
}

#    echo "${X_mem[${item}]} ${MEMORY_BACKGROUND[${item}]} ${MEMORY[${item}]} ${MEMORY_DELAY[${item}]} ${MEMORY_DELAY_MUNRO[${item}]} ${MEMORY_BACKGROUND_WAIT[${item}]}" >>"$RUNS_DATA-memory-$TYPE"
plot_data_mem() {
  echo "Ploting memory..."
  gnuplot -persist <<-EOF
    set terminal pngcairo
  set datafile separator whitespace
  set output 'add_mem_$TYPE.png'
  set title "Add Edge ($TYPE)"
  set grid
  set rmargin at screen 0.90
  set xlabel "n+m (10^6)"
  set ylabel "memory (MB)"
  set key left
  plot "$RUNS_DATA-memory-$TYPE" using 1:3 with linespoints t "add edge",\
        "$RUNS_DATA-memory-$TYPE" using 1:2 with linespoints t "add edge parallel",\
        "$RUNS_DATA-memory-$TYPE" using 1:6 with linespoints t "add edge parallel wait",\
        "$RUNS_DATA-memory-$TYPE" using 1:4 with linespoints t "add edge delay",\
        "$RUNS_DATA-memory-$TYPE" using 1:5 with linespoints t "add edge munro"
EOF
}

if [[ $2 != "-plot" ]]; then
  echo "Compiling and cleaning..."
  #  rm $RUNS_DATA-$TYPE
  make --keep-going clean all

  if [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      mkdir -p $vertices
      eval_time $vertices $vertices
    done
    prepared_data_time
    plot_data_time

    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      eval_memory $vertices $vertices
    done
    prepared_data_mem
    plot_data_mem
  fi

  if [[ $TYPE == "webgraph" ]]; then
    k=0
    for dataset in "${WEBGRAPH[@]}"; do
      mkdir -p "$dataset"
      eval_time "$dataset" "${WEBGRAPH_NODES[${k}]}"
      k=$((k + 1))
    done
    prepared_data_time
    plot_data_time

    k=0
    for dataset in "${WEBGRAPH[@]}"; do
      eval_memory "$dataset" "${WEBGRAPH_NODES[${k}]}"
      k=$((k + 1))
    done
    prepared_data_mem
    plot_data_mem
  fi

  gnuplot add_edge.gnuplot
fi