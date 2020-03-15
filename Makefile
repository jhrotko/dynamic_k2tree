all:
	cd include && g++ *.hpp
	
tests_edge:
	make all
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_dk2tree:
	make all
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

tests_all:
	make all
	cd test && cmake CMakeLists.txt && make
	cd test && ./edgeHashTableTests && ./dk2treeTest