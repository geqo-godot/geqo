#include "contexts/query_context2d.h"
#include "environment_query2d.h"
#include <godot_cpp/core/class_db.hpp>

Array QueryContext2D::get_context() {
	Array result;
	GDVIRTUAL_CALL(_get_context, result);
	return result;
}

PackedStringArray QueryContext2D::_get_configuration_warnings() const {
	PackedStringArray warnings;

	EnvironmentQuery2D *query = cast_to<EnvironmentQuery2D>(get_parent());

	if (!query)
		warnings.append("Must be a child of EnvironmentQuery2D");

	return warnings;
}
void QueryContext2D::_bind_methods() {
	GDVIRTUAL_BIND(_get_context);
	ClassDB::bind_method(D_METHOD("get_context"), &QueryContext2D::get_context);
	ClassDB::bind_method(D_METHOD("get_context_positions"), &QueryContext2D::get_context_positions);
	ClassDB::bind_method(D_METHOD("get_query_items"), &QueryContext2D::get_query_items);
}