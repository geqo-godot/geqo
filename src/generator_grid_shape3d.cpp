#include "generator_grid_shape3d.h"

void CGeneratorGridShape3D::set_grid_half_size(double size) {
	grid_half_size = size;
}

void CGeneratorGridShape3D::set_space_between(double space) {
	space_between = space;
}

void CGeneratorGridShape3D::set_generate_around(CQueryContext3D *context) {
	generate_around = context;
}

void CGeneratorGridShape3D::set_use_vertical_projection(bool use) {
	use_vertical_projection = use;
}

void CGeneratorGridShape3D::set_project_down(double project) {
	project_down = project;
}

void CGeneratorGridShape3D::set_project_up(double project) {
	project_up = project;
}

void CGeneratorGridShape3D::set_post_projection_vertical_offset(double offset) {
	post_projection_vertical_offset = offset;
}

void CGeneratorGridShape3D::set_projection_collision_mask(int mask) {
	projection_collision_mask = mask;
}

void CGeneratorGridShape3D::perform_generation(std::vector<CQueryItem> &query_item_list) {
	if (generate_around == nullptr) {
		print_error("Generator couldn't find Context");
		return;
	}
	int grid_size = std::round(grid_half_size * 2 / space_between) + 1;
	Array contexts = generate_around->get_context();

	for (Variant context : contexts) {
		Vector3 starting_pos;
		if (context.get_type() == Variant::VECTOR3)
			starting_pos = context;
		else {
			Node3D *current_context = Object::cast_to<Node3D>(context);
			if (current_context == nullptr) {
				print_error("Context is invalid, must be Node3D");
				continue;
			}
			starting_pos = current_context->get_global_position();
		}
		starting_pos.x -= grid_half_size;
		starting_pos.z -= grid_half_size;

		for (int z = 0; z < grid_size; z++) {
			for (int x = 0; x < grid_size; x++) {
				double pos_x = starting_pos.x + (x * space_between);
				double pos_z = starting_pos.z + (z * space_between);

				if (!use_vertical_projection) {
					query_item_list.push_back(CQueryItem(Vector3(pos_x, starting_pos.y, pos_z)));
					continue;
				}

				Vector3 ray_pos = Vector3(pos_x, starting_pos.y, pos_z);
				Dictionary ray_result = cast_ray_projection(
						ray_pos + (Vector3(0, project_up, 0)),
						ray_pos + (Vector3(0, -project_down, 0)), contexts, projection_collision_mask);

				if (!ray_result.is_empty()) {
					Vector3 casted_position = (Vector3)ray_result.get("position", Vector3());
					Node3D *collider = Object::cast_to<Node3D>(ray_result.get("collider", nullptr));
					query_item_list.push_back(
							CQueryItem(casted_position + Vector3(0, post_projection_vertical_offset, 0), collider));
				}
			}
		}
	}
}

void CGeneratorGridShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_grid_half_size"), &CGeneratorGridShape3D::get_grid_half_size);
	ClassDB::bind_method(D_METHOD("set_grid_half_size", "size"), &CGeneratorGridShape3D::set_grid_half_size);

	ClassDB::bind_method(D_METHOD("get_space_between"), &CGeneratorGridShape3D::get_space_between);
	ClassDB::bind_method(D_METHOD("set_space_between", "space"), &CGeneratorGridShape3D::set_space_between);

	ClassDB::bind_method(D_METHOD("get_generate_around"), &CGeneratorGridShape3D::get_generate_around);
	ClassDB::bind_method(D_METHOD("set_generate_around", "context"), &CGeneratorGridShape3D::set_generate_around);

	ClassDB::bind_method(D_METHOD("get_use_vertical_projection"), &CGeneratorGridShape3D::get_use_vertical_projection);
	ClassDB::bind_method(D_METHOD("set_use_vertical_projection", "use"), &CGeneratorGridShape3D::set_use_vertical_projection);

	ClassDB::bind_method(D_METHOD("get_project_down"), &CGeneratorGridShape3D::get_project_down);
	ClassDB::bind_method(D_METHOD("set_project_down", "project"), &CGeneratorGridShape3D::set_project_down);

	ClassDB::bind_method(D_METHOD("get_project_up"), &CGeneratorGridShape3D::get_project_up);
	ClassDB::bind_method(D_METHOD("set_project_up", "project"), &CGeneratorGridShape3D::set_project_up);

	ClassDB::bind_method(D_METHOD("get_post_projection_vertical_offset"), &CGeneratorGridShape3D::get_post_projection_vertical_offset);
	ClassDB::bind_method(D_METHOD("set_post_projection_vertical_offset", "offset"), &CGeneratorGridShape3D::set_post_projection_vertical_offset);

	ClassDB::bind_method(D_METHOD("get_projection_collision_mask"), &CGeneratorGridShape3D::get_projection_collision_mask);
	ClassDB::bind_method(D_METHOD("set_projection_collision_mask", "mask"), &CGeneratorGridShape3D::set_projection_collision_mask);

	ADD_GROUP("Generator", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "generate_around", PROPERTY_HINT_NODE_TYPE, "CQueryContext3D"), "set_generate_around", "get_generate_around");
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
}