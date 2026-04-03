#pragma once
#include "contexts/query_context3d.h"
#include "tests/query_test3d.h"
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/shape3d.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/typed_array.hpp>
using namespace godot;
class TestRaycastTo3D : public QueryTest3D {
	GDCLASS(TestRaycastTo3D, QueryTest3D)
public:
	enum RaycastMode {
		BODY,
		AREA,
		BODY_AREA
	};

private:
	QueryContext3D *context = nullptr;
	bool hitting_is_true = true;
	bool cast_from_context = false;
	TypedArray<NodePath> exclusions;
	int collision_mask = 1;
	RaycastMode raycast_mode = BODY;
	bool use_shape_cast = false;
	Ref<Shape3D> shape;

public:
	TestRaycastTo3D() : QueryTest3D() {
			set_cost(1.42);
	}
	~TestRaycastTo3D() {}

	void set_context(QueryContext3D *context_node);
	QueryContext3D *get_context() { return context; }

	void set_hitting_is_true(bool hitting);
	bool get_hitting_is_true() const { return hitting_is_true; }

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

	void set_shape(Ref<Shape3D> new_shape);
	Ref<Shape3D> get_shape() const { return shape; }

	Dictionary cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask = 1);
	TypedArray<Dictionary> cast_shape_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, Ref<Shape3D> shape, int col_mask = 1);

	void perform_test(Ref<QueryItem3D> projection) override;

protected:
	static void _bind_methods();
	void _validate_property(PropertyInfo &property) const;
};

VARIANT_ENUM_CAST(TestRaycastTo3D::RaycastMode);