#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <cmath>
#include <array>
using namespace std;

/*
 *
 * This is all just linear algebra operations there's nothing cool here
 * read the title of each function for a summary 
 */
class Vec3 {
public:
    float x, y, z;

    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return sqrt(lengthSquared());
    }

    float magnitude() const {
        return length();  // Alias for length()
    }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3& operator+=(const Vec3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 cross(const Vec3& other) const {
        return Vec3(
                y * other.z - z * other.y,
                z * other.x - x * other.z,
                x * other.y - y * other.x
        );
    }

    Vec3 normalize() const {
        float mag = sqrt(x * x + y * y + z * z);
        return Vec3(x / mag, y / mag, z / mag);
    }
};

class Mat3 {
public:
    array<array<float, 3>, 3> mat;

    Mat3() {
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j)
                mat[i][j] = (i == j) ? 1.0f : 0.0f;
    }

    Vec3 operator*(const Vec3& vec) const {
        return Vec3(
                mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2] * vec.z,
                mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2] * vec.z,
                mat[2][0] * vec.x + mat[2][1] * vec.y + mat[2][2] * vec.z
        );
    }

    Mat3 operator*(const Mat3& other) const {
        Mat3 result;
        for (int i = 0; i < 3; ++i)
            for (int j = 0; j < 3; ++j) {
                result.mat[i][j] = 0.0f;
                for (int k = 0; k < 3; ++k)
                    result.mat[i][j] += mat[i][k] * other.mat[k][j];
            }
        return result;
    }

    static Mat3 rotationMatrix(float angle, const Vec3& axis) {
        float cosA = cos(angle);
        float sinA = sin(angle);
        Vec3 u = axis.normalize();

        Mat3 result;
        result.mat[0][0] = cosA + u.x * u.x * (1 - cosA);
        result.mat[0][1] = u.x * u.y * (1 - cosA) - u.z * sinA;
        result.mat[0][2] = u.x * u.z * (1 - cosA) + u.y * sinA;

        result.mat[1][0] = u.y * u.x * (1 - cosA) + u.z * sinA;
        result.mat[1][1] = cosA + u.y * u.y * (1 - cosA);
        result.mat[1][2] = u.y * u.z * (1 - cosA) - u.x * sinA;

        result.mat[2][0] = u.z * u.x * (1 - cosA) - u.y * sinA;
        result.mat[2][1] = u.z * u.y * (1 - cosA) + u.x * sinA;
        result.mat[2][2] = cosA + u.z * u.z * (1 - cosA);

        return result;
    }
};

#endif // LINEAR_ALGEBRA_H
