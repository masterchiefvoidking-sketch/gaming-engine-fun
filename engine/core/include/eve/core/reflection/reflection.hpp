#pragma once

#include <eve/core/math/math.hpp>
#include <eve/core/types.hpp>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace eve {

enum class FieldType : u8 {
    Bool,
    Int,
    Float,
    String,
    Vec3,
};

struct FieldDescriptor {
    std::string name;
    FieldType type = FieldType::Int;
    std::size_t offset = 0;
};

struct TypeDescriptor {
    std::string name;
    std::size_t size = 0;
    std::vector<FieldDescriptor> fields;
};

class ReflectionRegistry {
public:
    static ReflectionRegistry& instance();

    void register_type(TypeDescriptor descriptor);
    [[nodiscard]] const TypeDescriptor* find(std::string_view name) const;
    [[nodiscard]] std::vector<std::string> type_names() const;

private:
    ReflectionRegistry() = default;

    std::unordered_map<std::string, TypeDescriptor> types_;
};

#define EVE_REFLECT_FIELD(type, field)                                                             \
    ::eve::FieldDescriptor {                                                                       \
        #field, ::eve::field_type_for<decltype(std::declval<type>().field)>(),                     \
            offsetof(type, field)                                                                  \
    }

template <typename T> constexpr FieldType field_type_for();

template <> constexpr FieldType field_type_for<bool>() {
    return FieldType::Bool;
}

template <> constexpr FieldType field_type_for<i32>() {
    return FieldType::Int;
}

template <> constexpr FieldType field_type_for<f32>() {
    return FieldType::Float;
}

template <> constexpr FieldType field_type_for<std::string>() {
    return FieldType::String;
}

template <> constexpr FieldType field_type_for<math::Vec3>() {
    return FieldType::Vec3;
}

} // namespace eve
