#pragma once

#include <fstream>
#include <google/protobuf/stubs/common.h>

#include "dynamic_info.hpp"
#include "dynamic_info.pb.h" 


namespace proto {

class DynInfoSerializer {
public:
    static int serialize(const DynamicInfo &info, const std::string& filename) {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        instrumentation::ExecutionData proto_data;

        auto* bb_map = proto_data.mutable_bb_counts();
        for (const auto& [id, count] : info.bb_counts()) {
            (*bb_map)[id] = count;
        }

        for (const auto& [edge, count] : info.call_edge_counts()) {
            auto* entry = proto_data.add_edge_counts();
            auto* edge_msg = entry->mutable_edge();
            edge_msg->set_caller_id(edge.first);
            edge_msg->set_callee_id(edge.second);
            entry->set_count(count);
        }

        auto* val_map = proto_data.mutable_call_values();
        for (const auto& [id, vec] : info.call_values()) {
            instrumentation::Int64List list_wrapper;
            for (int64_t val : vec) {
                list_wrapper.add_values(val);
            }
            (*val_map)[id] = std::move(list_wrapper);
        }

        std::ofstream output(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!output.is_open()) {
            std::cerr << "[LOG] Failed to open file: " << filename << std::endl;
            return 1;
        }

        if (!proto_data.SerializeToOstream(&output)) {
            std::cerr << "[LOG] Failed to write protobuf data" << std::endl;
            return 1;
        }
        
        return 0;
    }

    static void deserialize(DynamicInfo &info, const std::string& filepath) {
        instrumentation::ExecutionData proto_data;
        std::ifstream input(filepath, std::ios::binary);
        
        if (!input) {
            throw std::runtime_error("Failed to open dynamic info : " + filepath);
        }

        if (!proto_data.ParseFromIstream(&input)) {
            throw std::runtime_error("Failed to parse dynamic info protobuf.");
        }

        for (auto const& [id, count] : proto_data.bb_counts()) {
            info.set_bb_count(id, count);
        }

        for (const auto& entry : proto_data.edge_counts()) {
            if (!entry.has_edge()) continue;
            uint64_t caller = entry.edge().caller_id();
            uint64_t callee = entry.edge().callee_id();
            info.set_call_edge_count({caller, callee}, entry.count());
        }

        for (auto const& [id, list_msg] : proto_data.call_values()) {
            std::vector<int64_t> call_values;
            for (int64_t val : list_msg.values()) {
                call_values.push_back(val);
            }
            info.set_call_values(id, call_values);

        }
    }    
};

} // namespace gb