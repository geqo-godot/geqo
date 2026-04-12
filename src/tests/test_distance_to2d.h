#pragma once
#include "contexts/query_context2d.h"
#include "tests/query_test2d.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/ref.hpp>

using namespace godot;
class TestDistanceTo2D : public QueryTest2D {
	GDCLASS(TestDistanceTo2D, QueryTest2D)

private:
	Ref<Curve> scoring_curve;
	QueryContext2D *distance_to = nullptr;

public:
	TestDistanceTo2D() : QueryTest2D() {
		set_cost(1.39);
		set_test_type(TEST_TYPE_NUMERIC);
	}
	~TestDistanceTo2D() {}

	void set_scoring_curve(Ref<Curve> curve);
	Ref<Curve> get_scoring_curve() const { return scoring_curve; }

	void set_distance_to(QueryContext2D *context_node);
	QueryContext2D *get_distance_to() { return distance_to; }

	void perform_test(Ref<QueryInstance2D> query_instance) override;
	void _ready() override;

protected:
	static void _bind_methods();
};