#pragma once

#include <cstdint>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <vector>

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

class DynamicInfo {
private:
    std::unordered_map<uint64_t, uint64_t> bb_counts_;
    std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> call_edge_counts_;
    std::unordered_map<uint64_t, std::vector<int64_t>> call_values_;

public:
    const std::unordered_map<uint64_t, uint64_t> &bb_counts() const { return bb_counts_; }
    const std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> &call_edge_counts() const { return call_edge_counts_; }
    const std::unordered_map<uint64_t, std::vector<int64_t>> &call_values() const { return call_values_; }

    void set_bb_count(const uint64_t id, const uint64_t count) { bb_counts_[id] = count; }
    void set_call_edge_count(const std::pair<uint64_t, uint64_t> id, const uint64_t count) { call_edge_counts_[id] = count; }
    void set_call_values(const uint64_t id, const std::vector<int64_t> &counts) { call_values_[id] = counts; }

    void log_basic_block(uint64_t id) {
        bb_counts_[id]++;
    }

    void log_call_edge(uint64_t caller_id, uint64_t callee_id) {
        call_edge_counts_[{caller_id, callee_id}]++;
    }

    void log_result(uint64_t val_id, int64_t value) { // TODO: !!!! FIX
        call_values_[val_id].push_back(value);
    }
};