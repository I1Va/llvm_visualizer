#include "dot_graph.hpp"

namespace dot
{

void DotGraph::apply_dynamic_info(const DynamicInfo &info) {
    max_bb_count_ = 0;

    for (auto [block_id, block_cnt] : info.bb_counts()) {
        auto it = clusters_.find(block_id);
        if (it == clusters_.end()) {
            throw std::runtime_error("Can't find BB(" + std::to_string(block_id) + ") in static info");
        }
    
        Cluster *cluster = it->second.get();
        assert(cluster && cluster->type() == gb::ClusterTypes::BB);
        cluster->set_use_count(block_cnt);
        max_bb_count_ = std::max(max_bb_count_, block_cnt);
    }

    for (auto [call_edge_id, call_edge_count] : info.call_edge_counts()) {
        Edge *edge = get_call_edge(call_edge_id.first, call_edge_id.second);
        assert(edge && edge->type() == gb::EdgeTypes::Call);
        edge->set_use_count(call_edge_count);
    }

    for (auto &[call_id, call_values] : info.call_values()) {
        auto it = nodes_.find(call_id);
        if (it == nodes_.end()) {
            throw std::runtime_error("Can't find node(" + std::to_string(call_id) + ") in static info");
        }
        Node *node = it->second.get();
        assert(node && node->type() == gb::NodeTypes::Instr && node->label() == "call");
        for (int64_t value : call_values)
            node->add_call_value(value);
    }
}

void DotGraph::serialize_dot(std::ostream &stream) const {
    stream << "digraph "      << properties_.name    << " {\n";
    stream << "  rankdir=\""  << properties_.rankdir  << "\"\n";
    stream << "  splines=\""  << properties_.splines  << "\"\n";  
    stream << "  nodesep=\""  << properties_.nodesep  << "\"\n";       
    stream << "  ranksep=\""  << properties_.ranksep  << "\"\n";   
    stream << "  bgcolor=\""  << properties_.bgcolor  << "\"\n";      
    stream << "  fontname=\"" << properties_.fontname << "\"\n";   
    stream << "  fontsize=\"" << properties_.fontsize << "\"\n";   
    
    print_clusters(stream);

    const size_t indent = 2;
    
    for (auto &edge : edges_) edge->print(stream, indent);

    stream << "}\n";
}

void DotGraph::print_cluster_recursive (
    std::ostream &stream,
    const std::unordered_map<gb::IdT, std::vector<gb::IdT>>& cluster_graph,
    const gb::IdT cluster_id,
    const size_t indent
) const {
    const std::string indent_string(indent, ' ');
    clusters_.at(cluster_id)->print_open(stream, indent);

    if (cluster_graph.find(cluster_id) != cluster_graph.end()) {
        for (gb::IdT child_id : cluster_graph.at(cluster_id)) {
            print_cluster_recursive(stream, cluster_graph, child_id, indent + 2);
        }
    }
    clusters_.at(cluster_id)->print_close(stream, indent);
}

void DotGraph::print_clusters(std::ostream &stream) const {
    std::unordered_map<gb::IdT, std::vector<gb::IdT>> clusters_graph(clusters_.size());

    for (auto &[cluster_id, cluster] : clusters_) {
        gb::ICluster* parent = cluster->parent();
        if (parent) {
            clusters_graph[parent->id()].push_back(cluster_id);
            
        }
    }
    
    const size_t start_indent = 2; 
    for (auto &[cluster_id, cluster] : clusters_) {
        if (cluster->parent() == nullptr) {
            print_cluster_recursive(stream, clusters_graph, cluster_id, start_indent);
        }
    }
}

} // namespace dot
