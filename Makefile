all:
	cd include && g++ *.hpp
tests:
	make all
	cd test && make && ./edgeHashTableTests
tests_build:
	make all
	cd test && cmake CMakeLists.txt && make && ./edgeHashTableTests