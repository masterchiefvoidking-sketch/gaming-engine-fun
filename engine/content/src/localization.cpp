#include <eve/content/localization.hpp>
#include <eve/core/filesystem/filesystem.hpp>

#include <nlohmann/json.hpp>

namespace eve::content {

bool LocalizationTable::load(std::string_view path) {
    const std::string content = FileSystem::read_text_file(path);
    if (content.empty()) {
        return false;
    }
    try {
        const auto json = nlohmann::json::parse(content);
        for (const auto& [key, locales] : json.items()) {
            for (const auto& [locale, value] : locales.items()) {
                entries_[key][locale] = value.get<std::string>();
            }
        }
        return true;
    } catch (const nlohmann::json::exception&) {
        return false;
    }
}

bool LocalizationTable::save(std::string_view path) const {
    nlohmann::json json;
    for (const auto& [key, locales] : entries_) {
        for (const auto& [locale, value] : locales) {
            json[key][locale] = value;
        }
    }
    return FileSystem::write_text_file(path, json.dump(2));
}

void LocalizationTable::set_string(std::string_view key, std::string_view locale,
                                   std::string_view value) {
    entries_[std::string(key)][std::string(locale)] = std::string(value);
}

std::string LocalizationTable::get(std::string_view key, std::string_view locale) const {
    const auto key_it = entries_.find(std::string(key));
    if (key_it == entries_.end()) {
        return std::string(key);
    }
    const auto locale_it = key_it->second.find(std::string(locale));
    if (locale_it == key_it->second.end()) {
        return std::string(key);
    }
    return locale_it->second;
}

std::size_t LocalizationTable::entry_count() const {
    return entries_.size();
}

} // namespace eve::content
