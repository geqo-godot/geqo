#include "generators/query_generator3d.h"
#include "query_generator3d.h"
#include "tests/query_test3d.h"
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

void QueryGenerator3D::perform_generation(Ref<QueryInstance3D> query_instance) {
	GDVIRTUAL_CALL(_perform_generation, query_instance);
}

Dictionary QueryGenerator3D::cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask) {
	PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
	Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(start_pos, end_pos, col_mask);

	if (get_raycast_mode() == AREA)
		query->set_collide_with_bodies(false);
	if (get_raycast_mode() == AREA || get_raycast_mode() == BODY_AREA)
		query->set_collide_with_areas(true);

	Array exclusion_rids = Array();

	for (Variant exclusion : exclusions) {
		CollisionObject3D *node = Object::cast_to<CollisionObject3D>(exclusion.operator Object *());
		if (node == nullptr)
			continue;
		exclusion_rids.append(node->get_rid());
	}
	query->set_exclude(exclusion_rids);

	return space_state->intersect_ray(query);
}

TypedArray<Dictionary> QueryGenerator3D::cast_shape_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, Ref<Shape3D> shape, int col_mask) {
	PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
	Ref<PhysicsShapeQueryParameters3D> query;
	query.instantiate();

	query->set_shape(shape);
	query->set_collision_mask(col_mask);

	Transform3D transform = Transform3D();
	transform.set_origin(start_pos);
	query->set_transform(transform);

	query->set_motion(end_pos - start_pos);

	if (get_raycast_mode() == AREA)
		query->set_collide_with_bodies(false);
	if (get_raycast_mode() == AREA || get_raycast_mode() == BODY_AREA)
		query->set_collide_with_areas(true);

	Array exclusion_rids = Array();

	for (Variant exclusion : exclusions) {
		CollisionObject3D *node = Object::cast_to<CollisionObject3D>(exclusion.operator Object *());
		if (node == nullptr)
			continue;
		exclusion_rids.append(node->get_rid());
	}
	query->set_exclude(exclusion_rids);
	// Get safe motion proportion
	PackedFloat32Array motion_array = space_state->cast_motion(query);
	TypedArray<Dictionary> result;

	// Check for collision
	if (motion_array.size() >= 2 && motion_array[1] < 1.0) {
		// Calculate collision position
		Vector3 motion = end_pos - start_pos;
		Vector3 collision_pos = start_pos + (motion * motion_array[1]);

		// Update query tranfrom to collision pos
		transform.set_origin(collision_pos);
		query->set_transform(transform);
		query->set_motion(Vector3(0, 0, 0));

		Dictionary rest_info = space_state->get_rest_info(query);

		if (!rest_info.is_empty()) {
			rest_info["position"] = collision_pos; // Store center of shape
			result.append(rest_info);
		}
	}
	return result;
}

void QueryGenerator3D::_bind_methods() {
	GDVIRTUAL_BIND(_perform_generation, "query_instance");

	ClassDB::bind_method(D_METHOD("cast_ray_projection", "start_pos", "end_pos", "exclusions", "col_mask"), &QueryGenerator3D::cast_ray_projection);
	ClassDB::bind_method(D_METHOD("cast_shape_projection", "start_pos", "end_pos", "exclusions", "shape", "col_mask"), &QueryGenerator3D::cast_shape_projection);

	ClassDB::bind_method(D_METHOD("get_raycast_mode"), &QueryGenerator3D::get_raycast_mode);
	ClassDB::bind_method(D_METHOD("set_raycast_mode", "mode"), &QueryGenerator3D::set_raycast_mode);

	BIND_ENUM_CONSTANT(BODY);
	BIND_ENUM_CONSTANT(AREA);
	BIND_ENUM_CONSTANT(BODY_AREA);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "raycast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_raycast_mode", "get_raycast_mode");

	ADD_SIGNAL(MethodInfo("generator_finished"));
}
