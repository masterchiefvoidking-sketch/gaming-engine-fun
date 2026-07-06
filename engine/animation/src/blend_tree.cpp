#include <eve/animation/blend_tree.hpp>

namespace eve::animation {

void BlendTree::add_node(BlendTreeNode node) {
    nodes_.push_back(std::move(node));
}

f32 BlendTree::evaluate_parameter(std::string_view node_id) const {
    for (const BlendTreeNode& node : nodes_) {
        if (node.id == node_id) {
            return node.parameter_x;
        }
    }
    return 0.0f;
}

void BlendTree::set_parameter(std::string_view node_id, f32 x, f32 y) {
    for (BlendTreeNode& node : nodes_) {
        if (node.id == node_id) {
            node.parameter_x = x;
            node.parameter_y = y;
            return;
        }
    }
}

std::string BlendTree::resolve_clip(std::string_view root_id) const {
    for (const BlendTreeNode& node : nodes_) {
        if (node.id != root_id) {
            continue;
        }
        if (node.type == BlendNodeType::Clip) {
            return node.clip_id;
        }
        if (!node.children.empty()) {
            return resolve_clip(node.children.front());
        }
    }
    return "anim_idle";
}

} // namespace eve::animation
