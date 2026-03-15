#pragma once

#include <cassert>
#include "gb_interface.hpp"

namespace gb 
{

class Cluster : public ICluster {
protected:
    IdT id_;
    std::string label_;
    
    ICluster *parent_=nullptr;

    std::vector<INode *> nodes_;
    std::vector<ICluster *> clusters_;

public:
    Cluster(IdT cluster_id): id_(cluster_id) {}
    
    IdT id() const override { return id_; }
    const std::string &label() const override { return label_; }
    std::string &label() override { return label_; }

    const ICluster* parent() const override { return parent_; }
    ICluster* parent() override { return parent_; }

    const std::vector<INode*>& nodes() const override { return nodes_; }
    std::vector<INode*>& nodes() override { return nodes_; }

    const std::vector<ICluster*>& clusters() const override { return clusters_; }
    std::vector<ICluster*>& clusters() override { return clusters_; }

    ICluster* parent() override { return parent_; }
    const ICluster* parent() const override { return parent_; }

    void set_parent(ICluster* parent) override {
        if (parent_) parent_->remove_cluster(this);
        parent_ = parent;
    }

    void remove_cluster(ICluster* cluster) override {
        std::erase(clusters_, cluster);
    }
};

} // namespace dot