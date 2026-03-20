#pragma once
#include <iostream>
#include "gb_edge.hpp"

namespace dot
{

class DotGraph;

class Edge {
public:
    struct Properties {
        std::string color = "black";       
        std::string style = "solid";       
        std::string arrowhead = "normal";  
        int penwidth = 1;                  
        int weight = 1;                    
        std::string constraint = "true";   

        Properties() = default;
    };
private:
    Properties properties_;
    gb::IEdge *underlying_;
    DotGraph *graph_;
public: 
    explicit Edge(gb::IEdge *edge, DotGraph *graph): underlying_(edge), graph_(graph) {
        if (!underlying_) throw std::invalid_argument("Cannot wrap a null IEdge."); 
        if (!graph) throw std::invalid_argument("Nullptr DotGraph ptr passed."); 
    }

    std::pair<gb::IdT, gb::IdT> id() const { return underlying_->id(); }
    uint64_t type() const { return underlying_->type(); }
    const std::string &label() const { return underlying_->label(); }
    std::string &label() { return underlying_->label(); }


    void print(std::ostream& stream, const size_t indent) const;
    std::string get_endpoint_identifier(gb::IdT id) const;
};

} // namespace dot