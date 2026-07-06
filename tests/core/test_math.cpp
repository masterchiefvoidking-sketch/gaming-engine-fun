#include <cmath>
#include <eve/core/math/math.hpp>
#include <gtest/gtest.h>

using namespace eve::math;

TEST(MathTest, Vec3Normalization) {
    const Vec3 v{3.0f, 0.0f, 4.0f};
    const Vec3 n = v.normalized();
    EXPECT_NEAR(n.length(), 1.0f, 1e-5f);
}

TEST(MathTest, QuaternionRotation) {
    const Quat q = Quat::from_axis_angle({0.0f, 1.0f, 0.0f}, 3.14159265f / 2.0f);
    const Vec3 rotated = q.rotate({1.0f, 0.0f, 0.0f});
    EXPECT_NEAR(rotated.x, 0.0f, 1e-3f);
    EXPECT_NEAR(std::abs(rotated.z), 1.0f, 1e-3f);
}

TEST(MathTest, MatrixPerspective) {
    const Mat4 proj = Mat4::perspective(1.0f, 16.0f / 9.0f, 0.1f, 100.0f);
    EXPECT_NE(proj.m[0], 0.0f);
    EXPECT_NE(proj.m[5], 0.0f);
}

TEST(MathTest, AABBIntersection) {
    const AABB a = AABB::from_center_extents({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
    const AABB b = AABB::from_center_extents({1.5f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f});
    EXPECT_TRUE(a.intersects(b));
}
