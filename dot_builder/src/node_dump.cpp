#include "dot_graph.hpp"
#include "dot_edge.hpp"
#include "dot_node.hpp"
#include "dot_cluster.hpp"

namespace dot
{

void Node::print(std::ostream &stream, const size_t indent) const {
    const std::string indent_string(indent, ' ');
    stream << indent_string << "n" << id();
    stream << " [";
    stream << "label=\""        << label()                  << "\" ";
    stream << "color=\""        << properties_.color        << "\" "; 
    stream << "shape=\""        << properties_.shape        << "\" ";
    stream << "fillcolor=\""    << properties_.fillcolor    << "\" ";
    stream << "style=\""        << properties_.style        << "\" ";
    stream << "fontcolor=\""    << properties_.fontcolor    << "\" ";
    stream << "fontsize=\""     << properties_.fontsize     << "\" ";
    stream << "penwidth=\""     << properties_.penwidth     << "\"";
    stream << "];\n";
}

} // namespace dot