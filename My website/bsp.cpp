#include <vector>
#include <algorithm>
#include <iostream>

struct Point {
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};

struct Polygon {
    std::vector<Point> points;
    Polygon(const std::vector<Point>& points) : points(points) {}
};

struct BSPNode {
    Polygon polygon;
    BSPNode* front;
    BSPNode* back;
    BSPNode(const Polygon& polygon, BSPNode* front, BSPNode* back) :
        polygon(polygon), front(front), back(back) {}
};

class BSPTree {
public:
    BSPTree(const std::vector<Polygon>& polygons) {
        if (polygons.empty()) {
            root_ = nullptr;
        } else {
            root_ = buildBSP(polygons);
        }
    }

    ~BSPTree() {
        destroyBSP(root_);
    }

    void traverse() const {
        traverse(root_);
    }

private:
    BSPNode* root_;

    static BSPNode* buildBSP(const std::vector<Polygon>& polygons) {
        // Choose a splitting plane
        Polygon plane = polygons[0];
        Point a = plane.points[0];
        Point b = plane.points[1];
        Point normal = { b.y - a.y, a.x - b.x };

        // Split the polygons into front and back sets
        std::vector<Polygon> frontPolygons;
        std::vector<Polygon> backPolygons;
        for (const Polygon& polygon : polygons) {
            int frontCount = 0;
            int backCount = 0;
            int size = polygon.points.size();
            for (int i = 0; i < size; ++i) {
                const Point& p1 = polygon.points[i];
                const Point& p2 = polygon.points[(i + 1) % size];
                double d = (p2.x - p1.x) * normal.x + (p2.y - p1.y) * normal.y;
                if (d > 0) {
                    ++frontCount;
                } else if (d < 0) {
                    ++backCount;
                }
            }
            if (frontCount == size) {
                frontPolygons.push_back(polygon);
            } else if (backCount == size) {
                backPolygons.push_back(polygon);
            } else {
                std::vector<Point> frontPoints;
                std::vector<Point> backPoints;
                for (int i = 0; i < size; ++i) {
                    const Point& p1 = polygon.points[i];
                    const Point& p2 = polygon.points[(i + 1) % size];
                    double d = (p2.x - p1.x) * normal.x + (p2.y - p1.y) * normal.y;
                    if (d > 0) {
                        frontPoints.push_back(p1);
                    } else if (d < 0) {
                        backPoints.push_back(p1);
                    } else {
                        frontPoints.push_back(p1);
                        backPoints.push_back(p1);
                    }
                }
                frontPoints.insert(frontPoints.end(), backPoints.rbegin(), backPoints.rend());
                Polygon frontPolygon(frontPoints);
                frontPolygons.push_back(frontPolygon);
                Polygon backPolygon(backPoints);
                backPolygons.push_back(backPolygon);
            }
        }

        // Recursively build
