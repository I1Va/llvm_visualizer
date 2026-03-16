#pragma once

#include <fstream>
#include <google/protobuf/stubs/common.h>

#include "dynamic_info.hpp"
#include "dynamic_info.pb.h" 


namespace proto {

class DynInfoSerializer {
public:
    static int Serialize(const DynamicInfo &info, const std::string& filename) {
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        instrumentation::ExecutionData proto_data;
        {            
            auto* bb_map = proto_data.mutable_bb_counts();
            for (const auto& [id, count] : info.bb_counts()) {
                (*bb_map)[id] = count;
            }

            for (const auto& [edge, count] : info.edge_counts()) {
                auto* entry = proto_data.add_edge_counts();
                entry->mutable_edge()->set_caller_id(edge.first);
                entry->mutable_edge()->set_callee_id(edge.second);
                entry->set_count(count);
            }

            auto* val_map = proto_data.mutable_call_values();
            for (const auto& [id, val] : info.call_values()) {
                (*val_map)[id] = val;
            }
        }

        std::ofstream output(filename, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!proto_data.SerializeToOstream(&output)) {
            std::cerr << "[LOG] Failed to write protobuf data" << std::endl;
            return 1;
        }
        return 0;
    }
};

} // namespace gb