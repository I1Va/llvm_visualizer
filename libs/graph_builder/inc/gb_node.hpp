#pragma once

#include "gb_interface.hpp"

namespace gb 
{

class Node : public INode {
protected:
    IdT id_;
    uint64_t type_;
    std::string label_;
    ICluster *parent_=nullptr;

public:
    Node(IdT id, uint64_t type): id_(id), type_(type) {}

    IdT id() const override { return id_; }
    uint64_t type() const override { return type_; }
    const std::string &label() const override { return label_; }
    std::string &label() override { return label_; }

    void set_parent(ICluster *parent) override { 
        if (parent_) parent_->remove_node(this);
        parent_ = parent; 
        if (parent) parent->nodes().push_back(this);
    }

    const ICluster *parent() override { return parent_; }
    ICluster *parent() const override { return parent_; }
};

} // namespace dot