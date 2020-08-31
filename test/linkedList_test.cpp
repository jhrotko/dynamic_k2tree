#include "gtest/gtest.h"
#include "../include/util/LinkedList.hpp"

TEST(Initialization, createLinkedList) {
    LinkedList list;
}

TEST(Push, many) {
    LinkedList list;

    list.push(2);
    list.push(3);
    list.push(5);
    list.push(6);
    list.push(6);
    list.push(6);
    list.push(6);
    list.push(6);
    list.push(6);
}

TEST(Pop, unintialize) {
    LinkedList list;

    list.pop();
}

TEST(Pop, popHead) {
    LinkedList list;

    list.push(2);
    ASSERT_EQ(list.pop(), 2);
}

TEST(Pop, popMiddle) {
    LinkedList list;

    list.push(2);
    list.push(3);
    ASSERT_EQ(list.pop(), 2);
    ASSERT_EQ(list.pop(), 3);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}