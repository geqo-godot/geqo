#include "generators/generator_circle_shape3d.h"
#include "contexts/context_querier3d.h"
#include "generator_circle_shape3d.h"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/time.hpp"

GeneratorCircleShape3D::GeneratorCircleShape3D() {
	post_projection_vertical_offset = 0.5;
}

void GeneratorCircleShape3D::set_circle_center(QueryContext3D *context) {
	circle_center = context;
}

void GeneratorCircleShape3D::set_circle_radius(double radius) {
	circle_radius = radius;
}

void GeneratorCircleShape3D::set_space_between(double space) {
	space_between = space;
}

void GeneratorCircleShape3D::set_arc_angle(double angle) {
	arc_angle = angle;
}

void GeneratorCircleShape3D::set_use_arc_direction(double use) {
	use_arc_direction = use;
}

void GeneratorCircleShape3D::set_use_casting(bool use) {
	use_casting = use;
	notify_property_list_changed();
}

void GeneratorCircleShape3D::set_cast_collision_mask(int mask) {
	cast_collision_mask = mask;
}

void GeneratorCircleShape3D::set_use_cast_shape(bool use) {
	use_cast_shape = use;
	notify_property_list_changed();
}

void GeneratorCircleShape3D::set_cast_shape(Ref<Shape3D> new_shape) {
	cast_shape = new_shape;
}

void GeneratorCircleShape3D::set_use_vertical_projection(bool use) {
	use_vertical_projection = use;
	notify_property_list_changed();
}

void GeneratorCircleShape3D::set_project_down(double project) {
	project_down = project;
}

void GeneratorCircleShape3D::set_project_up(double project) {
	project_up = project;
}

void GeneratorCircleShape3D::set_post_projection_vertical_offset(double offset) {
	post_projection_vertical_offset = offset;
}

void GeneratorCircleShape3D::set_projection_collision_mask(int mask) {
	projection_collision_mask = mask;
}

void GeneratorCircleShape3D::set_use_shape(bool use) {
	use_shape = use;
	notify_property_list_changed();
}
void GeneratorCircleShape3D::set_shape(Ref<Shape3D> new_shape) {
	shape = new_shape;
}
void GeneratorCircleShape3D::perform_generation(Ref<QueryInstance3D> query_instance) {
	if (!circle_center)
		circle_center = Object::cast_to<QueryContext3D>(query_instance->get_querier_context());

	Array contexts = circle_center->get_context(query_instance);
	// arc_angle needs to be radian
	double arc_radians = Math::deg_to_rad(arc_angle);

	// Calculate arc length and points for arc
	double arc_length = arc_radians * circle_radius;
	int points_amount = UtilityFunctions::roundi(arc_length / space_between);

	if (points_amount <= 0)
		points_amount = 1;

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		Vector3 starting_pos;
		Node3D *context_ref = nullptr;

		double rotation_offset = -(Math_PI / 2);
		// TODO: Test if this doesn't crash for edge cases
		if (contexts[context].get_type() == Variant::VECTOR3)
			starting_pos = contexts[context];
		else {
			context_ref = Object::cast_to<Node3D>(contexts[context]);
			if (context_ref) {
				starting_pos = context_ref->get_global_position();
				if (use_arc_direction) {
					Vector3 forward = -(context_ref->get_global_transform().get_basis().get_column(2));
					rotation_offset = atan2(forward.z, forward.x);
				}
			}
		}

		// Center the arc
		double start_angle = -(arc_radians / 2.0) + rotation_offset;
		float angle_step = arc_radians / points_amount;
		double current_angle = start_angle;

		for (int point = _current_state.prev_context; point < points_amount; point++) {
			double pos_x = cos(current_angle) * circle_radius + starting_pos.x;
			double pos_z = sin(current_angle) * circle_radius + starting_pos.z;

			current_angle += angle_step;

			Vector3 final_pos = Vector3(pos_x, starting_pos.y, pos_z);

			if (use_casting) {
				Dictionary casted_ray;
				if (use_cast_shape) {
					TypedArray<Dictionary> dicts = cast_shape_projection(starting_pos, final_pos, contexts, cast_shape, cast_collision_mask);
					if (!dicts.is_empty())
						casted_ray = dicts[0];
				} else
					casted_ray = cast_ray_projection(starting_pos, final_pos, contexts, cast_collision_mask);
				if (!casted_ray.is_empty())
					final_pos = casted_ray.get("position", Vector3());
			}

			if (!use_vertical_projection) {
				query_instance->add_item(QueryItem3D::create(final_pos));
				continue;
			}

			Vector3 ray_pos = final_pos;

			Dictionary ray_result;

			if (use_shape) {
				TypedArray<Dictionary> dicts = cast_shape_projection(
						ray_pos + (Vector3(0, project_up, 0)),
						ray_pos + (Vector3(0, -project_down, 0)), contexts, shape, projection_collision_mask);
				if (!dicts.is_empty())
					ray_result = dicts[0];
			} else {
				ray_result = cast_ray_projection(
						ray_pos + (Vector3(0, project_up, 0)),
						ray_pos + (Vector3(0, -project_down, 0)), contexts, projection_collision_mask);
			}
			if (!ray_result.is_empty()) {
				Vector3 pos_result = ray_result.get("position", Vector3());
				pos_result += Vector3(0, post_projection_vertical_offset, 0);
				Node3D *collider = Object::cast_to<Node3D>(ray_result.get("collider", nullptr));
				query_instance->add_item(QueryItem3D::create(pos_result, collider));
			}
			// Check the time for stopping
			uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();

			if (!query_instance->has_time_left()) {
				_current_state.prev_point = point + 1;
				_current_state.prev_context = context;
				saved_instance = query_instance;

				// Stop and wait until next frame
				get_tree()->connect("process_frame", callable_mp(this, &GeneratorCircleShape3D::_on_next_process_frame), CONNECT_ONE_SHOT);
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

void GeneratorCircleShape3D::_on_next_process_frame() {
	saved_instance->refresh_timer();
	perform_generation(saved_instance);
}

void GeneratorCircleShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_circle_center", "context"), &GeneratorCircleShape3D::set_circle_center);
	ClassDB::bind_method(D_METHOD("get_circle_center"), &GeneratorCircleShape3D::get_circle_center);

	ClassDB::bind_method(D_METHOD("set_circle_radius", "radius"), &GeneratorCircleShape3D::set_circle_radius);
	ClassDB::bind_method(D_METHOD("get_circle_radius"), &GeneratorCircleShape3D::get_circle_radius);

	ClassDB::bind_method(D_METHOD("set_space_between", "space"), &GeneratorCircleShape3D::set_space_between);
	ClassDB::bind_method(D_METHOD("get_space_between"), &GeneratorCircleShape3D::get_space_between);

	ClassDB::bind_method(D_METHOD("set_arc_angle", "angle"), &GeneratorCircleShape3D::set_arc_angle);
	ClassDB::bind_method(D_METHOD("get_arc_angle"), &GeneratorCircleShape3D::get_arc_angle);

	ClassDB::bind_method(D_METHOD("set_use_arc_direction", "use"), &GeneratorCircleShape3D::set_use_arc_direction);
	ClassDB::bind_method(D_METHOD("get_use_arc_direction"), &GeneratorCircleShape3D::get_use_arc_direction);

	ClassDB::bind_method(D_METHOD("set_use_casting", "use"), &GeneratorCircleShape3D::set_use_casting);
	ClassDB::bind_method(D_METHOD("get_use_casting"), &GeneratorCircleShape3D::get_use_casting);

	ClassDB::bind_method(D_METHOD("set_cast_collision_mask", "mask"), &GeneratorCircleShape3D::set_cast_collision_mask);
	ClassDB::bind_method(D_METHOD("get_cast_collision_mask"), &GeneratorCircleShape3D::get_cast_collision_mask);

	ClassDB::bind_method(D_METHOD("set_use_cast_shape", "use"), &GeneratorCircleShape3D::set_use_cast_shape);
	ClassDB::bind_method(D_METHOD("get_use_cast_shape"), &GeneratorCircleShape3D::get_use_cast_shape);

	ClassDB::bind_method(D_METHOD("set_cast_shape", "new_cast_shape"), &GeneratorCircleShape3D::set_cast_shape);
	ClassDB::bind_method(D_METHOD("get_cast_shape"), &GeneratorCircleShape3D::get_cast_shape);

	ClassDB::bind_method(D_METHOD("set_use_vertical_projection", "use"), &GeneratorCircleShape3D::set_use_vertical_projection);
	ClassDB::bind_method(D_METHOD("get_use_vertical_projection"), &GeneratorCircleShape3D::get_use_vertical_projection);

	ClassDB::bind_method(D_METHOD("set_project_down", "project"), &GeneratorCircleShape3D::set_project_down);
	ClassDB::bind_method(D_METHOD("get_project_down"), &GeneratorCircleShape3D::get_project_down);

	ClassDB::bind_method(D_METHOD("set_project_up", "project"), &GeneratorCircleShape3D::set_project_up);
	ClassDB::bind_method(D_METHOD("get_project_up"), &GeneratorCircleShape3D::get_project_up);

	ClassDB::bind_method(D_METHOD("set_post_projection_vertical_offset", "offset"), &GeneratorCircleShape3D::set_post_projection_vertical_offset);
	ClassDB::bind_method(D_METHOD("get_post_projection_vertical_offset"), &GeneratorCircleShape3D::get_post_projection_vertical_offset);

	ClassDB::bind_method(D_METHOD("set_projection_collision_mask", "mask"), &GeneratorCircleShape3D::set_projection_collision_mask);
	ClassDB::bind_method(D_METHOD("get_projection_collision_mask"), &GeneratorCircleShape3D::get_projection_collision_mask);

	ClassDB::bind_method(D_METHOD("set_use_shape", "use"), &GeneratorCircleShape3D::set_use_shape);
	ClassDB::bind_method(D_METHOD("get_use_shape"), &GeneratorCircleShape3D::get_use_shape);

	ClassDB::bind_method(D_METHOD("set_shape", "new_shape"), &GeneratorCircleShape3D::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &GeneratorCircleShape3D::get_shape);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "circle_center", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_circle_center", "get_circle_center");
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
					PROPERTY_HINT_LAYERS_3D_PHYSICS),
			"set_cast_collision_mask",
			"get_cast_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_cast_shape"), "set_use_cast_shape", "get_use_cast_shape");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "cast_shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_cast_shape", "get_cast_shape");

	ADD_GROUP("Projection Data", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_vertical_projection"), "set_use_vertical_projection", "get_use_vertical_projection");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "project_up"), "set_project_up", "get_project_up");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "project_down"), "set_project_down", "get_project_down");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "post_projection_vertical_offset"), "set_post_projection_vertical_offset", "get_post_projection_vertical_offset");
	ADD_PROPERTY(
			PropertyInfo(
					Variant::INT,
					"projection_collision_mask",
					PROPERTY_HINT_LAYERS_3D_PHYSICS),
			"set_projection_collision_mask",
			"get_projection_collision_mask");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_shape"), "set_use_shape", "get_use_shape");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_shape", "get_shape");
}

void GeneratorCircleShape3D::_validate_property(PropertyInfo &property) const {
	if (property.name == StringName("cast_shape"))
		if (!use_cast_shape)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	if (property.name == StringName("shape"))
		if (!use_shape)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	TypedArray<StringName> cast_vars = { "cast_collision_mask", "use_cast_shape", "cast_shape" };
	if (cast_vars.has(property.name))
		if (!use_casting)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	TypedArray<StringName> projection_vars = { "project_up", "project_down", "post_projection_vertical_offset", "projection_collision_mask", "use_shape", "shape" };
	if (projection_vars.has(property.name))
		if (!use_vertical_projection)
			property.usage &= ~PROPERTY_USAGE_EDITOR;
}
