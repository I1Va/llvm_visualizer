#pragma once

#include "graph_builder.hpp"

namespace proto {

class GraphSerializer {
public:
    static int serialize(const gb::GraphBuilder &builder, const std::string &filename);
    static void deserialize(gb::GraphBuilder &builder, const std::string& filepath);
};

} // namespace gb