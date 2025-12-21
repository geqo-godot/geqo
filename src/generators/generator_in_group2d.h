#pragma once
#include "contexts/query_context2d.h"
#include "generators/query_generator2d.h"
#include <vector>
using namespace godot;
struct GeneratorInGroupState2D {
	int prev_context = 0;
	double time_budget_ms = 0;

	void reset() {
		prev_context = 0;
		time_budget_ms = 0;
	}
};
class GeneratorInGroup2D : public QueryGenerator2D {
	GDCLASS(GeneratorInGroup2D, QueryGenerator2D)

private:
	// QueryGenerator
	QueryContext2D *generate_around = nullptr;
	String group = "";
	double range_radius = 500.0;
	int collision_mask = 0;

	GeneratorInGroupState2D _current_state = GeneratorInGroupState2D();

public:
	GeneratorInGroup2D() {}
	~GeneratorInGroup2D() {}

	void set_generate_around(QueryContext2D *context);
	QueryContext2D *get_generate_around() { return generate_around; }

	void set_group(String new_group);
	String get_group() { return group; }

	void set_range_radius(double new_range);
	double get_range_radius() { return range_radius; }

	void set_collision_mask(int mask);
	double get_collision_mask() { return collision_mask; }

	void perform_generation(uint64_t initial_time_usec, int time_budget_ms) override;
	void _on_next_process_frame();

protected:
	static void _bind_methods();
};