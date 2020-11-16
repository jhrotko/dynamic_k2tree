#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
TYPE="dmgen"
DATA="time.data"
SCRIPT=""
RUNS=1

declare -a WEBGRAPH=("uk-2007-05@100000" "in-2004" "uk-2014-host")
declare -a WEBGRAPH_NODES=(100000 1382908 4769354)
#declare -a WEBGRAPH=("eu-2015-host")
#declare -a WEBGRAPH_NODES=(11264052)

if [[ $1 != "-dmgen" && $1 != "-webgraph" && $1 != "" ]]; then
  echo "Usage: ./run_test.sh [-dmgen/webgraph]"
  exit
elif [[ $1 == "-webgraph" ]]; then
  DATASETDIR="../../datasets/webgraph"
  TYPE="webgraph"
fi
SCRIPT=${2:1}

create_serialized_tree() { #[create_serialized_test $dataset $vertice]
  ./create "$DATASETDIR/$1/$1.tsv" "$2" "$1/test.kt"
}

declare NODES
declare ARCS
get_arcs_nodes_dmgen() {
  INFO=("$(grep -oP 'main: graph: \K[0-9 ]+' "$DATASETDIR/$1/$1.dmgen.time")")
  NODES=$(echo "$INFO" | cut -d' ' -f1)
  ARCS=$(echo "$INFO" | cut -d' ' -f2)
}
get_arcs_nodes_webgraph() {
  NODES=("$(grep -oP 'nodes=\K[0-9]+' "$DATASETDIR/$1/$1.properties")")
  ARCS=("$(grep -oP 'arcs=\K[0-9]+' "$DATASETDIR/$1/$1.properties")")
}

declare -a TIME=()
declare -a TIME_COMPLEXITY=()
eval_time() {
  command="./$SCRIPT $1/test.kt $1/test.kt $RUNS"
  TIME+=( "$(eval $command)" )
  TIME_COMPLEXITY+=("$(./complexity $SCRIPT "time" $NODES $ARCS)")
  echo "time $TIME"
#  echo "complexity $TIME_COMPLEXITY"
}

declare -a MEMORY=()
declare -a VERTICES=()
eval_memory() {
  #  valgrind --tool=massif --massif-out-file=$1/$1.union.mem.out ./union $1/test.kt $1/test.kt
  /usr/bin/time -v --output="$1/mem_union.txt" ./"$SCRIPT" "$1/test.kt" "$1/test.kt" 1
  MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$1/mem_union.txt")")
  VERTICES+=("$2")
  #  valgrind --tool=massif --massif-out-file=$vertices/$vertices.union.mem.out ./union $vertices/test.kt $vertices/test.kt
}

i=0
prepared_data() {
  for el in "${MEMORY[@]}"; do
    echo "${TIME[${i}]} ${MEMORY[${i}]} ${VERTICES[${i}]} ${TIME_COMPLEXITY[${i}]}"
    echo "${TIME[${i}]} ${MEMORY[${i}]} ${VERTICES[${i}]} ${TIME_COMPLEXITY[${i}]}" >>$DATA
    i=$((i + 1))
  done
  i=$((i - 1))
  echo "${TIME[${i}]} ${MEMORY[${i}]} ${VERTICES[${i}]} ${TIME_COMPLEXITY[${i}]}" >>"max.data"
}

#echo "Compiling..."
make --keep-going clean all
mkdir -p "$SCRIPT-test"
mv "$SCRIPT" "$SCRIPT-test"
mv "create" "$SCRIPT-test"
cp "complexity" "$SCRIPT-test"
cd "$SCRIPT-test" || exit

echo "Evaluating..."
if [[ $TYPE == "dmgen" ]]; then
  for vertices in $(ls $DATASETDIR | sort --version-sort); do
    rm -r $vertices
  done

  for vertices in $(ls $DATASETDIR | sort --version-sort); do
    mkdir -p $vertices
    create_serialized_tree $vertices $vertices

    get_arcs_nodes_dmgen $vertices
    eval_time $vertices
    eval_memory $vertices $vertices
  done

  #PLOT data
  echo "Preparing data..."
  prepared_data
fi

if [[ $TYPE == "webgraph" ]]; then
  for dataset in "${WEBGRAPH[@]}"; do
    rm -r $dataset
  done

  k=0
  for dataset in "${WEBGRAPH[@]}"; do
    mkdir -p "$dataset"

    create_serialized_tree "$dataset" "${WEBGRAPH_NODES[${k}]}"
    eval_time "$dataset"
    eval_memory "$dataset" "${WEBGRAPH_NODES[${k}]}"
    k=$((k + 1))
  done

  echo "Preparing data..."
  prepared_data
fi
