#ifndef QUERYGENERATOR3D_H
#define QUERYGENERATOR3D_H

#include "query_result.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <vector>
using namespace godot;

class QueryGenerator3D : public Node3D {
	GDCLASS(QueryGenerator3D, Node3D)

public:
	enum RaycastMode {
		BODY,
		AREA,
		BODY_AREA
	};

private:
	RaycastMode raycast_mode = BODY;
	// TODO: Check how many rays have been casted in total, cap it so it does a certain amount of rays per tick
	// Since rays are the heaviest calculations by far I think
	int casted_rays = 0;
	int rays_per_tick = 1000;

public:
	QueryGenerator3D() {}
	~QueryGenerator3D() {}

	void add_ray_tick();
	int get_rays_per_tick() { return rays_per_tick; };

	void set_raycast_mode(RaycastMode mode);
	RaycastMode get_raycast_mode() const { return raycast_mode; }

	virtual void perform_generation(std::vector<QueryItem> &query_item_list) = 0;
	bool _process_generation(uint64_t initial_time, int time_budget);
	void perform_tests(std::vector<QueryItem> &query_item_list);
	Dictionary cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask = 1);

protected:
	static void _bind_methods();
	void _notification(int p_what);
};

VARIANT_ENUM_CAST(QueryGenerator3D::RaycastMode);
#endif