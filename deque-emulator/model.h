#pragma once

#include <deque>
#include <string>
#include <random>

struct Model {
    using Deque = std::deque<std::string>;
    Deque items;
    Deque::iterator iterator = items.begin();
    size_t count = 0;
    std::mt19937 random_generator;
};

