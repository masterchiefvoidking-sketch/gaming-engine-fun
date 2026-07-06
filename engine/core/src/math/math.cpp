#include <algorithm>
#include <cmath>
#include <eve/core/math/math.hpp>

namespace eve::math {

namespace {

constexpr f32 kEpsilon = 1e-6f;

} // namespace

Vec2 Vec2::normalized() const {
    const f32 len = length();
    if (len <= kEpsilon) {
        return {};
    }
    return *this * (1.0f / len);
}

Vec3 Vec3::cross(const Vec3& other) const {
    return {
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x,
    };
}

Vec3 Vec3::normalized() const {
    const f32 len = length();
    if (len <= kEpsilon) {
        return {};
    }
    return *this * (1.0f / len);
}

Quat Quat::from_axis_angle(const Vec3& axis, f32 radians) {
    const Vec3 normalized_axis = axis.normalized();
    const f32 half = radians * 0.5f;
    const f32 s = std::sin(half);
    return {
        normalized_axis.x * s,
        normalized_axis.y * s,
        normalized_axis.z * s,
        std::cos(half),
    };
}

Quat Quat::slerp(const Quat& a, const Quat& b, f32 t) {
    f32 dot = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    Quat end = b;
    if (dot < 0.0f) {
        end = {-b.x, -b.y, -b.z, -b.w};
        dot = -dot;
    }

    if (dot > 0.9995f) {
        return Quat{
            a.x + t * (end.x - a.x),
            a.y + t * (end.y - a.y),
            a.z + t * (end.z - a.z),
            a.w + t * (end.w - a.w),
        }
            .normalized();
    }

    const f32 theta = std::acos(dot);
    const f32 sin_theta = std::sin(theta);
    const f32 wa = std::sin((1.0f - t) * theta) / sin_theta;
    const f32 wb = std::sin(t * theta) / sin_theta;
    return {
        wa * a.x + wb * end.x,
        wa * a.y + wb * end.y,
        wa * a.z + wb * end.z,
        wa * a.w + wb * end.w,
    };
}

Quat Quat::operator*(const Quat& other) const {
    return {
        w * other.x + x * other.w + y * other.z - z * other.y,
        w * other.y - x * other.z + y * other.w + z * other.x,
        w * other.z + x * other.y - y * other.x + z * other.w,
        w * other.w - x * other.x - y * other.y - z * other.z,
    };
}

Vec3 Quat::rotate(const Vec3& vector) const {
    const Quat qv{vector.x, vector.y, vector.z, 0.0f};
    const Quat result = (*this) * qv * Quat{-x, -y, -z, w};
    return {result.x, result.y, result.z};
}

Quat Quat::normalized() const {
    const f32 len = std::sqrt(x * x + y * y + z * z + w * w);
    if (len <= kEpsilon) {
        return identity();
    }
    return {x / len, y / len, z / len, w / len};
}

Mat4 Mat4::identity() {
    return {};
}

Mat4 Mat4::translation(const Vec3& t) {
    Mat4 result = identity();
    result.m[12] = t.x;
    result.m[13] = t.y;
    result.m[14] = t.z;
    return result;
}

Mat4 Mat4::scale(const Vec3& s) {
    Mat4 result = identity();
    result.m[0] = s.x;
    result.m[5] = s.y;
    result.m[10] = s.z;
    return result;
}

Mat4 Mat4::rotation(const Quat& q) {
    const Quat n = q.normalized();
    const f32 xx = n.x * n.x;
    const f32 yy = n.y * n.y;
    const f32 zz = n.z * n.z;
    const f32 xy = n.x * n.y;
    const f32 xz = n.x * n.z;
    const f32 yz = n.y * n.z;
    const f32 wx = n.w * n.x;
    const f32 wy = n.w * n.y;
    const f32 wz = n.w * n.z;

    Mat4 result = identity();
    result.m[0] = 1.0f - 2.0f * (yy + zz);
    result.m[1] = 2.0f * (xy + wz);
    result.m[2] = 2.0f * (xz - wy);
    result.m[4] = 2.0f * (xy - wz);
    result.m[5] = 1.0f - 2.0f * (xx + zz);
    result.m[6] = 2.0f * (yz + wx);
    result.m[8] = 2.0f * (xz + wy);
    result.m[9] = 2.0f * (yz - wx);
    result.m[10] = 1.0f - 2.0f * (xx + yy);
    return result;
}

Mat4 Mat4::perspective(f32 fov_y_radians, f32 aspect, f32 near_z, f32 far_z) {
    const f32 tan_half_fov = std::tan(fov_y_radians * 0.5f);
    Mat4 result{};
    result.m[0] = 1.0f / (aspect * tan_half_fov);
    result.m[5] = -1.0f / tan_half_fov;
    result.m[10] = far_z / (near_z - far_z);
    result.m[11] = -1.0f;
    result.m[14] = (far_z * near_z) / (near_z - far_z);
    return result;
}

Mat4 Mat4::look_at(const Vec3& eye, const Vec3& target, const Vec3& up) {
    const Vec3 f = (target - eye).normalized();
    const Vec3 s = f.cross(up).normalized();
    const Vec3 u = s.cross(f);

    Mat4 result = identity();
    result.m[0] = s.x;
    result.m[4] = s.y;
    result.m[8] = s.z;
    result.m[1] = u.x;
    result.m[5] = u.y;
    result.m[9] = u.z;
    result.m[2] = -f.x;
    result.m[6] = -f.y;
    result.m[10] = -f.z;
    result.m[12] = -s.dot(eye);
    result.m[13] = -u.dot(eye);
    result.m[14] = f.dot(eye);
    return result;
}

Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result{};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            f32 sum = 0.0f;
            for (int k = 0; k < 4; ++k) {
                sum += m[row + k * 4] * other.m[k + col * 4];
            }
            result.m[row + col * 4] = sum;
        }
    }
    return result;
}

Vec4 Mat4::transform_point(const Vec4& point) const {
    return {
        m[0] * point.x + m[4] * point.y + m[8] * point.z + m[12] * point.w,
        m[1] * point.x + m[5] * point.y + m[9] * point.z + m[13] * point.w,
        m[2] * point.x + m[6] * point.y + m[10] * point.z + m[14] * point.w,
        m[3] * point.x + m[7] * point.y + m[11] * point.z + m[15] * point.w,
    };
}

AABB AABB::from_center_extents(const Vec3& center, const Vec3& extents) {
    return {
        center - extents,
        center + extents,
    };
}

bool AABB::contains(const Vec3& point) const {
    return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y &&
           point.z >= min.z && point.z <= max.z;
}

bool AABB::intersects(const AABB& other) const {
    return min.x <= other.max.x && max.x >= other.min.x && min.y <= other.max.y &&
           max.y >= other.min.y && min.z <= other.max.z && max.z >= other.min.z;
}

std::ostream& operator<<(std::ostream& os, const Vec3& v) {
    os << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

} // namespace eve::math
