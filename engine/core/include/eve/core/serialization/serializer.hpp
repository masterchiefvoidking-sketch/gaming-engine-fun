#pragma once

#include <eve/core/reflection/reflection.hpp>
#include <sstream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>

namespace eve {

using SerializedValue = std::variant<bool, i32, f32, std::string, math::Vec3>;

class Serializer {
public:
    void set(std::string key, SerializedValue value);
    [[nodiscard]] bool has(std::string_view key) const;
    [[nodiscard]] const SerializedValue* get(std::string_view key) const;
    [[nodiscard]] std::string to_json() const;
    bool from_json(std::string_view json);

    template <typename T> bool serialize_object(const T& object, const TypeDescriptor& descriptor);

    template <typename T>
    bool deserialize_object(T& object, const TypeDescriptor& descriptor) const;

private:
    std::unordered_map<std::string, SerializedValue> values_;
};

template <typename T>
bool Serializer::serialize_object(const T& object, const TypeDescriptor& descriptor) {
    const auto* bytes = reinterpret_cast<const u8*>(&object);
    for (const FieldDescriptor& field : descriptor.fields) {
        switch (field.type) {
        case FieldType::Bool: {
            const bool value = *reinterpret_cast<const bool*>(bytes + field.offset);
            set(field.name, value);
            break;
        }
        case FieldType::Int: {
            const i32 value = *reinterpret_cast<const i32*>(bytes + field.offset);
            set(field.name, value);
            break;
        }
        case FieldType::Float: {
            const f32 value = *reinterpret_cast<const f32*>(bytes + field.offset);
            set(field.name, value);
            break;
        }
        case FieldType::String: {
            const auto& value = *reinterpret_cast<const std::string*>(bytes + field.offset);
            set(field.name, value);
            break;
        }
        case FieldType::Vec3: {
            const auto& value = *reinterpret_cast<const math::Vec3*>(bytes + field.offset);
            set(field.name, value);
            break;
        }
        }
    }
    return true;
}

template <typename T>
bool Serializer::deserialize_object(T& object, const TypeDescriptor& descriptor) const {
    auto* bytes = reinterpret_cast<u8*>(&object);
    for (const FieldDescriptor& field : descriptor.fields) {
        const SerializedValue* value = get(field.name);
        if (value == nullptr) {
            continue;
        }
        switch (field.type) {
        case FieldType::Bool:
            if (const auto* v = std::get_if<bool>(value)) {
                *reinterpret_cast<bool*>(bytes + field.offset) = *v;
            }
            break;
        case FieldType::Int:
            if (const auto* v = std::get_if<i32>(value)) {
                *reinterpret_cast<i32*>(bytes + field.offset) = *v;
            }
            break;
        case FieldType::Float:
            if (const auto* v = std::get_if<f32>(value)) {
                *reinterpret_cast<f32*>(bytes + field.offset) = *v;
            }
            break;
        case FieldType::String:
            if (const auto* v = std::get_if<std::string>(value)) {
                *reinterpret_cast<std::string*>(bytes + field.offset) = *v;
            }
            break;
        case FieldType::Vec3:
            if (const auto* v = std::get_if<math::Vec3>(value)) {
                *reinterpret_cast<math::Vec3*>(bytes + field.offset) = *v;
            }
            break;
        }
    }
    return true;
}

} // namespace eve
