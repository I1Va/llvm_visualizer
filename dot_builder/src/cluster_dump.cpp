#include "dot_graph.hpp"
#include "dot_edge.hpp"
#include "dot_node.hpp"
#include "dot_cluster.hpp"

namespace dot
{

static std::string get_heat_color(uint64_t count) {
    const uint64_t max_heat = 30; 
    double factor = std::min(static_cast<double>(count) / max_heat, 1.0);

    int r = static_cast<int>(220 + (255 - 220) * factor);
    int g = static_cast<int>(230 + (69 - 230) * factor);
    int b = static_cast<int>(241 + (0 - 241) * factor);

    char buf[8];
    snprintf(buf, sizeof(buf), "#%02x%02x%02x", r, g, b);
    return std::string(buf);
}

void Cluster::print_open(std::ostream &stream, const size_t indent) const {
    const std::string indent_string(indent, ' ');
    
    std::string dynamic_fill = properties_.fillcolor;
    if (use_count_ > 0 && type() != gb::ClusterTypes::F) {
        dynamic_fill = get_heat_color(use_count_);
    }

    std::string enhanced_label = label();
    if (use_count_ > 0) {
        enhanced_label += "\\n(uses: " + std::to_string(use_count_) + ")";
    }


    stream << indent_string << "subgraph "       << get_str_identifier(id()) << " {" << "// " << properties_.cluster_suffix << "\n";
    stream << indent_string << "  label =\""     << enhanced_label           << "\"\n";
    stream << indent_string << "  fillcolor =\"" << dynamic_fill             << "\"\n"; 
    stream << indent_string << "  color =\""     << properties_.color        << "\"\n"; 
    stream << indent_string << "  style =\""     << properties_.style        << "\"\n";
    stream << indent_string << "  penwidth =\""  << properties_.penwidth     << "\"\n";
    stream << indent_string << "  fontcolor=\""  << properties_.fontcolor    << "\"\n";     
    stream << indent_string << "  fontsize=\""   << properties_.fontsize     << "\"\n";
        
    for (const gb::INode *child : nodes()) {
        if (child == *nodes().begin() && child->type() == gb::NodeTypes::Instr) {
            graph_->nodes()[child->id()]->print(stream, indent + 2, /*highlighted*/true);
            continue;
        }   
        graph_->nodes()[child->id()]->print(stream, indent + 2);
    }

    if (nodes().empty()) {
        const std::string indent_string(indent + 2, ' ');
        stream << indent_string << get_fict_node_str_id(id()) << "[label=\"\", shape=\"point\"]\n";
    }
}

void Cluster::print_close(std::ostream &stream, const size_t indent) const {
    const std::string indent_string(indent, ' ');
    stream << indent_string << "}\n";
}

} // namespace dot