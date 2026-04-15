#pragma once

#include "generators/query_generator.h"
#include "query_instance.h"
#include "query_result.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/array.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

#include <vector>
using namespace godot;

class QueryGenerator2D : public Node2D, public QueryGeneratorBase<Vector2, QueryItem2D, QueryInstance2D> {
	GDCLASS(QueryGenerator2D, Node2D)

public:
	QueryGenerator2D() {}
	~QueryGenerator2D() {}

	Dictionary cast_ray_projection(Vector2 start_pos, Vector2 end_pos, Array exclusions, int col_mask = 1);
	// TODO: Replace this atrocity after working around Godot's binding restrictions
	void set_raycast_mode(RaycastMode mode) { return _set_raycast_mode(mode); }
	RaycastMode get_raycast_mode() const { return _get_raycast_mode(); }

	virtual void perform_generation(Ref<QueryInstance2D> query_instance);
	GDVIRTUAL1_REQUIRED(_perform_generation, Ref<QueryInstance2D>)

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryGenerator2D::RaycastMode);