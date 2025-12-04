#pragma once
#include "query_context3d.h"
#include "query_test3d.h"
using namespace godot;
class TestDotProduct3D : public QueryTest3D {
	GDCLASS(TestDotProduct3D, QueryTest3D)

private:
	double min_value = 0.5;
	double max_value = 1.0;

	QueryContext3D *from_place = nullptr;
	QueryContext3D *to_target = nullptr;

public:
	TestDotProduct3D() {}
	~TestDotProduct3D() {}

	void set_min_value(double value);
	double get_min_value() { return min_value; }

	void set_max_value(double value);
	double get_max_value() { return max_value; }

	void set_from_place(QueryContext3D *context);
	QueryContext3D *get_from_place() { return from_place; }

	void set_to_target(QueryContext3D *context);
	QueryContext3D *get_to_target() { return to_target; }

	void perform_test(QueryItem &projection) override;

protected:
	static void _bind_methods();
};