#include <cctype>
#include <eve/core/serialization/serializer.hpp>
#include <sstream>
#include <stdexcept>

namespace eve {

namespace {

std::string escape_json(std::string_view input) {
    std::ostringstream out;
    for (const char ch : input) {
        switch (ch) {
        case '"':
            out << "\\\"";
            break;
        case '\\':
            out << "\\\\";
            break;
        case '\n':
            out << "\\n";
            break;
        case '\r':
            out << "\\r";
            break;
        case '\t':
            out << "\\t";
            break;
        default:
            out << ch;
            break;
        }
    }
    return out.str();
}

std::string serialize_value(const SerializedValue& value) {
    if (const auto* v = std::get_if<bool>(&value)) {
        return *v ? "true" : "false";
    }
    if (const auto* v = std::get_if<i32>(&value)) {
        return std::to_string(*v);
    }
    if (const auto* v = std::get_if<f32>(&value)) {
        return std::to_string(*v);
    }
    if (const auto* v = std::get_if<std::string>(&value)) {
        return "\"" + escape_json(*v) + "\"";
    }
    if (const auto* v = std::get_if<math::Vec3>(&value)) {
        std::ostringstream out;
        out << "{\"x\":" << v->x << ",\"y\":" << v->y << ",\"z\":" << v->z << "}";
        return out.str();
    }
    return "null";
}

void skip_ws(std::string_view json, std::size_t& pos) {
    while (pos < json.size() && std::isspace(static_cast<unsigned char>(json[pos]))) {
        ++pos;
    }
}

std::string parse_string(std::string_view json, std::size_t& pos) {
    if (json[pos] != '"') {
        throw std::runtime_error("Expected string in JSON");
    }
    ++pos;
    std::ostringstream out;
    while (pos < json.size()) {
        const char ch = json[pos++];
        if (ch == '"') {
            break;
        }
        if (ch == '\\' && pos < json.size()) {
            const char esc = json[pos++];
            switch (esc) {
            case '"':
                out << '"';
                break;
            case '\\':
                out << '\\';
                break;
            case 'n':
                out << '\n';
                break;
            case 'r':
                out << '\r';
                break;
            case 't':
                out << '\t';
                break;
            default:
                out << esc;
                break;
            }
            continue;
        }
        out << ch;
    }
    return out.str();
}

SerializedValue parse_value(std::string_view json, std::size_t& pos);

SerializedValue parse_object(std::string_view json, std::size_t& pos) {
    math::Vec3 vec{};
    if (json.compare(pos, 3, "\"x\"") == 0 || json[pos] == '"') {
        // Not a vec3 object in this minimal parser branch.
    }
    (void)vec;
    throw std::runtime_error("Unsupported JSON object value");
}

SerializedValue parse_value(std::string_view json, std::size_t& pos) {
    skip_ws(json, pos);
    if (pos >= json.size()) {
        throw std::runtime_error("Unexpected end of JSON");
    }

    const char ch = json[pos];
    if (ch == '"') {
        return parse_string(json, pos);
    }
    if (ch == 't' && json.substr(pos, 4) == "true") {
        pos += 4;
        return true;
    }
    if (ch == 'f' && json.substr(pos, 5) == "false") {
        pos += 5;
        return false;
    }
    if (ch == '{') {
        ++pos;
        math::Vec3 vec{};
        skip_ws(json, pos);
        while (pos < json.size() && json[pos] != '}') {
            const std::string key = parse_string(json, pos);
            skip_ws(json, pos);
            if (json[pos] != ':') {
                throw std::runtime_error("Expected ':' in JSON object");
            }
            ++pos;
            skip_ws(json, pos);
            if (key == "x" || key == "y" || key == "z") {
                const f32 number =
                    std::stof(std::string(parse_string(json, pos).empty() ? "0" : ""));
                (void)number;
            }
            // Simplified numeric parse for vec3 components
            std::size_t end = pos;
            while (end < json.size() &&
                   (std::isdigit(json[end]) || json[end] == '.' || json[end] == '-')) {
                ++end;
            }
            const f32 number = std::stof(std::string(json.substr(pos, end - pos)));
            pos = end;
            if (key == "x") {
                vec.x = number;
            } else if (key == "y") {
                vec.y = number;
            } else if (key == "z") {
                vec.z = number;
            }
            skip_ws(json, pos);
            if (json[pos] == ',') {
                ++pos;
                skip_ws(json, pos);
            }
        }
        if (json[pos] != '}') {
            throw std::runtime_error("Expected '}' in JSON object");
        }
        ++pos;
        return vec;
    }

    std::size_t end = pos;
    while (end < json.size() && (std::isdigit(json[end]) || json[end] == '.' || json[end] == '-' ||
                                 json[end] == 'e' || json[end] == 'E' || json[end] == '+')) {
        ++end;
    }
    const std::string token = std::string(json.substr(pos, end - pos));
    pos = end;
    if (token.find('.') != std::string::npos || token.find('e') != std::string::npos ||
        token.find('E') != std::string::npos) {
        return static_cast<f32>(std::stof(token));
    }
    return static_cast<i32>(std::stoi(token));
}

} // namespace

void Serializer::set(std::string key, SerializedValue value) {
    values_[std::move(key)] = std::move(value);
}

bool Serializer::has(std::string_view key) const {
    return values_.find(std::string(key)) != values_.end();
}

const SerializedValue* Serializer::get(std::string_view key) const {
    const auto it = values_.find(std::string(key));
    if (it == values_.end()) {
        return nullptr;
    }
    return &it->second;
}

std::string Serializer::to_json() const {
    std::ostringstream out;
    out << '{';
    bool first = true;
    for (const auto& [key, value] : values_) {
        if (!first) {
            out << ',';
        }
        first = false;
        out << '"' << escape_json(key) << "\":" << serialize_value(value);
    }
    out << '}';
    return out.str();
}

bool Serializer::from_json(std::string_view json) {
    values_.clear();
    std::size_t pos = 0;
    skip_ws(json, pos);
    if (pos >= json.size() || json[pos] != '{') {
        return false;
    }
    ++pos;
    skip_ws(json, pos);
    while (pos < json.size() && json[pos] != '}') {
        const std::string key = parse_string(json, pos);
        skip_ws(json, pos);
        if (json[pos] != ':') {
            return false;
        }
        ++pos;
        values_[key] = parse_value(json, pos);
        skip_ws(json, pos);
        if (json[pos] == ',') {
            ++pos;
            skip_ws(json, pos);
        }
    }
    return true;
}

} // namespace eve
