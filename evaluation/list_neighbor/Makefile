list:
	g++ -o list list.cpp -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

time:
	g++ -o time time.cpp

clean:
	rm list time

all:
	make list time

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph