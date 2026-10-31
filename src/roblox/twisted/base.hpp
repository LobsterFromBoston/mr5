#pragma once
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>

enum class SourceType {
    Value,
    AttributeDouble,
    AttributeString
};

struct locations {
    std::vector<std::string> path;
    SourceType type;
    std::string attrName = "";
};

using results = std::unordered_map<std::string, std::variant<double, std::string>>;