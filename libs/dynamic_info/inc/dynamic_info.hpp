#pragma once

#include <cstdint>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

class DynamicInfo {
private:
    std::unordered_map<uint64_t, uint64_t> bb_counts_;
    std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> edge_counts_;
    std::unordered_map<uint64_t, int64_t> call_values_;

public:
    const std::unordered_map<uint64_t, uint64_t> &bb_counts() const { return bb_counts_; }
    const std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> &edge_counts() const { return edge_counts_; }
    const std::unordered_map<uint64_t, int64_t> &call_values() const { return call_values_; }

    void logBasicBlock(uint64_t id) {
        bb_counts_[id]++;
    }

    void logCallEdge(uint64_t caller_id, uint64_t callee_id) {
        edge_counts_[{caller_id, callee_id}]++;
    }

    void logResult(uint64_t val_id, int64_t value) {
        call_values_[val_id] = value;
    }
};