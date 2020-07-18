CFLAGS=-Wall -O9 -g -DNDEBUG -std=c++17

dktree:
	cd include/dktree && g++ $(CFLAGS) *.hpp

algorithm:
	cd include/algorithm && g++ $(CFLAGS) *.hpp

all:
	make dktree
	make algorithm

clean:
	cd include/dktree && rm *.hpp.gch
	cd include/algorithm && rm *.hpp.gch
	cd test && rm adjacencyListTest && rm dk2treeTest && rm edgeHashTableTests && rm algorithmTest

tests_adj:
	cd include/dktree && g++ $(CFLAGS) AdjacencyList.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make adjacencyListTest && ./adjacencyListTest

tests_edge:
	cd include/dktree && g++ $(CFLAGS) EdgeHashTable.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_dktree:
	make dktree
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

valgrind_dktree:
	make dktree
	cd test && make dk2treeTest && valgrind --leak-check=yes ./dk2treeTest

tests_dktree_all:
	make dktree
	cd test && cmake CMakeLists.txt && make
	cd test && ./edgeHashTableTests && ./adjacencyListTest &&./dk2treeTest

tests_algorithm:
	make algorithm
	cd test && cmake CMakeLists.txt && make algorithmTest && ./algorithmTest

tests_all:
	make tests_dktree_all
	make tests_algorithm

valgrind_test:
	cd test && valgrind --leak-check=yes ./$(test)

sdsl-path="../sdsl-lite"
update_repo:
	cd $(sdsl-path) && git pull || exit
	git pull --rebase --autostash

# Make sure you commited before
ship:
	make update_repo && make tests_all && git push