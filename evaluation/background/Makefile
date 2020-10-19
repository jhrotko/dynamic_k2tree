add_background:
	g++ -o add_background add_background.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization
add:
	g++ -o add add.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization
clean:
	rm add_background add
#memory:
#	valgrind --tool=massif ./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
#	echo "ms_print massif.out... > analize.txt"
#gprof:
#	rm create
#	g++ -o create -pg create_k2tree.cpp -L/usr/local/include/sdsl -lsdsl -pthread
#	./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
#	gprof create gmon.out > analyzis.txt