all:
	cd include && g++ *.hpp

tests_adj:
	cd include && g++ adjacency_list.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make adjacencyListTest && ./adjacencyListTest

tests_edge:
	cd include && g++ edge_hash_table.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_dk2tree:
	make all
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

tests_all:
	make all
	cd test && cmake CMakeLists.txt && make
	cd test && ./edgeHashTableTests && ./dk2treeTest && ./adjacencyListTest