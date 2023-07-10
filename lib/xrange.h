#include <iostream>

namespace extraAlgorithms {

    template<typename T>
    class XrangeIterator : public std::iterator_traits<std::input_iterator_tag> {
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::input_iterator_tag;
        using difference_type = std::ptrdiff_t;
        value_type value_ = 0;
        const value_type step_ = 1;
    public:
        XrangeIterator() = default;

        explicit XrangeIterator(value_type value) : value_(value) {}

        XrangeIterator(value_type value, value_type step) : value_(value), step_(step) {}

        XrangeIterator& operator++() {
            value_ += step_;
            return *this;
        }

        size_t operator-(XrangeIterator& other) {
            size_t tmp = value_ - other.value_;
            return tmp / step_ + 1 * (tmp % step_ != 0);
        }

        bool operator==(const XrangeIterator& other) const {
            return value_ == other.value_;
        }

        bool operator>(const XrangeIterator& other) const {
            return value_ > other.value_;
        }

        bool operator<(const XrangeIterator& other) const {
            return value_ < other.value_;
        }

        bool operator!=(const XrangeIterator& other) const {
            return (!(*this == other) && !(*this > other));
        }

        T operator*() {
            return value_;
        }
    };

    template<typename T>
    class xrange {
    public:
        using iterator = XrangeIterator<T>;
        using pointer = T*;
        using size_type = size_t;
        using reference = T&;
        using const_reference = const T&;
        using value_type = T;
    private:
        const iterator iterator_begin_;
        const iterator iterator_end_;
    public:
        explicit xrange(value_type end) : iterator_end_(end) {}

        xrange(value_type start, value_type end) : iterator_begin_(start), iterator_end_(end) {}

        xrange(value_type start, value_type end, value_type step) : iterator_begin_(start, step),
                                                                    iterator_end_(end, step) {}

        iterator begin() const {
            return iterator_begin_;
        }

        iterator end() const {
            return iterator_end_;
        }
    };

}
