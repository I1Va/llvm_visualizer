#pragma once

#include "dot_interface.hpp"

namespace dot 
{

struct NodeProperties {
    std::string color = "#000000";      
    std::string shape = "rect";          
    std::string fillcolor = "#ffffff";   
    std::string style = "filled,rounded";
    std::string fontcolor = "#000000";   
    int fontsize = 14;                   
    int penwidth = 1;                    
};

static inline const NodeProperties DEFAULT_INSTRNODE_PROPERTIES = 
{
    .color = "#d62728",          
    .shape = "rect",
    .fillcolor = "#f8d7da",      
    .style = "filled,rounded",
    .fontcolor = "#800000",      
    .fontsize = 14,
    .penwidth = 2
};

static inline const NodeProperties DEFAULT_VALUENODE_PROPERTIES = 
{
    .color = "#ff7f0e",         
    .shape = "rect",
    .fillcolor = "#ffe6cc",     
    .style = "filled,rounded",
    .fontcolor = "#803d00",     
    .fontsize = 14,
    .penwidth = 2
};

class Node : public INode {
protected:
    DotId id_;
    std::string label_;

    NodeProperties properties_;

public:
    Node(DotId id, const std::string& label):
        id_(id), label_(label) {}
    DotId id() const override { return id_; }
    std::string_view label() const override { return label_; }

    const NodeProperties& properties() const override { return properties_; }
    NodeProperties& properties() override { return properties_; }
   
    void print(std::ostream &stream, const size_t indent) const override {
        const std::string indent_string(indent, ' ');
        stream << indent_string << "n" << id_;
        stream << " [";
        stream << "label=\""        << label_                   << "\" ";
        stream << "color=\""        << properties_.color        << "\" "; 
        stream << "shape=\""        << properties_.shape        << "\" ";
        stream << "fillcolor=\""    << properties_.fillcolor    << "\" ";
        stream << "style=\""        << properties_.style        << "\" ";
        stream << "fontcolor=\""    << properties_.fontcolor    << "\" ";
        stream << "fontsize=\""     << properties_.fontsize     << "\" ";
        stream << "penwidth=\""     << properties_.penwidth     << "\"";
        stream << "];\n";
    }
};

class InstrNode final : public Node {
public:
    InstrNode(DotId id, const std::string& label): Node(id, label) {
        properties_ = DEFAULT_INSTRNODE_PROPERTIES;
    }
};

class ValueNode final : public Node {
public:
    ValueNode(DotId id, const std::string& label): Node(id, label) {
        properties_ = DEFAULT_VALUENODE_PROPERTIES;
    }
};

} // namespace dot