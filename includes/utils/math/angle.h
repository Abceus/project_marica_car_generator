#pragma once

class Angle {
public:
    Angle() = default;
    Angle(const Angle& other);
    Angle& operator=(const Angle& other);

    Angle operator+(const Angle& other) const;
    Angle operator-(const Angle& other) const;
    Angle operator*(const Angle& other) const;
    Angle operator/(const Angle& other) const;
    Angle operator/(int other) const;

    bool operator==(const Angle& other) const;
    bool operator>(const Angle& other) const;
    bool operator>=(const Angle& other) const;
    bool operator<(const Angle& other) const;
    bool operator<=(const Angle& other) const;

    void setDegrees(float degrees);
    float getDegrees() const;

    void setRadians(float radians);
    float getRadians() const;

    static Angle fromDegrees(float degrees);
    static Angle fromRadians(float radians);

private:
    float degrees = 0.0f;

    void normalize();
};