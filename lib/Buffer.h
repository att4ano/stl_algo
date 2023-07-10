#include <iostream>
#include <memory>
#include <initializer_list>

const static size_t kCapacityCoefficient = 2;

template<typename T>
class Iter : public std::iterator_traits<T> {
public:
    using iterator_category = std::random_access_iterator_tag;
    using pointer = T*;
    using size_type = size_t;
    using reference = T&;
    using value_type = T;
private:
    pointer begin_ = nullptr;
    pointer end_ = nullptr;
    pointer current_ptr_ = nullptr;
    size_type size_ = 0;
public:

    Iter() = default;

    Iter(pointer buff, size_type size) : begin_(buff), end_(buff + size - 1), current_ptr_(buff), size_(size) {}

    Iter(pointer buff, pointer new_current, size_type size) : begin_(buff), end_(buff + size - 1), current_ptr_(new_current), size_(size) {}

    Iter(const Iter& other) : begin_(other.begin_), end_(other.end_), current_ptr_(other.current_ptr_), size_(other.size_) {}

    constexpr bool operator==(const Iter& other) const {
        return current_ptr_ == other.current_ptr_;
    }

    constexpr bool operator==(pointer other) const {
        return current_ptr_ == other;
    }

    constexpr bool operator!=(const Iter& other) const {
        return current_ptr_ != other.current_ptr_;
    }

    constexpr Iter& operator=(const Iter& other) {
        if (this == &other) {
            return *this;
        }
        begin_ = other.begin_;
        end_ = other.end_;
        current_ptr_ = other.current_ptr_;
        size_ = other.size_;
        return *this;
    }

    constexpr Iter& operator++() {
        if (current_ptr_ == end_) {
            current_ptr_ = begin_;
        } else {
            ++current_ptr_;
        }
        return *this;
    }

    constexpr Iter operator++(int) {
        Iter temp = *this;
        if (current_ptr_ == end_) {
            current_ptr_ = begin_;
        } else {
            ++current_ptr_;
        }
        return temp;
    }

    constexpr Iter& operator--() {
        if (current_ptr_ == begin_) {
            current_ptr_ = end_;
        } else {
            current_ptr_--;
        }
        return *this;
    }

    constexpr Iter operator--(int) {
        Iter temp = *this;
        if (current_ptr_ == begin_) {
            current_ptr_ = end_;
        } else {
            current_ptr_--;
        }
        return temp;
    }

    constexpr Iter operator+(const int64_t n) {
        Iter<value_type> temp = *this;
        if (n >= 0) {
            if (n > temp.end_ - temp.current_ptr_) {
                temp.current_ptr_ = temp.begin_ + (n - (temp.end_ - temp.current_ptr_ + 1));
            } else {
                temp.current_ptr_ += n;
            }
        } else {
            if (n > current_ptr_ - begin_) {
                temp.current_ptr_ = temp.end_ - (n - (temp.current_ptr_ - temp.begin_ + 1));
            } else {
                temp.current_ptr_ -= n;
            }
        }
        return temp;
    }

    constexpr Iter operator-(const int64_t n) {
        Iter<value_type> temp = *this;
        if (n >= 0) {
            if (n > current_ptr_ - begin_) {
                temp.current_ptr_ = temp.end_ - (n - (temp.current_ptr_ - temp.begin_ + 1));
            } else {
                temp.current_ptr_ -= n;
            }
        } else {
            if (n > temp.end_ - temp.current_ptr_) {
                temp.current_ptr_ = temp.begin_ + (n - (temp.end_ - temp.current_ptr_ + 1));
            } else {
                temp.current_ptr_ += n;
            }
        }
        return temp;
    }

    constexpr size_type operator-(const Iter& other) const {
        int distance = current_ptr_ - other.current_ptr_;
        if (distance >= 0) {
            return distance;
        } else {
            return (size_ + distance);
        }
    }

    constexpr Iter& operator+=(const int64_t n) {
        if (n >= 0) {
            *this = *this + n;
            return *this;
        } else {
            *this = *this - (-1 * n);
            return *this;
        }
    }

    constexpr reference operator*() {
        return *current_ptr_;
    }

    constexpr Iter& operator[](const int64_t n) {
        return *this += n;
    }

    constexpr bool operator>(const Iter& other) {
        return current_ptr_ > other.current_ptr_;
    }

    constexpr bool operator<(const Iter& other) {
        return current_ptr_ < other.current_ptr_;
    }

    constexpr bool operator>=(const Iter& other) {
        return current_ptr_ >= other.current_ptr_;
    }

    constexpr bool operator<=(const Iter& other) {
        return current_ptr_ <= other.current_ptr_;
    }

    Iter<const T> MakeConst() const noexcept {
        Iter<const T> iter = Iter<const T>(begin_, current_ptr_, size_);
        return iter;
    }

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////        CCircularBuffer class       /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename alloc = std::allocator<T>>

class Buffer {
public:
    using iterator = Iter<T>;
    using const_iterator = Iter<const T>;
    using pointer = T*;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using value_type = T;
private:
    size_type capacity_ = 0;
    size_type size_ = 0;
    alloc allocator;
    pointer buff_;
    iterator head_;
    iterator tail_;
public:
    Buffer() = default;

    explicit Buffer(const size_type capacity) :
            capacity_(capacity + 1),
            buff_(std::allocator_traits<alloc>::allocate(allocator, capacity)),
            head_(Iter<value_type>(buff_, capacity + 1)),
            tail_(++Iter<value_type>(buff_, capacity + 1)) {}

    Buffer(const std::initializer_list<T>& list) :
            capacity_(list.size() + 1),
            buff_(std::allocator_traits<alloc>::allocate(allocator, list.size() + 1)),
            size_(list.size()),
            head_(Iter<value_type>(buff_, capacity_)),
            tail_(head_ + size_) {
        auto iter = list.begin();
        for (Buffer::iterator it = begin(); it != end(); ++it) {
            std::allocator_traits<alloc>::construct(allocator, &(*it), *iter);
            ++iter;
        }
    }

    template<typename U>
    Buffer(U begin, U end) : buff_(allocator.allocate(end - begin + 1)),
                capacity_((end - begin) * kCapacityCoefficient + 1),
                size_(end - begin),
                head_(Iter<T>(&buff_[0], size_ + 1)),
                tail_(Iter<T>(&buff_[0], size_ + 1)) {
            for (U it = begin; it != end; ++it) {
                push_back(*it);
            }
        }

    Buffer(value_type k, size_type amount) :
            capacity_(amount + 1),
            buff_(std::allocator_traits<alloc>::allocate(allocator, capacity_)),
            size_(amount),
            head_(Iter<value_type>(buff_, capacity_ + 1)),
            tail_(head_ + size_) {
        for (iterator iter = begin(); iter != end(); ++iter) {
            std::allocator_traits<alloc>::construct(allocator, &(*iter), k);
        }
    }

    void DestructElements() {
        for (iterator it = begin(); it != end(); ++it) {
            std::allocator_traits<alloc>::destroy(allocator, &(*it));
        }
    }

    ~Buffer() {
        DestructElements();
        std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
    }

    constexpr iterator begin() const noexcept {
        return head_;
    }

    constexpr iterator end() const noexcept {
        return tail_;
    }

    constexpr const_iterator cbegin() const noexcept {
        return head_.MakeConst();
    }

    constexpr const_iterator cend() const noexcept {
        return tail_.MakeConst();
    }

    constexpr reference operator[](const size_type n) {
        if (n < size_) {
            return *begin()[n];
        } else {
            throw std::invalid_argument("Index is out of range");
        }
    }

    constexpr const_reference operator[](const size_type n) const {
        if (n < size_) {
            return *begin()[n];
        } else {
            throw std::invalid_argument("Index is out of range");
        }
    }

    Buffer(const Buffer& other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        buff_ = std::allocator_traits<alloc>::allocate(allocator, other.capacity_);
        head_ = Iter<T>(buff_, capacity_);
        tail_ = head_ + size_;
        iterator j = other.head_;
        for (Buffer::iterator i = head_; i != tail_; ++i) {
            std::allocator_traits<alloc>::construct(allocator, &(*i), *j);
            ++j;
        }
    }

    Buffer& operator=(const Buffer& other) {
        if (this == &other) {
            return *this;
        }
        allocator.deallocate(buff_, capacity_);
        capacity_ = other.capacity_;
        size_ = other.size_;
        buff_ = std::allocator_traits<alloc>::allocate(allocator, other.capacity_);
        head_ = Iter<T>(buff_, capacity_);
        tail_ = head_ + size_;
        iterator j = other.head_;
        for (Buffer::iterator i = begin(); i != end(); ++i) {
            std::allocator_traits<alloc>::construct(allocator, &(*i), *j);
            ++j;
        }
        return *this;
    }

    constexpr bool operator==(Buffer& other) {
        if (size_ != other.size_) {
            return false;
        } else {
            iterator j = other.begin();
            for (Buffer::const_iterator i = cbegin(); i < cend(); ++i) {
                if (*i != *j) {
                    return false;
                }
                ++j;
            }
            return true;
        }
    }

    constexpr bool operator!=(const Buffer& other) const noexcept {
        return !(*this == other);
    }

    constexpr void push_back(const_reference n) noexcept {
        if (size_ == 0) {
            *head_ = n;
            ++size_;
        } else if (tail_ + 1 == head_) {
            *tail_ = n;
            ++tail_;
            ++head_;
        } else {
            *tail_ = n;
            ++tail_;
            ++size_;
        }
    }

    void pop_front() {
        if (size_ == 0) {
            throw std::invalid_argument("The buffer is already empty");
        } else {
            if (!(tail_ == head_ + 1 && size_ == 1)) {
                ++head_;
            }
            --size_;
        }
    }

    void swap(Buffer& other) {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(buff_, other.buff_);
    }

    static void swap(Buffer& first, Buffer& second) {
        std::swap(first.capacity_, second.capacity_);
        std::swap(first.size_, second.size_);
        std::swap(first.head_, second.head_);
        std::swap(first.tail_, second.tail_);
        std::swap(first.buff_, second.buff_);
    }

    size_type size() {
        return size_;
    }

    size_type max_size() {
        return capacity_ - 1;
    }

    bool empty() {
        return size_ == 0;
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////        CCircularBufferExt class       ///////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename alloc = std::allocator<T>>

class ExtBuffer {
public:
    using iterator = Iter<T>;
    using const_iterator = Iter<const T>;
    using pointer = T*;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using value_type = T;
private:
    size_type capacity_ = 0;
    size_type size_ = 0;
    alloc allocator;
    pointer buff_;
    iterator head_;
    iterator tail_;
public:
    ExtBuffer() = default;

    explicit ExtBuffer(const size_type capacity) :
            capacity_(capacity + 1),
            buff_(std::allocator_traits<alloc>::allocate(allocator, capacity)),
            head_(Iter<value_type>(buff_, capacity + 1)),
            tail_(++Iter<value_type>(buff_, capacity + 1)) {}

    ExtBuffer(const std::initializer_list<T>& list) :
            capacity_(list.size() * kCapacityCoefficient + 1),
            buff_(std::allocator_traits<alloc>::allocate(allocator, capacity_)),
            size_(list.size()),
            head_(Iter<value_type>(buff_, capacity_)),
            tail_(head_ + size_) {
        auto iter = list.begin();
        for (ExtBuffer::iterator it = begin(); it != end(); ++it) {
            std::allocator_traits<alloc>::construct(allocator, &(*it), *iter);
            ++iter;
        }
    }

    ExtBuffer(value_type k, size_type amount) :
            capacity_(amount * kCapacityCoefficient + 1),
            buff_(std::allocator_traits<alloc>::allocate(allocator, capacity_)),
            size_(amount),
            head_(Iter<value_type>(buff_, capacity_ + 1)),
            tail_(head_ + size_) {
        for (iterator iter = begin(); iter != end(); ++iter) {
            std::allocator_traits<alloc>::construct(allocator, &(*iter), k);
        }
    }

    template<typename U>
    ExtBuffer(U begin, U end) : buff_(allocator.allocate(end - begin + 1)),
              size_(end - begin),
              capacity_((end - begin) * kCapacityCoefficient + 1),
              head_(Iter<T>(&buff_[0], size_ + 1)),
              tail_(Iter<T>(&buff_[0], size_ + 1)) {
        for (U it = begin; it != end; ++it) {
            push_back(*it);
        }
    }

    ExtBuffer(iterator it1, iterator it2) {
        size_type capacity = 0;
        for (iterator iter = it1; iter != it2; ++iter) {
            ++capacity;
        }
        size_ = capacity;
        capacity_ = capacity * kCapacityCoefficient + 1;
        buff_ = std::allocator_traits<alloc>::allocate(allocator, capacity_);
        head_ = Iter<value_type>(buff_, capacity_);
        tail_ = head_ + size_;
        for (iterator it = it1, other_iter = begin(); it != it2; ++it, ++other_iter) {
            std::allocator_traits<alloc>::construct(allocator, &(*other_iter), *it);
        }
    }

    void DestructElements() {
        for (iterator it = begin(); it != end(); ++it) {
            std::allocator_traits<alloc>::destroy(allocator, &(*it));
        }
    }

    ~ExtBuffer() {
        DestructElements();
        std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
    }

    constexpr reference operator[](const size_type n) {
        if (n < size_) {
            return *begin()[n];
        } else {
            throw std::invalid_argument("Index is out of range");
        }
    }

    constexpr const_reference operator[](const size_type n) const {
        if (n < size_) {
            return *begin()[n];
        } else {
            throw std::invalid_argument("Index is out of range");
        }
    }

    constexpr iterator begin() noexcept {
        return head_;
    }

    constexpr iterator end() noexcept {
        return tail_;
    }

    const_iterator cbegin() const noexcept {
        return head_.MakeConst();
    }

    const_iterator cend() const noexcept {
        return tail_.MakeConst();
    }

    ExtBuffer(const ExtBuffer& other) {
        capacity_ = other.capacity_;
        size_ = other.size_;
        buff_ = std::allocator_traits<alloc>::allocate(allocator, other.capacity_);
        head_ = Iter<T>(buff_, capacity_);
        tail_ = head_ + size_;
        iterator j = other.head_;
        for (ExtBuffer::iterator i = begin(); i != end(); ++i) {
            std::allocator_traits<alloc>::construct(allocator, &(*i), *j);
            ++j;
        }
    }

    ExtBuffer& operator=(const ExtBuffer& other) {
        if (this == &other) {
            return *this;
        }
        allocator.deallocate(buff_, capacity_);
        capacity_ = other.capacity_;
        size_ = other.size_;
        buff_ = std::allocator_traits<alloc>::allocate(allocator, other.capacity_);
        head_ = Iter<T>(buff_, capacity_);
        tail_ = head_ + size_;
        iterator j = other.head_;
        for (ExtBuffer::iterator i = begin(); i != end(); ++i) {
            std::allocator_traits<alloc>::construct(allocator, &(*i), *j);
            ++j;
        }
        return *this;
    }

    constexpr bool operator==(ExtBuffer& other) {
        if (size_ != other.size_) {
            return false;
        } else {
            iterator j = other.begin();
            for (ExtBuffer::const_iterator i = cbegin(); i < cend(); ++i) {
                if (*i != *j) {
                    return false;
                }
                ++j;
            }
            return true;
        }
    }

    constexpr bool operator!=(const ExtBuffer& other) {
        return !(*this == other);
    }

    void push_back(const_reference n) {
        if (size_ == 0) {
            *head_ = n;
            ++size_;
        } else if (size_ == capacity_ - 1) {
            pointer new_buffer = std::allocator_traits<alloc>::allocate(allocator, capacity_ * kCapacityCoefficient + 1);
            size_t current_position = 0;
            for (iterator it = begin(); it != end(); ++it, ++current_position) {
                std::allocator_traits<alloc>::construct(allocator, new_buffer + current_position, *it);
            }
            allocator.deallocate(buff_, capacity_);
            capacity_ = capacity_ * kCapacityCoefficient;
            buff_ = std::move(new_buffer);
            head_ = Iter<T>(buff_, capacity_);
            tail_ = head_ + current_position;
            *tail_ = n;
            ++tail_;
            ++size_;
        } else {
            *tail_ = n;
            ++tail_;
            ++size_;
        }
    }

    constexpr void pop_front() {
        if (size_ == 0) {
            throw std::invalid_argument("The buffer is already empty");
        } else if (tail_ == head_ + 1 && size_ == 1) {
            --size_;
        } else {
            ++head_;
            --size_;
        }
    }

    void erase(size_type index) {
        if (size_ == 0 || index >= size_) {
            throw std::invalid_argument("erase in empty container");
        } else {
            pointer new_buffer = std::allocator_traits<alloc>::allocate(allocator, capacity_);
            size_type current_position = 0;
            for (iterator it = begin(); it != end(); ++it) {
                if (it != begin()[index]) {
                    std::allocator_traits<alloc>::construct(allocator, new_buffer + current_position, *it);
                    ++current_position;
                }
            }
            --size_;
            DestructElements();
            std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
            buff_ = std::move(new_buffer);
            head_ = Iter<T>(buff_, size_);
            tail_ = head_ + current_position;
        }
    }

    void assign(iterator it1, iterator it2) {
        allocator.deallocate(buff_, capacity_);
        size_type capacity = 0;
        for (iterator it = it1; it != it2; ++it) {
            ++capacity;
        }
        size_ = capacity;
        capacity_ = capacity * kCapacityCoefficient + 1;
        buff_ = std::allocator_traits<alloc>::allocate(allocator, capacity_);
        head_ = Iter<T>(buff_, capacity_);
        tail_ = head_ + size_;
        for (iterator it = it1, other_iter = begin(); it != it2; ++it, ++other_iter) {
            std::allocator_traits<alloc>::construct(allocator, &(*other_iter), *it);
        }
    }

    void assign(value_type k, size_type amount) {
        allocator.deallocate(buff_, capacity_);
        capacity_ = amount * kCapacityCoefficient + 1;
        size_ = amount;
        buff_ = std::allocator_traits<alloc>::allocate(allocator, capacity_);
        head_ = Iter<T>(buff_, capacity_);
        tail_ = head_ + size_;
        for (iterator it = begin(); it != end(); ++it) {
            std::allocator_traits<alloc>::construct(allocator, &(*it), k);
        }
    }

    void assign(const std::initializer_list<T>& il) {
        allocator.deallocate(buff_, capacity_);
        capacity_ = il.size() + kCapacityCoefficient + 1;
        size_ = il.size();
        buff_ = std::allocator_traits<alloc>::allocate(allocator, capacity_);
        head_ = Iter<T>(buff_, capacity_);
        tail_ = head_ + size_;
        iterator it1 = begin();
        for (auto it = il.begin(); it != il.end(); ++it) {
            std::allocator_traits<alloc>::construct(allocator, &(*it1), *it);
            ++it1;
        }
    }

    iterator insert(size_type position, value_type k) {
        if (position >= size_) {
            throw std::invalid_argument("Index out of range");
        }
        if (size_ == capacity_ - 1) {
            size_type new_capacity = capacity_ * kCapacityCoefficient + 1;
            pointer new_buff = allocator.allocate(capacity_);
            size_type current_index = 0;
            iterator iter = begin();
            while (iter != end()) {
                if (current_index == position) {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, k);
                    ++current_index;
                } else {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *iter);
                    ++current_index;
                    ++iter;
                }
            }
            DestructElements();
            std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
            buff_ = std::move(new_buff);
            capacity_ = new_capacity;
            ++size_;
            head_ = Iter<T>(buff_, capacity_);
            tail_ = head_ + size_;
        } else {
            pointer new_buff = std::allocator_traits<alloc>::allocate(allocator, capacity_);
            size_type current_index = 0;
            iterator iter = begin();
            while (iter != end()) {
                if (current_index == position) {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, k);
                    ++current_index;
                } else {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *iter);
                    ++current_index;
                    ++iter;
                }
            }
            DestructElements();
            std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
            buff_ = std::move(new_buff);
            ++size_;
            head_ = Iter<T>(buff_, capacity_);
            tail_ = head_ + size_;
        }
        iterator iter = Iter<T>(buff_, &buff_[position], capacity_);
        return iter;
    }

    iterator insert(size_type position, size_type number_of_copies, value_type k) {
        if (position >= size_) {
            throw std::invalid_argument("Index out of range");
        }
        if (size_ == capacity_ - 1 || number_of_copies + size_ >= capacity_) {
            size_type new_capacity = capacity_ * kCapacityCoefficient + number_of_copies;
            pointer new_buff = std::allocator_traits<alloc>::allocate(allocator, new_capacity);
            size_type current_index = 0;
            iterator iter = begin();
            while (iter != end()) {
                if (current_index == position) {
                    for (int i = 0; i < number_of_copies; ++i) {
                        std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, k);
                        ++current_index;
                        ++size_;
                    }
                } else {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *iter);
                    ++current_index;
                    ++iter;
                }
            }
            DestructElements();
            std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
            buff_ = std::move(new_buff);
            head_ = Iter<T>(buff_, capacity_);
            tail_ = head_ + size_;
        } else {
            pointer new_buff = std::allocator_traits<alloc>::allocate(allocator, capacity_);
            size_type current_index = 0;
            iterator iter = begin();
            while (iter != end()) {
                if (current_index == position) {
                    for (int i = 0; i < number_of_copies; ++i) {
                        std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, k);
                        ++current_index;
                        ++size_;
                    }
                } else {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *iter);
                    ++current_index;
                    ++iter;
                }
            }
            DestructElements();
            std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
            buff_ = std::move(new_buff);

            head_ = Iter<T>(buff_, capacity_);
            tail_ = head_ + size_;
        }
        iterator iter = Iter<T>(buff_, &buff_[position], capacity_);
        return iter;
    }

    iterator insert(size_type position, const std::initializer_list<T>& il) {
        if (position >= size_) {
            throw std::invalid_argument("Index out of range");
        }
        if (size_ == capacity_ - 1 || il.size() + size_ >= capacity_) {
            size_type new_capacity = capacity_ * kCapacityCoefficient + il.size();
            pointer new_buff = std::allocator_traits<alloc>::allocate(allocator, new_capacity);
            size_type current_index = 0;
            iterator iter = begin();
            while (iter != end()) {
                if (current_index == position) {
                    for (auto it = il.begin(); it != il.end(); ++it) {
                        std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *it);
                        ++current_index;
                        ++size_;
                    }
                } else {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *iter);
                    ++current_index;
                    ++iter;
                }
            }
            DestructElements();
            std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
            buff_ = std::move(new_buff);
            head_ = Iter<T>(buff_, capacity_);
            tail_ = head_ + size_;
        } else {
            pointer new_buff = std::allocator_traits<alloc>::allocate(allocator, capacity_);
            size_type current_index = 0;
            iterator iter = begin();
            while (iter != end()) {
                if (current_index == position) {
                    for (auto it = il.begin(); it != il.end(); ++it) {
                        std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *it);
                        ++current_index;
                        ++size_;
                    }
                } else {
                    std::allocator_traits<alloc>::construct(allocator, new_buff + current_index, *iter);
                    ++current_index;
                    ++iter;
                }
            }
            DestructElements();
            std::allocator_traits<alloc>::deallocate(allocator, buff_, capacity_);
            buff_ = std::move(new_buff);
            head_ = Iter<T>(buff_, capacity_);
            tail_ = head_ + size_;
        }
        iterator iter = Iter<T>(buff_, &buff_[position], capacity_);
        return iter;
    }

    constexpr void swap(ExtBuffer& other) {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(buff_, other.buff_);
    }

    constexpr static void swap(ExtBuffer& first, ExtBuffer& second) {
        std::swap(first.capacity_, second.capacity_);
        std::swap(first.size_, second.size_);
        std::swap(first.head_, second.head_);
        std::swap(first.tail_, second.tail_);
        std::swap(first.buff_, second.buff_);
    }

    constexpr size_type size() {
        return size_;
    }

    void clear() {
        head_ = Iter<value_type>(buff_, capacity_);
        tail_ = ++Iter<value_type>(buff_, capacity_);
        size_ = 0;
    }

    constexpr size_type max_size() {
        return capacity_ - 1;
    }

    constexpr bool empty() {
        return size_ == 0;
    }
};

