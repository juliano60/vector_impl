#include "catch.hpp"
#include "Vector.hpp"
#include <iostream>

using namespace tutorial;

TEST_CASE ("simpleConstruction") {
    Vector<int> v(10);

    REQUIRE(v.size() == 10);
    REQUIRE_FALSE(v.empty());

    for (auto& x: v) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}

TEST_CASE ("testReserve") {
    Vector<int> v;
    v.reserve(20);

    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());

    for (auto& x: v) {
        std::cout << x << " ";
    }
    std::cout << "\n";
}
/*
TEST_CASE ("testInsertion") {
    Vector<int> v;
    v.reserve(20);

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);

    //for (auto& x: v) {}
    
    REQUIRE(v.size() == 4);
}
*/
