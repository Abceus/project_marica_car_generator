#pragma once
#ifdef WITH_PHYSICS
#include "LinearMath/btVector3.h"
#endif
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>

enum class Vec3Type { Coords, Size };

template <typename T>
class Vec3 {
public:
    Vec3(T x = static_cast<T>(0), T y = static_cast<T>(0),
         T z = static_cast<T>(0));

    T getX() const;
    T getY() const;
    T getZ() const;

    void setX(T x);
    void setY(T y);
    void setZ(T z);

    Vec3(const Vec3<T>& other);
    Vec3<T>& operator=(const Vec3<T>& other);

    template <typename Y>
    operator Vec3<Y>() const;

    glm::vec3 toGLVec3(Vec3Type type = Vec3Type::Coords) const;
    static Vec3<T> fromGLVec3(const glm::vec3& other);

#ifdef WITH_PHYSICS
    btVector3 toBtVec3(Vec3Type type = Vec3Type::Coords) const;
    static Vec3<T> fromBtVec3(const btVector3& other);
#endif

    Vec3<T> operator+(const Vec3<T>& other) const;
    Vec3<T> operator-(const Vec3<T>& other) const;
    Vec3<T> operator*(const Vec3<T>& other) const;
    Vec3<T> operator/(const Vec3<T>& other) const;

    Vec3<T> operator+(T factor) const;
    Vec3<T> operator-(T factor) const;
    Vec3<T> operator*(T factor) const;
    Vec3<T> operator/(T factor) const;

    bool operator==(const Vec3<T>& other) const;

    Vec3<T> operator-() const;

    T length() const;

    Vec3<T> getNormalized() const;
    Vec3<T> getCrossProduct(const Vec3<T>& other) const;
    T getTripleProduct(const Vec3<T>& b, const Vec3<T>& c) const;
    T getDotProduct(const Vec3<T>& other) const;
    T componentsSum() const;

    Vec3<T> getRotated(const Vec3<T>& axis, T degrees) const;

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
glm::vec3 Vec3<T>::toGLVec3(Vec3Type type) const {
    glm::vec3 result{static_cast<float>(y), static_cast<float>(z),
                     static_cast<float>(-x)};
    if (type == Vec3Type::Size) {
        result = glm::abs(result);
    }
    return result;
}

template <typename T>
Vec3<T> Vec3<T>::fromGLVec3(const glm::vec3& other) {
    return {-other.z, other.x, other.y};
}

#ifdef WITH_PHYSICS
template <typename T>
btVector3 Vec3<T>::toBtVec3(Vec3Type type) const {
    btVector3 result{static_cast<float>(y), static_cast<float>(z),
                     static_cast<float>(-x)};
    if (type == Vec3Type::Size) {
        result = result.absolute();
    }
    return result;
}

template <typename T>
Vec3<T> Vec3<T>::fromBtVec3(const btVector3& other) {
    return {-other.z(), other.x(), other.y()};
}
#endif

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
bool Vec3<T>::operator==(const Vec3<T>& other) const {
    return x == other.x && y == other.y && z == other.z;
}

template <typename T>
Vec3<T> Vec3<T>::operator-() const {
    return {-x, -y, -z};
}

template <typename T>
T Vec3<T>::length() const {
    return sqrt(pow(x, static_cast<T>(2)) + pow(y, static_cast<T>(2)) +
                pow(z, static_cast<T>(2)));
}

template <typename T>
Vec3<T> Vec3<T>::getNormalized() const {
    auto locLength = length();
    Vec3<T> result(*this);
    result.x /= locLength;
    result.y /= locLength;
    result.z /= locLength;
    return result;
}

template <typename T>
Vec3<T> Vec3<T>::getCrossProduct(const Vec3<T>& other) const {
    return Vec3<T>{y * other.z - other.y * z, z * other.x - other.z * x,
                   x * other.y - other.x * y};
}

template <typename T>
T Vec3<T>::getTripleProduct(const Vec3<T>& b, const Vec3<T>& c) const {
    return getDotProduct(b.getCrossProduct(c));
}

template <typename T>
T Vec3<T>::getDotProduct(const Vec3<T>& other) const {
    return (*this * other).componentsSum();
}

template <typename T>
T Vec3<T>::componentsSum() const {
    return x + y + z;
}

template <typename T>
T Vec3<T>::getX() const {
    return x;
}
template <typename T>
T Vec3<T>::getY() const {
    return y;
}
template <typename T>
T Vec3<T>::getZ() const {
    return z;
}

template <typename T>
void Vec3<T>::setX(T x_) {
    x = x_;
}
template <typename T>
void Vec3<T>::setY(T y_) {
    y = y_;
}
template <typename T>
void Vec3<T>::setZ(T z_) {
    z = z_;
}

template <typename T>
std::istream& operator>>(std::istream& is, Vec3<T>& value) {
    float x, y, z;
    is >> x >> y >> z;
    value.setX(x);
    value.setY(y);
    value.setZ(z);
    return is;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vec3<T>& value) {
    os << value.getX() << " " << value.getY() << " " << value.getZ();
    return os;
}

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;