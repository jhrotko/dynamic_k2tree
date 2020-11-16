create:
	g++ -o create create_k2tree.cpp -L/usr/local/include/sdsl -lsdsl -lboost_serialization
union:
	g++ -o union union.cpp -L/usr/local/include/sdsl -lsdsl
clean:
	rm create && rm union
memory:
	valgrind --tool=massif ./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
	echo "ms_print massif.out... > analize.txt"
gprof:
	rm create
	g++ -o create -pg create_k2tree.cpp -L/usr/local/include/sdsl -lsdsl -pthread
	./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
	gprof create gmon.out > analyzis.txt
evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph