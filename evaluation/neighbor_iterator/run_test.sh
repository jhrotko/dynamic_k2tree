#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
TYPE="dmgen"
RUNS_DATA="data"
RUNS=3
RUNS_FILE="time.txt"

declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host" "indochina-2004")
declare -a WEBGRAPH_NODES=(100000 1382908 4769354 7414866)

if [[ $1 != "-dmgen" && $1 != "-webgraph" && $1 != "" ]]; then
  echo "Usage: ./run_test.sh [-dmgen/webgraph]"
  exit
elif [[ $1 == "-webgraph" ]]; then
  DATASETDIR="../../datasets/webgraph"
  TYPE="webgraph"
fi

eval_time() {
  echo "Evaluating time for $1..."
  rm "$1/$RUNS_FILE"
  rm "$1/$RUNS_FILE-list"
  ./neighbor_iterator "$DATASETDIR/$1/$1-lists.tsv" $RUNS "../serialized/$1" 1 >>"$1/$RUNS_FILE"
  ./neighbor_iterator "$DATASETDIR/$1/$1-lists.tsv" $RUNS "../serialized/$1" 2 >>"$1/$RUNS_FILE-list"
}

eval_memory() {
  echo "Evaluating memory for $1..."
  rm "$1/mem.txt"
  rm "$1/mem.list.txt"
  /usr/bin/time -v --output="$1/mem.txt" ./neighbor_iterator "$DATASETDIR/$1/$1-lists.tsv" 1 "../serialized/$1" 1
  /usr/bin/time -v --output="$1/mem-list.txt" ./neighbor_iterator "$DATASETDIR/$1/$1-lists.tsv" 1 "../serialized/$1" 2
}
i=0
declare -a TIME=()
declare -a TIME_LIST=()
declare -a TIME_COMPLEXITY=()
declare -a MEMORY_COMPLEXITY=()
prepared_data_time() {
  echo "Preparing data time..."
  rm "$RUNS_DATA-time-$TYPE"
  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      TIME+=("$(cat "$dataset/$RUNS_FILE")")
      TIME_LIST+=("$(cat "$dataset/$RUNS_FILE-list")")

      bo=$(./time "$DATASETDIR/$dataset/$dataset.tsv" "${WEBGRAPH[${i}]}")
      complexity=($(echo "$bo" | tr ' ' '\n'))
      TIME_COMPLEXITY+=("${complexity[0]}")
      MEMORY_COMPLEXITY+=("${complexity[1]}")
      i=$((i + 1))
    done

  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      TIME+=("$(cat "$vertices/$RUNS_FILE")")
      TIME_LIST+=("$(cat "$vertices/$RUNS_FILE-list")")

      bo=$(./time "$DATASETDIR/$vertices/$vertices.tsv" "$vertices")
      complexity=($(echo "$bo" | tr ' ' '\n'))
      TIME_COMPLEXITY+=("${complexity[0]}")
      MEMORY_COMPLEXITY+=("${complexity[1]}")
      i=$((i + 1))
    done
  fi

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${TIME_COMPLEXITY[${item}]} ${TIME[${item}]} ${TIME_LIST[${item}]}" >>"$RUNS_DATA-time-$TYPE"
  done
  i=$((i + 1))
}

i=0
declare -a MEMORY=()
declare -a MEMORY_LIST=()
prepared_data_memory() {
  echo "Preparing data memory..."
  rm "$RUNS_DATA-memory-$TYPE"

  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem.txt")")
      MEMORY_LIST+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem-list.txt")")
      i=$((i + 1))
    done

  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem.txt")")
      MEMORY_LIST+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem-list.txt")")
    done
  fi

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${MEMORY_COMPLEXITY[${item}]} ${MEMORY[${item}]} ${MEMORY_LIST[${item}]}" >>"$RUNS_DATA-memory-$TYPE"
  done
  i=$((i + 1))
}

plot_data_time() {
  echo "Ploting time..."
  gnuplot -persist <<-EOF
  set terminal pdfcairo mono font "sans, 12"
  set datafile separator whitespace
  set title "Neighbor Iterator ($TYPE)"
  set grid
  set output 'neighbor_iterator_time_$TYPE.pdf'
  set xlabel '√m'
  set ylabel "t (µs)"
  plot "$RUNS_DATA-time-$TYPE" using 1:2 with linespoints pt 8 t "iterator" ,\
       "$RUNS_DATA-time-$TYPE" using 1:3 with linespoints pt 8 t "list"
EOF
}

plot_data_mem() {
  echo "Ploting memory..."
  gnuplot -persist <<-EOF
  set terminal pdfcairo mono font "sans, 12"
  set datafile separator whitespace
  set output 'neighbor_iterator_mem_$TYPE.pdf'
  set title "Neighbor Iterator ($TYPE)"
  set grid
  set xlabel "n+m"
  set ylabel "memory (MB)"
  plot "$RUNS_DATA-memory-$TYPE" using 1:2 with linespoints pt 8 t "iterator",\
       "$RUNS_DATA-memory-$TYPE" using 1:3 with linespoints pt 8 t "list"
EOF
}

if [[ $2 != "-plot" ]]; then
  echo "Compiling..."
  make --keep-going clean all

  if [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      mkdir -p $vertices
      eval_time $vertices
    done
    prepared_data_time
    plot_data_time

    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      eval_memory $vertices
    done
    prepared_data_memory
    plot_data_mem
  fi

  if [[ $TYPE == "webgraph" ]]; then
    k=0
    for dataset in "${WEBGRAPH[@]}"; do
      mkdir -p "$dataset"
      eval_time "$dataset"
      k=$((k + 1))
    done
    prepared_data_time
    plot_data_time
    k=0
    for dataset in "${WEBGRAPH[@]}"; do
      eval_memory "$dataset"
      k=$((k + 1))
    done

    prepared_data_memory
    plot_data_mem
  fi
fi

