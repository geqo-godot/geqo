#pragma once

#include "generators/query_generator.h"
#include "query_result.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <vector>
using namespace godot;

class QueryGenerator3D : public Node3D, public QueryGeneratorBase<Vector3> {
	GDCLASS(QueryGenerator3D, Node3D)

public:
	QueryGenerator3D() {}
	~QueryGenerator3D() {}

	void perform_tests(int current_item) override;
	Dictionary cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask = 1);
	// TODO: Replace this atrocity after working around Godot's binding restrictions
	void set_raycast_mode(RaycastMode mode) { return _set_raycast_mode(mode); }
	RaycastMode get_raycast_mode() const { return _get_raycast_mode(); }

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryGenerator3D::RaycastMode);