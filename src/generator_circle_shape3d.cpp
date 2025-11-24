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

	ADD_GROUP("QueryGenerator", "");
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
			"set_cast_collision_mask", "get_cast_collision_mask");

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
			"set_projection_collision_mask", "get_projection_collision_mask");
}
