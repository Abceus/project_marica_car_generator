#pragma once
#include "utils/math/angle.h"
#include "utils/math/vec3.h"

class Rotor3 {
public:
    Rotor3(float pitchDegrees, float yawDegrees, float rollDegrees);

    Rotor3(Angle pitch = Angle(), Angle yaw = Angle(), Angle roll = Angle());

    Angle getPitch() const;
    Angle getYaw() const;
    Angle getRoll() const;

    void setPitch(Angle pitch);
    void setYaw(Angle yaw);
    void setRoll(Angle roll);

    glm::vec3 toGLVec3() const;
    static Rotor3 fromGLVec3(const glm::vec3& other);

    Rotor3(const Rotor3& other);
    Rotor3& operator=(const Rotor3& other);

    Rotor3 operator+(const Rotor3& other) const;
    Rotor3 operator-(const Rotor3& other) const;
    Rotor3 operator*(const Rotor3& other) const;
    Rotor3 operator/(const Rotor3& other) const;

    Rotor3 operator+(Angle factor) const;
    Rotor3 operator-(Angle factor) const;
    Rotor3 operator*(Angle factor) const;
    Rotor3 operator/(Angle factor) const;

    bool operator ==(const Rotor3& other) const;

    // Vec3<T> rotate(const Vec3<T>& origin) const;

private:
    Angle pitch;
    Angle yaw;
    Angle roll;
};
