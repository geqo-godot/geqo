#include <godot_cpp/classes/node3d.hpp>

#include "test_raycast_to3d.h"
#include "tests/test_raycast_to3d.h"
#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#define MAKE_OBJECT_TYPE_HINT(m_type) vformat("%s/%s:%s", Variant::NODE_PATH, PROPERTY_HINT_NODE_PATH_VALID_TYPES, m_type)

void TestRaycastTo3D::set_context(QueryContext3D *context_node) {
	context = context_node;
}

void TestRaycastTo3D::set_hitting_is_true(bool hitting) {
	hitting_is_true = hitting;
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

void TestRaycastTo3D::perform_test(Ref<QueryItem3D> projection) {
	if (!context) {
		print_error("TestRaycastTo3D: Test RaycastTo has no context");
		return;
	}
	Array context_nodes = context->get_context();

	int current_score = 0;

	for (Variant context : context_nodes) {
		Node3D *context_node = Object::cast_to<Node3D>(context);
		if (context_node == nullptr) {
			print_error("TestRaycastTo3D: RaycastTo context should be PhysicsBody3D");
			return;
		}
		Vector3 start_pos;
		Vector3 end_pos;

		if (cast_from_context) {
			start_pos = context_node->get_global_position();
			end_pos = projection->get_projection_position();
		} else {
			start_pos = projection->get_projection_position();
			end_pos = context_node->get_global_position();
		}
		Dictionary result;

		Array exclusion_nodes = Array();
		for (NodePath exclusion_path : exclusions) {
			CollisionObject3D *node = Object::cast_to<CollisionObject3D>(get_node_or_null(exclusion_path));
			if (node != nullptr)
				exclusion_nodes.append(node);
		}
		if (use_shape_cast) {
			TypedArray<Dictionary> shape_results = cast_shape_projection(start_pos, end_pos, exclusion_nodes, shape, collision_mask);
			if (!shape_results.is_empty()) {
				result = shape_results[0];
				for (Dictionary shape_result : shape_results) {
					Node3D *collider = Object::cast_to<Node3D>(shape_result.get("collider", nullptr));
					if (collider == context_node) {
						result = shape_result;
						break;
					}
				}
			}
		} else {
			result = cast_ray_projection(start_pos, end_pos, exclusion_nodes, collision_mask);
		}
		bool is_hit = false;

		Node3D *collider = Object::cast_to<Node3D>(result.get("collider", nullptr));
		if (collider == context_node)
			is_hit = true;

		// Invert if hitting_is_true = false
		if (!hitting_is_true) {
			is_hit = !is_hit;
		}

		if (is_hit) {
			current_score++;
			if (get_context_filter_operator() == ANY_PASS)
				break;
		}
	}

	bool filter = false;
	int final_score = 0;
	MultipleContextFilterOp filter_op = get_context_filter_operator();

	switch (filter_op) {
		case ANY_PASS: {
			if (current_score > 0) {
				final_score = 1;
			} else {
				filter = true;
			}
		} break;
		case ALL_PASS: {
			if (current_score == context_nodes.size()) {
				final_score = 1;
			} else {
				filter = true;
			}
		} break;
	}
	//
	//	switch (get_test_purpose()) {
	//		case FILTER_SCORE: {
	//			projection->set_is_filtered(filter);
	//			projection->add_score(final_score);
	//		} break;
	//		case FILTER_ONLY: {
	//			projection->set_is_filtered(filter);
	//		} break;
	//		case SCORE_ONLY: {
	//			projection->add_score(final_score);
	//		} break;
	//	}
}

// TODO: Move this to a utils class or something (duplicated code)
Dictionary TestRaycastTo3D::cast_ray_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, int col_mask) {
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

TypedArray<Dictionary> TestRaycastTo3D::cast_shape_projection(Vector3 start_pos, Vector3 end_pos, Array exclusions, Ref<Shape3D> shape, int col_mask) {
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
			uint64_t collider_id = rest_info.get("collider_id", 0);
			if (collider_id != 0) {
				Object *obj = ObjectDB::get_instance(ObjectID(collider_id));
				if (obj != nullptr) {
					// Add collider object
					rest_info["collider"] = obj;
				}
			}
			result.append(rest_info);
		}
	}
	return result;
}

void TestRaycastTo3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_context", "context_node"), &TestRaycastTo3D::set_context);
	ClassDB::bind_method(D_METHOD("get_context"), &TestRaycastTo3D::get_context);

	ClassDB::bind_method(D_METHOD("set_hitting_is_true", "hitting"), &TestRaycastTo3D::set_hitting_is_true);
	ClassDB::bind_method(D_METHOD("get_hitting_is_true"), &TestRaycastTo3D::get_hitting_is_true);

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
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "hitting_is_true"), "set_hitting_is_true", "get_hitting_is_true");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "cast_from_context"), "set_cast_from_context", "get_cast_from_context");
	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "exclusions", PROPERTY_HINT_ARRAY_TYPE, MAKE_OBJECT_TYPE_HINT("CollisionObject3D"), (PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_SCRIPT_VARIABLE)), "set_exclusions", "get_exclusions");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_3D_PHYSICS), "set_collision_mask", "get_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "raycast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_raycast_mode", "get_raycast_mode");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_shape_cast"), "set_use_shape_cast", "get_use_shape_cast");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_shape", "get_shape");
}

void TestRaycastTo3D::_validate_property(PropertyInfo &property) const {
	if (property.name == StringName("shape"))
		if (!use_shape_cast)
			property.usage &= ~PROPERTY_USAGE_EDITOR;
}
