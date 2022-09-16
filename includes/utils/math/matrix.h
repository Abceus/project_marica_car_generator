#pragma once

#include <array>
#include <glm/glm.hpp>
#include <vcruntime.h>

#include "utils/math/angle.h"
#include "utils/math/quaternion.h"
#include "utils/math/vec3.h"

template <typename T, size_t rowsAmount, size_t columnsAmount>
class Matrix {
public:
    Matrix();

    Matrix(T defaultValue);

    Matrix(const std::array<std::array<T, columnsAmount>, rowsAmount>& data);

    Matrix(std::initializer_list<std::array<T, columnsAmount>> l);

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
    Matrix<T, columnsAmount, rowsAmount> getInverted() const;

    T getDeterminant() const;
    T getMinor(size_t row, size_t column) const;

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

    static Matrix<T, 4, 4> rotate(const Matrix<T, 4, 4>& origin,
                                  const Quaternion& quaternion);

    static Vec3<T> apply(const Matrix<T, 4, 4>& matrix, const Vec3<T>& origin);

    glm::mat<columnsAmount, rowsAmount, float, glm::defaultp> getGLMat() const;

    static Matrix<T, rowsAmount, columnsAmount> fromGLMat(
        const glm::mat<columnsAmount, rowsAmount, float, glm::defaultp>& value);

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
Matrix<T, rowsAmount, columnsAmount>::Matrix(
    const std::array<std::array<T, columnsAmount>, rowsAmount>& data)
    : m_data(data) {}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, rowsAmount, columnsAmount>::Matrix(
    std::initializer_list<std::array<T, columnsAmount>> list) {
    // static_assert(l.size() == rowsAmount);
    size_t i = 0;
    for (const auto& l : list) {
        m_data[i] = l;
        ++i;
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
Matrix<T, columnsAmount, rowsAmount>
Matrix<T, rowsAmount, columnsAmount>::getInverted() const {
    Matrix<T, columnsAmount, rowsAmount> result;
    auto det = getDeterminant();
    if (det != 0) {
        for (size_t i = 0; i < rowsAmount; ++i) {
            for (size_t j = 0; j < columnsAmount; ++j) {
                result[i][j] = std::pow(-1, i + j) * getMinor(i, j);
            }
        }
        return result.getTranspose() * (1 / det);
    }
    return result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
T Matrix<T, rowsAmount, columnsAmount>::getDeterminant() const {
    static_assert(rowsAmount == columnsAmount);
    T result = 0;
    if constexpr (rowsAmount == 1) {
        return m_data[0][0];
    }
    else {
        for (size_t column = 0; column < columnsAmount; ++column) {
            result +=
                std::pow(-1, column) * m_data[0][column] * getMinor(0, column);
        }
    }
    return result;
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
T Matrix<T, rowsAmount, columnsAmount>::getMinor(size_t row,
                                                 size_t column) const {
    Matrix<T, rowsAmount - 1, columnsAmount - 1> submatrix;
    for (size_t i = 0; i < rowsAmount; ++i) {
        if (i != row) {
            size_t subrow = i > row ? i - 1 : i;
            for (size_t j = 0; j < columnsAmount; ++j) {
                if (j != column) {
                    size_t subcolumn = j > column ? j - 1 : j;
                    submatrix[subrow][subcolumn] = m_data[i][j];
                }
            }
        }
    }
    return submatrix.getDeterminant();
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
    result[0][0] = factor.getX();
    result[1][1] = factor.getY();
    result[2][2] = factor.getZ();
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
Matrix<T, 4, 4>
Matrix<T, rowsAmount, columnsAmount>::rotate(const Matrix<T, 4, 4>& origin,
                                             const Quaternion& quaternion) {
    auto x = quaternion.getX();
    auto y = quaternion.getY();
    auto z = quaternion.getZ();
    auto w = quaternion.getW();
    auto xPow = std::pow(x, 2.0f);
    auto yPow = std::pow(y, 2.0f);
    auto zPow = std::pow(z, 2.0f);
    return origin *
           Matrix<T, 4, 4>{{1 - 2 * yPow - 2 * zPow, 2 * x * y - 2 * w * z,
                            2 * x * z + 2 * w * y, 0.0f},
                           {2 * x * y + 2 * w * z, 1 - 2 * xPow - 2 * zPow,
                            2 * y * z - 2 * w * x, 0.0f},
                           {2 * x * z - 2 * w * y, 2 * y * z + 2 * w * x,
                            1 - 2 * xPow - 2 * yPow, 0.0f},
                           {0, 0, 0, 1.0f}};
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Vec3<T>
Matrix<T, rowsAmount, columnsAmount>::apply(const Matrix<T, 4, 4>& matrix,
                                            const Vec3<T>& origin) {
    Matrix<T, 4, 1> resultMatrix;
    resultMatrix[0][0] = origin.getX();
    resultMatrix[1][0] = origin.getY();
    resultMatrix[2][0] = origin.getZ();
    resultMatrix[3][0] = static_cast<T>(1);

    resultMatrix = matrix * resultMatrix;
    return Vec3<T>{resultMatrix[0][0], resultMatrix[1][0], resultMatrix[2][0]};
}

// template <typename T, size_t rowsAmount, size_t columnsAmount>
// glm::mat<columnsAmount, rowsAmount, float, glm::defaultp>
// Matrix<T, rowsAmount, columnsAmount>::getGLMat() const {
//     return {
//         m_data[0][0], m_data[1][0], m_data[2][0], m_data[3][0],
//         m_data[0][1], m_data[1][1], m_data[2][1], m_data[3][1],
//         m_data[0][2], m_data[1][2], m_data[2][2], m_data[3][2],
//         m_data[0][3], m_data[1][3], m_data[2][3], m_data[3][3],
//     };
// }

template <typename T, size_t rowsAmount, size_t columnsAmount>
glm::mat<columnsAmount, rowsAmount, float, glm::defaultp>
Matrix<T, rowsAmount, columnsAmount>::getGLMat() const {
    Matrixf44 result;
    result[2][0] = -1.0f;
    result[0][1] = 1.0f;
    result[1][2] = 1.0f;
    result[3][3] = 1.0f;
    result = result * *this;
    return {result[0][0], result[1][0], result[2][0], result[3][0],
            result[0][1], result[1][1], result[2][1], result[3][1],
            result[0][2], result[1][2], result[2][2], result[3][2],
            result[0][3], result[1][3], result[2][3], result[3][3]};
}

template <typename T, size_t rowsAmount, size_t columnsAmount>
Matrix<T, rowsAmount, columnsAmount>
Matrix<T, rowsAmount, columnsAmount>::fromGLMat(
    const glm::mat<columnsAmount, rowsAmount, float, glm::defaultp>& value) {
    Matrixf44 result;
    result[1][0] = 1.0f;
    result[2][1] = 1.0f;
    result[0][2] = -1.0f;
    result[3][3] = 1.0f;
    result = result * *this;
    return {{value[0][0], value[1][0], value[2][0], value[3][0]},
            {value[0][1], value[1][1], value[2][1], value[3][1]},
            {value[0][2], value[1][2], value[2][2], value[3][2]},
            {value[0][3], value[1][3], value[2][3], value[3][3]}};
}
