#pragma once
#include "contexts/query_context3d.h"
#include "tests/query_test3d.h"
#include <godot_cpp/classes/shape3d.hpp>
using namespace godot;
class TestIntersectsArea3D : public QueryTest3D {
	GDCLASS(TestIntersectsArea3D, QueryTest3D)

public:
	enum ShapeCastMode {
		BODY,
		AREA,
		BODY_AREA
	};

private:
	ShapeCastMode shape_cast_mode = BODY;
	QueryContext3D *context = nullptr;
	int collision_mask = 1;
	Ref<Shape3D> shape;

public:
	TestIntersectsArea3D() : QueryTest3D() {
		set_cost(0.98);
		set_test_type(TEST_TYPE_BOOLEAN);
	}
	~TestIntersectsArea3D() {}

	void set_context(QueryContext3D *new_context);
	QueryContext3D *get_context() { return context; }

	void set_shape(Ref<Shape3D> new_shape);
	Ref<Shape3D> get_shape() const { return shape; }
	void set_collision_mask(int mask);
	int get_collision_mask() const { return collision_mask; }
	void set_shape_cast_mode(ShapeCastMode mode);
	ShapeCastMode get_shape_cast_mode() const { return shape_cast_mode; }

	void perform_test(Ref<QueryItem3D> projection) override;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(TestIntersectsArea3D::ShapeCastMode);