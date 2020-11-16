contains:
	g++ -o contains contains.cpp -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

create:
	g++ -o create create.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

clean:
	rm contains create

all:
	make contains create

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph