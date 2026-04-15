#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
#include <godot_cpp/classes/collision_object2d.hpp>
#include <godot_cpp/classes/shape2d.hpp>
using namespace godot;

class TestIntersectsArea2D : public QueryTest2D {
	GDCLASS(TestIntersectsArea2D, QueryTest2D)

public:
	enum ShapeCastMode {
		BODY,
		AREA,
		BODY_AREA
	};

private:
	ShapeCastMode shape_cast_mode = BODY;
	QueryContext2D *context = nullptr;
	int collision_mask = 1;
	Ref<Shape2D> shape;
	Ref<QueryInstance2D> stored_instance;

public:
	TestIntersectsArea2D() : QueryTest2D() {
		set_cost(0.98);
		set_test_type(GEQOEnums::TEST_TYPE_BOOLEAN);
	}
	~TestIntersectsArea2D() {}

	void set_context(QueryContext2D *new_context);
	QueryContext2D *get_context() { return context; }

	void set_shape(Ref<Shape2D> new_shape);
	Ref<Shape2D> get_shape() const { return shape; }

	void set_collision_mask(int mask);
	int get_collision_mask() const { return collision_mask; }

	void set_shape_cast_mode(ShapeCastMode mode);
	ShapeCastMode get_shape_cast_mode() const { return shape_cast_mode; }

	bool evaluate_context_intersection(Ref<QueryItem2D> item, const Array &context_nodes);

	void _on_next_process_frame();
	void perform_test(Ref<QueryInstance2D> query_instance) override;

	PackedStringArray _get_configuration_warnings() const;

protected:
	static void _bind_methods();
	void _notification(int p_what);
};

VARIANT_ENUM_CAST(TestIntersectsArea2D::ShapeCastMode);