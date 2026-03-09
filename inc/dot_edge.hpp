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
    std::string_view label_;
    EdgeProperties properties_;

public:
    Edge(Endpoint left, Endpoint right, std::string_view label="") : 
        left_(left), right_(right), label_(label) {}

    std::string_view label() const override { return label_; }

    const EdgeProperties& properties() const override {
        return properties_;
    }

    EdgeProperties& properties() override {
        return properties_;
    }

    void print(std::ostream &stream, const size_t indent) const override {
        const std::string indent_string(indent, ' ');
        
        // std::string left_name = left_.get_string_identifier();
        // std::string right_name = right_.get_string_identifier();

        // stream << indent_string << left_name << " -> n" << dst_id_;
        // stream << "[label=\"" << get_kind_label(kind_) << "\"";
        // stream << "color="    << color;
        // stream << "penwidth=" << penwidth;
        // stream << "style="    << style;
        // stream << "arrowhead=normal";
        // stream << "];\n";
    }
};

class DataEdge final : public Edge {
public:
    DataEdge(Endpoint left, Endpoint right, std::string_view label=""):
        Edge(left, right, label) {properties_ = DEFAULT_DATAEDGE_PROPERTIES; }
};

class FlowEdge final : public Edge {
public:
    FlowEdge(Endpoint left, Endpoint right, std::string_view label=""):
        Edge(left, right, label) {properties_ = DEFAULT_FLOWEDGE_PROPERTIES; }
};

} // namespace dot