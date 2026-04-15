#pragma once

#include "generators/query_generator.h"
#include "query_instance.h"
#include "query_result.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <vector>
using namespace godot;

class QueryGenerator2D : public Node2D, public QueryGeneratorBase<Vector2, QueryItem2D, QueryInstance2D> {
	GDCLASS(QueryGenerator2D, Node2D)

public:
	QueryGenerator2D() {}
	~QueryGenerator2D() {}

	void set_query_instance(Ref<QueryInstance2D> instance) {
		_set_query_instance(instance);
	}
	Ref<QueryInstance2D> get_query_instance() {
		return _get_query_instance();
	}
	void add_query_item(Ref<QueryItem2D> query_item) { return _add_query_item(query_item); }
	Dictionary cast_ray_projection(Vector2 start_pos, Vector2 end_pos, Array exclusions, int col_mask = 1);
	// TODO: Replace this atrocity after working around Godot's binding restrictions
	void set_raycast_mode(RaycastMode mode) { return _set_raycast_mode(mode); }
	RaycastMode get_raycast_mode() const { return _get_raycast_mode(); }

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryGenerator2D::RaycastMode);