cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link edgeHashTableTests with what we want to test and the GTest and pthread library
add_executable(edgeHashTableTests edge_hash_table_test.cpp)
add_executable(adjacencyListTest adjacency_list_test.cpp)
add_executable(dk2treeTest dktree_test.cpp)
add_executable(algorithmTest algorithm_test.cpp)
add_executable(heavyTest heavy_test.cpp)

target_link_libraries(edgeHashTableTests ${GTEST_LIBRARIES} pthread sdsl boost_serialization)
target_link_libraries(adjacencyListTest ${GTEST_LIBRARIES} pthread sdsl)
target_link_libraries(dk2treeTest ${GTEST_LIBRARIES} pthread sdsl)
target_link_libraries(algorithmTest ${GTEST_LIBRARIES} pthread sdsl)
target_link_libraries(heavyTest ${GTEST_LIBRARIES} pthread sdsl)

add_test(NAME t1 COMMAND edgeHashTableTests)
add_test(NAME t2 COMMAND adjacencyListTest)
add_test(NAME t4 COMMAND dk2treeTest)
add_test(NAME t5 COMMAND algorithmTest)
add_test(NAME t6 COMMAND heavyTest)
