#include <eve/core/logging/logger.hpp>
#include <eve/scripting/python_bridge.hpp>

namespace eve::scripting {

bool PythonScriptHost::initialize() {
    // Python embedding is plugin-ready; enable when CPython dev package is linked.
    available_ = false;
    EVE_LOG(Info, "Scripting", "Python bridge initialized in stub mode (plugin-ready)");
    return true;
}

void PythonScriptHost::shutdown() {
    callbacks_.clear();
    loaded_scripts_.clear();
}

bool PythonScriptHost::load_script(std::string_view path) {
    loaded_scripts_.emplace_back(path);
    EVE_LOG(Info, "Scripting", "Registered Python script: ", path);
    return true;
}

bool PythonScriptHost::call_function(std::string_view name, const std::vector<ScriptValue>& args,
                                     ScriptValue& out_result) {
    (void)args;
    out_result.type = ScriptValue::Type::Bool;
    out_result.bool_value = available_;
    EVE_LOG(Debug, "Scripting", "Python call stub: ", name);
    return true;
}

void PythonScriptHost::register_callback(std::string_view event_name,
                                         std::function<void()> callback) {
    callbacks_[std::string(event_name)] = std::move(callback);
}

void ScriptEngine::register_host(std::unique_ptr<IScriptHost> host) {
    if (host == nullptr) {
        return;
    }
    hosts_[host->language()] = std::move(host);
}

IScriptHost* ScriptEngine::host(ScriptLanguage language) const {
    const auto it = hosts_.find(language);
    if (it == hosts_.end()) {
        return nullptr;
    }
    return it->second.get();
}

void ScriptEngine::register_callback(std::string_view event_name, std::function<void()> callback) {
    callbacks_[std::string(event_name)] = std::move(callback);
}

bool ScriptEngine::load_visual_graph(const VisualScriptGraph& graph) {
    visual_graphs_.push_back(graph);
    return true;
}

bool ScriptEngine::dispatch_event(std::string_view event_name) {
    const auto it = callbacks_.find(std::string(event_name));
    if (it == callbacks_.end()) {
        if (IScriptHost* python = host(ScriptLanguage::Python)) {
            ScriptValue result;
            return python->call_function(event_name, {}, result);
        }
        return false;
    }
    it->second();
    return true;
}

} // namespace eve::scripting
