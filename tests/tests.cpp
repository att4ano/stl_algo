#include <gtest/gtest.h>
#include "lib/ExtraAlgorithms.h"
#include "lib/Buffer.h"

bool FirstCompareWith(int i) {
    return i < 11;
}

bool SecondCompareWith(int i) {
    return i > 5;
}

bool ThirdCompareWith(int i) {
    return i == 0;
}

bool CompareTwoValues(int i, int j) {
    return i < j;
}

bool mod_2(int i) {
    return i % 2 == 0;
}

bool mod_3(int i) {
    return i % 3 == 0;
}

TEST(AlgorithmsTests, all_of_tests_true) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_TRUE(extraAlgorithms::all_of(arr.begin(), arr.end(), FirstCompareWith));
    ASSERT_TRUE(extraAlgorithms::all_of(arr.begin(), arr.end(), [](int i) { return i < 11; }));
}

TEST(AlgorithmsTests, all_of_tests_false) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_FALSE(extraAlgorithms::all_of(arr.begin(), arr.end(), SecondCompareWith));
    ASSERT_FALSE(extraAlgorithms::all_of(arr.begin(), arr.end(), [](int i){return i > 5;}));
}

TEST(AlgorithmsTests, any_of_tests_true) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_TRUE(extraAlgorithms::any_of(arr.begin(), arr.end(), SecondCompareWith));
    ASSERT_TRUE(extraAlgorithms::any_of(arr.begin(), arr.end(), [](int i) { return i > 5; }));
}

TEST(AlgorithmsTests, any_of_tests_false) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_FALSE(extraAlgorithms::any_of(arr.begin(), arr.end(), ThirdCompareWith));
    ASSERT_FALSE(extraAlgorithms::any_of(arr.begin(), arr.end(), [](int i){return i == 0;}));
}

TEST(AlgorithmsTests, none_of_tests_true) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_TRUE(extraAlgorithms::none_of(arr.begin(), arr.end(), ThirdCompareWith));
    ASSERT_TRUE(extraAlgorithms::none_of(arr.begin(), arr.end(), [](int i) { return i == 0; }));
}

TEST(AlgorithmsTests, none_of_tests_false) {
    std::vector<int> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_FALSE(extraAlgorithms::none_of(arr.begin(), arr.end(), FirstCompareWith));
    ASSERT_FALSE(extraAlgorithms::none_of(arr.begin(), arr.end(), [](int i){return i < 11;}));
}

TEST(AlgorithmsTests, one_of_tests_true) {
    std::vector<int> first_arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> second_arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    ASSERT_TRUE(extraAlgorithms::one_of(second_arr.begin(), second_arr.end(), [](int i) { return i == 0; }));
    ASSERT_TRUE(extraAlgorithms::one_of(second_arr.begin(), second_arr.end(), ThirdCompareWith));
}
TEST(AlgorithmsTests, one_of_tests_false) {
    std::vector<int> second_arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    ASSERT_FALSE(extraAlgorithms::one_of(second_arr.begin(), second_arr.end(), [](int i){return i > 5;}));
    ASSERT_FALSE(extraAlgorithms::one_of(second_arr.begin(), second_arr.end(), SecondCompareWith));
}

TEST(AlgorithmsTests, is_sorted_tests_true) {
    std::vector<int> first_arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_TRUE(extraAlgorithms::is_sorted(first_arr.begin(), first_arr.end(), [](int i, int j) { return i < j; }));
    ASSERT_TRUE(extraAlgorithms::is_sorted(first_arr.begin(), first_arr.end(), CompareTwoValues));
}

TEST(AlgorithmsTests, is_sorted_tests_false) {
    std::vector<int> second_arr = {1, 2, 3, 1, 2, 3};
    ASSERT_FALSE(extraAlgorithms::is_sorted(second_arr.begin(), second_arr.end(), [](int i, int j){return i < j;}));
    ASSERT_FALSE(extraAlgorithms::is_sorted(second_arr.begin(), second_arr.end(), CompareTwoValues));
}

TEST(AlgosTestSuite, IsPartitionedTrue) {
    std::vector<int> vec = { 3, 6, 9, 10, 11, 13 };
    ASSERT_TRUE(extraAlgorithms::is_partitioned(vec.begin(), vec.end(), [](int i) { return i % 3 == 0; }));
}

TEST(AlgosTestSuite, IsPartitionedFalse) {
    std::vector<int> vec = { 3, 6, 9, 10, 12, 13 };
    ASSERT_FALSE(extraAlgorithms::is_partitioned(vec.begin(), vec.end(), [](int i) { return i % 3 == 0; }));
}

TEST(AlgorithmsTests, is_partioned_false) {
    std::vector<int> arr = {0, 4, 8, 2, 1, 3, 5};
    ASSERT_FALSE(extraAlgorithms::is_partitioned(arr.begin(), arr.end(), [](int i) {return (i % 3) == 0;}));
    ASSERT_FALSE(extraAlgorithms::is_partitioned(arr.begin(), arr.end(), mod_3));
}

TEST(AlgorithmsTests, find_not) {
    std::vector<int> arr = {2, 2, 2, 2, 4, 2, 2};
    ASSERT_EQ(extraAlgorithms::find_not(arr.begin(), arr.end(), 2), 4);
    ASSERT_EQ(extraAlgorithms::find_not(arr.begin(), arr.end(), 4), 2);
}

TEST(AlgorithmsTests, find_backward) {
    std::vector<int> arr = {0, 4, 8, 2, 1, 3, 5};
    ASSERT_EQ(*extraAlgorithms::find_backward(arr.begin(), arr.end(), 3), 3);
    ASSERT_EQ(*extraAlgorithms::find_backward(arr.begin(), arr.end(), 1), 1);
}

TEST(AlgorithmsTests, is_palindrome_true) {
    std::vector<int> first_arr = {1, 2, 3, 3, 2, 1};
    std::vector<int> second_arr = {5, 6, 7, 6, 5};
    ASSERT_TRUE(extraAlgorithms::is_palindrome(first_arr.begin(), first_arr.end()));
    ASSERT_TRUE(extraAlgorithms::is_palindrome(second_arr.begin(), second_arr.end()));
}

TEST(AlgorithmsTests, is_palindrome_false) {
    std::vector<int> first_arr = {0, 4, 8, 2, 1, 3, 5};
    std::vector<int> second_arr = {2, 2, 2, 2, 4, 2, 2};
    ASSERT_FALSE(extraAlgorithms::is_palindrome(first_arr.begin(), first_arr.end()));
    ASSERT_FALSE(extraAlgorithms::is_palindrome(second_arr.begin(), second_arr.end()));
}

TEST(XrangeTestSuite, WithoutStepTestInt) {
    int k = 2;
    for(auto i : extraAlgorithms::xrange(2, 7)) {
        ASSERT_EQ(i, k);
        k += 1;
    }
}

TEST(XrangeTestSuite, WithoutStepTestDouble) {
    double k = 1.5;
    for(auto i : extraAlgorithms::xrange(1.5, 5.5)) {
        ASSERT_EQ(i, k);
        k += 1;
    }
}

TEST(XrangeTestSuite, StepTest) {
    int k = 1;
    for(auto i : extraAlgorithms::xrange(1, 6, 2)) {
        ASSERT_EQ(i, k);
        k += 2;
    }
}

TEST(XrangeTestSuite, MinusStepTest) {
    int k = 6;
    for(auto i : extraAlgorithms::xrange(6, 1, -1)) {
        ASSERT_EQ(i, k);
        k--;
    }
}

TEST(ZipTest, LessTest) {
    std::vector<int> l = {6, 7, 8};
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    int i = 0;
    for(auto value : extraAlgorithms::zip(l, v)) {
        ASSERT_EQ(value.first, l[i]);
        ASSERT_EQ(value.second, v[i]);
        i++;
    }
}

TEST(ZipTest, EqTest) {
    std::vector<int> l = {10, 11, 12, 13};
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    int i = 0;
    for(auto value : extraAlgorithms::zip(l, v)) {
        ASSERT_EQ(value.first, l[i]);
        ASSERT_EQ(value.second, v[i]);
        i++;
    }
}

TEST(ZipTest, MoreTest) {
    ExtBuffer<int> l = {5, 4, 3, 2, 1};
    std::vector<char> v = {'a', 'b', 'c', 'd'};

    int i = 0;
    for(auto value : extraAlgorithms::zip(l, v)) {
        ASSERT_EQ(value.first, l[i]);
        ASSERT_EQ(value.second, v[i]);
        i++;
    }
}
