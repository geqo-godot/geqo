#pragma once
#include "contexts/query_context3d.h"
#include "generators/query_generator3d.h"
#include <vector>
using namespace godot;
struct GridShapeState3D {
	int time_budget_ms = 0;
	int prev_z = 0;
	int prev_x = 0;
	int prev_context = 0;

	void reset() {
		prev_z = 0;
		prev_x = 0;
		prev_context = 0;
	}
};
class GeneratorGridShape3D : public QueryGenerator3D {
	GDCLASS(GeneratorGridShape3D, QueryGenerator3D)

private:
	// QueryGenerator
	double grid_half_size = 20.0;
	double space_between = 5.0;
	QueryContext3D *generate_around = nullptr;

	// Projection Data
	bool use_vertical_projection = true;
	double project_down = 100.0;
	double project_up = 100.0;
	double post_projection_vertical_offset = 0.0;
	int projection_collision_mask = 1;
	GridShapeState3D _current_state = GridShapeState3D();

public:
	GeneratorGridShape3D() {}
	~GeneratorGridShape3D() {}

	void set_grid_half_size(double size);
	double get_grid_half_size() const { return grid_half_size; }

	void set_space_between(double space);
	double get_space_between() const { return space_between; }

	void set_generate_around(QueryContext3D *context);
	QueryContext3D *get_generate_around() { return generate_around; }

	void set_use_vertical_projection(bool use);
	bool get_use_vertical_projection() const { return use_vertical_projection; }

	void set_project_down(double project);
	double get_project_down() const { return project_down; }

	void set_project_up(double project);
	double get_project_up() const { return project_up; }

	void set_post_projection_vertical_offset(double offset);
	double get_post_projection_vertical_offset() const { return post_projection_vertical_offset; }

	void set_projection_collision_mask(int mask);
	int get_projection_collision_mask() const { return projection_collision_mask; }

	void perform_generation(uint64_t initial_time_usec, int time_budget_ms) override;
	void _on_next_process_frame();

protected:
	static void _bind_methods();
};