all:
	cd include && g++ -std=c++17 *.hpp

clean:
	cd include && rm *.hpp.gch
	cd test && rm adjacencyListTest && rm dk2treeTest && rm edgeHashTableTests && rm kExtendedTest

tests_adj:
	cd include && g++ -std=c++17 AdjacencyList.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make adjacencyListTest && ./adjacencyListTest

tests_edge:
	cd include && g++ -std=c++17 EdgeHashTable.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_k_extended:
	cd include && g++ -std=c++17 KTreeExtended.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make kExtendedTest && ./kExtendedTest

tests_dk2tree:
	make all
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

valgrind_a:
	cd include && g++ -std=c++17 -I/usr/local/include -L/usr/local/lib *.hpp *.cpp -lsdsl  && valgrind -v --leak-check=yes ./a.out && rm a.out

valgrind_dk2tree:
	make all
	cd test && make dk2treeTest && valgrind --leak-check=yes ./dk2treeTest

tests_all:
	make all
	cd test && cmake CMakeLists.txt && make
	cd test && ./edgeHashTableTests && ./adjacencyListTest && ./kExtendedTest &&./dk2treeTest