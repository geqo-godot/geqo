#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
using namespace godot;
class TestDotProduct2D : public QueryTest2D {
	GDCLASS(TestDotProduct2D, QueryTest2D)
public:
	enum LineMode {
		LINE_ROTATION,
		LINE_TWO_POINTS
	};

private:
	// Line A
	LineMode line_a_mode = LINE_ROTATION;
	QueryContext2D *line_a_rotation = nullptr;
	QueryContext2D *line_a_from = nullptr;
	QueryContext2D *line_a_to = nullptr;

	// Line B

	LineMode line_b_mode = LINE_ROTATION;
	QueryContext2D *line_b_rotation = nullptr;
	QueryContext2D *line_b_from = nullptr;
	QueryContext2D *line_b_to = nullptr;

	bool use_absolute_value = false;

public:
	TestDotProduct2D() : QueryTest2D() {
		set_cost(0.78);
		set_test_type(GEQOEnums::TEST_TYPE_NUMERIC);
	}
	~TestDotProduct2D() {}

	void set_line_a_mode(LineMode mode);
	LineMode get_line_a_mode() const { return line_a_mode; }
	void set_line_a_rotation(QueryContext2D *context);
	QueryContext2D *get_line_a_rotation() { return line_a_rotation; }
	void set_line_a_from(QueryContext2D *context);
	QueryContext2D *get_line_a_from() { return line_a_from; }
	void set_line_a_to(QueryContext2D *context);
	QueryContext2D *get_line_a_to() { return line_a_to; }

	void set_line_b_mode(LineMode mode);
	LineMode get_line_b_mode() const { return line_b_mode; }
	void set_line_b_rotation(QueryContext2D *context);
	QueryContext2D *get_line_b_rotation() { return line_b_rotation; }
	void set_line_b_from(QueryContext2D *context);
	QueryContext2D *get_line_b_from() { return line_b_from; }
	void set_line_b_to(QueryContext2D *context);
	QueryContext2D *get_line_b_to() { return line_b_to; }

	void set_use_absolute_value(bool use);
	bool get_use_absolute_value() { return use_absolute_value; }

	Vector2 get_line(LineMode mode, QueryContext2D *rotation, QueryContext2D *from, QueryContext2D *to, Ref<QueryInstance2D> query_instance);
	Vector2 get_line_a(Ref<QueryInstance2D> query_instance);
	Vector2 get_line_b(Ref<QueryInstance2D> query_instance);
	void perform_test(Ref<QueryInstance2D> query_instance) override;

protected:
	static void _bind_methods();
};

VARIANT_ENUM_CAST(TestDotProduct2D::LineMode);