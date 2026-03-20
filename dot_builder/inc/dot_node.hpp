#pragma once
#include <string>
#include <iostream>
#include "gb_node.hpp"

namespace dot
{

class DotGraph;

class Node {
public:
    struct Properties {
        std::string color = "#000000";      
        std::string shape = "rect";          
        std::string fillcolor = "#ffffff";   
        std::string style = "filled,rounded";
        std::string fontcolor = "#000000";   
        int fontsize = 14;                   
        int penwidth = 1;                    

        Properties() = default;
    };    
private:
    gb::INode *underlying_;
    DotGraph *graph_;
    Properties properties_;
public:
    explicit Node(gb::INode *node, DotGraph *graph): underlying_(node), graph_(graph) {
        if (!node) throw std::invalid_argument("Cannot wrap a null INode.");
        if (!graph) throw std::invalid_argument("Nullptr DotGraph ptr passed."); 
    }

    gb::IdT id() const { return underlying_->id(); }
    uint64_t type() const { return underlying_->type(); }
    
    const std::string& label() const { return underlying_->label(); }
    std::string& label() { return underlying_->label(); }

    void set_parent(gb::ICluster* parent) { underlying_->set_parent(parent); }
    const gb::ICluster* parent() { return underlying_->parent(); } 

    void print(std::ostream &stream, const size_t indent) const;

    static std::string get_str_identifier(gb::IdT id) {
        return "n" + std::to_string(id);
    }
};

} // namespace dot