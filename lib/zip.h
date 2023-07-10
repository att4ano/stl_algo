#include <iostream>

namespace extraAlgorithms {

    template<typename FirstSequence, typename SecondSequence>
    class zip {
    private:
        class ZipIterator {
        public:
            using iterator1 = FirstSequence::iterator;
            using iterator2 = SecondSequence::iterator;
        private:
            iterator1 first_iterator;
            iterator2 second_iterator;
        public:
            ZipIterator(iterator1 first, iterator2 second) : first_iterator(first), second_iterator(second) {}

            constexpr ZipIterator& operator++() {
                ++first_iterator;
                ++second_iterator;
                return *this;
            }

            constexpr ZipIterator operator++(int) {
                ZipIterator tmp = *this;
                ++first_iterator;
                ++second_iterator;
                return tmp;
            }

            constexpr bool operator!=(const ZipIterator& other) const {
                return (first_iterator != other.first_iterator) && (second_iterator != other.second_iterator);
            }

            constexpr bool operator==(const ZipIterator& other) const {
                return !(*this != other);
            }

            constexpr auto operator*() {
                return std::make_pair(*first_iterator, *second_iterator);
            }
        };

    public:
        using iterator = ZipIterator;
    private:
        const iterator iterator_begin_;
        const iterator iterator_end_;
    public:
        zip(FirstSequence& first, SecondSequence& second) : iterator_begin_(std::begin(first), std::begin(second)),
                                                            iterator_end_(std::end(first), std::end(second)) {}

        iterator begin() const {
            return iterator_begin_;
        }

        iterator end() const {
            return iterator_end_;
        }

    };
}
