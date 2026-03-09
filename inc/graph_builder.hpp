#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace dot
{

struct ClusterProperties {
    std::string color = "red";
    std::string fillcolor = "gray";
    std::string style = "filled";
    int penwidth = 2; 
    std::string fontcolor = "red";
    int fontsize = 20;
};

struct GraphProperties {
    std::string_view name = "G";
    std::string_view rankdir = "TD";
    std::string_view splines = "polyline"; 
    double nodesep = 1.0; 
    double ranksep = 1.5;
};

struct NodeProperties {

};

struct EdgeProperties {

};

class INode {
public:
    virtual uint64_t id() const = 0;
    virtual std::string label() const = 0;

    virtual const NodeProperties &properties() const = 0;
    virtual NodeProperties &properties() = 0;
    
    virtual ~INode() = default;
};

class IEdge {
    virtual uint64_t left() const = 0;
    virtual uint64_t right() const = 0;

    virtual const EdgeProperties &properties() const = 0;
    virtual EdgeProperties &properties() = 0;

    virtual ~IEdge() = default;
};

class ICluster {
    virtual uint64_t id() const = 0;
    virtual std::string label() const = 0;

    virtual void add_node(INode &node) = 0;

    virtual const ClusterProperties &properties() const = 0;
    virtual ClusterProperties &properties() = 0;

    virtual const std::vector<INode*> &nodes() const = 0;
};

class DotBuilder {
    GraphProperties properties;

    uint64_t next_id_{1};

    std::unordered_map<uint64_t, std::unique_ptr<INode>> nodes_;
    std::unordered_map<uint64_t, std::unique_ptr<ICluster>> clusters_;
    std::vector<std::unique_ptr<IEdge>> edges_;

public:
    DotBuilder() = default;
    

    template <typename NodeT, typename ... ArgT>   
    NodeT &create_node(ArgT ... arg) {
        // 
    }

    template <typename NodeT, typename ... ArgT>   
    NodeT &create_node_in_cluster(ICluster &cluster, ArgT ... arg) {
        // 
    }

    template <typename ClusterT, typename ... ArgT>   
    ClusterT &create_cluster(ArgT ... arg) {

    };
    template <typename ClusterT, typename ... ArgT>   
    ClusterT &create_cluster_with_parent(ICluster &parent, ArgT ... arg) {

    };
    
    template <typename EdgeT, typename ... ArgT>   
    EdgeT &create_edge(INode &left, INode &right, ArgT ... arg) {
        // 
    }
    template <typename EdgeT, typename ... ArgT>   
    EdgeT &create_edge(ICluster &left, INode &right, ArgT ... arg) {
        // 
    }
    template <typename EdgeT, typename ... ArgT>   
    EdgeT &create_edge(INode &left, ICluster &right, ArgT ... arg) {
        // 
    }
    template <typename EdgeT, typename ... ArgT>   
    EdgeT &create_edge(ICluster &left, ICluster &right, ArgT ... arg) {
        // 
    }

};


    
} // namespace dot
