#include "generators/generator_circle_shape2d.h"
#include "contexts/context_target_node2d.h"
#include "generator_circle_shape2d.h"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/time.hpp"

void GeneratorCircleShape2D::set_circle_center(QueryContext2D *context) {
	circle_center = context;
}

void GeneratorCircleShape2D::set_circle_radius(double radius) {
	circle_radius = radius;
}

void GeneratorCircleShape2D::set_space_between(double space) {
	space_between = space;
}

void GeneratorCircleShape2D::set_arc_angle(double angle) {
	arc_angle = angle;
}

void GeneratorCircleShape2D::set_use_arc_direction(double use) {
	use_arc_direction = use;
}

void GeneratorCircleShape2D::set_use_casting(bool use) {
	use_casting = use;
}

void GeneratorCircleShape2D::set_cast_collision_mask(int mask) {
	cast_collision_mask = mask;
}

void GeneratorCircleShape2D::perform_generation(Ref<QueryInstance2D> query_instance) {
	if (!circle_center)
		circle_center = Object::cast_to<QueryContext2D>(query_instance->get_querier_context());

	Array contexts = circle_center->get_context(query_instance);
	// arc_angle needs to be radian
	double arc_radians = Math::deg_to_rad(arc_angle);

	// Calculate arc length and points for arc
	double arc_length = arc_radians * circle_radius;
	int points_amount = UtilityFunctions::roundi(arc_length / space_between);

	if (points_amount <= 0)
		points_amount = 1;

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		Vector2 starting_pos;
		Node2D *context_ref = nullptr;

		double rotation_offset = 0.0;
		// TODO: Test if this doesn't crash for edge cases
		if (contexts[context].get_type() == Variant::VECTOR2)
			starting_pos = contexts[context];
		else {
			context_ref = Object::cast_to<Node2D>(contexts[context]);
			if (context_ref) {
				starting_pos = context_ref->get_global_position();
				if (use_arc_direction)
					rotation_offset = context_ref->get_global_rotation();
			}
		}

		// Center the arc
		double start_angle = -(arc_radians / 2.0) + rotation_offset;
		float angle_step = arc_radians / points_amount;
		double current_angle = start_angle;

		for (int point = _current_state.prev_context; point < points_amount; point++) {
			double pos_x = cos(current_angle) * circle_radius + starting_pos.x;
			double pos_y = sin(current_angle) * circle_radius + starting_pos.y;

			current_angle += angle_step;

			Vector2 final_pos = Vector2(pos_x, pos_y);

			if (use_casting) {
				Dictionary casted_ray = cast_ray_projection(starting_pos, final_pos, contexts, cast_collision_mask);
				if (!casted_ray.is_empty())
					final_pos = casted_ray.get("position", Vector2());
			}

			query_instance->add_item(QueryItem2D::create(final_pos));

			// Check the time for stopping
			uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();

			if (!query_instance->has_time_left()) {
				_current_state.prev_point = point + 1;
				_current_state.prev_context = context;
				// Stop and wait until next frame
				get_tree()->connect("process_frame", callable_mp(this, &GeneratorCircleShape2D::perform_generation), CONNECT_ONE_SHOT);
				return;
			}
		}
		// Start over on new context
		_current_state.prev_point = 0;
	}
	// Finished the generation, continue on, and reset the state
	emit_signal("generator_finished");
	_current_state.reset();
}

void GeneratorCircleShape2D::_on_next_process_frame() {
	saved_instance->refresh_timer();
	perform_generation(saved_instance);
}
void GeneratorCircleShape2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_circle_center", "context"), &GeneratorCircleShape2D::set_circle_center);
	ClassDB::bind_method(D_METHOD("get_circle_center"), &GeneratorCircleShape2D::get_circle_center);

	ClassDB::bind_method(D_METHOD("set_circle_radius", "radius"), &GeneratorCircleShape2D::set_circle_radius);
	ClassDB::bind_method(D_METHOD("get_circle_radius"), &GeneratorCircleShape2D::get_circle_radius);

	ClassDB::bind_method(D_METHOD("set_space_between", "space"), &GeneratorCircleShape2D::set_space_between);
	ClassDB::bind_method(D_METHOD("get_space_between"), &GeneratorCircleShape2D::get_space_between);

	ClassDB::bind_method(D_METHOD("set_arc_angle", "angle"), &GeneratorCircleShape2D::set_arc_angle);
	ClassDB::bind_method(D_METHOD("get_arc_angle"), &GeneratorCircleShape2D::get_arc_angle);

	ClassDB::bind_method(D_METHOD("set_use_arc_direction", "use"), &GeneratorCircleShape2D::set_use_arc_direction);
	ClassDB::bind_method(D_METHOD("get_use_arc_direction"), &GeneratorCircleShape2D::get_use_arc_direction);

	ClassDB::bind_method(D_METHOD("set_use_casting", "use"), &GeneratorCircleShape2D::set_use_casting);
	ClassDB::bind_method(D_METHOD("get_use_casting"), &GeneratorCircleShape2D::get_use_casting);

	ClassDB::bind_method(D_METHOD("set_cast_collision_mask", "mask"), &GeneratorCircleShape2D::set_cast_collision_mask);
	ClassDB::bind_method(D_METHOD("get_cast_collision_mask"), &GeneratorCircleShape2D::get_cast_collision_mask);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "circle_center", PROPERTY_HINT_NODE_TYPE, "QueryContext2D"), "set_circle_center", "get_circle_center");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "circle_radius"), "set_circle_radius", "get_circle_radius");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "space_between"), "set_space_between", "get_space_between");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "arc_angle", PROPERTY_HINT_RANGE, "0.0,360.0,0.5"), "set_arc_angle", "get_arc_angle");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_arc_direction"), "set_use_arc_direction", "get_use_arc_direction");

	ADD_GROUP("Cast Data", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_casting"), "set_use_casting", "get_use_casting");
	ADD_PROPERTY(
			PropertyInfo(
					Variant::INT,
					"cast_collision_mask",
					PROPERTY_HINT_LAYERS_2D_PHYSICS),
			"set_cast_collision_mask",
			"get_cast_collision_mask");
}
