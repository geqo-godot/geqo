#pragma once
#include "contexts/query_context3d.h"
#include "tests/query_test3d.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/ref.hpp>

using namespace godot;
class TestDistanceTo3D : public QueryTest3D {
	GDCLASS(TestDistanceTo3D, QueryTest3D)

private:
	Ref<Curve> scoring_curve;
	QueryContext3D *distance_to = nullptr;

public:
	TestDistanceTo3D() : QueryTest3D() {
		set_cost(1.39);
		set_test_type(TEST_TYPE_NUMERIC);
	}
	~TestDistanceTo3D() {}

	void set_scoring_curve(Ref<Curve> curve);
	Ref<Curve> get_scoring_curve() const { return scoring_curve; }

	void set_distance_to(QueryContext3D *context_node);
	QueryContext3D *get_distance_to() { return distance_to; }

	void perform_test(Ref<QueryInstance3D> query_instance) override;
	void _ready() override;

protected:
	static void _bind_methods();
};