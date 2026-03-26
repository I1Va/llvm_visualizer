#include "dyn_info_serializer.hpp"

static DynamicInfo *getGInfo(bool delete_info=false) {
    static DynamicInfo* instance = new DynamicInfo();
    if (delete_info) {
        delete instance;
        instance = nullptr;
    }
    return instance;
}

extern "C" void basic_block_start_logger(uint64_t id) {
    DynamicInfo *info = getGInfo();
    if (info) info->log_basic_block(id);
}

extern "C" void call_logger(uint64_t caller_id, uint64_t callee_id) {
    DynamicInfo *info = getGInfo();
    if (info) info->log_call_edge(caller_id, callee_id);
}

extern "C" void res_int_logger(int64_t res, uint64_t val_id) {
    DynamicInfo *info = getGInfo();
    if (info) info->log_result(val_id, res);
}

extern "C" void dump_dynamic_logger_info(char *dump_path) {
    DynamicInfo *info = getGInfo();
    if (info) proto::DynInfoSerializer::serialize(*info, dump_path);
    getGInfo(/*delete*/true);
}