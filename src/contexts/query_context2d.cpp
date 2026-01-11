#include "contexts/query_context2d.h"
#include <godot_cpp/core/class_db.hpp>

Array QueryContext2D::get_context() {
	Array result;
	GDVIRTUAL_CALL(_get_context, result);
	return result;
}

void QueryContext2D::_bind_methods() {
	GDVIRTUAL_BIND(_get_context);
	ClassDB::bind_method(D_METHOD("get_context"), &QueryContext2D::get_context);
	ClassDB::bind_method(D_METHOD("get_context_positions"), &QueryContext2D::get_context_positions);
}