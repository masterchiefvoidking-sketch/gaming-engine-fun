#include <eve/core/logging/logger.hpp>
#include <eve/core/plugin/plugin_manager.hpp>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace eve {

namespace {

void* load_dynamic_library(std::string_view path) {
#if defined(_WIN32)
    return LoadLibraryA(std::string(path).c_str());
#else
    return dlopen(std::string(path).c_str(), RTLD_LAZY);
#endif
}

void* get_symbol(void* handle, const char* name) {
#if defined(_WIN32)
    return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(handle), name));
#else
    return dlsym(handle, name);
#endif
}

void unload_dynamic_library(void* handle) {
#if defined(_WIN32)
    FreeLibrary(static_cast<HMODULE>(handle));
#else
    dlclose(handle);
#endif
}

} // namespace

PluginManager& PluginManager::instance() {
    static PluginManager manager;
    return manager;
}

bool PluginManager::load_plugin(std::string_view path) {
    void* handle = load_dynamic_library(path);
    if (handle == nullptr) {
        EVE_LOG(Error, "Plugin", "Failed to load plugin: ", path);
        return false;
    }

    auto* create = reinterpret_cast<PluginCreateFn>(get_symbol(handle, "eve_plugin_create"));
    auto* destroy = reinterpret_cast<PluginDestroyFn>(get_symbol(handle, "eve_plugin_destroy"));
    if (create == nullptr || destroy == nullptr) {
        unload_dynamic_library(handle);
        EVE_LOG(Error, "Plugin", "Plugin missing entry points: ", path);
        return false;
    }

    IPlugin* instance = create();
    if (instance == nullptr) {
        unload_dynamic_library(handle);
        return false;
    }

    instance->on_load();
    const std::string name(instance->name());
    LoadedPlugin loaded{
        handle,
        std::unique_ptr<IPlugin, PluginDestroyFn>(instance, destroy),
        PluginDescriptor{std::string(path), name, create, destroy},
    };
    plugins_.emplace(name, std::move(loaded));
    EVE_LOG(Info, "Plugin", "Loaded plugin: ", name);
    return true;
}

void PluginManager::unload_plugin(std::string_view name) {
    const auto it = plugins_.find(std::string(name));
    if (it == plugins_.end()) {
        return;
    }
    it->second.instance->on_unload();
    unload_dynamic_library(it->second.handle);
    plugins_.erase(it);
    EVE_LOG(Info, "Plugin", "Unloaded plugin: ", name);
}

void PluginManager::unload_all() {
    for (auto& [name, plugin] : plugins_) {
        plugin.instance->on_unload();
        unload_dynamic_library(plugin.handle);
        (void)name;
    }
    plugins_.clear();
}

IPlugin* PluginManager::get(std::string_view name) const {
    const auto it = plugins_.find(std::string(name));
    if (it == plugins_.end()) {
        return nullptr;
    }
    return it->second.instance.get();
}

std::vector<std::string> PluginManager::loaded_plugins() const {
    std::vector<std::string> names;
    names.reserve(plugins_.size());
    for (const auto& [name, _] : plugins_) {
        names.push_back(name);
    }
    return names;
}

} // namespace eve
