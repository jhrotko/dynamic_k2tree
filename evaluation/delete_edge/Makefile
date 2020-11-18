delete:
	g++ -o delete delete.cpp -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

time:
	g++ -o time time.cpp -lboost_serialization

clean:
	rm delete time

all:
	make delete time

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph