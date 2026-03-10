#pragma once

#include "dot_interface.hpp"

namespace dot
{

struct EdgeProperties {
    std::string color = "red";
    std::string style = "solid";
    std::string arrowhead = "normal";
    int penwidth = 2;
    int weight = 2;
    std::string constraint = "true";

    EdgeProperties() = default;
};

static inline const struct EdgeProperties DEFAULT_DATAEDGE_PROPERTIES = 
{
    .color = "blue",
    .style = "solid",
    .arrowhead = "normal",
    .penwidth = 1,
    .weight = 1,
    .constraint = "false"
};

static inline const struct EdgeProperties DEFAULT_FLOWEDGE_PROPERTIES = 
{
    .color = "red",
    .style = "solid",
    .arrowhead = "normal",
    .penwidth = 2,
    .weight = 100,
    .constraint = "true"
};


class Edge : public IEdge { 
protected:
    Endpoint left_, right_;
    std::string label_;
    EdgeProperties properties_;

public:
    Edge(Endpoint left, Endpoint right, std::string label = "") : 
        left_(left), right_(right), label_(std::move(label)) {}

    std::string_view label() const override { return label_; }

    const EdgeProperties& properties() const override {
        return properties_;
    }

    EdgeProperties& properties() override {
        return properties_;
    }

    void print(
        std::ostream& stream, 
        const std::unordered_map<DotId, std::unique_ptr<ICluster>>& clusters,
        const size_t indent
    ) const override {
        const std::string indent_string(indent, ' ');
        
        std::string left = get_endpoint_identifier(clusters, left_);
        std::string right = get_endpoint_identifier(clusters, right_);

        stream << indent_string << left << " -> "<< right;
        stream << "[";
        stream << "label=\""      << label_                 << "\" ";
        stream << "color=\""      << properties_.color      << "\" ";
        stream << "penwidth=\""   << properties_.penwidth   << "\" ";
        stream << "weight=\""     << properties_.weight     << "\" ";
        stream << "style=\""      << properties_.style      << "\" ";
        stream << "constraint=\"" << properties_.constraint << "\" ";
        stream << "arrowhead=\""  << properties_.arrowhead  << "\"";
        stream << "];\n";
    }

private:
    std::string get_endpoint_identifier(
        const std::unordered_map<DotId, std::unique_ptr<ICluster>>& clusters, 
        const Endpoint endpoint
    ) const {
        switch (endpoint.kind)
        {
        case Endpoint::Kind::Node:
            return INode::get_str_identifier(endpoint.id);
        case Endpoint::Kind::Cluster:
            if (clusters.find(endpoint.id) == clusters.end()) {
                throw std::runtime_error("edge endpoint contain incorrect cluster id");
            }
            ICluster *cluster = clusters.find(endpoint.id)->second.get();
            if (cluster->children().empty()) {
                throw std::runtime_error("edge endpoint cluster has no nodes to connect");
            }
            return INode::get_str_identifier(cluster->children().front()->id());
        }
    }
};

class DataEdge final : public Edge {
public:
    DataEdge(Endpoint left, Endpoint right, std::string label = ""):
        Edge(left, right, std::move(label)) {properties_ = DEFAULT_DATAEDGE_PROPERTIES; }
};

class FlowEdge final : public Edge {
public:
    FlowEdge(Endpoint left, Endpoint right, std::string label = ""):
        Edge(left, right, std::move(label)) {properties_ = DEFAULT_FLOWEDGE_PROPERTIES; }
};

} // namespace dot