#pragma once

#include "vec3.h"

class vertex {
public:
	vertex(const point3& position, const vec3& normal) : pos(position), normal(normal) {}

	point3 pos;
	vec3 normal;
};