#include <algorithm>
#include <eve/core/reflection/reflection.hpp>

namespace eve {

ReflectionRegistry& ReflectionRegistry::instance() {
    static ReflectionRegistry registry;
    return registry;
}

void ReflectionRegistry::register_type(TypeDescriptor descriptor) {
    types_.emplace(descriptor.name, std::move(descriptor));
}

const TypeDescriptor* ReflectionRegistry::find(std::string_view name) const {
    const auto it = types_.find(std::string(name));
    if (it == types_.end()) {
        return nullptr;
    }
    return &it->second;
}

std::vector<std::string> ReflectionRegistry::type_names() const {
    std::vector<std::string> names;
    names.reserve(types_.size());
    for (const auto& [name, _] : types_) {
        names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

} // namespace eve
