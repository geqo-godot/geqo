#include "query_item.h"
using namespace godot;

void QueryItem2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_score"), &QueryItem2D::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &QueryItem2D::get_score);
	ClassDB::bind_method(D_METHOD("set_is_filtered"), &QueryItem2D::set_is_filtered);
	ClassDB::bind_method(D_METHOD("get_is_filtered"), &QueryItem2D::get_is_filtered);
	ClassDB::bind_method(D_METHOD("set_projection_position"), &QueryItem2D::set_projection_position);
	ClassDB::bind_method(D_METHOD("get_projection_position"), &QueryItem2D::get_projection_position);
	ClassDB::bind_method(D_METHOD("set_collided_with"), &QueryItem2D::set_collided_with);
	ClassDB::bind_method(D_METHOD("get_collided_with"), &QueryItem2D::get_collided_with);
	ClassDB::bind_method(D_METHOD("add_score_numeric", "test_purpose", "filter_type", "amount", "min_threshold", "max_threshold"), &QueryItem2D::add_score_numeric);
	ClassDB::bind_method(D_METHOD("add_score_boolean", "test_purpose", "value", "expected_boolean"), &QueryItem2D::add_score_numeric);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "set_score", "get_score");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collided_with", PROPERTY_HINT_NODE_TYPE, "Node2D"), "set_collided_with", "get_collided_with");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_filtered"), "set_is_filtered", "get_is_filtered");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR2, "projection_position"), "set_projection_position", "get_projection_position");

	ClassDB::bind_static_method("QueryItem2D", D_METHOD("create", "position", "collider"), &QueryItem2D::create);
}

void QueryItem3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_score"), &QueryItem3D::set_score);
	ClassDB::bind_method(D_METHOD("get_score"), &QueryItem3D::get_score);
	ClassDB::bind_method(D_METHOD("set_is_filtered"), &QueryItem3D::set_is_filtered);
	ClassDB::bind_method(D_METHOD("get_is_filtered"), &QueryItem3D::get_is_filtered);
	ClassDB::bind_method(D_METHOD("set_projection_position"), &QueryItem3D::set_projection_position);
	ClassDB::bind_method(D_METHOD("get_projection_position"), &QueryItem3D::get_projection_position);
	ClassDB::bind_method(D_METHOD("set_collided_with"), &QueryItem3D::set_collided_with);
	ClassDB::bind_method(D_METHOD("get_collided_with"), &QueryItem3D::get_collided_with);
	ClassDB::bind_method(D_METHOD("add_score_numeric", "test_purpose", "filter_type", "amount", "min_threshold", "max_threshold"), &QueryItem3D::add_score_numeric);
	ClassDB::bind_method(D_METHOD("add_score_boolean", "test_purpose", "value", "expected_boolean"), &QueryItem3D::add_score_numeric);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score"), "set_score", "get_score");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "collided_with", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_collided_with", "get_collided_with");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "is_filtered"), "set_is_filtered", "get_is_filtered");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "projection_position"), "set_projection_position", "get_projection_position");

	ClassDB::bind_static_method("QueryItem3D", D_METHOD("create", "position", "collider"), &QueryItem3D::create);
}