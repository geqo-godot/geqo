#include "tests/test_intersects_area2d.h"
#include "contexts/context_querier2d.h"
#include "test_intersects_area2d.h"
#include <godot_cpp/classes/collision_object2d.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/physics_direct_space_state2d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters2d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/world2d.hpp>
#include <godot_cpp/core/class_db.hpp>

void TestIntersectsArea2D::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		if (Engine::get_singleton()->is_editor_hint()) {
			update_configuration_warnings();
			return;
		}
		update_configuration_warnings();
	}
}

void TestIntersectsArea2D::set_context(QueryContext2D *new_context) { context = new_context; }
void TestIntersectsArea2D::set_shape(Ref<Shape2D> new_shape) {
	shape = new_shape;
	update_configuration_warnings();
}
void TestIntersectsArea2D::set_collision_mask(int mask) { collision_mask = mask; }
void TestIntersectsArea2D::set_shape_cast_mode(ShapeCastMode mode) { shape_cast_mode = mode; }

bool TestIntersectsArea2D::evaluate_context_intersection(Ref<QueryItem2D> item, const Array &context_nodes) {
	if (!shape.is_valid())
		return false;

	PhysicsDirectSpaceState2D *space_state = get_world_2d()->get_direct_space_state();

	Ref<PhysicsShapeQueryParameters2D> query;
	query.instantiate();
	query->set_shape(shape);
	query->set_collision_mask(collision_mask);

	Transform2D transform;
	transform.set_origin(item->get_projection_position());
	query->set_transform(transform);

	if (shape_cast_mode == AREA)
		query->set_collide_with_bodies(false);
	if (shape_cast_mode == AREA || shape_cast_mode == BODY_AREA)
		query->set_collide_with_areas(true);

	TypedArray<Dictionary> results = space_state->intersect_shape(query);

	switch (get_multiple_context_filter_operator()) {
		case GEQOEnums::OP_ANY_PASS: {
			for (int i = 0; i < results.size(); i++) {
				Dictionary result = results[i];
				if (context_nodes.has(result.get("collider", nullptr)))
					return true;
			}
			return false;
		}
		case GEQOEnums::OP_ALL_PASS: {
			int matched = 0;
			for (int i = 0; i < results.size(); i++) {
				Dictionary result = results[i];
				if (context_nodes.has(result.get("collider", nullptr)))
					matched++;
			}
			return matched == context_nodes.size();
		}
		default:
			for (int i = 0; i < results.size(); i++) {
				Dictionary result = results[i];
				if (context_nodes.has(result.get("collider", nullptr)))
					return true;
			}
			return false;
	}
}

void TestIntersectsArea2D::perform_test(Ref<QueryInstance2D> query_instance) {
	if (!context)
		context = Object::cast_to<QueryContext2D>(query_instance->get_querier_context());

	Array context_nodes = context->get_context_nodes(query_instance);
	if (context_nodes.is_empty()) {
		end_test();
		return;
	}

	while (query_instance->has_items()) {
		Ref<QueryItem2D> item = query_instance->get_next_item();
		if (item->get_is_filtered())
			continue;

		bool passes = evaluate_context_intersection(item, context_nodes);
		item->add_score_boolean(get_test_purpose(), passes, get_bool_match());

		if (!query_instance->has_time_left()) {
			stored_instance = query_instance;
			get_tree()->connect("process_frame", callable_mp(this, &TestIntersectsArea2D::_on_next_process_frame), CONNECT_ONE_SHOT);
			return;
		}
	}

	end_test();
}

void TestIntersectsArea2D::_on_next_process_frame() {
	stored_instance->refresh_timer();
	perform_test(stored_instance);
}

PackedStringArray TestIntersectsArea2D::_get_configuration_warnings() const {
	PackedStringArray warnings;
	if (!shape.is_valid())
		warnings.push_back("A shape must be provided for TestIntersectsArea2D to function. Please create a shape resource for it.");
	return warnings;
}

void TestIntersectsArea2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_context", "new_context"), &TestIntersectsArea2D::set_context);
	ClassDB::bind_method(D_METHOD("get_context"), &TestIntersectsArea2D::get_context);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &TestIntersectsArea2D::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &TestIntersectsArea2D::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_shape", "new_shape"), &TestIntersectsArea2D::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &TestIntersectsArea2D::get_shape);

	ClassDB::bind_method(D_METHOD("set_shape_cast_mode", "mode"), &TestIntersectsArea2D::set_shape_cast_mode);
	ClassDB::bind_method(D_METHOD("get_shape_cast_mode"), &TestIntersectsArea2D::get_shape_cast_mode);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "context", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_context", "get_context");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape_cast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_shape_cast_mode", "get_shape_cast_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape2D"), "set_shape", "get_shape");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "collision_mask", PROPERTY_HINT_LAYERS_2D_PHYSICS), "set_collision_mask", "get_collision_mask");
}