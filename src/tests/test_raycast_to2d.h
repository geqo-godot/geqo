#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
#include <godot_cpp/classes/collision_object2d.hpp>
#include <godot_cpp/classes/shape2d.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/typed_array.hpp>
using namespace godot;

class TestRaycastTo2D : public QueryTest2D {
	GDCLASS(TestRaycastTo2D, QueryTest2D)
public:
	enum RaycastMode {
		BODY,
		AREA,
		BODY_AREA
	};

private:
	QueryContext2D *context = nullptr;
	bool cast_from_context = false;
	TypedArray<NodePath> exclusions;
	int collision_mask = 1;
	RaycastMode raycast_mode = BODY;
	bool use_shape_cast = false;
	Ref<Shape2D> shape;
	Ref<QueryInstance2D> stored_instance;

public:
	TestRaycastTo2D() : QueryTest2D() {
		set_cost(1.42);
		set_test_type(GEQOEnums::TEST_TYPE_BOOLEAN);
	}
	~TestRaycastTo2D() {}

	void set_context(QueryContext2D *context_node);
	QueryContext2D *get_context() { return context; }

	void set_cast_from_context(bool cast);
	bool get_cast_from_context() const { return cast_from_context; }

	void set_exclusions(TypedArray<NodePath> exclusion);
	TypedArray<NodePath> get_exclusions() const { return exclusions; }

	void set_collision_mask(int mask);
	int get_collision_mask() const { return collision_mask; }

	void set_raycast_mode(RaycastMode mode);
	RaycastMode get_raycast_mode() const { return raycast_mode; }

	void set_use_shape_cast(bool use);
	bool get_use_shape_cast() const { return use_shape_cast; }

	void set_shape(Ref<Shape2D> new_shape);
	Ref<Shape2D> get_shape() const { return shape; }

	Array resolve_exclusions();
	bool cast_to_node(Ref<QueryItem2D> item, Node2D *context_node, const Array &exclusion_nodes);
	bool evaluate_context_hit_any(Ref<QueryItem2D> item, const Array &context_nodes, const Array &exclusion_nodes);
	bool evaluate_context_hit_all(Ref<QueryItem2D> item, const Array &context_nodes, const Array &exclusion_nodes);

	Dictionary cast_ray_projection(Vector2 start_pos, Vector2 end_pos, Array exclusions, int col_mask = 1);
	TypedArray<Dictionary> cast_shape_projection(Vector2 start_pos, Vector2 end_pos, Array exclusions, Ref<Shape2D> shape, int col_mask = 1);

	void _on_next_process_frame();
	void perform_test(Ref<QueryInstance2D> query_instance) override;

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;
};

VARIANT_ENUM_CAST(TestRaycastTo2D::RaycastMode);