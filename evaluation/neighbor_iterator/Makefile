neighbor_iterator:
	g++ -o neighbor_iterator neighbor_iterator.cpp -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

time:
	g++ -o time time.cpp -lboost_serialization

clean:
	rm neighbor_iterator time

all:
	make neighbor_iterator time

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph