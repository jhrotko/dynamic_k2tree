#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
TYPE="dmgen"
DATA="time-data"
RUNS_FILE="time_run.txt"
RUNS=5

declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host" "indochina-2004" "eu-2015-host")
declare -a WEBGRAPH_NODES=(100000 1382908 4769354 7414866 11264052)
#declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host")
#declare -a WEBGRAPH_NODES=(100000 1382908 4769354)
#declare -a WEBGRAPH=("eu-2015-host")
#declare -a WEBGRAPH_NODES=(11264052)

if [[ $1 != "-dmgen" && $1 != "-webgraph" && $1 != "" ]]; then
  echo "Usage: ./run_test.sh [-dmgen/webgraph]"
  exit
elif [[ $1 == "-webgraph" ]]; then
  DATASETDIR="../../datasets/webgraph"
  TYPE="webgraph"
fi

declare -a TIME_COMPLEXITY=()
declare -a MEMORY_COMPLEXITY=()
time_complexity() { #[create_serialized_test $dataset $vertice]
  echo "$1"
  ./time "$DATASETDIR/$1/$1.tsv" "$2">>"$1/time_complexity.txt"
#  bo=$(<"$1/time_complexity.txt")
#  complexity=($(echo "$bo" | tr ' ' '\n'))
#  TIME_COMPLEXITY+=("${complexity[0]}")
#  MEMORY_COMPLEXITY+=("${complexity[1]}")
}

eval_memory_union() {
  /usr/bin/time -v --output="$1/mem.txt" ./pagerank "../serialized/$1" "$RUNS" >>"$1/$RUNS_FILE"
}

i=0
declare -a TIME=()
declare -a MEMORY=()
prepared_data() {
  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      TIME+=("$(cat "$dataset/$RUNS_FILE")")
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$dataset/mem.txt")")
      bo=$(<"$dataset/time_complexity.txt")
      complexity=($(echo "$bo" | tr ' ' '\n'))
      TIME_COMPLEXITY+=("${complexity[0]}")
      MEMORY_COMPLEXITY+=("${complexity[1]}")
    done
  elif [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
#      TIME+=("$(cat "$vertices/$RUNS_FILE")")
      a="$(<$vertices/$RUNS_FILE)"
      TIME+=( "$a" )
      MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem.txt")")
      bo=$(<"$vertices/time_complexity.txt")
      complexity=($(echo "$bo" | tr ' ' '\n'))
      TIME_COMPLEXITY+=("${complexity[0]}")
      MEMORY_COMPLEXITY+=("${complexity[1]}")
    done
  fi

  for el in "${TIME_COMPLEXITY[@]}"; do
    echo "${TIME[${i}]}"
    echo "$el ${TIME[${i}]} ${MEMORY_COMPLEXITY[${i}]} ${MEMORY[${i}]}">>"$DATA-$TYPE"
    i=$((i + 1))
  done
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal pdfcairo mono font "sans, 16"
  set datafile separator whitespace
  set output 'pagerank_time_$TYPE.pdf'
  set grid
  set title "PageRank ($TYPE)"
  set xlabel "n √m + m"
  set ylabel "t (s)"
  set style data linespoints
  plot "$DATA-$TYPE" using 1:2 pt 4 notitle
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal pdfcairo mono font "sans, 16"
  set datafile separator whitespace
  set output 'pagerank_mem_$TYPE.pdf'
  set grid
  set title "PageRank ($TYPE)"
  set xlabel "n+m"
  set ylabel "memory (MB)"
  set style data linespoints
  plot "$DATA-$TYPE" using 3:4 pt 4 notitle
EOF
}

if [[ $2 != "-plot" ]]; then
  echo "Compiling..."
  make --keep-going clean all

  echo "Cleaning up..."
  rm "$DATA-$TYPE"

  echo "Evaluating..."
  if [[ $TYPE == "dmgen" ]]; then
    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      rm "$vertices/mem.txt"
      rm "$vertices/$RUNS_FILE"
      rm "$vertices/time_complexity.txt"
    done

    for vertices in $(ls $DATASETDIR | sort --version-sort); do
      mkdir -p $vertices
      time_complexity $vertices $vertices
      eval_memory_union $vertices
    done
  fi

  if [[ $TYPE == "webgraph" ]]; then
    for dataset in "${WEBGRAPH[@]}"; do
      rm "$dataset/mem.txt"
      rm "$dataset/$RUNS_FILE"
      rm "$dataset/time_complexity.txt"
    done

    k=0
    for dataset in "${WEBGRAPH[@]}"; do
      mkdir -p "$dataset"

      time_complexity "$dataset" "${WEBGRAPH_NODES[${k}]}"
      eval_memory_union "$dataset"
      k=$((k + 1))
    done
  fi
fi
#PLOT data
echo "Preparing data..."
prepared_data

echo "Ploting time..."
plot_data_time

echo "Ploting memory..."
plot_data_mem
