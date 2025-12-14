#pragma once

#include "generators/query_generator.h"
#include "query_result.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <vector>
using namespace godot;

class QueryGenerator2D : public Node2D, public QueryGeneratorBase<Vector2> {
	GDCLASS(QueryGenerator2D, Node2D)

public:
	QueryGenerator2D() {}
	~QueryGenerator2D() {}

	void perform_tests(int current_item) override;
	Dictionary cast_ray_projection(Vector2 start_pos, Vector2 end_pos, Array exclusions, int col_mask = 1);
	// TODO: Replace this atrocity after working around Godot's binding restrictions
	void set_raycast_mode(RaycastMode mode) { return _set_raycast_mode(mode); }
	RaycastMode get_raycast_mode() const { return _get_raycast_mode(); }

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryGenerator2D::RaycastMode);