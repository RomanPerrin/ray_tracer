#pragma once

#include "vec3.h"

class Hit {
public:
	Hit() {}
	Hit(const point3& position,
		const double& t) :
		pos(position),
		t(t) {}

	point3 pos;
	//vec3 normal;
	double t = INFINITY;
	//float u;
	//float v;
};