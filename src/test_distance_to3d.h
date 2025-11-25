#pragma once
#include "query_context3d.h"
#include "query_test3d.h"
#include <godot_cpp/classes/curve.hpp>
#include <godot_cpp/classes/ref.hpp>

using namespace godot;
class CTestDistanceTo3D : public CQueryTest3D {
	GDCLASS(CTestDistanceTo3D, CQueryTest3D)

private:
	Ref<Curve> scoring_curve;
	CQueryContext3D *distance_to = nullptr;

	double min_distance = 0.0;
	double max_distance = 10.0;

public:
	CTestDistanceTo3D() {}
	~CTestDistanceTo3D() {}

	void set_scoring_curve(Ref<Curve> curve);
	Ref<Curve> get_scoring_curve() const { return scoring_curve; }

	void set_distance_to(CQueryContext3D *context_node);
	CQueryContext3D *get_distance_to() { return distance_to; }

	void set_min_distance(double dist);
	double get_min_distance() const { return min_distance; }

	void set_max_distance(double dist);
	double get_max_distance() const { return max_distance; }

	void perform_test(CQueryItem &projection) override;
	void _ready() override;

protected:
	static void _bind_methods();
};