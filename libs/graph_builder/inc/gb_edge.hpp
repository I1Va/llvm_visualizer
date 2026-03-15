#pragma once

#include "gb_interface.hpp"

namespace gb
{

class Edge : public IEdge { 
protected:
    IdT left_, right_;
    std::string label_;
    
public:
    Edge(IdT left, IdT right) : left_(left), right_(right) {}
    
    std::pair<IdT, IdT> id() const override { return {left_, right_}; }
    const std::string &label() const override { return label_; }
    std::string &label() override { return label_; }
 
};

} // namespace dot