#pragma once

#include <eve/scripting/script_host.hpp>

namespace eve::scripting {

class PythonScriptHost final : public IScriptHost {
public:
    ScriptLanguage language() const override { return ScriptLanguage::Python; }
    bool initialize() override;
    void shutdown() override;
    bool load_script(std::string_view path) override;
    bool call_function(std::string_view name, const std::vector<ScriptValue>& args,
                       ScriptValue& out_result) override;
    void register_callback(std::string_view event_name, std::function<void()> callback) override;

    [[nodiscard]] bool is_available() const { return available_; }

private:
    bool available_ = false;
    std::unordered_map<std::string, std::function<void()>> callbacks_;
    std::vector<std::string> loaded_scripts_;
};

} // namespace eve::scripting
