#pragma once
#include "contexts/query_context3d.h"
#include "tests/query_test3d.h"
using namespace godot;
class TestDotProduct3D : public QueryTest3D {
	GDCLASS(TestDotProduct3D, QueryTest3D)
public:
	enum LineMode {
		LINE_ROTATION,
		LINE_TWO_POINTS
	};

private:
	double min_value = 0.5;
	double max_value = 1.0;

	QueryContext3D *from_place = nullptr;
	QueryContext3D *to_target = nullptr;

	// NEW
	// Line A
	LineMode line_a_mode = LINE_ROTATION;
	QueryContext3D *line_a_rotation = nullptr;
	QueryContext3D *line_a_from = nullptr;
	QueryContext3D *line_a_to = nullptr;

	// Line B

	LineMode line_b_mode = LINE_ROTATION;
	QueryContext3D *line_b_rotation = nullptr;
	QueryContext3D *line_b_from = nullptr;
	QueryContext3D *line_b_to = nullptr;

	bool use_absolute_value = false;

public:
	TestDotProduct3D() : QueryTest3D() {
		set_cost(0.78);
		set_test_type(GEQOEnums::TEST_TYPE_NUMERIC);
	}
	~TestDotProduct3D() {}

	void set_line_a_mode(LineMode mode);
	LineMode get_line_a_mode() const { return line_a_mode; }
	void set_line_a_rotation(QueryContext3D *context);
	QueryContext3D *get_line_a_rotation() { return line_a_rotation; }
	void set_line_a_from(QueryContext3D *context);
	QueryContext3D *get_line_a_from() { return line_a_from; }
	void set_line_a_to(QueryContext3D *context);
	QueryContext3D *get_line_a_to() { return line_a_to; }

	void set_line_b_mode(LineMode mode);
	LineMode get_line_b_mode() const { return line_b_mode; }
	void set_line_b_rotation(QueryContext3D *context);
	QueryContext3D *get_line_b_rotation() { return line_b_rotation; }
	void set_line_b_from(QueryContext3D *context);
	QueryContext3D *get_line_b_from() { return line_b_from; }
	void set_line_b_to(QueryContext3D *context);
	QueryContext3D *get_line_b_to() { return line_b_to; }

	void set_use_absolute_value(bool use);
	bool get_use_absolute_value() { return use_absolute_value; }

	Vector3 get_line(LineMode mode, QueryContext3D *rotation, QueryContext3D *from, QueryContext3D *to, Ref<QueryInstance3D> query_instance);
	Vector3 get_line_a(Ref<QueryInstance3D> query_instance);
	Vector3 get_line_b(Ref<QueryInstance3D> query_instance);
	void perform_test(Ref<QueryInstance3D> query_instance) override;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(TestDotProduct3D::LineMode);