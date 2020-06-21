all:
	cd include/dktree && g++ -std=c++17 *.hpp
	cd include/algorithm && g++ -std=c++17 *.hpp

clean:
	cd include/dktree && rm *.hpp.gch
	cd include/algorithm && rm *.hpp.gch
	cd test && rm adjacencyListTest && rm dk2treeTest && rm edgeHashTableTests && rm kExtendedTest && rm algorithmTest

tests_adj:
	cd include/dktree && g++ -std=c++17 adjacency_list.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make adjacencyListTest && ./adjacencyListTest

tests_edge:
	cd include/dktree && g++ -std=c++17 edge_hash_table.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_k_extended:
	cd include/dktree && g++ -std=c++17 ktree_extended.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make kExtendedTest && ./kExtendedTest

tests_dk2tree:
	make all
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

valgrind_dk2tree:
	make all
	cd test && make dk2treeTest && valgrind --leak-check=yes ./dk2treeTest

tests_graph_all:
	make all
	cd test && cmake CMakeLists.txt && make
	cd test && ./edgeHashTableTests && ./adjacencyListTest && ./kExtendedTest &&./dk2treeTest

tests_algorithms:
	make all
	cd test && cmake CMakeLists.txt && make algorithmTest && ./algorithmTest

tests_all:
	make tests_graph_all
	make tests_algorithms