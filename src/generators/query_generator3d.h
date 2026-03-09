#pragma once

#include "generators/query_generator.h"
#include "query_result.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <vector>
using namespace godot;

class QueryGenerator3D : public Node3D, public QueryGeneratorBase<Vector3, QueryItem3D> {
	GDCLASS(QueryGenerator3D, Node3D)

public:
	QueryGenerator3D() {}
	~QueryGenerator3D() {}

	void set_query_items_ref(std::vector<Ref<QueryItem3D>> &query_items) { return _set_query_items_ref(query_items); }
	void add_query_item(Ref<QueryItem3D> query_item) { return _add_query_item(query_item); }
	void perform_tests(int current_item) override;
	Dictionary cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask = 1);
	TypedArray<Dictionary> cast_shape_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, Ref<Shape3D> shape, int col_mask = 1);
	// TODO: Replace this atrocity after working around Godot's binding restrictions
	void set_raycast_mode(RaycastMode mode) { return _set_raycast_mode(mode); }
	RaycastMode get_raycast_mode() const { return _get_raycast_mode(); }

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(QueryGenerator3D::RaycastMode);