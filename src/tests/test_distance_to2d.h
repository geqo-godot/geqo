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

	double min_distance = 0.0;
	double max_distance = 200.0;

public:
	TestDistanceTo2D() {}
	~TestDistanceTo2D() {}

	void set_scoring_curve(Ref<Curve> curve);
	Ref<Curve> get_scoring_curve() const { return scoring_curve; }

	void set_distance_to(QueryContext2D *context_node);
	QueryContext2D *get_distance_to() { return distance_to; }

	void set_min_distance(double dist);
	double get_min_distance() const { return min_distance; }

	void set_max_distance(double dist);
	double get_max_distance() const { return max_distance; }

	void perform_test(QueryItem<Vector2> &projection) override;
	void _ready() override;

protected:
	static void _bind_methods();
};