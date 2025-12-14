#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
using namespace godot;
class TestDotProduct2D : public QueryTest2D {
	GDCLASS(TestDotProduct2D, QueryTest2D)

private:
	double min_value = 0.5;
	double max_value = 1.0;

	QueryContext2D *from_place = nullptr;
	QueryContext2D *to_target = nullptr;

public:
	TestDotProduct2D() {}
	~TestDotProduct2D() {}

	void set_min_value(double value);
	double get_min_value() { return min_value; }

	void set_max_value(double value);
	double get_max_value() { return max_value; }

	void set_from_place(QueryContext2D *context);
	QueryContext2D *get_from_place() { return from_place; }

	void set_to_target(QueryContext2D *context);
	QueryContext2D *get_to_target() { return to_target; }

	void perform_test(QueryItem<Vector2> &projection) override;

protected:
	static void _bind_methods();
};