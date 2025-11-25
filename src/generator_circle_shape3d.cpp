#include "generator_circle_shape3d.h"

void CGeneratorCircleShape3D::set_circle_center(CQueryContext3D *context) {
	circle_center = context;
}

void CGeneratorCircleShape3D::set_circle_radius(double radius) {
	circle_radius = radius;
}

void CGeneratorCircleShape3D::set_space_between(double space) {
	space_between = space;
}

void CGeneratorCircleShape3D::set_arc_angle(double angle) {
	arc_angle = angle;
}

void CGeneratorCircleShape3D::set_use_casting(bool use) {
	use_casting = use;
}

void CGeneratorCircleShape3D::set_cast_collision_mask(int mask) {
	cast_collision_mask = mask;
}

void CGeneratorCircleShape3D::set_use_vertical_projection(bool use) {
	use_vertical_projection = use;
}

void CGeneratorCircleShape3D::set_project_down(double project) {
	project_down = project;
}

void CGeneratorCircleShape3D::set_project_up(double project) {
	project_up = project;
}

void CGeneratorCircleShape3D::set_post_projection_vertical_offset(double offset) {
	post_projection_vertical_offset = offset;
}

void CGeneratorCircleShape3D::set_projection_collision_mask(int mask) {
	projection_collision_mask = mask;
}

void CGeneratorCircleShape3D::perform_generation(std::vector<CQueryItem> &query_item_list) {
	if (circle_center == nullptr) {
		print_error("CircleShape circle_center context not found.");
		return;
	}
	Array contexts = circle_center->get_context();

	int points_amount = UtilityFunctions::roundi(circle_radius / space_between);

	for (Variant &context : contexts) {
		Vector3 starting_pos;
		Node3D *context_ref = nullptr;

		// TODO: Test if this doesn't crash for edge cases
		if (context.get_type() == Variant::VECTOR3)
			starting_pos = context;
		else {
			context_ref = Object::cast_to<Node3D>(context);
			if (context_ref)
				starting_pos = context_ref->get_global_position();
		}

		double previous_angle = 0.0;
		float angle_step = Math_TAU / points_amount;

		for (int point = 0; point < points_amount; point++) {
			double pos_x = cos(previous_angle) * circle_radius + starting_pos.x;
			double pos_z = sin(previous_angle) * circle_radius + starting_pos.z;

			previous_angle += angle_step;

			Vector3 final_pos = Vector3(pos_x, starting_pos.y, pos_z);

			if (use_casting) {
				Dictionary casted_ray = cast_ray_projection(starting_pos, final_pos, contexts, cast_collision_mask);
				if (!casted_ray.is_empty())
					final_pos = casted_ray.get("position", Vector3());
			}

			if (!use_vertical_projection) {
				query_item_list.push_back(CQueryItem(final_pos));
				continue;
			}

			Vector3 ray_pos = final_pos;

			Dictionary ray_result = cast_ray_projection(
					ray_pos + Vector3(0, project_up, 0),
					ray_pos + Vector3(0, -project_down, 0),
					contexts,
					projection_collision_mask);

			if (!ray_result.is_empty()) {
				Vector3 pos_result = ray_result.get("position", Vector3());
				pos_result += Vector3(0, post_projection_vertical_offset, 0);
				Node *collider = Object::cast_to<Node>(ray_result.get("collider", nullptr));
				query_item_list.push_back(CQueryItem(pos_result, collider));
			}
		}
	}
}

void CGeneratorCircleShape3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_circle_center", "context"), &CGeneratorCircleShape3D::set_circle_center);
	ClassDB::bind_method(D_METHOD("get_circle_center"), &CGeneratorCircleShape3D::get_circle_center);

	ClassDB::bind_method(D_METHOD("set_circle_radius", "radius"), &CGeneratorCircleShape3D::set_circle_radius);
	ClassDB::bind_method(D_METHOD("get_circle_radius"), &CGeneratorCircleShape3D::get_circle_radius);

	ClassDB::bind_method(D_METHOD("set_space_between", "space"), &CGeneratorCircleShape3D::set_space_between);
	ClassDB::bind_method(D_METHOD("get_space_between"), &CGeneratorCircleShape3D::get_space_between);

	ClassDB::bind_method(D_METHOD("set_arc_angle", "angle"), &CGeneratorCircleShape3D::set_arc_angle);
	ClassDB::bind_method(D_METHOD("get_arc_angle"), &CGeneratorCircleShape3D::get_arc_angle);

	ClassDB::bind_method(D_METHOD("set_use_casting", "use"), &CGeneratorCircleShape3D::set_use_casting);
	ClassDB::bind_method(D_METHOD("get_use_casting"), &CGeneratorCircleShape3D::get_use_casting);

	ClassDB::bind_method(D_METHOD("set_cast_collision_mask", "mask"), &CGeneratorCircleShape3D::set_cast_collision_mask);
	ClassDB::bind_method(D_METHOD("get_cast_collision_mask"), &CGeneratorCircleShape3D::get_cast_collision_mask);

	ClassDB::bind_method(D_METHOD("set_use_vertical_projection", "use"), &CGeneratorCircleShape3D::set_use_vertical_projection);
	ClassDB::bind_method(D_METHOD("get_use_vertical_projection"), &CGeneratorCircleShape3D::get_use_vertical_projection);

	ClassDB::bind_method(D_METHOD("set_project_down", "project"), &CGeneratorCircleShape3D::set_project_down);
	ClassDB::bind_method(D_METHOD("get_project_down"), &CGeneratorCircleShape3D::get_project_down);

	ClassDB::bind_method(D_METHOD("set_project_up", "project"), &CGeneratorCircleShape3D::set_project_up);
	ClassDB::bind_method(D_METHOD("get_project_up"), &CGeneratorCircleShape3D::get_project_up);

	ClassDB::bind_method(D_METHOD("set_post_projection_vertical_offset", "offset"), &CGeneratorCircleShape3D::set_post_projection_vertical_offset);
	ClassDB::bind_method(D_METHOD("get_post_projection_vertical_offset"), &CGeneratorCircleShape3D::get_post_projection_vertical_offset);

	ClassDB::bind_method(D_METHOD("set_projection_collision_mask", "mask"), &CGeneratorCircleShape3D::set_projection_collision_mask);
	ClassDB::bind_method(D_METHOD("get_projection_collision_mask"), &CGeneratorCircleShape3D::get_projection_collision_mask);

	ADD_GROUP("Generator", "");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "circle_center", PROPERTY_HINT_NODE_TYPE, "CQueryContext3D"), "set_circle_center", "get_circle_center");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "circle_radius"), "set_circle_radius", "get_circle_radius");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "space_between"), "set_space_between", "get_space_between");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "arc_angle", PROPERTY_HINT_RANGE, "0.0,360.0,0.5"), "set_arc_angle", "get_arc_angle");

	ADD_GROUP("Cast Data", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_casting"), "set_use_casting", "get_use_casting");
	ADD_PROPERTY(
			PropertyInfo(
					Variant::INT,
					"cast_collision_mask",
					PROPERTY_HINT_LAYERS_3D_PHYSICS),
			"set_cast_collision_mask",
			"get_cast_collision_mask");

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
