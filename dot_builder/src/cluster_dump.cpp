#include "dot_graph.hpp"
#include "dot_edge.hpp"
#include "dot_node.hpp"
#include "dot_cluster.hpp"

namespace dot
{

void Cluster::print_open(std::ostream &stream, const size_t indent) const {
    const std::string indent_string(indent, ' ');
    stream << indent_string << "subgraph "       << get_str_identifier(id()) << " {" << "// " << properties_.cluster_suffix << "\n";
    stream << indent_string << "  label =\""     << label()                  << "\"\n";
    stream << indent_string << "  fillcolor =\"" << properties_.fillcolor    << "\"\n"; 
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