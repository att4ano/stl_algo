#include <iostream>

template<typename T>
class XrangeInterface {
public:
    virtual size_t operator-(const T& other);

    virtual bool operator==(const T& other) const;

    virtual bool operator>(const T& other) const;

    virtual bool operator<(const T& other) const;

    virtual bool operator>=(const T& other) const;

    virtual bool operator<=(const T& other) const;

    virtual bool operator!=(const T& other) const;

    virtual T& operator++();

    virtual T& operator+=(const T& other);

    virtual T& operator+=(int other);

    virtual T& operator=(int other);
};

