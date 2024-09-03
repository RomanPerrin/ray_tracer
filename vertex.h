#pragma once

#include "vec3.h"

class vertex {
public:
	vertex() {}
	vertex(const point3& position) : pos(position) {}
	vertex(const point3& position, const vec3& normal) : pos(position), normal(normal) {}
	vertex(const point3& position, const vec3& normal, const float& u, const float& v) : pos(position), normal(normal), u(u), v(v) {}

	point3 pos;
	vec3 normal;
	float u;
	float v;
};