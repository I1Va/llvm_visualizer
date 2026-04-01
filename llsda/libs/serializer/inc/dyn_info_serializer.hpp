#pragma once
#include <string>
#include "dynamic_info.hpp"

namespace proto {

class DynInfoSerializer {
public:
    static int serialize(const DynamicInfo &info, const std::string& filename);
    static void deserialize(DynamicInfo &info, const std::string& filepath);
};

} // namespace gb