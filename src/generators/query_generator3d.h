#pragma once

#include "generators/query_generator.h"
#include "query_instance.h"
#include "query_result.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/array.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include <vector>
using namespace godot;

class QueryGenerator3D : public Node3D, public QueryGeneratorBase<Vector3, QueryItem3D, QueryInstance3D> {
	GDCLASS(QueryGenerator3D, Node3D)

public:
	QueryGenerator3D() {}
	~QueryGenerator3D() {}

	Dictionary cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask = 1);
	TypedArray<Dictionary> cast_shape_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, Ref<Shape3D> shape, int col_mask = 1);
	// TODO: Replace this atrocity after working around Godot's binding restrictions
	void set_raycast_mode(RaycastMode mode) { return _set_raycast_mode(mode); }
	RaycastMode get_raycast_mode() const { return _get_raycast_mode(); }

	virtual void perform_generation(Ref<QueryInstance3D> query_instance);
	GDVIRTUAL1_REQUIRED(_perform_generation, Ref<QueryInstance3D>)

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryGenerator3D::RaycastMode);