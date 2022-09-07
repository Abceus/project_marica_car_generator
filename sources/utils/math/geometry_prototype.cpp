#include "utils/math/geometry_prototype.h"
#include "resources/model.h"
#include "utils/math/line.h"
#include "utils/math/plane.h"
#include "utils/math/vec3.h"
#include "utils/shapes/sphere.h"
#include <algorithm>
#include <iterator>
#include <limits>
#include <list>
#include <numeric>
#include <optional>
#include <set>
#include <stack>
#include <vector>


Vec3f getCenter(const std::vector<Vec3f>& points) {
    Vec3f result;
    for (const auto& point : points) {
        result.setX(result.getX() + point.getX());
        result.setY(result.getY() + point.getY());
        result.setZ(result.getZ() + point.getZ());
    }
    result = result / static_cast<float>(points.size());
    return result;
}

struct Triangle {
    Vec3f* a = 0;
    Vec3f* b = 0;
    Vec3f* c = 0;
    std::set<Vec3f*> points;
    std::set<Triangle*> neighbors;
};

Vec3f getCenter(const std::list<Triangle>& triangles) {
    std::set<Vec3f*> uniquePoints;
    for (const auto& triangle : triangles) {
        uniquePoints.emplace(triangle.a);
        uniquePoints.emplace(triangle.b);
        uniquePoints.emplace(triangle.c);
    }
    Vec3f result;
    for (const auto& point : uniquePoints) {
        result.setX(result.getX() + point->getX());
        result.setY(result.getY() + point->getY());
        result.setZ(result.getZ() + point->getZ());
    }
    result = result / static_cast<float>(uniquePoints.size());
    return result;
}

enum class SideResult {
    Same,
    Different,
    FirstPointOnPlane,
    SecondPointOnPlane,
    BothPointsOnPlane
};

SideResult pointsOnSide(const Vec3f& a, const Vec3f& b, const Vec3f& c,
                        const Vec3f& p1, const Vec3f& p2) {
    auto tpa = (a - p1).getTripleProduct(a - b, a - c);
    if (std::abs(tpa) < 0.00001f) {
        auto d = Plane::createFromPoints(a, b, c).getDistanceToPoint(p1);
        if (d <= std::numeric_limits<float>::epsilon()) {
            tpa = 0.0f;
        }
    }
    auto tpb = (a - p2).getTripleProduct(a - b, a - c);
    if (std::abs(tpb) < 0.00001f) {
        auto d = Plane::createFromPoints(a, b, c).getDistanceToPoint(p2);
        if (d <= std::numeric_limits<float>::epsilon()) {
            tpb = 0.0f;
        }
    }
    if (tpa == 0.0f && tpb == 0.0f) {
        return SideResult::BothPointsOnPlane;
    }
    if (tpa == 0.0f) {
        return SideResult::FirstPointOnPlane;
    }
    if (tpb == 0.0f) {
        return SideResult::SecondPointOnPlane;
    }
    if (tpa * tpb > 0.0f) {
        return SideResult::Same;
    }
    return SideResult::Different;
}

struct Edge {
    Vec3f* a = nullptr;
    Vec3f* b = nullptr;
};

bool isPointCorner(const Triangle& triangle, const Vec3f& point) {
    if (*triangle.a == point) {
        return true;
    }
    if (*triangle.b == point) {
        return true;
    }
    if (*triangle.c == point) {
        return true;
    }
    return false;
}

Edge getEdgeBetweenTriangles(Triangle& a, Triangle& b) {
    Edge result;
    if (isPointCorner(b, *a.a)) {
        result.a = a.a;
    }

    if (isPointCorner(b, *a.b)) {
        if (result.a) {
            result.b = a.b;
            return result;
        } else {
            result.a = a.b;
        }
    }

    if (isPointCorner(b, *a.c)) {
        if (result.a) {
            result.b = a.c;
        } else {
            result.a = a.c;
        }
    }
    return result;
}

struct EdgeTrianglePair {
    Edge edge;
    Triangle* triangle = nullptr;
    Triangle* notVisibleTriangle = nullptr;
};

struct FindHorizontHistory {
    Triangle* triangle;
    std::set<Triangle*> done;
};

struct FindHorizontResult {
    std::set<Triangle*> triangles;
    std::vector<EdgeTrianglePair> edges;
};

FindHorizontResult getHorizont(Triangle& firstTriangle, const Vec3f& point,
                               const Vec3f& center) {
    std::vector<EdgeTrianglePair> result;
    std::set<Triangle*> doneTriangles;
    Triangle* lastDoneTriangle = nullptr;

    std::stack<FindHorizontHistory> history;
    history.emplace(&firstTriangle);

    do {
        do {
            doneTriangles.emplace(history.top().triangle);
            lastDoneTriangle = history.top().triangle;
            for (auto neighbor : history.top().triangle->neighbors) {
                if (doneTriangles.contains(neighbor) ||
                    history.top().done.contains(neighbor)) {
                    continue;
                }
                history.top().done.emplace(neighbor);
                auto side = pointsOnSide(*neighbor->a, *neighbor->b,
                                         *neighbor->c, point, center);
                if (side == SideResult::Same) {
                    EdgeTrianglePair newEdge;
                    newEdge.edge = getEdgeBetweenTriangles(
                        *history.top().triangle, *neighbor);
                    newEdge.triangle = history.top().triangle;
                    newEdge.notVisibleTriangle = neighbor;
                    result.emplace_back(newEdge);
                } else {
                    history.emplace(neighbor);
                    break;
                }
            }
        } while (lastDoneTriangle != history.top().triangle);

        history.pop();
    } while (!history.empty());
    return FindHorizontResult{.triangles = doneTriangles, .edges = result};
}

void setPointToTriangles(std::set<Triangle*>& triangles,
                         std::set<Vec3f*>& points, const Vec3f& center) {
    for (auto& triangle : triangles) {
        for (auto it = std::begin(points); it != std::end(points);) {
            auto side = pointsOnSide(*triangle->a, *triangle->b, *triangle->c,
                                     center, *(*it));
            if (side == SideResult::Different) {
                triangle->points.emplace(*it);
                it = points.erase(it);
            } else if (side == SideResult::FirstPointOnPlane ||
                       side == SideResult::SecondPointOnPlane ||
                       side == SideResult::BothPointsOnPlane) {
                it = points.erase(it);
            } else {
                ++it;
            }
        }
    }
}

Model getConvexHull(std::vector<Vec3f> vertices) {
    auto vertexRemoveIterator =
        std::unique(std::begin(vertices), std::end(vertices));
    vertices.erase(vertexRemoveIterator, std::end(vertices));

    std::set<Vec3f*> availablePoints;
    for (auto& vertex : vertices) {
        availablePoints.insert(&vertex);
    }
    // ищем большие и меньше координаты
    auto minX = std::numeric_limits<float>::max();
    Vec3f* minXPoint = nullptr;
    auto minY = std::numeric_limits<float>::max();
    Vec3f* minYPoint = nullptr;
    auto minZ = std::numeric_limits<float>::max();
    Vec3f* minZPoint = nullptr;

    auto maxX = std::numeric_limits<float>::lowest();
    Vec3f* maxXPoint = nullptr;
    auto maxY = std::numeric_limits<float>::lowest();
    Vec3f* maxYPoint = nullptr;
    auto maxZ = std::numeric_limits<float>::lowest();
    Vec3f* maxZPoint = nullptr;

    for (auto& v : vertices) {
        if (v.getX() < minX) {
            minX = v.getX();
            minXPoint = &v;
        }
        if (v.getX() > maxX) {
            maxX = v.getX();
            maxXPoint = &v;
        }
        if (v.getY() < minY) {
            minY = v.getY();
            minYPoint = &v;
        }
        if (v.getY() > maxY) {
            maxY = v.getY();
            maxYPoint = &v;
        }
        if (v.getZ() < minZ) {
            minZ = v.getZ();
            minZPoint = &v;
        }
        if (v.getZ() > maxZ) {
            maxZ = v.getZ();
            maxZPoint = &v;
        }
    }

    // ищем самые дальние точки
    std::pair<Vec3f*, Vec3f*> maxDistPoints;
    float currentMaxDist = std::numeric_limits<float>::lowest();
    std::vector<Vec3f*> selectedPoints = {minXPoint, maxXPoint, minYPoint,
                                          maxYPoint, minZPoint, maxZPoint};

    for (auto i = 0; i < selectedPoints.size() - 1; ++i) {
        auto selectedPointI = selectedPoints[i];
        for (auto j = i + 1; j < selectedPoints.size(); ++j) {
            auto selectedPointJ = selectedPoints[j];
            auto dist =
                std::abs(selectedPointI->getX() - selectedPointJ->getX()) +
                std::abs(selectedPointI->getY() - selectedPointJ->getY()) +
                std::abs(selectedPointI->getZ() - selectedPointJ->getZ());
            if (dist > currentMaxDist) {
                currentMaxDist = dist;
                maxDistPoints = {selectedPointI, selectedPointJ};
            }
        }
    }

    availablePoints.erase(maxDistPoints.first);
    availablePoints.erase(maxDistPoints.second);

    // ищем самую дальнюю точку от прямой, образованной найденными точками
    auto maxDistance = std::numeric_limits<float>::lowest();
    Vec3f* maxDistPoint = nullptr;
    for (auto& vertex : availablePoints) {
        auto d = Line(*maxDistPoints.first, *maxDistPoints.second)
                     .getDistanceToPoint(*vertex);
        if (d > maxDistance) {
            maxDistance = d;
            maxDistPoint = vertex;
        }
    }
    availablePoints.erase(maxDistPoint);

    std::list<Triangle> triangles;
    triangles.push_back(
        {maxDistPoints.first, maxDistPoints.second, maxDistPoint});

    // ищем самую дальную точку от треугольника
    maxDistance = std::numeric_limits<float>::lowest();
    maxDistPoint = nullptr;
    for (auto& vertex : availablePoints) {
        auto d =
            Plane::createFromPoints(*triangles.back().a, *triangles.back().b,
                                    *triangles.back().c)
                .getDistanceToPoint(*vertex);
        if (d > maxDistance) {
            maxDistance = d;
            maxDistPoint = vertex;
        }
    }
    availablePoints.erase(maxDistPoint);

    // добавляем новые треугольники
    triangles.push_back(
        {triangles.front().a, triangles.front().b, maxDistPoint});
    triangles.push_back(
        {triangles.front().b, triangles.front().c, maxDistPoint});
    triangles.push_back(
        {triangles.front().c, triangles.front().a, maxDistPoint});

    for (auto& triangle1 : triangles) {
        for (auto& triangle2 : triangles) {
            if (&triangle1 != &triangle2) {
                triangle1.neighbors.emplace(&triangle2);
            }
        }
    }

    std::set<Vec3f*> pointsSetForCheck = availablePoints;
    std::set<Triangle*> addedTriangles;
    for (auto& triangle : triangles) {
        addedTriangles.emplace(&triangle);
    }
    while (!pointsSetForCheck.empty() ||
           std::any_of(
               std::begin(triangles), std::end(triangles),
               [](const auto& triangle) { return !triangle.points.empty(); })) {

        // находим центр оболочки
        auto center = getCenter(triangles);

        // Сортируем точки по граням
        setPointToTriangles(addedTriangles, pointsSetForCheck, center);

        if (std::none_of(std::begin(triangles), std::end(triangles),
                         [](const auto& triangle) {
                             return !triangle.points.empty();
                         })) {
            break;
        }

        // находим самую дальную точку от вычисленной оболочки
        float maxDist = std::numeric_limits<float>::lowest();
        Triangle* maxDistTriangle = nullptr;
        Vec3f* maxDistPoint = nullptr;
        for (auto& triangle : triangles) {
            for (auto& point : triangle.points) {
                auto dist = Plane::createFromPoints(*triangle.a, *triangle.b,
                                                    *triangle.c)
                                .getDistanceToPoint(*point);

                if (dist > maxDist) {
                    maxDistTriangle = &triangle;
                    maxDistPoint = point;
                    maxDist = dist;
                }
            }
        }

        // Ищем горизонт
        auto horizont = getHorizont(*maxDistTriangle, *maxDistPoint, center);

        // создаём новые треугольники
        addedTriangles.clear();
        for (auto& edge : horizont.edges) {
            Triangle newTriangle;
            newTriangle.a = edge.edge.a;
            newTriangle.b = edge.edge.b;
            newTriangle.c = maxDistPoint;

            triangles.emplace_back(newTriangle);
            addedTriangles.emplace(&triangles.back());

            edge.notVisibleTriangle->neighbors.emplace(&triangles.back());
            triangles.back().neighbors.emplace(edge.notVisibleTriangle);
        }

        for (auto it = std::begin(addedTriangles);
             it != std::prev(std::end(addedTriangles), 1); ++it) {
            for (auto it2 = std::next(it, 1); it2 != std::end(addedTriangles);
                 ++it2) {
                auto edge = getEdgeBetweenTriangles(**it, **it2);
                if (edge.a != nullptr && edge.b != nullptr) {
                    (*it)->neighbors.emplace(*it2);
                    (*it2)->neighbors.emplace(*it);
                }
            }
        }

        pointsSetForCheck.clear();

        for (auto triangleForDelete : horizont.triangles) {
            pointsSetForCheck.merge(triangleForDelete->points);
            for (auto neighbor : triangleForDelete->neighbors) {
                assert(neighbor != triangleForDelete);
                neighbor->neighbors.erase(triangleForDelete);
            }
            auto found =
                std::find_if(std::begin(triangles), std::end(triangles),
                             [triangleForDelete](auto& triangle) {
                                 return &triangle == triangleForDelete;
                             });
            if (found != std::end(triangles)) {
                triangles.erase(found);
            }
        }
    }

    Model result;
    result.materials.emplace_back("");
    for (const auto& triangle : triangles) {
        result.vertices.push_back({triangle.a->getX(), triangle.a->getY(),
                                   triangle.a->getZ(), 0.0f, 0.0f, 0});
        result.vertices.push_back({triangle.b->getX(), triangle.b->getY(),
                                   triangle.b->getZ(), 1.0f, 0.0f, 0});
        result.vertices.push_back({triangle.c->getX(), triangle.c->getY(),
                                   triangle.c->getZ(), 0.0f, 1.0f, 0});

        result.faces.push_back(
            {static_cast<GLuint>(result.vertices.size() - 3),
             static_cast<GLuint>(result.vertices.size() - 2),
             static_cast<GLuint>(result.vertices.size() - 1)});
    }
    return result;
}