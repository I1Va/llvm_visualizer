#pragma once

#include "dot_interface.hpp"

namespace dot 
{

struct NodeProperties {
    std::string color = "red";
    std::string shape = "rect";
    std::string fillcolor = "#FFD0D0";
    std::string style = "filled";

    NodeProperties() = default;
};

static inline const NodeProperties DEFAULT_INSTRNODE_PROPERTIES = 
{
    .color = "red",
    .shape = "rect",
    .fillcolor = "#FFD0D0",
    .style = "filled"
};

class Node : public INode {
protected:
    DotId id_;
    std::string_view label_;

    NodeProperties properties_;

public:
    Node(DotId id, const std::string& label):
        id_(id), label_(label) {}
    DotId id() const override { return id_; }
    std::string_view label() const override { return label_;}

    const NodeProperties& properties() const override { return properties_; }
    NodeProperties& properties() override { return properties_; }
};

class InstrNode final : public Node {
public:
    InstrNode(DotId id, const std::string& label): Node(id, label) {
        properties_ = DEFAULT_INSTRNODE_PROPERTIES;
    }
};

} // namespace dot