all:
	cd include && g++ *.hpp
tests:
	make all
	cd test && make && ./edgeHashTableTests
test_build:
	make all
	cd test && cmake CMakeLists.txt && make && ./edgeHashTableTests