#pragma once

#include <iostream>
#include <functional>
#include <type_traits>

#include "xrange.h"
#include "zip.h"

namespace extraAlgorithms {

    template<typename Predicate, typename... Parameter>
    concept Function = requires(Predicate func, Parameter...parameters) {
        {func(parameters...)} -> std::same_as<bool>;
    };

    template<typename T>
    concept Iterator = requires(T t) {
        typename std::iterator_traits<T>::iterator_category;
    };

    template<Iterator iterator, Function<typename std::iterator_traits<iterator>::value_type> Predicate>
    inline bool all_of(iterator first, iterator last, Predicate predicate) noexcept {
        for (iterator i = first; i != last; ++i) {
            if (!predicate(*i)) {
                return false;
            }
        }
        return true;
    }

    template<Iterator iterator, Function<typename std::iterator_traits<iterator>::value_type> Predicate>
    inline bool any_of(iterator first, iterator last, Predicate predicate) noexcept {
        for (iterator i = first; i != last; ++i) {
            if (predicate(*i)) {
                return true;
            }
        }
        return false;
    }

    template<Iterator iterator, Function<typename std::iterator_traits<iterator>::value_type> Predicate>
    inline bool none_of(iterator first, iterator last, Predicate predicate) noexcept {
        return !any_of(first, last, predicate);
    }

    template<Iterator iterator, Function<typename std::iterator_traits<iterator>::value_type> Predicate>
    inline bool one_of(iterator first, iterator last, Predicate predicate) noexcept {
        bool flag = false;
        for (iterator i = first; i != last; ++i) {
            if (predicate(*i)) {
                if (flag) {
                    return false;
                } else {
                    flag = true;
                }
            }
        }
        return flag;
    }

    template<Iterator iterator, Function<typename std::iterator_traits<iterator>::value_type, typename std::iterator_traits<iterator>::value_type> Predicate>
    inline std::enable_if<std::is_same<typename std::iterator_traits<iterator>::iterator_category,
            std::random_access_iterator_tag>::value, bool>::type
    is_sorted(iterator first, iterator last, Predicate predicate) noexcept {
        for (iterator next = first + 1; next != last; ++next) {
            if (!predicate(*first, *next)) {
                return false;
            }
            first = next;
        }
        return true;
    }

    template<Iterator iterator, Function<typename std::iterator_traits<iterator>::value_type> Predicate>
    inline typename std::enable_if<std::is_same<typename std::iterator_traits<iterator>::iterator_category,
            std::random_access_iterator_tag>::value, bool>::type
    is_partitioned(iterator first, iterator last, Predicate predicate) noexcept {
        bool flag;
        flag = predicate(*first);
        while (first != last && predicate(*first) == flag) {
            ++first;
        }
        while (first != last) {
            if (predicate(*first) == flag) {
                return false;
            }
            ++first;
        }
        return true;
    }

    template<Iterator iterator, typename T>
    inline T find_not(iterator first, iterator last, T n) noexcept {
        for (iterator i = first; i != last; ++i) {
            if (*i != n) {
                return *i;
            }
        }
        return n;
    }

    template<Iterator iterator, typename T>
    inline iterator find_backward(iterator first, iterator last, T n) noexcept {
        iterator ans = last;
        for (iterator i = first; i != last; ++i) {
            if (*i == n) {
                ans = i;
            }
        }
        return ans;
    }

    template<Iterator iterator>
    inline typename std::enable_if<std::is_same<typename std::iterator_traits<iterator>::iterator_category,
            std::random_access_iterator_tag>::value, bool>::type
    is_palindrome(iterator first, iterator last) noexcept {
        --last;
        while (first < last) {
            if (*last != *first) {
                return false;
            }
            ++first;
            --last;
        }
        return true;
    }

}
