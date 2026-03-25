#include "dot_graph.hpp"
#include "dot_edge.hpp"        
#include "dot_cluster.hpp"

namespace dot {

void Edge::print(std::ostream& stream, const size_t indent) const {
    const std::string indent_string(indent, ' ');
    
    std::string left_str = get_endpoint_identifier(left());
    std::string right_str = get_endpoint_identifier(right());
    if (left_str.empty() || right_str.empty()) {
        return;
    }

    std::string enhanced_label = label();
    if (use_count_ > 0 && type() == gb::EdgeTypes::Call) {
        enhanced_label += "\\n(" + std::to_string(use_count_) + ")";
    }

    stream << indent_string << left_str << " -> "<< right_str;
    stream << "[";
    stream << "label=\""      << enhanced_label          << "\" ";
    stream << "color=\""      << properties_.color      << "\" ";
    stream << "penwidth=\""   << properties_.penwidth   << "\" ";
    stream << "weight=\""     << properties_.weight     << "\" ";
    stream << "style=\""      << properties_.style      << "\" ";
    stream << "constraint=\"" << properties_.constraint << "\" ";
    stream << "arrowhead=\""  << properties_.arrowhead  << "\"";
    stream << "];\n";
}


std::string Edge::get_endpoint_identifier(gb::IdT id) const {
    if (graph_->get_node(id)) {
        return Node::get_str_identifier(id);
    }

    if (graph_->get_cluster(id)) {
        Cluster *cluster = graph_->clusters().find(id)->second.get();
        assert(cluster);
        if (cluster->type() == gb::ClusterTypes::F) {
            gb::ICluster *BB = cluster->clusters().empty() ? nullptr : cluster->clusters().front();
            gb::INode *BB_fnode = (!BB || BB->nodes().empty()) ? nullptr : BB->nodes().front();
            if (BB_fnode) return Node::get_str_identifier(BB_fnode->id());
            return cluster->get_fict_node_str_id(id);
        } else {
            assert(cluster->type() == gb::ClusterTypes::BB);
            assert(!cluster->nodes().empty());
            assert(cluster->nodes().front());
            return Node::get_str_identifier(cluster->nodes().front()->id());
        }
    }
    throw std::runtime_error("Incorrect edge endpoint type");
}

} // namespace dot      

