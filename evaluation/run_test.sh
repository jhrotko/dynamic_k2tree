#!/bin/sh
DATASETDIR="../datasets/dmgen/prepared_datasets/dmgen"

#make clean create union

for vertices in $(ls $DATASETDIR)
do
  mkdir -p $vertices
	echo $DATASETDIR/$vertices/$vertices.tsv
	#time analisys
	./create $DATASETDIR/$vertices/$vertices.tsv $vertices $vertices/test.kt
	#memory analisys
	valgrind --tool=massif --massif-out-file=$vertices/$vertices.union.mem.out ./union $vertices/test.kt $vertices/test.kt
done

