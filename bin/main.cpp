#include <vector>
#include <list>
#include <set>
#include <map>
#include <memory>
#include "../lib/ExtraAlgorithms.h"
#include "lib/task.h"

class Cat {
public:
    int age_ = 0;
    double mass_ = 0;

    Cat(int age, int mass) : age_(age), mass_(mass) {}

    size_t operator-(const Cat& other) const {
        size_t tmp = age_ - other.age_;
        return tmp;
    }

    bool  operator==(const Cat& other) const {
        return age_ == other.age_;
    }

    bool operator>(const Cat& other) const {
        return age_ > other.age_;
    }

    bool operator<(const Cat& other) const {
        return age_ < other.age_;
    }

    bool operator>=(const Cat& other) const {
        return !(*this < other);
    }

    bool operator<=(const Cat& other) const {
        return !(*this > other);
    }

    bool operator!=(const Cat& other) const {
        return !(*this == other);
    }

    Cat& operator++() {
        ++age_;
        return *this;
    }

    Cat& operator+=(const Cat& other) {
        age_ += other.age_;
        return *this;
    }

    Cat& operator+=(int other) {
        age_ += other;
        return *this;
    }

    Cat& operator=(const int i) {
        age_ = i;
        return *this;
    }

};

int main() {
    Cat cat1(3, 3);
    Cat cat2(10, 10);
    Cat step(1, 1);
    auto x = extraAlgorithms::xrange(cat1, cat2, step);
    for (const auto& i : x) {
        std::cout << i.age_ << " ";
    }
    std::cout << "\n";
    std::vector <Cat> v(x.begin(), x.end());
    for (const auto& i : v) {
        std::cout << i.age_ << " ";
    }
}
