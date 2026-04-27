#include "context_querier3d.h"
#include "environment_query3d.h"

Array ContextQuerier3D::get_context(Ref<QueryInstance3D> query_instance) {
	Array result = Array();
	result.append(query_instance->get_environment_query()->get_querier());
	return result;
}

void ContextQuerier3D::_bind_methods() {}