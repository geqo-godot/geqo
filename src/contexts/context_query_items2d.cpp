#include "contexts/context_query_items2d.h"
#include "query_result.h"

Array ContextQueryItems2D::get_context() {
	Array result;
	for (Ref<QueryItem2D> item : get_query_items()) {
		if (item->get_is_filtered())
			continue;
		result.append(item->get_projection_position());
	}
	return result;
}

void ContextQueryItems2D::_bind_methods() {
}