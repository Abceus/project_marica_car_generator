#pragma once

#include "drawable.h"
#include "render_system/color.h"
#include "render_system/edge_array.h"
#include "render_system/shader_program.h"
#include "resources/wireframe_model.h"

class WireframeMesh : public Drawable {
public:
    WireframeMesh();
    void init(const WireframeModel& model, const Color& color = Color());
    void draw(ShaderProgram* shader) const override;

private:
    std::shared_ptr<EdgeArray> edgeArray;
    Color m_color;
};
