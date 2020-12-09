clean:
	rm add
add:
	g++ -o add -O9 add.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization -lboost_thread -lboost_system
evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph