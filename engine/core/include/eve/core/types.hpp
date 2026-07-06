#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <string_view>

namespace eve {

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;

using EntityId = u32;
using ComponentTypeId = u32;
using SystemId = u32;

constexpr EntityId kInvalidEntity = 0;
constexpr ComponentTypeId kInvalidComponentType = 0;

constexpr const char* kEngineName = "EVE Adult Engine";
constexpr const char* kEngineVersion = "0.1.0";

} // namespace eve
