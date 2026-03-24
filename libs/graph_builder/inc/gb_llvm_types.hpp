#pragma once

#include <cstdint>

namespace gb
{

enum ClusterTypes : uint64_t {
    BB = 1,
    F = 2,
};

enum EdgeTypes : uint64_t {
    Flow = 1,
    Data = 2,
    Call = 3,
};

enum NodeTypes : uint64_t {
    Instr = 1,
    Constant = 2,
};

} // namespace gb 