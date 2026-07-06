#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace eve::content {

class LocalizationTable {
public:
    bool load(std::string_view path);
    bool save(std::string_view path) const;
    void set_string(std::string_view key, std::string_view locale, std::string_view value);
    [[nodiscard]] std::string get(std::string_view key, std::string_view locale) const;
    [[nodiscard]] std::size_t entry_count() const;

private:
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> entries_;
};

} // namespace eve::content
