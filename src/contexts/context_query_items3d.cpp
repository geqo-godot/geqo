#include "contexts/context_query_items3d.h"
#include "query_result.h"

Array ContextQueryItems3D::get_context(Ref<QueryInstance3D> query_instance) {
	Array result;
	for (Ref<QueryItem3D> item : query_instance->get_items()) {
		if (item->get_is_filtered())
			continue;
		result.append(item->get_projection_position());
	}
	return result;
}

void ContextQueryItems3D::_bind_methods() {
}