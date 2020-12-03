#pragma once

#include "resources/model.h"
#include "resources/resource_pointer.h"

#include <optional>

#include <QString>
#include <QVector>

struct WireframeModel
{
    struct Vertex
    {
        float X, Y, Z;
    };

    struct Edge
    {
        size_t vertexes[2];
    };

    WireframeModel() = default;
    WireframeModel( const WireframeModel& copy );
    QVector<Vertex> vertices;
    QVector<Edge> edges;
    static std::optional<WireframeModel> fromModel( const Model& model );
};
