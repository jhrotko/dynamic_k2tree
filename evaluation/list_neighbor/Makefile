list:
	g++ -o list list.cpp -lstdc++fs -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

create:
	g++ -o create create.cpp -L/usr/local/include/sdsl -lsdsl -pthread -lboost_serialization

clean:
	rm list create

all:
	make list create

evaluate:
	./run_test.sh -dmgen
	./run_test.sh -webgraph