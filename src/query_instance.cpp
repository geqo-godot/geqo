#include "query_instance.h"
#include "contexts/context_target_node2d.h"
#include "contexts/context_target_node3d.h"

void QueryInstance2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_querier_context", "node"), &QueryInstance2D::set_querier_context);
	ClassDB::bind_method(D_METHOD("get_querier_context"), &QueryInstance2D::get_querier_context);

	ClassDB::bind_method(D_METHOD("set_test_data_max", "test", "max"), &QueryInstance2D::set_test_data_max);
	ClassDB::bind_method(D_METHOD("get_test_data_max", "test"), &QueryInstance2D::get_test_data_max);

	ClassDB::bind_method(D_METHOD("set_test_data_min", "test", "min"), &QueryInstance2D::set_test_data_min);
	ClassDB::bind_method(D_METHOD("get_test_data_min", "test"), &QueryInstance2D::get_test_data_min);

	ClassDB::bind_method(D_METHOD("has_test_data", "test"), &QueryInstance2D::has_test_data);

	ClassDB::bind_method(D_METHOD("get_item_count"), &QueryInstance2D::get_item_count);
	ClassDB::bind_method(D_METHOD("get_item", "index"), &QueryInstance2D::get_item);
	ClassDB::bind_method(D_METHOD("add_item", "item"), &QueryInstance2D::add_item);
	ClassDB::bind_method(D_METHOD("has_items"), &QueryInstance2D::has_items);
	ClassDB::bind_method(D_METHOD("get_next_item"), &QueryInstance2D::get_next_item);

	ClassDB::bind_method(D_METHOD("has_time_left"), &QueryInstance2D::has_time_left);
	ClassDB::bind_method(D_METHOD("refresh_timer"), &QueryInstance2D::refresh_timer);
	ClassDB::bind_method(D_METHOD("reset_iterator"), &QueryInstance2D::reset_iterator);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "querier_context"), "set_querier_context", "get_querier_context");
}

void QueryInstance3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_querier_context", "node"), &QueryInstance3D::set_querier_context);
	ClassDB::bind_method(D_METHOD("get_querier_context"), &QueryInstance3D::get_querier_context);

	ClassDB::bind_method(D_METHOD("set_test_data_max", "test", "max"), &QueryInstance3D::set_test_data_max);
	ClassDB::bind_method(D_METHOD("get_test_data_max", "test"), &QueryInstance3D::get_test_data_max);

	ClassDB::bind_method(D_METHOD("set_test_data_min", "test", "min"), &QueryInstance3D::set_test_data_min);
	ClassDB::bind_method(D_METHOD("get_test_data_min", "test"), &QueryInstance3D::get_test_data_min);

	ClassDB::bind_method(D_METHOD("has_test_data", "test"), &QueryInstance3D::has_test_data);

	ClassDB::bind_method(D_METHOD("get_item_count"), &QueryInstance3D::get_item_count);
	ClassDB::bind_method(D_METHOD("get_item", "index"), &QueryInstance3D::get_item);
	ClassDB::bind_method(D_METHOD("add_item", "item"), &QueryInstance3D::add_item);
	ClassDB::bind_method(D_METHOD("has_items"), &QueryInstance3D::has_items);
	ClassDB::bind_method(D_METHOD("get_next_item"), &QueryInstance3D::get_next_item);

	ClassDB::bind_method(D_METHOD("has_time_left"), &QueryInstance3D::has_time_left);
	ClassDB::bind_method(D_METHOD("refresh_timer"), &QueryInstance3D::refresh_timer);
	ClassDB::bind_method(D_METHOD("reset_iterator"), &QueryInstance3D::reset_iterator);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "querier_context"), "set_querier_context", "get_querier_context");
}