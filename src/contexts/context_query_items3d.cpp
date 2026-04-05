#include "contexts/context_query_items3d.h"
#include "query_result.h"

Array ContextQueryItems3D::get_context() {
	Array result;
	for (Ref<QueryItem3D> item : get_query_items()) {
		if (item->get_is_filtered())
			continue;
		Node3D *collided_with = item->get_collided_with();
		if (collided_with)
			result.append(collided_with);
		else
			result.append(item->get_projection_position());
	}
	return result;
}

void ContextQueryItems3D::_bind_methods() {
}