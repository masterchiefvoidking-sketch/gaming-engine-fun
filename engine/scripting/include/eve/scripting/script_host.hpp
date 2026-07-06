#pragma once

#include <eve/core/types.hpp>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace eve::scripting {

enum class ScriptLanguage : u8 {
    Python = 0,
    Visual,
    Native,
};

struct ScriptValue {
    enum class Type : u8 { Null, Bool, Int, Float, String };
    Type type = Type::Null;
    bool bool_value = false;
    i32 int_value = 0;
    f32 float_value = 0.0f;
    std::string string_value;
};

struct VisualScriptNode {
    std::string id;
    std::string type;
    std::unordered_map<std::string, ScriptValue> properties;
    std::vector<std::string> inputs;
    std::vector<std::string> outputs;
};

struct VisualScriptGraph {
    std::string id;
    std::string name;
    std::vector<VisualScriptNode> nodes;
    std::vector<std::pair<std::string, std::string>> links;
};

class IScriptHost {
public:
    virtual ~IScriptHost() = default;
    virtual ScriptLanguage language() const = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
    virtual bool load_script(std::string_view path) = 0;
    virtual bool call_function(std::string_view name, const std::vector<ScriptValue>& args,
                               ScriptValue& out_result) = 0;
    virtual void register_callback(std::string_view event_name, std::function<void()> callback) = 0;
};

class ScriptEngine {
public:
    void register_host(std::unique_ptr<IScriptHost> host);
    void register_callback(std::string_view event_name, std::function<void()> callback);
    IScriptHost* host(ScriptLanguage language) const;
    bool load_visual_graph(const VisualScriptGraph& graph);
    bool dispatch_event(std::string_view event_name);

private:
    std::unordered_map<ScriptLanguage, std::unique_ptr<IScriptHost>> hosts_;
    std::unordered_map<std::string, std::function<void()>> callbacks_;
    std::vector<VisualScriptGraph> visual_graphs_;
};

} // namespace eve::scripting
