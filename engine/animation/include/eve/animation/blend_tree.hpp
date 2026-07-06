#pragma once

#include <eve/core/types.hpp>
#include <string>
#include <vector>

namespace eve::animation {

enum class BlendNodeType : u8 {
    Clip = 0,
    Blend1D,
    Blend2D,
    Additive,
};

struct BlendTreeNode {
    std::string id;
    BlendNodeType type = BlendNodeType::Clip;
    std::string clip_id;
    f32 parameter_x = 0.0f;
    f32 parameter_y = 0.0f;
    std::vector<std::string> children;
};

class BlendTree {
public:
    void add_node(BlendTreeNode node);
    [[nodiscard]] f32 evaluate_parameter(std::string_view node_id) const;
    void set_parameter(std::string_view node_id, f32 x, f32 y = 0.0f);
    [[nodiscard]] std::string resolve_clip(std::string_view root_id) const;

private:
    std::vector<BlendTreeNode> nodes_;
};

} // namespace eve::animation
