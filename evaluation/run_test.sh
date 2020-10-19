#!/bin/bash
DATASETDIR="../datasets/dmgen/prepared_datasets/dmgen"
UNION_FILE="time_union.txt"
UNION_DATA="time_union.data"

create_serialized_tree() {
  #  $1 - number of vertices of the current test file
  ./create $DATASETDIR/$1/$1.tsv $1 $1/test.kt >>$UNION_FILE
}

eval_time_union() {
  #  time analisys -  run $RUN times and save it in array file
  ./union $1/test.kt $1/test.kt >>$UNION_FILE
}

eval_memory_union() {
  #  $1 - number of vertices of the current test file
#  valgrind --tool=massif --massif-out-file=$1/$1.union.mem.out ./union $1/test.kt $1/test.kt
  /usr/bin/time -v --output=$1/mem_union_$1.txt ./union $1/test.kt $1/test.kt
  #  valgrind --tool=massif --massif-out-file=$vertices/$vertices.union.mem.out ./union $vertices/test.kt $vertices/test.kt
}

x=0
y=0
i=0
declare -a Z=()
prepared_data() {
  n=0
  for vertices in $(ls $DATASETDIR); do
      Z+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' $vertices/mem_union_$vertices.txt)")
  done
  while read line; do
    n=$((n + 1))
    if ((n % 2)); then
      x=$line
    else
      y=$line
      echo "$x $y ${Z[${i}]}" >> $UNION_DATA
      i=$((i+1))
    fi
  done < $UNION_FILE
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'union_time.png'
  set title "Union Time" font ",14" textcolor rgbcolor "royalblue"
  set xrange [0:$x]
  set yrange [0:$y]
  set xlabel "n + m"
  set ylabel "time"
  plot "$UNION_DATA" using 1:2 with linespoints title "union operation"
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'union_mem.png'
  set title "Union Memory" font ",14" textcolor rgbcolor "royalblue"
  set xrange [0:$x]
  set yrange [3000:${Z[${i}]}]
  set xlabel "n + m"
  set ylabel "memory"
  plot "$UNION_DATA" using 1:3 with linespoints title "union operation"
EOF
}

echo "Compiling..."
make clean create union
echo "Evaluating..."
for vertices in $(ls $DATASETDIR); do
#for vertices in "${VERTICES[@]}"; do
  mkdir -p $vertices
  create_serialized_tree $vertices

  #TIME analysis
  eval_time_union $vertices
  #MEMORY analysis
  eval_memory_union $vertices
done
#PLOT data
echo "Preparing data..."
prepared_data

echo "Ploting time..."
plot_data_time

echo "Ploting memory..."
plot_data_mem

echo "Cleaning up..."
rm $UNION_DATA
rm $UNION_FILE
echo "Done!"