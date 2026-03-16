#include <iostream>
#include <cstdint>
#include <map>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <cassert>
#include <fstream>

struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
};

class DynamicInfo {
private:
    mutable std::shared_mutex mutex_;

    std::unordered_map<uint64_t, uint64_t> bb_counts;
    std::unordered_map<std::pair<uint64_t, uint64_t>, uint64_t, PairHash> edge_counts;
    std::unordered_map<uint64_t, int64_t> call_values;

public:
    void logBasicBlock(uint64_t id) {
        std::unique_lock lock(mutex_);
        bb_counts[id]++;
    }

    void logCallEdge(uint64_t caller_id, uint64_t callee_id) {
        std::unique_lock lock(mutex_);
        edge_counts[{caller_id, callee_id}]++;
    }

    void logResult(uint64_t val_id, int64_t value) {
        std::unique_lock lock(mutex_);
        call_values[val_id] = value;
    }

    void dump(char* path) const {
        std::shared_lock lock(mutex_);
        std::ofstream f(path);
        if (!f.is_open()) return;

        f << "--- Basic Blocks ---" << std::endl;
        for (const auto& [id, count] : bb_counts) 
            f << id << ":" << count << "\n";

        f << "\n--- Call Edges ---\n";
        for (const auto& [edge, count] : edge_counts) 
            f << edge.first << "->" << edge.second << ":" << count << "\n";

        f << "\n--- Call Values ---\n";
        for (const auto& [id, val] : call_values) 
            f << id << ":" << val << "\n";
    }
};

static DynamicInfo *getGInfo(bool delete_info=false) {
    static DynamicInfo* instance = new DynamicInfo();
    if (delete_info) instance = nullptr;
    return instance;
}

extern "C" void basic_block_start_logger(uint64_t id) {
    DynamicInfo *info = getGInfo();
    if (info) info->logBasicBlock(id);
}

extern "C" void call_logger(uint64_t caller_id, uint64_t callee_id) {
    DynamicInfo *info = getGInfo();
    if (info) info->logCallEdge(caller_id, callee_id);
}

extern "C" void res_int_logger(int64_t res, uint64_t val_id) {
    DynamicInfo *info = getGInfo();
    if (info) info->logResult(val_id, res);
}

extern "C" void dump_dynamic_logger_info(char *dump_path) {
    DynamicInfo *info = getGInfo();
    if (info) info->dump(dump_path);
    getGInfo(/*delete*/true);
}