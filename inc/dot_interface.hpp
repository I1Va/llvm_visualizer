#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>


namespace dot
{
    
struct ClusterProperties;
struct NodeProperties;
struct EdgeProperties;

using DotId = uint64_t;
static inline constexpr DotId INVALID_ID = 0;


struct Endpoint {
    enum class Kind { Node, Cluster } kind;
    DotId id;
    static Endpoint node(DotId id_) { return Endpoint{Kind::Node, id_}; }
    static Endpoint cluster(DotId id_) { return Endpoint{Kind::Cluster, id_}; }
};

class INode {
public:
    virtual DotId id() const = 0;
    virtual std::string_view label() const = 0;
    virtual const NodeProperties& properties() const = 0;
    virtual NodeProperties& properties() = 0;
    
    virtual ~INode() = default;
};

class IEdge {
public:
    virtual std::string_view label() const = 0;
    virtual const EdgeProperties& properties() const = 0;
    virtual EdgeProperties& properties() = 0;

    virtual ~IEdge() = default;
};

class ICluster {
public:
    virtual DotId id() const = 0;
    virtual std::string_view label() const = 0;
    virtual const ClusterProperties& properties() const = 0;
    virtual ClusterProperties& properties() = 0;
    virtual const ICluster* get_parent() const = 0;
    virtual ICluster* get_parent() = 0;
    virtual const std::vector<INode*>& children() const = 0;

    virtual void add_child(INode* node) = 0;
    virtual void set_parent(ICluster *parent) = 0;
    
    virtual ~ICluster() = default;
};

} // namespace dot
