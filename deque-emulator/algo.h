#pragma once

#include <deque>

template<class T, class Comp>
std::deque<T> Merge(const std::deque<T>& half1, const std::deque<T>& half2, const Comp& comparator) {

    std::deque<T> result;
    auto iter1 = half1.begin(), iter2 = half2.begin();
    while (iter1 != half1.end() && iter2 != half2.end()) {
        if (comparator(*iter1, *iter2)) {
            result.push_back(*iter1);
            ++iter1;
        } else {
            result.push_back(*iter2);
            ++iter2;
        }
    }
    while (iter1 != half1.end()) {
        result.push_back(*iter1);
        ++iter1;
    }
    while (iter2 != half2.end()) {
        result.push_back(*iter2);
        ++iter2;
    }
    return result;
}

template<class T, class Comp>
std::deque<T> MergeSort(const std::deque<T>& src, const Comp& comparator) {

    if (src.size() <= 1) {
        return src;
    }
    auto beg = src.begin();
    auto mid = beg + src.size() / 2;
    auto end = src.end();
    auto half1 = MergeSort<T>({beg, mid}, comparator);
    auto half2 = MergeSort<T>({mid, end}, comparator);
    return Merge(half1, half2, comparator);
}
