delete:
	g++ -o delete delete.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization
union:
	g++ -o union union.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization
create:
	g++ -o create create.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization
complexity:
	g++ -o complexity complexity.cpp -L/usr/local/include/sdsl -lsdsl
clean:
	rm complexity
	rm -r delete-test
	rm -r union-test
all:
	make delete union create complexity
evaluate:
	./run_test.sh -dmgen -delete
	./plot -dmgen -delete clean
	./run_test.sh -dmgen -union
	./plot -dmgen -union clean
	./run_test.sh -webgraph -delete
	./plot -webgraph -delete clean
	./run_test.sh -webgraph -union
	./plot -webgraph -union clean
#memory:
#	valgrind --tool=massif ./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
#	echo "ms_print massif.out... > analize.txt"
#gprof:
#	rm create
#	g++ -o create -pg create_k2tree.cpp -L/usr/local/include/sdsl -lsdsl -pthread
#	./create ../datasets/dmgen/prepared_datasets/dmgen/10000/10000.tsv 10000 bla
#	gprof create gmon.out > analyzis.txt