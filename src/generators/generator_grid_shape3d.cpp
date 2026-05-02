#include "generators/generator_grid_shape3d.h"
#include "contexts/context_querier3d.h"
#include "generator_grid_shape3d.h"
#include "godot_cpp/classes/scene_tree.hpp"
#include "godot_cpp/classes/time.hpp"

GeneratorGridShape3D::GeneratorGridShape3D() {
	post_projection_vertical_offset = 0.5;
}

void GeneratorGridShape3D::set_grid_half_size(double size) {
	grid_half_size = size;
}

void GeneratorGridShape3D::set_space_between(double space) {
	space_between = space;
}

void GeneratorGridShape3D::set_generate_around(QueryContext3D *context) {
	generate_around = context;
}

void GeneratorGridShape3D::set_use_vertical_projection(bool use) {
	use_vertical_projection = use;
	notify_property_list_changed();
}

void GeneratorGridShape3D::set_project_down(double project) {
	project_down = project;
}

void GeneratorGridShape3D::set_project_up(double project) {
	project_up = project;
}

void GeneratorGridShape3D::set_post_projection_vertical_offset(double offset) {
	post_projection_vertical_offset = offset;
}

void GeneratorGridShape3D::set_projection_collision_mask(int mask) {
	projection_collision_mask = mask;
}

void GeneratorGridShape3D::set_use_shape_cast(bool use) {
	use_shape_cast = use;
	notify_property_list_changed();
}
void GeneratorGridShape3D::set_shape(Ref<Shape3D> new_shape) {
	shape = new_shape;
}
void GeneratorGridShape3D::perform_generation(Ref<QueryInstance3D> query_instance) {
	if (!generate_around)
		generate_around = Object::cast_to<QueryContext3D>(query_instance->get_querier_context());

	int grid_size = std::round(grid_half_size * 2 / space_between) + 1;
	//UtilityFunctions::print("The size of the grid: ", grid_size * grid_size);
	Array contexts = generate_around->get_context(query_instance);

	for (int context = _current_state.prev_context; context < contexts.size(); context++) {
		Vector3 starting_pos;
		if (contexts[context].get_type() == Variant::VECTOR3)
			starting_pos = contexts[context];
		else {
			Node3D *current_context = Object::cast_to<Node3D>(contexts[context]);
			if (current_context == nullptr) {
				print_error("Context is invalid, must be Node3D");
				continue;
			}
			starting_pos = current_context->get_global_position();
		}
		starting_pos.x -= grid_half_size;
		starting_pos.z -= grid_half_size;

		for (int z = _current_state.prev_z; z < grid_size; z++) {
			for (int x = _current_state.prev_x; x < grid_size; x++) {
				double pos_x = starting_pos.x + (x * space_between);
				double pos_z = starting_pos.z + (z * space_between);

				if (use_vertical_projection) {
					Vector3 ray_pos = Vector3(pos_x, starting_pos.y, pos_z);
					Dictionary ray_result;
					// TODO: Get the best result, in this case, the closest to the center of the shape. For now, just the first one.
					if (use_shape_cast) {
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
						Node3D *collider = Object::cast_to<Node3D>(ray_result.get("collider", nullptr));
						Vector3 casted_position;
						casted_position = ray_result.get("position", Vector3());
						query_instance->add_item(QueryItem3D::create(casted_position + Vector3(0, post_projection_vertical_offset, 0), collider));
					}
				} else {
					query_instance->add_item(QueryItem3D::create(Vector3(pos_x, starting_pos.y, pos_z)));
				}

				if (!query_instance->has_time_left()) {
					// UtilityFunctions::print("No time left, continue to next frame.");
					// Stop and wait until next frame
					_current_state.prev_context = context;
					_current_state.prev_x = x + 1;
					_current_state.prev_z = z;
					saved_instance = query_instance;
					get_tree()->connect("process_frame", callable_mp(this, &GeneratorGridShape3D::_on_next_process_frame), CONNECT_ONE_SHOT);
					return;
				}
			}
			// Start new row
			_current_state.prev_x = 0;
		}
		// Start over on new context
		_current_state.prev_z = 0;
	}
	// Finished the generation, continue on, and reset the state
	emit_signal("generator_finished");
	_current_state.reset();
}

void GeneratorGridShape3D::_on_next_process_frame() {
	saved_instance->refresh_timer();
	perform_generation(saved_instance);
}

void GeneratorGridShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_grid_half_size", "size"), &GeneratorGridShape3D::set_grid_half_size);
	ClassDB::bind_method(D_METHOD("get_grid_half_size"), &GeneratorGridShape3D::get_grid_half_size);

	ClassDB::bind_method(D_METHOD("set_space_between", "space"), &GeneratorGridShape3D::set_space_between);
	ClassDB::bind_method(D_METHOD("get_space_between"), &GeneratorGridShape3D::get_space_between);

	ClassDB::bind_method(D_METHOD("set_generate_around", "context"), &GeneratorGridShape3D::set_generate_around);
	ClassDB::bind_method(D_METHOD("get_generate_around"), &GeneratorGridShape3D::get_generate_around);

	ClassDB::bind_method(D_METHOD("set_use_vertical_projection", "use"), &GeneratorGridShape3D::set_use_vertical_projection);
	ClassDB::bind_method(D_METHOD("get_use_vertical_projection"), &GeneratorGridShape3D::get_use_vertical_projection);

	ClassDB::bind_method(D_METHOD("set_project_down", "project"), &GeneratorGridShape3D::set_project_down);
	ClassDB::bind_method(D_METHOD("get_project_down"), &GeneratorGridShape3D::get_project_down);

	ClassDB::bind_method(D_METHOD("set_project_up", "project"), &GeneratorGridShape3D::set_project_up);
	ClassDB::bind_method(D_METHOD("get_project_up"), &GeneratorGridShape3D::get_project_up);

	ClassDB::bind_method(D_METHOD("set_post_projection_vertical_offset", "offset"), &GeneratorGridShape3D::set_post_projection_vertical_offset);
	ClassDB::bind_method(D_METHOD("get_post_projection_vertical_offset"), &GeneratorGridShape3D::get_post_projection_vertical_offset);

	ClassDB::bind_method(D_METHOD("set_projection_collision_mask", "mask"), &GeneratorGridShape3D::set_projection_collision_mask);
	ClassDB::bind_method(D_METHOD("get_projection_collision_mask"), &GeneratorGridShape3D::get_projection_collision_mask);

	ClassDB::bind_method(D_METHOD("set_use_shape_cast", "use"), &GeneratorGridShape3D::set_use_shape_cast);
	ClassDB::bind_method(D_METHOD("get_use_shape_cast"), &GeneratorGridShape3D::get_use_shape_cast);

	ClassDB::bind_method(D_METHOD("set_shape", "new_shape"), &GeneratorGridShape3D::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &GeneratorGridShape3D::get_shape);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "generate_around", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_generate_around", "get_generate_around");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "grid_half_size"), "set_grid_half_size", "get_grid_half_size");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "space_between"), "set_space_between", "get_space_between");

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
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_shape_cast"), "set_use_shape_cast", "get_use_shape_cast");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_shape", "get_shape");
}

void GeneratorGridShape3D::_validate_property(PropertyInfo &property) const {
	if (property.name == StringName("shape"))
		if (!use_shape_cast)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	TypedArray<StringName> projection_vars = { "project_up", "project_down", "post_projection_vertical_offset", "projection_collision_mask", "use_shape_cast", "shape" };
	if (projection_vars.has(property.name))
		if (!use_vertical_projection)
			property.usage &= ~PROPERTY_USAGE_EDITOR;
}
