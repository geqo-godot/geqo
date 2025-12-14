#pragma once
#include "contexts/query_context2d.h"
#include "generators/query_generator2d.h"
#include <vector>
using namespace godot;
struct GridShapeState2D {
	int time_budget_ms = 0;
	int prev_y = 0;
	int prev_x = 0;
	int prev_context = 0;

	void reset() {
		prev_y = 0;
		prev_x = 0;
		prev_context = 0;
	}
};
class GeneratorGridShape2D : public QueryGenerator2D {
	GDCLASS(GeneratorGridShape2D, QueryGenerator2D)

private:
	// QueryGenerator
	double grid_half_size = 200.0;
	double space_between = 50.0;
	QueryContext2D *generate_around = nullptr;

	GridShapeState2D _current_state = GridShapeState2D();

public:
	GeneratorGridShape2D() {}
	~GeneratorGridShape2D() {}

	void set_grid_half_size(double size);
	double get_grid_half_size() const { return grid_half_size; }

	void set_space_between(double space);
	double get_space_between() const { return space_between; }

	void set_generate_around(QueryContext2D *context);
	QueryContext2D *get_generate_around() { return generate_around; }

	void perform_generation(uint64_t initial_time_usec, int time_budget_ms) override;
	void _on_next_process_frame();

protected:
	static void _bind_methods();
};