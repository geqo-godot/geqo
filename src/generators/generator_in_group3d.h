#pragma once
#include "contexts/query_context3d.h"
#include "generators/query_generator3d.h"
#include <vector>
using namespace godot;
struct GeneratorInGroupState3D {
	int prev_context = 0;
	double time_budget_ms = 0;

	void reset() {
		prev_context = 0;
		time_budget_ms = 0;
	}
};
class GeneratorInGroup3D : public QueryGenerator3D {
	GDCLASS(GeneratorInGroup3D, QueryGenerator3D)

private:
	// QueryGenerator
	QueryContext3D *generate_around = nullptr;
	String group = "";
	double range_radius = 50.0;
	int collision_mask = 0;

	GeneratorInGroupState3D _current_state = GeneratorInGroupState3D();

public:
	GeneratorInGroup3D() {}
	~GeneratorInGroup3D() {}

	void set_generate_around(QueryContext3D *context);
	QueryContext3D *get_generate_around() { return generate_around; }

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