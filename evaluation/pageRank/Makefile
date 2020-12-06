pagerank:
	g++ -o pagerank pagerank.cpp -O3 -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

time:
	g++ -o time time.cpp

clean:
	rm pagerank time

all:
	make pagerank time

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph