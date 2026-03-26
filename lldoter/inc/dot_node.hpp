#pragma once
#include <string>
#include <iostream>

#include "gb_node.hpp"
#include "gb_llvm_types.hpp"

namespace dot
{

class DotGraph;

struct NodeProperties {
    std::string color = "#000000";      
    std::string shape = "rect";          
    std::string fillcolor = "#ffffff";   
    std::string style = "filled,rounded";
    std::string fontcolor = "#000000";   
    int fontsize = 14;                   
    int penwidth = 1;                    
};  

static inline const NodeProperties DEFAULT_INSTRNODE_PROPERTIES = 
{
    .color = "#d62728",          
    .shape = "rect",
    .fillcolor = "#f8d7da",      
    .style = "filled,rounded",
    .fontcolor = "#800000",      
    .fontsize = 14,
    .penwidth = 2
};

static inline const NodeProperties DEFAULT_VALUENODE_PROPERTIES = 
{
    .color = "#ff7f0e",         
    .shape = "rect",
    .fillcolor = "#ffe6cc",     
    .style = "filled,rounded",
    .fontcolor = "#803d00",     
    .fontsize = 14,
    .penwidth = 2
};

class Node {
    gb::INode *underlying_;
    DotGraph *graph_;
    NodeProperties properties_;

// dynamic info
    std::unordered_map<int64_t, uint64_t> call_values_;

public:
    explicit Node(gb::INode *node, DotGraph *graph): underlying_(node), graph_(graph) {
        if (!node) throw std::invalid_argument("Cannot wrap a null INode.");
        if (!graph) throw std::invalid_argument("Nullptr DotGraph ptr passed."); 
        switch (node->type()) {
            case gb::NodeTypes::Instr: properties_ = DEFAULT_INSTRNODE_PROPERTIES; break;
            case gb::NodeTypes::Constant: properties_ = DEFAULT_VALUENODE_PROPERTIES; break;
            default: throw std::runtime_error("Got unknown node type: '" + std::to_string(node->type()) + "' in wrapper constructor.");
        }        
    }

    gb::IdT id() const { return underlying_->id(); }
    uint64_t type() const { return underlying_->type(); }
    
    const std::string& label() const { return underlying_->label(); }
    std::string& label() { return underlying_->label(); }

    void set_parent(gb::ICluster* parent) { underlying_->set_parent(parent); }
    const gb::ICluster* parent() { return underlying_->parent(); } 

    void print(std::ostream &stream, const size_t indent, bool highlighted=false) const;

    static std::string get_str_identifier(gb::IdT id) {
        return "n" + std::to_string(id);
    }

// dynamic info methods
    void add_call_value(const int64_t val) {
        call_values_[val]++;
    }
};

} // namespace dot