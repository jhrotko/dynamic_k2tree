add_mean:
	g++ -o add_mean add_mean.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization
add_deviation:
	g++ -o add_deviation add_deviation.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization
clean:
	rm add_mean add_deviation
all:
	make add_mean add_deviation
#memory:
#	valgrind --tool=massif ./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
#	echo "ms_print massif.out... > analize.txt"
#gprof:
#	rm create
#	g++ -o create -pg create_k2tree.cpp -L/usr/local/include/sdsl -lsdsl -pthread
#	./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
#	gprof create gmon.out > analyzis.txt