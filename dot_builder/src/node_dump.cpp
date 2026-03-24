#include "dot_graph.hpp"
#include "dot_edge.hpp"
#include "dot_node.hpp"
#include "dot_cluster.hpp"

namespace dot
{
void Node::print(std::ostream &stream, const size_t indent, bool highlighted) const {
    const std::string indent_string(indent, ' ');
    const std::string h_color     = "#ff0000"; // Bright Red
    const std::string h_fillcolor = "#ffff00"; // Bright Yellow
    const std::string h_style     = "filled,bold,rounded";
    const int         h_penwidth  = 4;

    stream << indent_string << "n" << id();
    stream << " [";
    stream << "label=\""     << label() << "\" ";

    if (highlighted) {
        stream << "color=\""     << h_color      << "\" ";
        stream << "fillcolor=\"" << h_fillcolor  << "\" ";
        stream << "style=\""     << h_style      << "\" ";
        stream << "penwidth=\""  << h_penwidth   << "\" ";
    } else {
        stream << "color=\""     << properties_.color     << "\" ";
        stream << "fillcolor=\"" << properties_.fillcolor << "\" ";
        stream << "style=\""     << properties_.style     << "\" ";
        stream << "penwidth=\""  << properties_.penwidth  << "\" ";
    }

    stream << "shape=\""     << properties_.shape     << "\" ";
    stream << "fontcolor=\"" << properties_.fontcolor << "\" ";
    stream << "fontsize=\""  << properties_.fontsize  << "\" ";
    stream << "];\n";
}

} // namespace dot