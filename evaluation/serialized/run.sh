#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
TYPE="dmgen"

declare -a WEBGRAPH=("eu-2015-host")
declare -a WEBGRAPH_NODES=(11264052)
make --keep-going clean create
if [[ $1 != "-dmgen" && $1 != "-webgraph" && $1 != "" ]]; then
  echo "Usage: ./run.sh [-dmgen/webgraph]"
  exit
elif [[ $1 == "-webgraph" ]]; then
  DATASETDIR="../../datasets/webgraph"
  TYPE="webgraph"
fi

create_serialized_tree() { #[create_serialized_test $dataset $vertice]
  ./create "$DATASETDIR/$1/$1.tsv" "$2" "$1"
}

if [[ $TYPE == "dmgen" ]]; then
  for vertices in $(ls $DATASETDIR | sort --version-sort); do
    rm -r $vertices
  done

  for vertices in $(ls $DATASETDIR | sort --version-sort); do
    mkdir -p $vertices
    create_serialized_tree $vertices $vertices
  done
fi

if [[ $TYPE == "webgraph" ]]; then
  for dataset in "${WEBGRAPH[@]}"; do
    rm -r $dataset
  done

  k=0
  for dataset in "${WEBGRAPH[@]}"; do
    mkdir -p "$dataset"
    create_serialized_tree "$dataset" "${WEBGRAPH_NODES[${k}]}"
    k=$((k + 1))
  done
fi
