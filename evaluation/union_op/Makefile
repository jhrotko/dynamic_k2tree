PATH_SDK:=../../../../../sdk2tree/implementations/sdk2tree/
OBJS:=$(PATH_SDK)basic.o $(PATH_SDK)bitrankw32int.o $(PATH_SDK)misBits.o $(PATH_SDK)kTree.o $(PATH_SDK)Entry.o $(PATH_SDK)NodeQueue.o $(PATH_SDK)Queue.o $(PATH_SDK)kt_setOperations.o
INCLUDES:=$(PATH_SDK)basic.h $(PATH_SDK)bitrankw32int.h $(PATH_SDK)misBits.h $(PATH_SDK)kTree.h $(PATH_SDK)Entry.h $(PATH_SDK)NodeQueue.h $(PATH_SDK)Queue.h $(PATH_SDK)kt_setOperations.h
INCLUDES_C:=$(PATH_SDK)basic.c $(PATH_SDK)bitrankw32int.c $(PATH_SDK)misBits.c $(PATH_SDK)kTree.c $(PATH_SDK)Entry.c $(PATH_SDK)NodeQueue.c $(PATH_SDK)Queue.c $(PATH_SDK)kt_setOperations.c

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

union_sdk:
	gcc -o union_sdk union_sdk.c $(OBJS) -lm -O3
union_sdk.c:
	gcc -c $(INCLUDES_C) -o project -O3
clean_sdk:
	rm create_sdk union_sdk
create_sdk:
	g++ -o create_sdk -O3 create_k2tree_sdk.cpp $(PATH_SDK)kTree.h $(PATH_SDK)kTree.c $(PATH_SDK)basic.h $(PATH_SDK)basic.c $(PATH_SDK)bitrankw32int.h $(PATH_SDK)bitrankw32int.c
