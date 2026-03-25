#pragma once
#include <iostream>

#include "gb_edge.hpp"
#include "gb_llvm_types.hpp"

namespace dot
{

class DotGraph;

struct EdgeProperties {
    std::string color = "black";       
    std::string style = "solid";       
    std::string arrowhead = "normal";  
    int penwidth = 1;                  
    int weight = 1;                    
    std::string constraint = "true";   
};

static inline const EdgeProperties DEFAULT_DATAEDGE_PROPERTIES = 
{
    .color = "#1f77b4",   
    .style = "dashed",    
    .arrowhead = "vee",
    .penwidth = 1,
    .weight = 1,
    .constraint = "true" 
};

static inline const EdgeProperties DEFAULT_FLOWEDGE_PROPERTIES = 
{
    .color = "#000000",   
    .style = "solid",
    .arrowhead = "normal",
    .penwidth =2,
    .weight = 20,         
    .constraint = "true"
};

static inline const EdgeProperties DEFAULT_CALL_EDGE_PROPERTIES = 
{
    .color = "#2ca02c",  
    .style = "dashed",     
    .arrowhead = "diamond",
    .penwidth = 2,
    .weight = 5,           
    .constraint = "true"
};

class Edge {
    EdgeProperties properties_;
    gb::IEdge *underlying_;
    DotGraph *graph_;

    // dynamic info
    uint64_t use_count_ = 0;

public: 
    explicit Edge(gb::IEdge *edge, DotGraph *graph): underlying_(edge), graph_(graph) {
        if (!underlying_) throw std::invalid_argument("Cannot wrap a null IEdge."); 
        if (!graph) throw std::invalid_argument("Nullptr DotGraph ptr passed."); 
        switch (edge->type()) {
            case gb::EdgeTypes::Flow: properties_ = DEFAULT_FLOWEDGE_PROPERTIES; break;
            case gb::EdgeTypes::Data: properties_ = DEFAULT_DATAEDGE_PROPERTIES; break;
            case gb::EdgeTypes::Call: properties_ = DEFAULT_CALL_EDGE_PROPERTIES; break;
            default: throw std::runtime_error("Got unknown edge type: '" + std::to_string(edge->type()) + "' in wrapper constructor.");
        }  
    }
     
    gb::IdT left() const { return underlying_->left(); }
    gb::IdT right() const { return underlying_->right(); }
    uint64_t type() const { return underlying_->type(); }
    const std::string &label() const { return underlying_->label(); }
    std::string &label() { return underlying_->label(); }


    void print(std::ostream& stream, const size_t indent) const;
    std::string get_endpoint_identifier(gb::IdT id) const;

// dynamic info methods
    void set_use_count(const uint64_t use_count) { use_count_ = use_count; }
};

} // namespace dot