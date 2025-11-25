#pragma once
#include "query_context3d.h"
#include "query_test3d.h"
using namespace godot;
class CTestDotProduct3D : public CQueryTest3D {
	GDCLASS(CTestDotProduct3D, CQueryTest3D)

private:
	double min_value = 0.5;
	double max_value = 1.0;

	CQueryContext3D *from_place = nullptr;
	CQueryContext3D *to_target = nullptr;

public:
	CTestDotProduct3D() {}
	~CTestDotProduct3D() {}

	void set_min_value(double value);
	double get_min_value() { return min_value; }

	void set_max_value(double value);
	double get_max_value() { return max_value; }

	void set_from_place(CQueryContext3D *context);
	CQueryContext3D *get_from_place() { return from_place; }

	void set_to_target(CQueryContext3D *context);
	CQueryContext3D *get_to_target() { return to_target; }

	void perform_test(CQueryItem &projection) override;

protected:
	static void _bind_methods();
};