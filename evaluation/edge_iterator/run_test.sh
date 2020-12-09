#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
TYPE="dmgen"
RUNS_DATA="data"
RUNS=5
RUNS_FILE="time.txt"

declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host" "indochina-2004" "eu-2015-host")
declare -a WEBGRAPH_NODES=(100000 1382908 4769354 7414866 11264052)

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
  ./edge_iterator $RUNS "../serialized/$1" >>"$1/$RUNS_FILE"
}

eval_memory() {
  echo "Evaluating memory for $1..."
  rm "$1/mem.txt"
  /usr/bin/time -v --output="$1/mem.txt" ./edge_iterator 1 "../serialized/$1"
}
i=0
declare -a TIME=()
declare -a TIME_COMPLEXITY=()
declare -a MEMORY_COMPLEXITY=()
prepared_data_time() {
  echo "Preparing data time..."
  rm "$RUNS_DATA-time-$TYPE"
  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      TIME+=("$(cat "$dataset/$RUNS_FILE")")

      bo=$(./time "$DATASETDIR/$dataset/$dataset.tsv" "${WEBGRAPH[${i}]}")
      complexity=($(echo "$bo" | tr ' ' '\n'))
      TIME_COMPLEXITY+=("${complexity[0]}")
      MEMORY_COMPLEXITY+=("${complexity[1]}")
      i=$((i + 1))
    done

  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      TIME+=("$(cat "$vertices/$RUNS_FILE")")

      bo=$(./time "$DATASETDIR/$vertices/$vertices.tsv" "$vertices")
      complexity=($(echo "$bo" | tr ' ' '\n'))
      TIME_COMPLEXITY+=("${complexity[0]}")
      MEMORY_COMPLEXITY+=("${complexity[1]}")
      i=$((i + 1))
    done
  fi

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${TIME_COMPLEXITY[${item}]} ${TIME[${item}]}" >>"$RUNS_DATA-time-$TYPE"
  done
  i=$((i + 1))
}

i=0
declare -a MEMORY=()
prepared_data_memory() {
  echo "Preparing data memory..."
  rm "$RUNS_DATA-memory-$TYPE"

  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem.txt")")
      i=$((i + 1))
    done

  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem.txt")")
    done
  fi

  i=$((i - 1))
  for item in $(seq 0 $i); do
    echo "${MEMORY_COMPLEXITY[${item}]} ${MEMORY[${item}]}" >>"$RUNS_DATA-memory-$TYPE"
  done
  i=$((i + 1))
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal pdfcairo mono font "sans, 12"
  set datafile separator whitespace
  set title "Edge Iterator ($TYPE)"
  set grid
  set output 'edge_iterator_time_$TYPE.pdf'
  set xlabel "log_k n"
  set ylabel "t (µs)"
  plot "$DATA-$TYPE" using 1:2 with linespoints pt 8 notitle
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal pdfcairo mono font "sans, 12"
  set datafile separator whitespace
  set title "Edge Iterator ($TYPE)"
  set grid
  set output 'edge_iterator_mem_$TYPE.pdf'
  set xlabel "n+m (10^6)"
  set ylabel "memory (MB)"
  plot "$DATA-$TYPE" using 3:4 with linespoints pt 8 notitle
EOF
}

if [[ $2 != "-plot" ]]; then
  echo "Compiling..."
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
    prepared_data_memory
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

    prepared_data_memory
    plot_data_mem
  fi
fi