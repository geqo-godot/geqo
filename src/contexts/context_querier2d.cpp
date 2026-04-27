#include "context_querier2d.h"
#include "environment_query2d.h"

Array ContextQuerier2D::get_context(Ref<QueryInstance2D> query_instance) {
	Array result = Array();
	result.append(query_instance->get_environment_query()->get_querier());
	return result;
}
void ContextQuerier2D::_bind_methods() {}