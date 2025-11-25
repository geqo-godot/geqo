#pragma once
#include "query_context3d.h"
#include "query_test3d.h"
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/variant/node_path.hpp>
#include <godot_cpp/variant/typed_array.hpp>
using namespace godot;
class CTestRaycastTo3D : public CQueryTest3D {
	GDCLASS(CTestRaycastTo3D, CQueryTest3D)

private:
	CQueryContext3D *context = nullptr;
	bool hitting_is_true = true;
	bool cast_from_context = false;
	TypedArray<NodePath> exclusions;
	int collision_mask = 1;

public:
	CTestRaycastTo3D() {}
	~CTestRaycastTo3D() {}

	void set_context(CQueryContext3D *context_node);
	CQueryContext3D *get_context() { return context; }

	void set_hitting_is_true(bool hitting);
	bool get_hitting_is_true() const { return hitting_is_true; }

	void set_cast_from_context(bool cast);
	bool get_cast_from_context() const { return cast_from_context; }

	void set_exclusions(TypedArray<NodePath> exclusion);
	TypedArray<NodePath> get_exclusions() const { return exclusions; }

	void set_collision_mask(int mask);
	int get_collision_mask() const { return collision_mask; }

	void perform_test(CQueryItem &projection) override;

protected:
	static void _bind_methods();
};