#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace gb
{
    
using IdT = uint64_t;
static inline constexpr IdT INVALID_ID = 0;

class ICluster;

class INode {
public:
    virtual IdT id() const = 0;
    virtual const std::string &label() const = 0;
    virtual std::string &label() = 0;

    virtual void set_parent(ICluster *parent) = 0;
    virtual const ICluster *parent() = 0;
    virtual ICluster *parent() const = 0;
    virtual ~INode() = default;
};


class ICluster {
public:
    virtual IdT id() const = 0;
    virtual const std::string &label() const = 0;
    virtual std::string &label() = 0;

    virtual const ICluster* parent() const = 0;
    virtual ICluster* parent() = 0;
    virtual void set_parent(ICluster *parent) = 0;

    virtual const std::vector<INode*>& nodes() const = 0;
    virtual std::vector<INode*>& nodes() = 0;

    virtual const std::vector<ICluster*>& clusters() const = 0;
    virtual std::vector<ICluster*>& clusters() = 0;

    virtual void remove_cluster(ICluster* cluster) = 0;
    virtual void remove_node(INode* node) = 0;

    virtual ~ICluster() = default;
};

class IEdge {
public:
    virtual std::pair<IdT, IdT> id() const = 0;
    virtual const std::string &label() const = 0;
    virtual std::string &label() = 0;
    virtual ~IEdge() = default;
};

} // namespace gb
