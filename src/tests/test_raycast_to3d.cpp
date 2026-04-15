#include "tests/test_raycast_to3d.h"
#include "contexts/context_target_node3d.h"
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/core/class_db.hpp>

#define MAKE_OBJECT_TYPE_HINT(m_type) vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, m_type)

void TestRaycastTo3D::set_context(QueryContext3D *context_node) {
	context = context_node;
}

void TestRaycastTo3D::set_cast_from_context(bool cast) {
	cast_from_context = cast;
}

void TestRaycastTo3D::set_exclusions(TypedArray<NodePath> exclusion) {
	exclusions = exclusion;
}

void TestRaycastTo3D::set_collision_mask(int mask) {
	collision_mask = mask;
}

void TestRaycastTo3D::set_raycast_mode(RaycastMode mode) {
	raycast_mode = mode;
}

void TestRaycastTo3D::set_use_shape_cast(bool use) {
	use_shape_cast = use;
	notify_property_list_changed();
}

void TestRaycastTo3D::set_shape(Ref<Shape3D> new_shape) {
	shape = new_shape;
}

Array TestRaycastTo3D::resolve_exclusions() {
	Array exclusion_nodes;
	for (int i = 0; i < exclusions.size(); i++) {
		CollisionObject3D *node = Object::cast_to<CollisionObject3D>(get_node_or_null(exclusions[i]));
		if (node != nullptr)
			exclusion_nodes.append(node);
	}
	return exclusion_nodes;
}

bool TestRaycastTo3D::cast_to_node(Ref<QueryItem3D> item, Node3D *context_node, const Array &exclusion_nodes) {
	Vector3 start_pos, end_pos;

	if (cast_from_context) {
		start_pos = context_node->get_global_position();
		end_pos = item->get_projection_position();
	} else {
		start_pos = item->get_projection_position();
		end_pos = context_node->get_global_position();
	}

	Node3D *collider = nullptr;

	if (use_shape_cast) {
		if (!shape.is_valid())
			return false;

		TypedArray<Dictionary> hits = cast_shape_projection(start_pos, end_pos, exclusion_nodes, shape, collision_mask);

		if (!hits.is_empty()) {
			Dictionary first_hit = hits[0];
			collider = Object::cast_to<Node3D>(first_hit.get("collider", nullptr));
		}
	} else {
		Dictionary result = cast_ray_projection(start_pos, end_pos, exclusion_nodes, collision_mask);
		collider = Object::cast_to<Node3D>(result.get("collider", nullptr));
	}

	return collider == context_node;
}

bool TestRaycastTo3D::evaluate_context_hit_any(Ref<QueryItem3D> item, const Array &context_nodes, const Array &exclusion_nodes) {
	for (int i = 0; i < context_nodes.size(); i++) {
		Node3D *node = Object::cast_to<Node3D>(context_nodes[i]);
		if (node && cast_to_node(item, node, exclusion_nodes))
			return true;
	}
	return false;
}

bool TestRaycastTo3D::evaluate_context_hit_all(Ref<QueryItem3D> item, const Array &context_nodes, const Array &exclusion_nodes) {
	for (int i = 0; i < context_nodes.size(); i++) {
		Node3D *node = Object::cast_to<Node3D>(context_nodes[i]);
		if (!node || !cast_to_node(item, node, exclusion_nodes))
			return false;
	}
	return true;
}

void TestRaycastTo3D::perform_test(Ref<QueryInstance3D> query_instance) {
	if (!context)
		context = Object::cast_to<QueryContext3D>(query_instance->get_querier_context());

	Array context_nodes = context->get_context_nodes(query_instance);
	if (context_nodes.is_empty()) {
		end_test();
		return;
	}

	Array exclusion_nodes = resolve_exclusions();

	while (query_instance->has_items()) {
		Ref<QueryItem3D> item = query_instance->get_next_item();
		if (item->get_is_filtered())
			continue;

		bool passes = false;

		switch (get_multiple_context_filter_operator()) {
			case GEQOEnums::OP_ANY_PASS:
				passes = evaluate_context_hit_any(item, context_nodes, exclusion_nodes);
				break;
			case GEQOEnums::OP_ALL_PASS:
				passes = evaluate_context_hit_all(item, context_nodes, exclusion_nodes);
				break;
			default:
				passes = evaluate_context_hit_any(item, context_nodes, exclusion_nodes);
				break;
		}

		item->add_score_boolean(get_test_purpose(), passes, get_bool_match());

		if (!query_instance->has_time_left()) {
			stored_instance = query_instance;
			get_tree()->connect("process_frame", callable_mp(this, &TestRaycastTo3D::_on_next_process_frame), CONNECT_ONE_SHOT);
			return;
		}
	}

	end_test();
}

void TestRaycastTo3D::_on_next_process_frame() {
	stored_instance->refresh_timer();
	perform_test(stored_instance);
}

Dictionary TestRaycastTo3D::cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask) {
	PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();

	Ref<PhysicsRayQueryParameters3D> query = PhysicsRayQueryParameters3D::create(start_pos, end_pos, col_mask);

	if (raycast_mode == AREA)
		query->set_collide_with_bodies(false);
	if (raycast_mode == AREA || raycast_mode == BODY_AREA)
		query->set_collide_with_areas(true);

	Array exclusion_rids;
	for (Variant exclusion : exclusions) {
		CollisionObject3D *node = Object::cast_to<CollisionObject3D>(exclusion);
		if (node)
			exclusion_rids.append(node->get_rid());
	}
	query->set_exclude(exclusion_rids);

	return space_state->intersect_ray(query);
}

TypedArray<Dictionary> TestRaycastTo3D::cast_shape_projection(
		Vector3 start_pos,
		Vector3 end_pos,
		Array exclusions,
		Ref<Shape3D> shape,
		int col_mask) {
	PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();

	Ref<PhysicsShapeQueryParameters3D> query;
	query.instantiate();

	query->set_shape(shape);
	query->set_collision_mask(col_mask);

	Transform3D transform;
	transform.set_origin(start_pos);
	query->set_transform(transform);

	Vector3 motion = end_pos - start_pos;
	query->set_motion(motion);

	if (raycast_mode == AREA)
		query->set_collide_with_bodies(false);
	if (raycast_mode == AREA || raycast_mode == BODY_AREA)
		query->set_collide_with_areas(true);

	Array exclusion_rids;
	for (Variant exclusion : exclusions) {
		CollisionObject3D *node = Object::cast_to<CollisionObject3D>(exclusion);
		if (node)
			exclusion_rids.append(node->get_rid());
	}
	query->set_exclude(exclusion_rids);

	PackedFloat32Array motion_array = space_state->cast_motion(query);
	TypedArray<Dictionary> result;

	// No collision
	if (motion_array.size() < 2 || motion_array[1] >= 1.0)
		return result;

	// Compute impact position
	Vector3 collision_pos = start_pos + motion * motion_array[1];

	// Snap shape to collision point
	transform.set_origin(collision_pos);
	query->set_transform(transform);
	query->set_motion(Vector3(0, 0, 0));

	Dictionary rest_info = space_state->get_rest_info(query);

	if (!rest_info.is_empty()) {
		uint64_t collider_id = rest_info.get("collider_id", 0);
		if (collider_id != 0) {
			Object *obj = ObjectDB::get_instance(ObjectID(collider_id));
			if (obj)
				rest_info["collider"] = obj;
		}

		rest_info["position"] = collision_pos;

		result.append(rest_info);
	}

	return result;
}

void TestRaycastTo3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_context", "context_node"), &TestRaycastTo3D::set_context);
	ClassDB::bind_method(D_METHOD("get_context"), &TestRaycastTo3D::get_context);

	ClassDB::bind_method(D_METHOD("set_cast_from_context", "cast"), &TestRaycastTo3D::set_cast_from_context);
	ClassDB::bind_method(D_METHOD("get_cast_from_context"), &TestRaycastTo3D::get_cast_from_context);

	ClassDB::bind_method(D_METHOD("set_exclusions", "exclusion"), &TestRaycastTo3D::set_exclusions);
	ClassDB::bind_method(D_METHOD("get_exclusions"), &TestRaycastTo3D::get_exclusions);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &TestRaycastTo3D::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &TestRaycastTo3D::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_raycast_mode", "mode"), &TestRaycastTo3D::set_raycast_mode);
	ClassDB::bind_method(D_METHOD("get_raycast_mode"), &TestRaycastTo3D::get_raycast_mode);

	ClassDB::bind_method(D_METHOD("set_use_shape_cast", "use"), &TestRaycastTo3D::set_use_shape_cast);
	ClassDB::bind_method(D_METHOD("get_use_shape_cast"), &TestRaycastTo3D::get_use_shape_cast);

	ClassDB::bind_method(D_METHOD("set_shape", "new_shape"), &TestRaycastTo3D::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &TestRaycastTo3D::get_shape);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "context", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_context", "get_context");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cast_from_context"), "set_cast_from_context", "get_cast_from_context");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "exclusions", PROPERTY_HINT_ARRAY_TYPE, MAKE_OBJECT_TYPE_HINT("CollisionObject3D"), (PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_SCRIPT_VARIABLE)), "set_exclusions", "get_exclusions");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "raycast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_raycast_mode", "get_raycast_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_shape_cast"), "set_use_shape_cast", "get_use_shape_cast");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_shape", "get_shape");
}

void TestRaycastTo3D::_validate_property(PropertyInfo &property) const {
	if (property.name == StringName("shape") && !use_shape_cast)
		property.usage &= ~PROPERTY_USAGE_EDITOR;
}