#include "dot_graph.hpp"
#include "dot_edge.hpp"
#include "dot_node.hpp"
#include "dot_cluster.hpp"

namespace dot
{

void Node::print(std::ostream &stream, const size_t indent, bool highlighted) const {
    const std::string indent_string(indent, ' ');
    const std::string h_color     = "#ff0000"; 
    const std::string h_fillcolor = "#ffff00"; 
    const std::string h_style     = "filled,bold,rounded";
    const int         h_penwidth  = 4;

    const size_t max_num_to_show = 3;

    std::string enhanced_label = label();
    
    if (type() == gb::NodeTypes::Instr && !call_values_.empty()) {
        std::vector<std::pair<int64_t, uint64_t>> sorted_values(
            call_values_.begin(), call_values_.end()
        );
        std::sort(sorted_values.begin(), sorted_values.end(),
                  [](const auto& a, const auto& b) { return a.second > b.second; });
        
        size_t num_to_show = std::min(size_t(max_num_to_show), sorted_values.size());
        
        enhanced_label += "\\nresults:";
        for (size_t i = 0; i < num_to_show; ++i) {
            enhanced_label += "\\n  " + std::to_string(sorted_values[i].first);
            enhanced_label += " (×" + std::to_string(sorted_values[i].second) + ")";
        }
        
        if (sorted_values.size() > max_num_to_show) {
            enhanced_label += "\\n  ... and " + 
                              std::to_string(sorted_values.size() - 3) + " more";
        }
    }

    stream << indent_string << "n" << id();
    stream << " [";
    stream << "label=\""     << enhanced_label << "\" ";

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