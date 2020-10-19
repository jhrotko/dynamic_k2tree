#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
RUNS=5
RUNS_FILE="runs_time.txt"
RUNS_DATA="runs.data"

eval_time() {
  #  $1 - number of vertices of the current test file
  for _ in $(seq 1 $RUNS); do
    ./add_background $DATASETDIR/$1/$1.tsv $1 >>"$1/$RUNS_FILE"
  done
}

eval_memory() {
  #  $1 - number of vertices of the current test file
  /usr/bin/time -v --output="$1/mem_add_background_$1.txt" ./add_background "$DATASETDIR/$1/$1.tsv" $1
}

declare -a X=() #time
declare -a Y=() #n+m
declare -a Z=() #memory
i=0
prepared_data() {
  for vertices in $(ls $DATASETDIR); do
    sum=0
    while read line; do
      sum=$(echo "$sum + $line" | bc)
    done < "$vertices/$RUNS_FILE"
    X+=("$(echo "scale=3; $sum / $RUNS" | bc)")
    edges=$(wc -l $DATASETDIR/$vertices/$vertices.tsv | awk '{ print $1 }')
    Y+=("$(echo "$vertices + $edges" | bc)")
    Z+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_background_$vertices.txt")")
    i=$((i+1))
  done

  i=$((i-1))
  for item in $(seq 0 $i); do
    echo "${X[${item}]} ${Y[${item}]} ${Z[${item}]}" >> $RUNS_DATA
  done
  i=$((i+1))
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'add_background_time.png'
  set title "Add edge (Background) Time" font ",14" textcolor rgbcolor "royalblue"
  set xrange [0:${Y[${i}]}]
  set xlabel "n + m"
  set yrange [0:${X[${i}]}]
  set ylabel "time"
  plot "$RUNS_DATA" using 2:1 with linespoints title "add edge background"
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'add_background_mem.png'
  set title "Add edge (Background) Memory" font ",14" textcolor rgbcolor "royalblue"
  set xrange [0:${Y[${i}]}]
  set xlabel "n + m"
  set yrange [4300:${Z[${i}]}]
  set ylabel "memory"
  plot "$RUNS_DATA" using 2:3 with linespoints title "add edge background"
EOF
}

echo "Compiling..."
make clean add_background
for vertices in $(ls $DATASETDIR); do
  rm -r "$vertices"
done

echo "Evaluating..."
for vertices in $(ls $DATASETDIR); do
  mkdir -p $vertices

  eval_time $vertices
  eval_memory $vertices
done

echo "Preparing data..."
prepared_data
cat $RUNS_DATA

echo "Ploting time..."
plot_data_time

echo "Ploting memory..."
plot_data_mem

echo "Cleaning up..."
rm $RUNS_DATA
echo "Done!"
