#!/bin/bash
DATASETDIR="../../datasets/dmgen/prepared_datasets/dmgen"
RUNS=1
RUNS_FILE_BACKGROUND="runs_time_background.txt"
RUNS_FILE="runs_time.txt"
RUNS_FILE_DELAY="runs_time_delay.txt"
RUNS_DATA="runs.data"

eval_time() {
  #  $1 - number of vertices of the current test file
  for _ in $(seq 1 $RUNS); do
    ./add_background $DATASETDIR/$1/$1.tsv $1 >>"$1/$RUNS_FILE_BACKGROUND"
    ./add $DATASETDIR/$1/$1.tsv $1 >>"$1/$RUNS_FILE"
    ./add_delay $DATASETDIR/$1/$1.tsv $1 >>"$1/$RUNS_FILE_DELAY"
  done
}

eval_memory() {
  #  $1 - number of vertices of the current test file
  /usr/bin/time -v --output="$1/mem_add_background_$1.txt" ./add_background "$DATASETDIR/$1/$1.tsv" $1
  /usr/bin/time -v --output="$1/mem_add_$1.txt" ./add "$DATASETDIR/$1/$1.tsv" $1
  /usr/bin/time -v --output="$1/mem_add_delay_$1.txt" ./add_delay "$DATASETDIR/$1/$1.tsv" $1
}

declare -a X=() #n+m
declare -a TIME_BACKGROUND=() #time background
declare -a MEMORY_BACKGROUND=() #memory background
declare -a TIME=() #time background
declare -a MEMORY=() #memory background
declare -a TIME_DELAY=() #time background
declare -a MEMORY_DELAY=() #memory background
i=0
prepared_data() {
  for vertices in $(ls $DATASETDIR); do
    edges=$(wc -l $DATASETDIR/$vertices/$vertices.tsv | awk '{ print $1 }')
    X+=("$(echo "$vertices + $edges" | bc)")

    sum_background=0
    sum=0
    sum_delay=0

    while read line; do
      sum_background=$(echo "$sum_background + $line" | bc)
    done < "$vertices/$RUNS_FILE_BACKGROUND"

    while read line_2; do
      sum=$(echo "$sum + $line_2" | bc)
    done < "$vertices/$RUNS_FILE"

    while read line_3; do
      sum_delay=$(echo "$sum_delay + $line_3" | bc)
    done < "$vertices/$RUNS_FILE_DELAY"

    TIME_BACKGROUND+=("$(echo "scale=3; $sum_background / $RUNS" | bc)")
    TIME+=("$(echo "scale=3; $sum / $RUNS" | bc)")
    TIME_DELAY+=("$(echo "scale=3; $sum_delay / $RUNS" | bc)")
    MEMORY_BACKGROUND+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_background_$vertices.txt")")
    MEMORY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_$vertices.txt")")
    MEMORY_DELAY+=("$(grep -oP 'Maximum resident set size \(kbytes\): \K[0-9]+' "$vertices/mem_add_delay_$vertices.txt")")
    i=$((i+1))
  done

  i=$((i-1))
  for item in $(seq 0 $i); do
    echo "${X[${item}]} ${TIME_BACKGROUND[${item}]} ${MEMORY_BACKGROUND[${item}]} ${TIME[${item}]} ${MEMORY[${item}]} ${TIME_DELAY[${item}]} ${MEMORY_DELAY[${item}]}" >> $RUNS_DATA
  done
  i=$((i+1))
}

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'add_background_time.png'
  set title "Add edge Time" font ",14" textcolor rgbcolor "royalblue"
  set xrange [0:${X[${i}]}]
  set xlabel "n + m"
  set yrange [0:${TIME[${i}]}]
  set ylabel "time"
  plot "$RUNS_DATA" using 1:2 with linespoints title "add edge parallel",\
       "$RUNS_DATA" using 1:4 with linespoints title "add edge",\
       "$RUNS_DATA" using 1:6 with linespoints title "add edge delay"
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'add_background_mem.png'
  set title "Add edge Memory" font ",14" textcolor rgbcolor "royalblue"
  set xrange [0:${X[${i}]}]
  set xlabel "n + m"
  set yrange [4300:${MEMORY[${i}]}]
  set ylabel "memory"
  plot "$RUNS_DATA" using 1:3 with linespoints title "add edge parallel",\
       "$RUNS_DATA" using 1:5 with linespoints title "add edge",\
       "$RUNS_DATA" using 1:7 with linespoints title "add edge delay"
EOF
}

echo "Compiling..."
make clean add_background add add_delay
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
