// SPDX-License-Identifier: GPL-3.0-or-later
// OpenIDM / Flux — Ultra-Fast, Safe C++20 Bencode Parser & Serializer
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "common/types.hpp"

namespace openidm::torrent {

struct BencodeValue;
using BencodeDict = std::map<std::string, BencodeValue>;
using BencodeList = std::vector<BencodeValue>;

struct BencodeValue {
    enum class Type { kInteger, kString, kList, kDict };
    Type type{Type::kString};
    std::variant<int64_t, std::string, BencodeList, BencodeDict> data;

    BencodeValue() : type(Type::kString), data(std::string()) {}
    BencodeValue(int64_t val) : type(Type::kInteger), data(val) {}
    BencodeValue(std::string val) : type(Type::kString), data(std::move(val)) {}
    BencodeValue(const char* val) : type(Type::kString), data(std::string(val)) {}
    BencodeValue(BencodeList val) : type(Type::kList), data(std::move(val)) {}
    BencodeValue(BencodeDict val) : type(Type::kDict), data(std::move(val)) {}

    bool is_int() const { return type == Type::kInteger; }
    bool is_string() const { return type == Type::kString; }
    bool is_list() const { return type == Type::kList; }
    bool is_dict() const { return type == Type::kDict; }

    int64_t as_int(int64_t def = 0) const {
        return is_int() ? std::get<int64_t>(data) : def;
    }
    const std::string& as_string() const {
        static const std::string empty;
        return is_string() ? std::get<std::string>(data) : empty;
    }
    const BencodeList& as_list() const {
        static const BencodeList empty;
        return is_list() ? std::get<BencodeList>(data) : empty;
    }
    const BencodeDict& as_dict() const {
        static const BencodeDict empty;
        return is_dict() ? std::get<BencodeDict>(data) : empty;
    }

    bool contains(const std::string& key) const {
        if (!is_dict()) return false;
        const auto& d = std::get<BencodeDict>(data);
        return d.find(key) != d.end();
    }

    const BencodeValue& operator[](const std::string& key) const {
        static const BencodeValue null_val;
        if (!is_dict()) return null_val;
        const auto& d = std::get<BencodeDict>(data);
        auto it = d.find(key);
        return (it != d.end()) ? it->second : null_val;
    }
};

class BencodeParser {
public:
    static Result<BencodeValue> Decode(std::string_view input) {
        size_t pos = 0;
        auto res = ParseValue(input, pos, 0);
        if (!res.ok()) return res;
        return res;
    }

    static std::string Encode(const BencodeValue& val) {
        std::string out;
        EncodeInternal(val, out);
        return out;
    }

private:
    static constexpr size_t kMaxDepth = 64;

    static Result<BencodeValue> ParseValue(std::string_view s, size_t& pos, size_t depth) {
        if (depth > kMaxDepth) {
            return Result<BencodeValue>::Err("ERR_BENCODE_DEPTH", "Maximum Bencode recursion depth exceeded");
        }
        if (pos >= s.size()) {
            return Result<BencodeValue>::Err("ERR_BENCODE_EOF", "Unexpected end of bencode buffer");
        }

        char c = s[pos];
        if (c == 'i') {
            // Integer: i<digits>e
            ++pos;
            size_t end_pos = s.find('e', pos);
            if (end_pos == std::string_view::npos) {
                return Result<BencodeValue>::Err("ERR_BENCODE_INT", "Unterminated bencode integer");
            }
            std::string_view num_str = s.substr(pos, end_pos - pos);
            pos = end_pos + 1;
            if (num_str.empty()) {
                return Result<BencodeValue>::Err("ERR_BENCODE_INT", "Empty bencode integer");
            }
            if (num_str == "-0") {
                return Result<BencodeValue>::Err("ERR_BENCODE_INT", "Negative zero is not allowed in bencode");
            }
            if (num_str.size() > 1 && num_str[0] == '0') {
                return Result<BencodeValue>::Err("ERR_BENCODE_INT", "Leading zero in positive bencode integer");
            }
            if (num_str.size() > 2 && num_str[0] == '-' && num_str[1] == '0') {
                return Result<BencodeValue>::Err("ERR_BENCODE_INT", "Leading zero in negative bencode integer");
            }
            if (num_str.size() == 1 && num_str[0] == '-') {
                return Result<BencodeValue>::Err("ERR_BENCODE_INT", "Standalone minus sign in bencode integer");
            }

            char* end_ptr = nullptr;
            errno = 0;
            int64_t val = std::strtoll(std::string(num_str).c_str(), &end_ptr, 10);
            if (errno == ERANGE || end_ptr != std::string(num_str).c_str() + num_str.size()) {
                return Result<BencodeValue>::Err("ERR_BENCODE_INT", "Integer overflow or invalid digits in bencode");
            }
            return Result<BencodeValue>::Ok(BencodeValue(val));
        } else if (c == 'l') {
            // List: l<items>e
            ++pos;
            BencodeList list;
            while (pos < s.size() && s[pos] != 'e') {
                auto item = ParseValue(s, pos, depth + 1);
                if (!item.ok()) return item;
                list.push_back(std::move(item.value.value()));
            }
            if (pos >= s.size() || s[pos] != 'e') {
                return Result<BencodeValue>::Err("ERR_BENCODE_LIST", "Unterminated bencode list");
            }
            ++pos; // Skip 'e'
            return Result<BencodeValue>::Ok(BencodeValue(std::move(list)));
        } else if (c == 'd') {
            // Dict: d<key><val>...e
            ++pos;
            BencodeDict dict;
            while (pos < s.size() && s[pos] != 'e') {
                auto key_res = ParseValue(s, pos, depth + 1);
                if (!key_res.ok() || !key_res.value->is_string()) {
                    return Result<BencodeValue>::Err("ERR_BENCODE_DICT", "Dictionary key must be a bencode string");
                }
                std::string key = key_res.value->as_string();
                auto val_res = ParseValue(s, pos, depth + 1);
                if (!val_res.ok()) return val_res;
                dict[std::move(key)] = std::move(val_res.value.value());
            }
            if (pos >= s.size() || s[pos] != 'e') {
                return Result<BencodeValue>::Err("ERR_BENCODE_DICT", "Unterminated bencode dictionary");
            }
            ++pos; // Skip 'e'
            return Result<BencodeValue>::Ok(BencodeValue(std::move(dict)));
        } else if (std::isdigit(static_cast<unsigned char>(c))) {
            // String: <len>:<data>
            size_t colon = s.find(':', pos);
            if (colon == std::string_view::npos) {
                return Result<BencodeValue>::Err("ERR_BENCODE_STR", "Missing colon in bencode string length");
            }
            std::string_view len_str = s.substr(pos, colon - pos);
            if (len_str.size() > 1 && len_str[0] == '0') {
                return Result<BencodeValue>::Err("ERR_BENCODE_STR", "Leading zero in string length");
            }
            char* end_ptr = nullptr;
            errno = 0;
            unsigned long long u_len = std::strtoull(std::string(len_str).c_str(), &end_ptr, 10);
            if (errno == ERANGE || end_ptr != std::string(len_str).c_str() + len_str.size()) {
                return Result<BencodeValue>::Err("ERR_BENCODE_STR", "Invalid string length in bencode");
            }
            pos = colon + 1;
            if (u_len > (s.size() - pos)) {
                return Result<BencodeValue>::Err("ERR_BENCODE_STR_BOUNDS", "String length exceeds buffer boundaries");
            }
            size_t len = static_cast<size_t>(u_len);
            std::string str(s.substr(pos, len));
            pos += len;
            return Result<BencodeValue>::Ok(BencodeValue(std::move(str)));
        }

        return Result<BencodeValue>::Err("ERR_BENCODE_SYNTAX", "Invalid bencode leading character");
    }

    static void EncodeInternal(const BencodeValue& val, std::string& out) {
        if (val.is_int()) {
            out += "i" + std::to_string(val.as_int()) + "e";
        } else if (val.is_string()) {
            const std::string& s = val.as_string();
            out += std::to_string(s.size()) + ":" + s;
        } else if (val.is_list()) {
            out += "l";
            for (const auto& item : val.as_list()) {
                EncodeInternal(item, out);
            }
            out += "e";
        } else if (val.is_dict()) {
            out += "d";
            for (const auto& [k, v] : val.as_dict()) {
                out += std::to_string(k.size()) + ":" + k;
                EncodeInternal(v, out);
            }
            out += "e";
        }
    }
};

} // namespace openidm::torrent
