#include "render_system/box.h"

Box::Box( Vector3D size, QColor color )
    : Drawable()
    , m_color( std::move( color ) )
{
    auto halfX = size.x() / 2.0f;
    auto halfY = size.y() / 2.0f;
    auto halfZ = size.z() / 2.0f;

    float vertexes[] = {
                         -halfX, halfY, halfZ,    // 0 near left up
                         -halfX, -halfY, halfZ,     // 1 near left down
                         halfX, halfY, halfZ,     // 2 near right up
                         halfX, -halfY, halfZ,      // 3 near right down
                         -halfX, halfY, -halfZ,     // 4 further left up
                         -halfX, -halfY, -halfZ,      // 5 further left down
                         halfX, halfY, -halfZ,      // 6 further right up
                         halfX, -halfY, -halfZ,       // 7 further right down
                       };

    unsigned indices[] = {
        // near
        0, 2, 1,
        1, 2, 3,
        // further
        4, 5, 6,
        5, 6, 7,
        // left
        4, 0, 5,
        5, 0, 1,
        // right
        2, 3, 7,
        2, 6, 7,
        // down
        1, 3, 7,
        1, 7, 5,
        // up
        0, 2, 6,
        0, 4, 6
    };
}

DrawBuffers Box::getDrawBuffers() const
{
    return DrawBuffers();
}
