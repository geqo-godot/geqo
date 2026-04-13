#include "query_instance.h"

void QueryInstance2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_item_count"), &QueryInstance2D::get_item_count);
	ClassDB::bind_method(D_METHOD("get_item", "index"), &QueryInstance2D::get_item);
	ClassDB::bind_method(D_METHOD("add_item", "item"), &QueryInstance2D::add_item);
	ClassDB::bind_method(D_METHOD("has_items"), &QueryInstance2D::has_items);
	ClassDB::bind_method(D_METHOD("get_next_item"), &QueryInstance2D::get_next_item);
	ClassDB::bind_method(D_METHOD("has_time_left"), &QueryInstance2D::has_time_left);
	ClassDB::bind_method(D_METHOD("refresh_timer"), &QueryInstance2D::refresh_timer);
}

void QueryInstance3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_item_count"), &QueryInstance3D::get_item_count);
	ClassDB::bind_method(D_METHOD("get_item", "index"), &QueryInstance3D::get_item);
	ClassDB::bind_method(D_METHOD("add_item", "item"), &QueryInstance3D::add_item);
	ClassDB::bind_method(D_METHOD("has_items"), &QueryInstance3D::has_items);
	ClassDB::bind_method(D_METHOD("get_next_item"), &QueryInstance3D::get_next_item);
	ClassDB::bind_method(D_METHOD("has_time_left"), &QueryInstance3D::has_time_left);
	ClassDB::bind_method(D_METHOD("refresh_timer"), &QueryInstance3D::refresh_timer);
}