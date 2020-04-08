all:
	cd include && g++ -std=c++11 *.hpp

tests_adj:
	cd include && g++ -std=c++11 adjacency_list.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make adjacencyListTest && ./adjacencyListTest

tests_edge:
	cd include && g++ -std=c++11 edge_hash_table.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_dk2tree:
	make all
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

valgrind_a:
	cd include && g++ -std=c++11 -I/usr/local/include -L/usr/local/lib *.hpp *.cpp -lsdsl  && valgrind -v --leak-check=yes ./a.out && rm a.out

valgrind_dk2tree:
	make all
	cd test && make dk2treeTest && valgrind --leak-check=yes ./dk2treeTest

tests_all:
	make all
	cd test && cmake CMakeLists.txt && make
	cd test && ./edgeHashTableTests && ./dk2treeTest && ./adjacencyListTest