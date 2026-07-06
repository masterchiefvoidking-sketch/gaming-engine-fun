#include <eve/core/serialization/serializer.hpp>
#include <gtest/gtest.h>

TEST(SerializationTest, RoundTripsPrimitiveValues) {
    eve::Serializer writer;
    writer.set("enabled", true);
    writer.set("count", static_cast<eve::i32>(7));
    writer.set("ratio", 0.5f);
    writer.set("label", std::string("scene"));

    const std::string json = writer.to_json();
    eve::Serializer reader;
    ASSERT_TRUE(reader.from_json(json));

    const auto* enabled = reader.get("enabled");
    const auto* count = reader.get("count");
    const auto* ratio = reader.get("ratio");
    const auto* label = reader.get("label");

    ASSERT_NE(enabled, nullptr);
    ASSERT_NE(count, nullptr);
    ASSERT_NE(ratio, nullptr);
    ASSERT_NE(label, nullptr);
    EXPECT_TRUE(std::get<bool>(*enabled));
    EXPECT_EQ(std::get<eve::i32>(*count), 7);
    EXPECT_NEAR(std::get<eve::f32>(*ratio), 0.5f, 1e-5f);
    EXPECT_EQ(std::get<std::string>(*label), "scene");
}

TEST(SerializationTest, SerializesVec3) {
    eve::Serializer writer;
    writer.set("position", eve::math::Vec3{1.0f, 2.0f, 3.0f});
    const std::string json = writer.to_json();
    EXPECT_NE(json.find("\"position\""), std::string::npos);
}
