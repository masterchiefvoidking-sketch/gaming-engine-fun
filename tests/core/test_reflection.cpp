#include <eve/core/reflection/reflection.hpp>
#include <gtest/gtest.h>

TEST(ReflectionTest, RegistersAndFindsTypes) {
    auto& registry = eve::ReflectionRegistry::instance();
    registry.register_type(eve::TypeDescriptor{
        "TestType",
        16,
        {
            {"value", eve::FieldType::Int, 0},
        },
    });

    const eve::TypeDescriptor* desc = registry.find("TestType");
    ASSERT_NE(desc, nullptr);
    EXPECT_EQ(desc->fields.size(), 1u);
    EXPECT_EQ(desc->fields[0].name, "value");
}
