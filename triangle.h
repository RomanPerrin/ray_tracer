#pragma once

#include "vec3.h"
#include "vertex.h"
#include "ray.h"
#include "hit.h"

class triangle {
public:
    triangle() {}
	triangle(const vertex& vert1, const vertex& vert2, const vertex& vert3) : vert1(vert1), vert2(vert2), vert3(vert3) {}

	const vertex vert1;
	const vertex vert2;
	const vertex vert3;

    bool sameSide(const vec3& hit) {
        vec3 v1 = vert2.pos - vert3.pos;
        vec3 a1 = cross(v1, (hit - vert3.pos));
        vec3 b1 = cross(v1, (vert1.pos - vert3.pos));
        double c1 = dot(a1, b1);

        vec3 v2 = vert1.pos - vert2.pos;
        vec3 a2 = cross(v2, (hit - vert2.pos));
        vec3 b2 = cross(v2, (vert3.pos - vert2.pos));
        double c2 = dot(a2, b2);

        vec3 v3 = vert2.pos - vert1.pos;
        vec3 a3 = cross(v3, (hit - vert1.pos));
        vec3 b3 = cross(v3, (vert3.pos - vert1.pos));
        double c3 = dot(a3, b3);

        if (c1 >= 0 && c2 >= 0 && c3 >= 0) {
            return true;
        }

        return false;
    }

    Hit hit(const ray& r) {
        vec3 planeVector1 = vert2.pos - vert1.pos;
        vec3 planeVector2 = vert3.pos - vert1.pos;

        vec3 normal = cross(planeVector1, planeVector2);
        double k = dot(-normal, vert1.pos);

        double t = -(dot(normal, r.origin()) + k) / dot(normal, r.direction());

        //hit not in view
        if (0.0 <= t && t <= 5000.0) {
            point3 pos = r.at(t);

            //hit not in the triangle bounding box
            auto minX = std::min(std::min(vert1.pos[0], vert2.pos[0]), vert3.pos[0]);
            auto minY = std::min(std::min(vert1.pos[1], vert2.pos[1]), vert3.pos[1]);
            auto minZ = std::min(std::min(vert1.pos[2], vert2.pos[2]), vert3.pos[2]);
            auto maxX = std::max(std::max(vert1.pos[0], vert2.pos[0]), vert3.pos[0]);
            auto maxY = std::max(std::max(vert1.pos[1], vert2.pos[1]), vert3.pos[1]);
            auto maxZ = std::max(std::max(vert1.pos[2], vert2.pos[2]), vert3.pos[2]);
            if ((minX <= pos[0] && pos[0] <= maxX) && (minY <= pos[1] && pos[1] <= maxY) && (minZ <= pos[2] && pos[2] <= maxZ)) {
                //hit not in the triangle
                if (sameSide(pos)) {
                    return Hit(pos, t);
                }
            }
        }
        return Hit();
    }
};