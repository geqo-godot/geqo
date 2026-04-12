#include "contexts/context_query_items2d.h"
#include "context_query_items2d.h"
#include "query_result.h"

Array ContextQueryItems2D::get_context(Ref<QueryInstance2D> query_instance) {
	Array result;
	for (Ref<QueryItem2D> item : query_instance->get_items()) {
		if (item->get_is_filtered())
			continue;
		result.append(item->get_projection_position());
	}
	return result;
}

void ContextQueryItems2D::_bind_methods() {
}