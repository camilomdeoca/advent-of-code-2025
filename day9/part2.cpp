#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <print>
#include <string>
#include <vector>
#include <sstream>

struct Vec2 {
    uint64_t x, y;

    uint64_t area(const Vec2 &v)
    {
        uint64_t side_x = (x > v.x ? x - v.x : v.x - x) + 1;
        uint64_t side_y = (y > v.y ? y - v.y : v.y - y) + 1;
        return side_x * side_y;
    }
};

struct Edge {
    Vec2 p1;
    Vec2 p2;    
};

bool intersects(const Edge& a, const Edge& b) {
    const bool a_vertical   = (a.p1.x == a.p2.x);
    const bool a_horizontal = (a.p1.y == a.p2.y);
    const bool b_vertical   = (b.p1.x == b.p2.x);
    const bool b_horizontal = (b.p1.y == b.p2.y);

    if (a_vertical && b_vertical) {
        if (a.p1.x != b.p1.x) return false;


        uint64_t a_min = std::min(a.p1.y, a.p2.y);
        uint64_t a_max = std::max(a.p1.y, a.p2.y);
        uint64_t b_min = std::min(b.p1.y, b.p2.y);
        uint64_t b_max = std::max(b.p1.y, b.p2.y);

        if (!(a_max <= b_min || b_max <= a_min))
        {
            // If the edges dont have the same direction its intersecting from the outside so 
            // it is intersecting a zone outside the shape
            if ((a.p1.y > a.p2.y) != (b.p1.y > b.p2.y)) return true;
            return false;
        }
    }

    if (a_horizontal && b_horizontal) {
        if (a.p1.y != b.p1.y) return false;

        uint64_t a_min = std::min(a.p1.x, a.p2.x);
        uint64_t a_max = std::max(a.p1.x, a.p2.x);
        uint64_t b_min = std::min(b.p1.x, b.p2.x);
        uint64_t b_max = std::max(b.p1.x, b.p2.x);

        if (!(a_max <= b_min || b_max <= a_min))
        {
            // If the edges dont have the same direction its intersecting from the outside so 
            // it is intersecting a zone outside the shape
            if ((a.p1.x > a.p2.x) != (b.p1.x > b.p2.x)) return true;
            return false;
        }
    }

    const Edge& v = a_vertical ? a : b;
    const Edge& h = a_vertical ? b : a;

    uint64_t vy_min = std::min(v.p1.y, v.p2.y);
    uint64_t vy_max = std::max(v.p1.y, v.p2.y);
    uint64_t hx_min = std::min(h.p1.x, h.p2.x);
    uint64_t hx_max = std::max(h.p1.x, h.p2.x);

    uint64_t vx = v.p1.x;
    uint64_t hy = h.p1.y;

    if (hx_min <= vx && vx <= hx_max && vy_min <= hy && hy <= vy_max)
    {
        // corner with corner
        if ((vy_min == hy || vy_max == hy) && (hx_min == vx || hx_max == vx))
        {
            return false;
        }

        if (a_vertical) // v is the poligon side, h is the rectangle edge
        {

            if (hx_min == vx)
                return v.p1.y < v.p2.y; // return true if pointing up
            if (hx_max == vx)
                return v.p1.y > v.p2.y; // return true if pointing down
            if (vy_min == hy)
                return h.p1.x < h.p2.x;
            if (vy_max == hy)
                return h.p1.x > h.p2.x;
            return true;
        }
        else // h is the poligon side, v is the rectangle edge
        {
            if (vy_min == hy)
                return h.p1.x > h.p2.x; // return true if poligon side points left
            if (vy_max == hy)
                return h.p1.x < h.p2.x; // return true if poligon side points right
            if (hx_min == vx)
                return v.p1.y > v.p2.y;
            if (hx_max == vx)
                return v.p1.y < v.p2.y;
            return true; // Else edges intersect in the middle (always bad)
        }
    }
    return false;
}

bool is_inside_rect(const Vec2 &p, const std::array<Vec2, 2> rectangle_corners)
{
    return p.x > std::min(rectangle_corners[0].x, rectangle_corners[1].x)
        && p.x < std::max(rectangle_corners[0].x, rectangle_corners[1].x)
        && p.y > std::min(rectangle_corners[0].y, rectangle_corners[1].y)
        && p.y < std::max(rectangle_corners[0].y, rectangle_corners[1].y);
}

std::array<Edge, 4> edges_for_rect(const Vec2 &corner1, const Vec2 &corner2)
{
    const uint64_t x_max = std::max(corner1.x, corner2.x);
    const uint64_t x_min = std::min(corner1.x, corner2.x);
    const uint64_t y_max = std::max(corner1.y, corner2.y);
    const uint64_t y_min = std::min(corner1.y, corner2.y);
    
    return {{
        {{ x_max, y_min }, { x_max, y_max }}, // Edge 0 tr-br
        {{ x_max, y_max }, { x_min, y_max }}, // Edge 3 br-bl
        {{ x_min, y_max }, { x_min, y_min }}, // Edge 2 bl-tl
        {{ x_min, y_min }, { x_max, y_min }}, // Edge 1 tl-tr
    }};
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::println(stderr, "usage: %s input", argv[0]);
        return 1;
    }

    std::ifstream ifs(argv[1]);

    std::vector<Vec2> positions;

    std::string line;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        Vec2 pos;
        char comma;
        ss >> pos.x >> comma >> pos.y;
        positions.emplace_back(pos);
    }
    std::println("Loaded positions.");
    const size_t pos_count = positions.size();

    std::vector<Edge> edges;
    for (size_t i = 0; i < pos_count; i++)
    {
        edges.emplace_back(positions[i], positions[(i + 1) % pos_count]);
    }

    uint64_t max_area = 0;
    for (size_t i = 0; i < pos_count; i++)
    {
        for (size_t j = i+1; j < pos_count; j++)
        {
            // Hope the solution is not a thin rectangle
            if (positions[i].x == positions[j].x || positions[i].y == positions[j].y) continue;

            // Dont do expensive intersection detection if it isnt larger than the maximum
            uint64_t area = positions[i].area(positions[j]);
            if (area <= max_area) continue;

            std::println();
            std::println(
                "Trying: [{}, {}] - [{}, {}]  \tArea: {}",
                positions[i].x, positions[i].y,
                positions[j].x, positions[j].y,
                area
            );

            const std::array<Edge, 4> rectangle_edges = edges_for_rect(positions[i], positions[j]);

            bool intersects_edge = false;
            for (const Edge &intersecting_candidate : edges)
            {
                // If no point of the edge is inside the rectangle it doesnt affect
                // if (is_inside_rect(intersecting_candidate.p1, {positions[i], positions[j]})
                //     && is_inside_rect(intersecting_candidate.p2, {positions[i], positions[j]})
                // ) {
                //     std::println("INSIDE");
                //     intersects_edge = true;
                //     break;
                // }

                {
                    for (const Edge &rect_edge : rectangle_edges)
                    {
                        if (intersects(intersecting_candidate, rect_edge))
                        {
                            std::println(
                                "INTERSECT [{}, {}] - [{}, {}]",
                                intersecting_candidate.p1.x, intersecting_candidate.p1.y,
                                intersecting_candidate.p2.x, intersecting_candidate.p2.y
                            );
                            std::println(
                                "BECAUSEOF [{}, {}] - [{}, {}]",
                                rect_edge.p1.x, rect_edge.p1.y,
                                rect_edge.p2.x, rect_edge.p2.y
                            );
                            intersects_edge = true;
                            break;
                        }
                    }
                }
                if (intersects_edge) break;
            }
            if (!intersects_edge)
            {
                std::println(
                    "Found larger rectangle: [{}, {}] - [{}, {}]  \tArea: {}",
                    positions[i].x, positions[i].y,
                    positions[j].x, positions[j].y,
                    area
                );
                max_area = area;
            }
        }
    }

    std::println("Largest area: {}", max_area);

    return 0;
}
