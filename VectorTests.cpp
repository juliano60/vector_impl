#include "catch.hpp"
#include "Vector.hpp"
#include <iostream>

namespace {
    template <typename C>
    void print_vec(const C& v) {
        for (const auto& x:v) {
            std::cout << x << " ";
        }
        std::cout << "\n";
    }
}

using namespace tutorial;

TEST_CASE ("simpleConstruction") {
    Vector<int> v(10);

    REQUIRE(v.size() == 10);
    REQUIRE_FALSE(v.empty());

    print_vec(v);
}

TEST_CASE ("testCopying") {
    Vector<int> v(10);
    Vector<int> c(v);

    REQUIRE(v.size() == 10);
    REQUIRE(c.size() == 10);
    
    print_vec(v);
    print_vec(c);
}

TEST_CASE ("testReserve") {
    Vector<int> v;
    v.reserve(20);

    REQUIRE(v.size() == 0);
    REQUIRE(v.empty());

    print_vec(v);
}

TEST_CASE ("testInsertion") {
    Vector<int> v;
    v.reserve(20);

    v.push_back(10);
    v.push_back(20);
    v.push_back(30);
    v.push_back(40);

    REQUIRE(v.size() == 4);
    print_vec(v);
}

TEST_CASE ("constructWithInitializerList") {
    Vector<std::string> v = {"ab", "ac", "ad"};

    REQUIRE(v.size() == 3);
    print_vec(v);
}
