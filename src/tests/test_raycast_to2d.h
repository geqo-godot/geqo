#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
#include <godot_cpp/classes/collision_object2d.hpp>
#include <godot_cpp/classes/curve.hpp>
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
	bool hitting_is_true = true;
	bool cast_from_context = false;
	TypedArray<NodePath> exclusions;
	int collision_mask = 1;
	RaycastMode raycast_mode = BODY;

public:
	TestRaycastTo2D() {}
	~TestRaycastTo2D() {}

	void set_context(QueryContext2D *context_node);
	QueryContext2D *get_context() { return context; }

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

	void perform_test(Ref<QueryItem2D> projection) override;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(TestRaycastTo2D::RaycastMode);