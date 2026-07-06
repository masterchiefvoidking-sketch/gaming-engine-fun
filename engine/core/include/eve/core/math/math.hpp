#pragma once

#include <cmath>
#include <eve/core/types.hpp>
#include <ostream>

namespace eve::math {

struct Vec2 {
    f32 x = 0.0f;
    f32 y = 0.0f;

    Vec2() = default;
    Vec2(f32 x_, f32 y_)
        : x(x_)
        , y(y_) {}

    Vec2 operator+(const Vec2& other) const { return {x + other.x, y + other.y}; }
    Vec2 operator-(const Vec2& other) const { return {x - other.x, y - other.y}; }
    Vec2 operator*(f32 scalar) const { return {x * scalar, y * scalar}; }

    f32 dot(const Vec2& other) const { return x * other.x + y * other.y; }
    f32 length() const { return std::sqrt(dot(*this)); }
    Vec2 normalized() const;
};

struct Vec3 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;

    Vec3() = default;
    Vec3(f32 x_, f32 y_, f32 z_)
        : x(x_)
        , y(y_)
        , z(z_) {}

    Vec3 operator+(const Vec3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vec3 operator-(const Vec3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Vec3 operator*(f32 scalar) const { return {x * scalar, y * scalar, z * scalar}; }

    f32 dot(const Vec3& other) const { return x * other.x + y * other.y + z * other.z; }
    Vec3 cross(const Vec3& other) const;
    f32 length() const { return std::sqrt(dot(*this)); }
    Vec3 normalized() const;
};

struct Vec4 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 w = 0.0f;

    Vec4() = default;
    Vec4(f32 x_, f32 y_, f32 z_, f32 w_)
        : x(x_)
        , y(y_)
        , z(z_)
        , w(w_) {}
    explicit Vec4(const Vec3& v, f32 w_ = 1.0f)
        : x(v.x)
        , y(v.y)
        , z(v.z)
        , w(w_) {}
};

struct Quat {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 w = 1.0f;

    static Quat identity() { return {}; }
    static Quat from_axis_angle(const Vec3& axis, f32 radians);
    static Quat slerp(const Quat& a, const Quat& b, f32 t);

    Quat operator*(const Quat& other) const;
    Vec3 rotate(const Vec3& vector) const;
    Quat normalized() const;
};

struct Mat4 {
    f32 m[16] = {
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    static Mat4 identity();
    static Mat4 translation(const Vec3& t);
    static Mat4 scale(const Vec3& s);
    static Mat4 rotation(const Quat& q);
    static Mat4 perspective(f32 fov_y_radians, f32 aspect, f32 near_z, f32 far_z);
    static Mat4 look_at(const Vec3& eye, const Vec3& target, const Vec3& up);

    Mat4 operator*(const Mat4& other) const;
    Vec4 transform_point(const Vec4& point) const;
};

struct AABB {
    Vec3 min{};
    Vec3 max{};

    static AABB from_center_extents(const Vec3& center, const Vec3& extents);
    bool contains(const Vec3& point) const;
    bool intersects(const AABB& other) const;
};

std::ostream& operator<<(std::ostream& os, const Vec3& v);

} // namespace eve::math
