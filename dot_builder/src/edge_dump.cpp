#include "dot_graph.hpp"
#include "dot_edge.hpp"        
#include "dot_cluster.hpp"

namespace dot {

void Edge::print(std::ostream& stream, const size_t indent) const {
    const std::string indent_string(indent, ' ');
    
    std::string left = get_endpoint_identifier(id().first);
    std::string right = get_endpoint_identifier(id().second);
    if (left.empty() || right.empty()) {
        return;
    }

    stream << indent_string << left << " -> "<< right;
    stream << "[";
    stream << "label=\""      << label()                << "\" ";
    stream << "color=\""      << properties_.color      << "\" ";
    stream << "penwidth=\""   << properties_.penwidth   << "\" ";
    stream << "weight=\""     << properties_.weight     << "\" ";
    stream << "style=\""      << properties_.style      << "\" ";
    stream << "constraint=\"" << properties_.constraint << "\" ";
    stream << "arrowhead=\""  << properties_.arrowhead  << "\"";
    stream << "];\n";
}


std::string Edge::get_endpoint_identifier(gb::IdT id) const {
    if (graph_->is_node(id)) {
        return Node::get_str_identifier(id);
    }

    if (graph_->is_cluster(id)) {
        Cluster *cluster = graph_->clusters().find(id)->second.get();
        if (!cluster->nodes().empty()) {
            return Node::get_str_identifier(cluster->nodes().front()->id());
        }
        return cluster->get_fict_node_str_id(id);
    }
    throw std::runtime_error("Incorrect edge endpoint type");
}

} // namespace dot      

