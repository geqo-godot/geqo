#pragma once

#include "generators/query_generator.h"
#include "query_result.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <vector>
using namespace godot;

class QueryGenerator3D : public Node3D, QueryGeneratorBase {
	GDCLASS(QueryGenerator3D, Node3D)

public:
	QueryGenerator3D() {}
	~QueryGenerator3D() {}

	void perform_tests(int current_item);
	Dictionary cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask = 1);

protected:
	static void _bind_methods();
};