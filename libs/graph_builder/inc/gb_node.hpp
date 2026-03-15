#pragma once

#include "gb_interface.hpp"

namespace gb 
{

class Node : public INode {
protected:
    IdT id_;
    std::string label_;
    ICluster *parent_;

public:
    Node(IdT id): id_(id) {}

    IdT id() const override { return id_; }

    const std::string &label() const override { return label_; }
    std::string &label() override { return label_; }

    void set_parent(ICluster *parent) override { 
        if (parent_) parent_->remove_node(this);
        parent_ = parent; 
    }

    const ICluster *parent() { return parent_; }
    ICluster *parent() const { return parent_; }
};

} // namespace dot