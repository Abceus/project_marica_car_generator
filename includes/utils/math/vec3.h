#pragma once
#include <cmath>

template <typename T>
class Vec3 {
public:
    Vec3(T x = static_cast<T>(0), T y = static_cast<T>(0),
         T z = static_cast<T>(0));

    Vec3(const Vec3<T>& other);
    Vec3<T>& operator=(const Vec3<T>& other);

    template <typename Y>
    operator Vec3<Y>() const;

    Vec3<T> operator+(const Vec3<T>& other) const;
    Vec3<T> operator-(const Vec3<T>& other) const;
    Vec3<T> operator*(const Vec3<T>& other) const;
    Vec3<T> operator/(const Vec3<T>& other) const;

    Vec3<T> operator+(T factor) const;
    Vec3<T> operator-(T factor) const;
    Vec3<T> operator*(T factor) const;
    Vec3<T> operator/(T factor) const;

    T length() const;

    using type = T;

private:
    T x;
    T y;
    T z;
};

template <typename T>
Vec3<T>::Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

template <typename T>
Vec3<T>::Vec3(const Vec3<T>& other) : x(other.x), y(other.y), z(other.z) {}

template <typename T>
Vec3<T>& Vec3<T>::operator=(const Vec3<T>& other) {
    x = other.x;
    y = other.y;
    z = other.z;
    return *this;
}

template <typename T>
template <typename Y>
Vec3<T>::operator Vec3<Y>() const {
    return {static_cast<Y>(x), static_cast<Y>(y), static_cast<Y>(z)};
}

template <typename T>
Vec3<T> Vec3<T>::operator+(const Vec3<T>& other) const {
    return {x + other.x, y + other.y, z + other.z};
}

template <typename T>
Vec3<T> Vec3<T>::operator-(const Vec3<T>& other) const {
    return {x - other.x, y - other.y, z - other.z};
}

template <typename T>
Vec3<T> Vec3<T>::operator*(const Vec3<T>& other) const {
    return {x * other.x, y * other.y, z * other.z};
}

template <typename T>
Vec3<T> Vec3<T>::operator/(const Vec3<T>& other) const {
    return {x / other.x, y / other.y, z / other.z};
}

template <typename T>
Vec3<T> Vec3<T>::operator+(T factor) const {
    return {x + factor, y + factor, z + factor};
}

template <typename T>
Vec3<T> Vec3<T>::operator-(T factor) const {
    return {x - factor, y - factor, z - factor};
}

template <typename T>
Vec3<T> Vec3<T>::operator*(T factor) const {
    return {x * factor, y * factor, z * factor};
}

template <typename T>
Vec3<T> Vec3<T>::operator/(T factor) const {
    return {x / factor, y / factor, z / factor};
}

template <typename T>
T Vec3<T>::length() const {
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;