#pragma once

#include <cassert>
#include "dot_interface.hpp"

namespace dot 
{

struct ClusterProperties {
    std::string color = "red";
    std::string fillcolor = "gray";
    std::string style = "filled";
    int penwidth = 2; 
    std::string fontcolor = "red";
    int fontsize = 20;

    ClusterProperties() = default;
};

static inline const ClusterProperties DEFAULT_BB_CLUSTER_PROPERTIES = 
{
    .color = "red",
    .fillcolor = "gray",
    .style = "filled",
    .penwidth = 2,
    .fontcolor = "red",
    .fontsize = 20,
};

static inline const ClusterProperties DEFAULT_F_CLUSTER_PROPERTIES = 
{
    .color = "green",
    .fillcolor = "gray",
    .style = "filled",
    .penwidth = 2,
    .fontcolor = "red",
    .fontsize = 20,
};

class Cluster : public ICluster {
protected:
    ClusterProperties properties_;

    DotId id_;
    std::string_view label_;
    
    ICluster *parent_=nullptr;
    std::vector<INode *> children_;
public:
    Cluster(DotId cluster_id, std::string_view label):
        id_(cluster_id), label_(label) {}
    
    DotId id() const override { return id_; }
    std::string_view label() const override { return label_; }

    const ClusterProperties& properties() const override {
        return properties_;
    }

    ClusterProperties& properties() override {
        return properties_;
    }

    const ICluster* get_parent() const override {
        return parent_;
    }

    void add_child(INode* node) override {
        assert(node);
    
        children_.push_back(node);
    }

    ICluster* get_parent() override {
        return parent_;
    }

    void set_parent(ICluster* parent) override {
        parent_ = parent;
    }

    const std::vector<INode*>& children() const override {
        return children_;
    }
};

class BBCluster final : public Cluster {
public:
    BBCluster(DotId cluster_id, std::string_view label) : 
        Cluster(cluster_id, label) {
            properties_ = DEFAULT_BB_CLUSTER_PROPERTIES;
        }
};

class FCluster final : public Cluster {
public:
    FCluster(DotId cluster_id, std::string_view label) : 
        Cluster(cluster_id, label) {
            properties_ = DEFAULT_F_CLUSTER_PROPERTIES;
        }
};


} // namespace dot 