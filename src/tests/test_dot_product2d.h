#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
using namespace godot;
class TestDotProduct2D : public QueryTest2D {
	GDCLASS(TestDotProduct2D, QueryTest2D)

private:
	QueryContext2D *from_place = nullptr;
	QueryContext2D *to_target = nullptr;

public:
	TestDotProduct2D() : QueryTest2D() {
		set_cost(0.78);
		set_test_type(TEST_TYPE_NUMERIC);
	}
	~TestDotProduct2D() {}

	void set_from_place(QueryContext2D *context);
	QueryContext2D *get_from_place() { return from_place; }

	void set_to_target(QueryContext2D *context);
	QueryContext2D *get_to_target() { return to_target; }

	void perform_test(Ref<QueryInstance2D> query_instance) override;

protected:
	static void _bind_methods();
};