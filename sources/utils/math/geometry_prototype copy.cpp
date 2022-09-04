#include "utils/math/geometry_prototype.h"
#include "resources/model.h"
#include "utils/math/angle.h"
#include "utils/math/vec3.h"
#include <algorithm>
#include <iterator>
#include <limits>
#include <map>
#include <numeric>
#include <set>
#include <utility>

Vec3f getCenter(const std::vector<Vec3f>& vertices) {
    return {std::accumulate(
                std::begin(vertices), std::end(vertices), 0.0f,
                [](float acc, const Vec3f& v) { return acc + v.getX(); }) /
                vertices.size(),
            std::accumulate(
                std::begin(vertices), std::end(vertices), 0.0f,
                [](float acc, const Vec3f& v) { return acc + v.getY(); }) /
                vertices.size(),
            std::accumulate(
                std::begin(vertices), std::end(vertices), 0.0f,
                [](float acc, const Vec3f& v) { return acc + v.getZ(); }) /
                vertices.size()};
}

struct Triangle {
    Vec3f* vertices[3];
    Vec3f center;
    Vec3f normal;
};

template <typename T>
struct Neighbors {
    T* triangle = nullptr;
    std::set<Neighbors<T>*> neighbors;
};
    

Vec3f getTriangleCenter(const Triangle& triangle) {
    return getCenter(
        {*triangle.vertices[0], *triangle.vertices[1], *triangle.vertices[2]});
}

Angle getAngleBetween(const Vec3f& first, const Vec3f& second) {
    auto q = first * second;
    return Angle::fromRadians(std::acos((q.getX() + q.getY() + q.getZ()) /
                                        (first.length() * second.length())));
}

Model getConvexHull(const std::vector<Vec3f>& v) {
    std::vector<Triangle> triangles;
    auto verticesStorage = v;

    auto vertexRemoveIterator =
        std::unique(std::begin(verticesStorage), std::end(verticesStorage));
    verticesStorage.erase(vertexRemoveIterator, std::end(verticesStorage));

    std::vector<Vec3f*> vertices;
    for (auto& v1 : verticesStorage) {
        vertices.emplace_back(&v1);
    }

    // max x
    auto maxX = std::numeric_limits<float>::lowest();
    auto maxXIdx = -1;
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i]->getX() > maxX) {
            maxX = vertices[i]->getX();
            maxXIdx = i;
        }
    }
    auto maxXVertex = vertices[maxXIdx];
    vertices.erase(std::next(std::begin(vertices), maxXIdx));

    // max y
    auto maxY = std::numeric_limits<float>::lowest();
    auto maxYIdx = -1;
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i]->getY() > maxY) {
            maxX = vertices[i]->getY();
            maxYIdx = i;
        }
    }
    auto maxYVertex = vertices[maxYIdx];
    vertices.erase(std::next(std::begin(vertices), maxYIdx));

    // max z
    auto maxZ = std::numeric_limits<float>::lowest();
    auto maxZIdx = -1;
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i]->getZ() > maxZ) {
            maxZ = vertices[i]->getZ();
            maxZIdx = i;
        }
    }
    auto maxZVertex = vertices[maxZIdx];
    vertices.erase(std::next(std::begin(vertices), maxZIdx));

    // min z
    auto minZ = std::numeric_limits<float>::max();
    auto minZIdx = -1;
    for (int i = 0; i < vertices.size(); ++i) {
        if (vertices[i]->getZ() < minZ) {
            minZ = vertices[i]->getZ();
            minZIdx = i;
        }
    }
    auto minZVertex = vertices[minZIdx];
    vertices.erase(std::next(std::begin(vertices), minZIdx));

    triangles.push_back({{maxXVertex, maxYVertex, maxZVertex}});
    triangles.push_back({{minZVertex, maxYVertex, maxZVertex}});
    triangles.push_back({{maxXVertex, minZVertex, maxZVertex}});
    triangles.push_back({{maxXVertex, maxYVertex, minZVertex}});

    auto center =
        getCenter({*maxXVertex, *maxYVertex, *maxZVertex, *minZVertex});

    for (auto& triangle : triangles) {
        triangle.center = getTriangleCenter(triangle);
        triangle.normal =
            (*triangle.vertices[0] - *triangle.vertices[1])
                .getCrossProduct(*triangle.vertices[0] - *triangle.vertices[2]);

        auto angle = getAngleBetween(center - triangle.center, triangle.normal);

        if (angle.getDegrees() > 180.0f) {
            triangle.normal = -triangle.normal;
        }
    }
    auto vert_i = 0;
    for (auto& vert : vertices) {
        std::vector<Neighbors<Triangle>> owners;
        for (auto& triangle : triangles) {
            auto vecFromTriangleCenterToVert = triangle.center - *vert;
            if (getAngleBetween(vecFromTriangleCenterToVert, triangle.normal)
                    .getDegrees() <= 180.0f) {
                owners.push_back({&triangle});
            }
        }
        if (owners.empty()) {
            continue;
        } else if (owners.size() == 1) {
            auto deleteTriangle = *owners.back().triangle;
            auto found = std::find_if(
                std::begin(triangles), std::end(triangles),
                [forDelete = owners.back().triangle](auto& element) {
                    return &element == forDelete;
                });
            triangles.erase(found);

            for (size_t i = 0; i < 3; ++i) {
                triangles.push_back(
                    {{deleteTriangle.vertices[i],
                      deleteTriangle.vertices[(i + 1) % 3], vert}});
            }
        } else {
            for (auto& triangle : owners) {
                for (auto& triangle2 : owners) {
                    if (triangle.triangle != triangle2.triangle) {
                        for (const auto& vert1 : triangle.triangle->vertices) {
                            for (const auto& vert2 :
                                 triangle2.triangle->vertices) {
                                if (vert1 == vert2) {
                                    triangle.neighbors.insert(&triangle2);
                                    triangle2.neighbors.insert(&triangle);
                                }
                            }
                        }
                    }
                }
            }

            std::set<Vec3f*> done;
            std::vector<Vec3f*> doneSecuence;
            std::set<Triangle*> doneTriangles;
            std::vector<Triangle*> doneTrianglesSequence;
            auto maxDist = std::numeric_limits<float>::lowest();
            Neighbors<Triangle>* ownerWithMaxDist;
            Vec3f* vertWithMaxDist = nullptr;
            std::map<Vec3f*, float> distances;

            for (auto& owner : owners) {
                for (auto& ve : owner.triangle->vertices) {
                    auto d = (*vert - *ve).length();
                    if (d > maxDist) {
                        maxDist = d;
                        ownerWithMaxDist = &owner;
                        vertWithMaxDist = ve;
                    }
                    distances[ve] = d;
                }
            }

            done.insert(vertWithMaxDist);
            doneSecuence.push_back(vertWithMaxDist);
            doneTriangles.insert(ownerWithMaxDist->triangle);
            doneTrianglesSequence.push_back(ownerWithMaxDist->triangle);

            Neighbors<Triangle>* currentOwner = ownerWithMaxDist;
            Vec3f* pretendent = nullptr;
            int iterationNum = 0;
            while (pretendent != vertWithMaxDist) {
                auto maxDist = std::numeric_limits<float>::lowest();
                Vec3f* localVertWithMaxDist = nullptr;
                Neighbors<Triangle>* newCurrentOwner;
                for (auto& ve : currentOwner->triangle->vertices) {
                    if (done.contains(ve) &&
                        !(iterationNum >= 2 && ve == vertWithMaxDist)) {
                        continue;
                    }
                    if (distances[ve] > maxDist) {
                        localVertWithMaxDist = ve;
                        maxDist = distances[ve];
                        newCurrentOwner = currentOwner;
                    }
                }

                for (auto& n : currentOwner->neighbors) {
                    for (auto& ve : n->triangle->vertices) {
                        if (done.contains(ve) ||
                            !(iterationNum >= 2 && ve == vertWithMaxDist)) {
                            continue;
                        }
                        if (distances[ve] > maxDist) {
                            localVertWithMaxDist = ve;
                            maxDist = distances[ve];
                            newCurrentOwner = n;
                        }
                    }
                }
                ++iterationNum;
                pretendent = localVertWithMaxDist;
                done.insert(pretendent);
                doneSecuence.push_back(pretendent);
                currentOwner = newCurrentOwner;
                doneTriangles.insert(currentOwner->triangle);
                doneTrianglesSequence.push_back(currentOwner->triangle);
            }

            auto it =
                std::remove_if(std::begin(triangles), std::end(triangles),
                               [owners](auto& element) {
                                   auto found = std::find_if(
                                       std::begin(owners), std::end(owners),
                                       [element](auto& element2) {
                                           return element2.triangle == &element;
                                       });
                                   return found != std::end(owners);
                               });
            triangles.erase(it, std::end(triangles));

            for (size_t i = 0; i < doneSecuence.size() - 1; ++i) {
                triangles.push_back(
                    {{doneSecuence[i], doneSecuence[i + 1], vert}});
            }
        }
        vert_i++;
    }

    Model result;
    for (const auto& triangle : triangles) {
        for (const auto& vert : triangle.vertices) {
            result.vertices.push_back(
                {vert->getX(), vert->getY(), vert->getZ()});
        }

        result.faces.push_back(
            {static_cast<GLuint>(result.vertices.size() - 3),
             static_cast<GLuint>(result.vertices.size() - 2),
             static_cast<GLuint>(result.vertices.size() - 1)});
    }
    result.materials.emplace_back("");
    return result;
}