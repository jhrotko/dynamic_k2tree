DATA="time.data"
TYPE="dmgen"

if [[ $1 != "-dmgen" && $1 != "-webgraph" && $1 != "" ]]; then
  echo "Usage: ./run_test.sh [-dmgen/webgraph]"
  exit
elif [[ $1 == "-webgraph" ]]; then
  TYPE="webgraph"
fi
SCRIPT=${2:1}

XLABEL_TIME=""
XLABEL_MEM=""
if [[ $SCRIPT == "delete" ]]; then
  XLABEL_TIME="log_k(n)/\epsilon + log_k(n)log(log(m))"
  XLABEL_MEM="n + m"
elif [[ $SCRIPT == "union" ]]; then
  XLABEL_TIME="n + m"
  XLABEL_MEM="n"
fi

cd "$SCRIPT-test" || exit
declare -a $MAX
while read line; do
  MAX=( "$line" )
  done < "max.data"

plot_data_time() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'union_time_$SCRIPT_$TYPE.png'
  set xrange [0:${MAX[3]}]
  set yrange [0:${MAX[0]}]
  set xlabel "$XLABEL_TIME"
  set ylabel "Time (s)"
  plot "$DATA" using 4:1 with linespoints title "delete operation"
EOF
}

plot_data_mem() {
  gnuplot -persist <<-EOF
  set terminal png
  set datafile separator whitespace
  set output 'union_mem_$SCRIPT_$TYPE.png'
  set xrange [0:${MAX[2]}]
  set yrange [0:${MAX[1]}]
  set xlabel "$XLABEL_MEM"
  set ylabel "Memory (kbytes)"
  plot "$DATA" using 3:2 with linespoints title "delete operation"
EOF
}

echo "Ploting time..."
plot_data_time

echo "Ploting memory..."
plot_data_mem

if [[ $2 == "clean" ]]; then
  echo "Cleaning up..."
  rm $DATA
fi
echo "Done!"
