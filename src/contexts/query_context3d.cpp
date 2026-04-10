#include "contexts/query_context3d.h"
#include "environment_query3d.h"
#include <godot_cpp/core/class_db.hpp>

Array QueryContext3D::get_context() {
	Array result;
	GDVIRTUAL_CALL(_get_context, result);
	return result;
}

PackedStringArray QueryContext3D::_get_configuration_warnings() const {
	PackedStringArray warnings;

	EnvironmentQuery3D *query = cast_to<EnvironmentQuery3D>(get_parent());

	if (!query)
		warnings.append("Must be a child of EnvironmentQuery3D");

	return warnings;
}

void QueryContext3D::_bind_methods() {
	GDVIRTUAL_BIND(_get_context);
	ClassDB::bind_method(D_METHOD("get_context"), &QueryContext3D::get_context);
	ClassDB::bind_method(D_METHOD("get_context_positions"), &QueryContext3D::get_context_positions);
	ClassDB::bind_method(D_METHOD("get_query_items"), &QueryContext3D::get_query_items);
}