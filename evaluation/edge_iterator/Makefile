edge_iterator:
	g++ -o edge_iterator edge_iterator.cpp -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

time:
	g++ -o time time.cpp -lboost_serialization

clean:
	rm edge_iterator time

all:
	make edge_iterator time

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph