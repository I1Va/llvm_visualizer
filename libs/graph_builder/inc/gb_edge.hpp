#pragma once

#include "gb_interface.hpp"

namespace gb
{


class Edge : public IEdge { 
protected:
    IdT left_, right_;
    uint64_t type_;

    std::string label_;
    
public:
    Edge(IdT left, IdT right, uint64_t type) : left_(left), right_(right), type_(type) {}
    
    IdT left() const override { return left_; }
    IdT right() const override { return right_; }

    uint64_t type() const override { return type_; }
    const std::string &label() const override { return label_; }
    std::string &label() override { return label_; }
};

} // namespace gb