#pragma once

#include <array>

#include "utils/math/angle.h"
#include "utils/math/vec3.h"

template <typename T, size_t rowsAmount, size_t columnsAmount>
class Matrix {
public:
    Matrix();

    Matrix(T defaultValue);

    template <size_t otherRowsAmount, size_t otherColumnsAmount,
              typename Enabled = typename std::enable_if<
                  columnsAmount == otherRowsAmount, T>::type>
    Matrix<T, rowsAmount, otherColumnsAmount> operator*(
        const Matrix<T, otherRowsAmount, otherColumnsAmount>& other) const;

    Matrix<T, rowsAmount, columnsAmount> operator*(const T& other) const;

    Matrix<T, rowsAmount, columnsAmount>
    operator+(const Matrix<T, rowsAmount, columnsAmount>& other) const;

    Matrix<T, rowsAmount, columnsAmount>
    operator-(const Matrix<T, rowsAmount, columnsAmount>& other) const;

    Matrix<T, columnsAmount, rowsAmount> getTranspose() const;

    std::array<T, columnsAmount>& operator[](std::size_t idx);

    const std::array<T, columnsAmount>& operator[](std::size_t idx) const;

    template <size_t otherRowsAmount, size_t otherColumnsAmount,
              typename Enabled =
                  typename std::enable_if<columnsAmount == otherColumnsAmount &&
                                              rowsAmount == otherRowsAmount,
                                          T>::type>
    Matrix<T, rowsAmount, columnsAmount>&
    operator=(const Matrix<T, otherRowsAmount, otherColumnsAmount>& other);

    bool operator==(const Matrix<T, rowsAmount, columnsAmount>& other) const;

    bool operator!=(const Matrix<T, rowsAmount, columnsAmount>& other) const;

    static Matrix<T, 4, 4> translate(const Matrix<T, 4, 4>& origin,
                                     const Vec3<T>& offset);

    static Matrix<T, 4, 4> scale(const Matrix<T, 4, 4>& origin,
                                 const Vec3<T>& factor);

    static Matrix<T, 4, 4> rotate(const Matrix<T, 4, 4>& origin,
                                  const Vec3<T>& axis, Angle angle);

    static Vec3<T> apply(const Matrix<T, 4, 4>& matrix, const Vec3<T>& origin);

private:
    std::array<std::array<T, columnsAmount>, rowsAmount> m_data;
};

using Matrixf44 = Matrix<float, 4, 4>;

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, rowsAmount, columnsAmount>::Matrix() {
    m_data.fill(std::array<T, columnsAmount>());
    for (auto& data : m_data) {
        data.fill(0);
    }
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, rowsAmount, columnsAmount>::Matrix(T defaultValue) {
    m_data.fill(std::array<T, columnsAmount>());
    for (auto& data : m_data) {
        data.fill(0);
    }

    for (size_t i = 0; i < std::min(rowsAmount, columnsAmount); i++) {
        m_data[i][i] = defaultValue;
    }
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
std::array<T, columnsAmount>&
Matrix<T, rowsAmount, columnsAmount>::operator[](std::size_t idx) {
    return m_data[idx];
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
const std::array<T, columnsAmount>&
Matrix<T, rowsAmount, columnsAmount>::operator[](std::size_t idx) const {
    return m_data[idx];
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
template <size_t otherRowsAmount, size_t otherColumnsAmount, typename Enabled>
Matrix<T, rowsAmount, columnsAmount>&
Matrix<T, rowsAmount, columnsAmount>::operator=(
    const Matrix<T, otherRowsAmount, otherColumnsAmount>& other) {
    m_data = other.m_data;
    return *this;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
template <size_t otherRowsAmount, size_t otherColumnsAmount, typename Enabled>
Matrix<T, rowsAmount, otherColumnsAmount>
Matrix<T, rowsAmount, columnsAmount>::operator*(
    const Matrix<T, otherRowsAmount, otherColumnsAmount>& other) const {
    auto res = Matrix<T, rowsAmount, otherColumnsAmount>();
    for (size_t j = 0; j < rowsAmount; j++) {
        for (size_t i = 0; i < otherColumnsAmount; i++) {
            for (size_t k = 0; k < columnsAmount; k++) {
                res[j][i] += m_data[j][k] * other[k][i];
            }
        }
    }
    return res;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, rowsAmount, columnsAmount>
Matrix<T, rowsAmount, columnsAmount>::operator*(const T& other) const {
    Matrix<T, rowsAmount, columnsAmount> result;
    for (int i = 0; i < rowsAmount; i++) {
        for (int j = 0; j < rowsAmount; j++) {
            result[j][i] = m_data[j][i] * other;
        }
    }
    return result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, rowsAmount, columnsAmount>
Matrix<T, rowsAmount, columnsAmount>::operator+(
    const Matrix<T, rowsAmount, columnsAmount>& other) const {
    Matrix<T, rowsAmount, columnsAmount> result;
    for (int i = 0; i < rowsAmount; i++) {
        for (int j = 0; j < rowsAmount; j++) {
            result[j][i] = m_data[j][i] + other[j][i];
        }
    }
    return result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, rowsAmount, columnsAmount>
Matrix<T, rowsAmount, columnsAmount>::operator-(
    const Matrix<T, rowsAmount, columnsAmount>& other) const {
    Matrix<T, rowsAmount, columnsAmount> result;
    for (int i = 0; i < rowsAmount; i++) {
        for (int j = 0; j < rowsAmount; j++) {
            result[j][i] = m_data[j][i] - other[j][i];
        }
    }
    return result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, columnsAmount, rowsAmount>
Matrix<T, rowsAmount, columnsAmount>::getTranspose() const {
    Matrix<T, columnsAmount, rowsAmount> result;
    for (int i = 0; i < rowsAmount; i++) {
        for (int j = 0; j < rowsAmount; j++) {
            result[i][j] = m_data[j][i];
        }
    }
    return result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
bool Matrix<T, rowsAmount, columnsAmount>::operator==(
    const Matrix<T, rowsAmount, columnsAmount>& other) const {
    return m_data == other.m_data;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
bool Matrix<T, rowsAmount, columnsAmount>::operator!=(
    const Matrix<T, rowsAmount, columnsAmount>& other) const {
    return !(*this == other);
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, 4, 4>
Matrix<T, rowsAmount, columnsAmount>::translate(const Matrix<T, 4, 4>& origin,
                                                const Vec3<T>& offset) {
    Matrix<T, 4, 4> result(1);

    result[0][3] = offset.getX();
    result[1][3] = offset.getY();
    result[2][3] = offset.getZ();

    return origin * result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, 4, 4>
Matrix<T, rowsAmount, columnsAmount>::scale(const Matrix<T, 4, 4>& origin,
                                            const Vec3<T>& factor) {
    Matrix<T, 4, 4> result(1);
    result[0][0] = factor.x;
    result[1][1] = factor.y;
    result[2][3] = factor.z;
    return origin * result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, 4, 4>
Matrix<T, rowsAmount, columnsAmount>::rotate(const Matrix<T, 4, 4>& origin,
                                             const Vec3<T>& axis, Angle angle) {
    Matrix<T, 4, 4> result(1);
    auto rad = angle.getRadians();
    auto sin = std::sin(rad);
    auto cos = std::cos(rad);

    result[0][0] = cos + std::pow(axis.getX(), 2.0f) * (1.0f - cos);
    result[0][1] = axis.getX() * axis.getY() * (1.0f - cos) - axis.getZ() * sin;
    result[0][2] = axis.getX() * axis.getZ() * (1.0f - cos) + axis.getY() * sin;

    result[1][0] = axis.getY() * axis.getX() * (1.0f - cos) + axis.getZ() * sin;
    result[1][1] = cos + std::pow(axis.getY(), 2.0f) * (1.0f - cos);
    result[1][2] = axis.getY() * axis.getZ() * (1.0f - cos) - axis.getX() * sin;

    result[2][0] = axis.getZ() * axis.getX() * (1.0f - cos) - axis.getY() * sin;
    result[2][1] = axis.getZ() * axis.getY() * (1.0f - cos) + axis.getX() * sin;
    result[2][2] = cos + std::pow(axis.getZ(), 2.0f) * (1.0f - cos);

    return origin * result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Vec3<T> Matrix<T, rowsAmount, columnsAmount>::apply(const Matrix<T, 4, 4>& matrix, const Vec3<T>& origin) {
    Matrix<T, 4, 1> resultMatrix;
    resultMatrix[0][0] = origin.getX();
    resultMatrix[1][0] = origin.getY();
    resultMatrix[2][0] = origin.getZ();
    resultMatrix[3][0] = static_cast<T>(1);

    resultMatrix = matrix * resultMatrix;
    return Vec3<T>{resultMatrix[0][0], resultMatrix[1][0], resultMatrix[2][0]};
}