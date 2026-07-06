#pragma once

#include <eve/core/types.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace eve {

class IPlugin {
public:
    virtual ~IPlugin() = default;
    virtual std::string_view name() const = 0;
    virtual void on_load() = 0;
    virtual void on_unload() = 0;
};

using PluginCreateFn = IPlugin* (*)();
using PluginDestroyFn = void (*)(IPlugin*);

struct PluginDescriptor {
    std::string path;
    std::string name;
    PluginCreateFn create = nullptr;
    PluginDestroyFn destroy = nullptr;
};

class PluginManager {
public:
    static PluginManager& instance();

    bool load_plugin(std::string_view path);
    void unload_plugin(std::string_view name);
    void unload_all();

    [[nodiscard]] IPlugin* get(std::string_view name) const;
    [[nodiscard]] std::vector<std::string> loaded_plugins() const;

private:
    struct LoadedPlugin {
        void* handle = nullptr;
        std::unique_ptr<IPlugin, PluginDestroyFn> instance{nullptr, nullptr};
        PluginDescriptor descriptor;
    };

    std::unordered_map<std::string, LoadedPlugin> plugins_;
};

} // namespace eve
