#pragma once
#include "contexts/query_context2d.h"
#include "generators/query_generator2d.h"
#include <vector>
using namespace godot;
struct CircleShapeState2D {
	int prev_point = 0;
	int prev_context = 0;

	void reset() {
		prev_point = 0;
		prev_context = 0;
	}
};
class GeneratorCircleShape2D : public QueryGenerator2D {
	GDCLASS(GeneratorCircleShape2D, QueryGenerator2D)

private:
	// QueryGenerator
	QueryContext2D *circle_center = nullptr;
	double circle_radius = 200.0;
	double space_between = 20.0;

	double arc_angle = 360.0;

	// Cast Data
	bool use_casting = true;
	int cast_collision_mask = 1;

	CircleShapeState2D _current_state = CircleShapeState2D();

public:
	GeneratorCircleShape2D() {}
	~GeneratorCircleShape2D() {}

	void set_circle_center(QueryContext2D *context);
	QueryContext2D *get_circle_center() { return circle_center; }

	void set_circle_radius(double radius);
	double get_circle_radius() const { return circle_radius; }

	void set_space_between(double space);
	double get_space_between() const { return space_between; }

	void set_arc_angle(double angle);
	double get_arc_angle() const { return arc_angle; }

	void set_use_casting(bool use);
	bool get_use_casting() const { return use_casting; }

	void set_cast_collision_mask(int mask);
	int get_cast_collision_mask() const { return cast_collision_mask; }

	void perform_generation(uint64_t initial_time_usec, int time_budget_ms) override;

protected:
	static void _bind_methods();
};