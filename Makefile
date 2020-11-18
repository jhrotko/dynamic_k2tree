CFLAGS=-Wall -O9 -g -DNDEBUG -std=c++17

dktree:
	cd include/dktree && g++ $(CFLAGS) *.hpp
	cd include/graph && g++ $(CFLAGS) *.hpp

algorithm:
	cd include/algorithm && g++ $(CFLAGS) *.hpp

all:
	make dktree
	make algorithm

clean:
	cd include/dktree && rm *.hpp.gch
	cd include/algorithm && rm *.hpp.gch
	cd test && rm heavyTest
	cd test && rm edgeHashTableTests
	cd test && rm algorithmTest
	cd test && rm dk2treeTest

tests_edge:
	cd include/dktree && g++ $(CFLAGS) EdgeHashTable.hpp utils.hpp
	cd test && cmake CMakeLists.txt && make edgeHashTableTests && ./edgeHashTableTests

tests_dktree:
	make dktree
	cd test && cmake CMakeLists.txt && make dk2treeTest && ./dk2treeTest

tests_dktree_all:
	make dktree
	cd test && cmake CMakeLists.txt && make
	cd test && ./edgeHashTableTests &&./dk2treeTest

tests_algorithm:
	make algorithm
	cd test && cmake CMakeLists.txt && make algorithmTest && ./algorithmTest

tests_heavy:
	make dktree
	cd test && cmake CMakeLists.txt && make
	cd test && ./heavyTest
tests_all:
	make tests_dktree_all
	make tests_algorithm

valgrind_dktree:
	cd test && cmake CMakeLists.txt && make
	cd test && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out-heavy.txt ./heavyTest

sdsl-path="../sdsl-lite"
update_repo:
	cd $(sdsl-path) && git pull || exit
	git pull --rebase --autostash

# Make sure you commited before
ship:
	make update_repo && make tests_all && git push

evaluate:
	cd evaluation && cd add_edge && make evaluate
	cd evaluation && cd contains && make evaluate
	cd evaluation && cd delay_edge && make evaluate
	cd evaluation && cd delete_edge && make evaluate
	cd evaluation && cd list_neighbour && make evaluate
	cd evaluation && cd union_op && make evaluate
