cmake_minimum_required(VERSION 2.6)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link edgeHashTableTests with what we want to test and the GTest and pthread library
add_executable(edgeHashTableTests edge_hash_table_test.cpp)
add_executable(dk2treeTest dktree_test.cpp)
add_executable(algorithmTest algorithm_test.cpp)
add_executable(heavyTest heavy_test.cpp)
#add_executable(LinkedListTest linkedList_test.cpp)

target_link_libraries(edgeHashTableTests ${GTEST_LIBRARIES} pthread sdsl boost_serialization)
target_link_libraries(dk2treeTest ${GTEST_LIBRARIES} pthread sdsl boost_serialization stdc++fs)
target_link_libraries(algorithmTest ${GTEST_LIBRARIES} pthread sdsl)
target_link_libraries(heavyTest ${GTEST_LIBRARIES} pthread sdsl)
#target_link_libraries(LinkedListTest ${GTEST_LIBRARIES} pthread)

add_test(NAME t1 COMMAND edgeHashTableTests)
add_test(NAME t4 COMMAND dk2treeTest)
add_test(NAME t5 COMMAND algorithmTest)
add_test(NAME t6 COMMAND heavyTest)
#add_test(NAME t2 COMMAND LinkedListTest)
