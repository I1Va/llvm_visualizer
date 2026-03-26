#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>

#include "static_info.pb.h"
#include "gb_llvm_types.hpp"
#include "dot_graph.hpp"
#include "dynamic_info.pb.h"
#include "dynamic_info.hpp"
#include "dyn_info_serializer.hpp"
#include "graph_serializer.hpp"

void generate_static_dot(const std::string& static_bin_path, const std::string& dynamic_bin_path, const std::string& out_dot_path) {
    gb::GraphBuilder builder;
    proto::GraphSerializer::deserialize(builder, static_bin_path);

    dot::DotGraph dot_graph(builder);

    DynamicInfo dyn_info;
    proto::DynInfoSerializer::deserialize(dyn_info, dynamic_bin_path);
    
    try {
        dot_graph.apply_dynamic_info(dyn_info);
    } catch (const std::exception& e) {
        std::cerr << "dynamic info application failure : '" << e.what() << "'\n";
    }
    
    std::ofstream out_file(out_dot_path);
    if (!out_file) {
        throw std::runtime_error("Failed to create " + out_dot_path);
    }
    dot_graph.serialize_dot(out_file);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <static_info.bin> <dynamic_info.bin> <output.dot>\n";
        return 1;
    }

    try {
        generate_static_dot(argv[1], argv[2], argv[3]);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}