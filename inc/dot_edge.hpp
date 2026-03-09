#pragma once

#include "dot_interface.hpp"

namespace dot
{

struct EdgeProperties {
    std::string color = "red";
    std::string style = "solid";
    std::string arrowhead = "normal";
    int penwidth = 2;

    EdgeProperties() = default;
};

static inline const struct EdgeProperties DEFAULT_DATAEDGE_PROPERTIES = 
{
    .color = "blue",
    .style = "solid",
    .arrowhead = "normal",
    .penwidth = 2
};

static inline const struct EdgeProperties DEFAULT_FLOWEDGE_PROPERTIES = 
{
    .color = "red",
    .style = "solid",
    .arrowhead = "normal",
    .penwidth = 2
};


class Edge : public IEdge { 
protected:
    Endpoint left_, right_;
    EdgeProperties properties_;

public:
    Edge(Endpoint left, Endpoint right) : 
        left_(left), right_(right) {}

    Endpoint left() const override { return left_; }
    Endpoint right() const override { return right_; }

    const EdgeProperties& properties() const override {
        return properties_;
    }

    EdgeProperties& properties() override {
        return properties_;
    }
};

class DataEdge final : public Edge {
    DataEdge(Endpoint left, Endpoint right):
        Edge(left, right) {properties_ = DEFAULT_DATAEDGE_PROPERTIES; }
};

class FlowEdge final : public Edge {
    FlowEdge(Endpoint left, Endpoint right):
        Edge(left, right) {properties_ = DEFAULT_FLOWEDGE_PROPERTIES; }
};

} // namespace dot