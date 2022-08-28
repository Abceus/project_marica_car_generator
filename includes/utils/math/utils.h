#pragma once

#include "utils/math/vec3.h"
#include "utils/math/rot3.h"
#include "utils/math/quaternion.h"
#include "utils/math/matrix.h"

Vec3f rotate(const Vec3f& origin, const Rotor3& rot);
Quaternion getQuaternionFromMatrix(const Matrixf44& matrix);