dktree:
	cd include/dktree && g++ -std=c++17 *.hpp

algorithm:
	cd include/algorithm && g++ -std=c++17 *.hpp

all:
	make dktree
	make algorithm

clean:
	cd include/dktree && rm *.hpp.gch
	cd include/algorithm && rm *.hpp.gch
	cd test && rm adjacencyListTest && rm dk2treeTest && rm edgeHashTableTests && rm algorithmTest

tests_adj:
	cd include/dktree && g++ -std=c++17 adjacency_list.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make adjacencyListTest && ./adjacencyListTest

tests_edge:
	cd include/dktree && g++ -std=c++17 edge_hash_table.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_dktree:
	make dktree
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

valgrind_dk2tree:
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