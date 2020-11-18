contains:
	g++ -o contains contains.cpp -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

time:
	g++ -o time time.cpp

clean:
	rm contains time

all:
	make contains time

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph