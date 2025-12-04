#pragma once
#include "query_context3d.h"
#include "query_generator3d.h"
#include <vector>
using namespace godot;
class GeneratorCircleShape3D : public QueryGenerator3D {
	GDCLASS(GeneratorCircleShape3D, QueryGenerator3D)

private:
	// QueryGenerator
	QueryContext3D *circle_center = nullptr;
	double circle_radius = 10.0;
	double space_between = 1.0;

	double arc_angle = 360.0;

	// Cast Data
	bool use_casting = true;
	int cast_collision_mask = 1;

	// Projection Data
	bool use_vertical_projection = false;
	double project_down = 100.0;
	double project_up = 100.0;
	double post_projection_vertical_offset = 0.0;
	int projection_collision_mask = 1;

public:
	GeneratorCircleShape3D() {}
	~GeneratorCircleShape3D() {}

	void set_circle_center(QueryContext3D *context);
	QueryContext3D *get_circle_center() { return circle_center; }

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

	void perform_generation(std::vector<QueryItem> &query_item_list) override;

protected:
	static void _bind_methods();
};