OBJS := ../../../../../sdk2tree/implementations/sdk2tree/basic.o ../../../../../sdk2tree/implementations/sdk2tree/bitrankw32int.o ../../../../../sdk2tree/implementations/sdk2tree/misBits.o ../../../../../sdk2tree/implementations/sdk2tree/kTree.o ../../../../../sdk2tree/implementations/sdk2tree/Entry.o ../../../../../sdk2tree/implementations/sdk2tree/NodeQueue.o ../../../../../sdk2tree/implementations/sdk2tree/Queue.o ../../../../../sdk2tree/implementations/sdk2tree/kt_setOperations.o
INCLUDES := ../../../../../sdk2tree/implementations/sdk2tree/basic.h ../../../../../sdk2tree/implementations/sdk2tree/bitrankw32int.h ../../../../../sdk2tree/implementations/sdk2tree/misBits.h ../../../../../sdk2tree/implementations/sdk2tree/kTree.h ../../../../../sdk2tree/implementations/sdk2tree/Entry.h ../../../../../sdk2tree/implementations/sdk2tree/NodeQueue.h ../../../../../sdk2tree/implementations/sdk2tree/Queue.h ../../../../../sdk2tree/implementations/sdk2tree/kt_setOperations.h
INCLUDES_C := ../../../../../sdk2tree/implementations/sdk2tree/basic.c ../../../../../sdk2tree/implementations/sdk2tree/bitrankw32int.c ../../../../../sdk2tree/implementations/sdk2tree/misBits.c ../../../../../sdk2tree/implementations/sdk2tree/kTree.c ../../../../../sdk2tree/implementations/sdk2tree/Entry.c ../../../../../sdk2tree/implementations/sdk2tree/NodeQueue.c ../../../../../sdk2tree/implementations/sdk2tree/Queue.c ../../../../../sdk2tree/implementations/sdk2tree/kt_setOperations.c
create:
	g++ -o create -O3 create_k2tree.cpp -L/usr/local/include/sdsl -lsdsl -lboost_serialization
union:
	g++ -o union -O3 union.cpp -L/usr/local/include/sdsl -lsdsl
clean:
	rm create && rm union
memory:
	valgrind --tool=massif ./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
	echo "ms_print massif.out... > analize.txt"
gprof:
	rm create
	g++ -o create -O3 -pg create_k2tree.cpp -L/usr/local/include/sdsl -lsdsl -pthread
	./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
	gprof create gmon.out > analyzis.txt
evaluate_dmgen:
	./run_test.sh -dmgen
	./run_test_sdk.sh -dmgen
	gnuplot union_dmgen.gnuplot
evaluate_webgraph:
	./run_test.sh -webgraph
	./run_test_sdk.sh -webgraph
	gnuplot union_webgraph.gnuplot
evaluate: evaluate_dmgen evaluate_webgraph

create_sdk:
	g++ -o create_sdk -O3 create_k2tree_sdk.cpp ../../../../../sdk2tree/implementations/sdk2tree/kTree.h ../../../../../sdk2tree/implementations/sdk2tree/kTree.c ../../../../../sdk2tree/implementations/sdk2tree/basic.h ../../../../../sdk2tree/implementations/sdk2tree/basic.c ../../../../../sdk2tree/implementations/sdk2tree/bitrankw32int.h ../../../../../sdk2tree/implementations/sdk2tree/bitrankw32int.c
union_sdk:
	gcc -o union_sdk union_sdk.c $(OBJS) -lm -O3
union_sdk.c:
	gcc -c $(INCLUDES_C) -o project -O3
clean_sdk:
	rm create_sdk union_sdk
