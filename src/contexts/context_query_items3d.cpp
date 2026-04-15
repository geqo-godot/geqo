#include "contexts/context_query_items3d.h"
#include "context_query_items3d.h"
#include "query_result.h"

void ContextQueryItems3D::set_use_current_query_item(bool use) {
	use_current_query_item = use;
}

Array ContextQueryItems3D::get_context(Ref<QueryInstance3D> query_instance) {
	Array result;
	if (use_current_query_item)
		return { query_instance->get_current_query_item() };
	for (Ref<QueryItem3D> item : query_instance->get_items()) {
		if (item->get_is_filtered())
			continue;
		if (item->get_collided_with())
			result.append(item->get_collided_with());
		else
			result.append(item->get_projection_position());
	}
	return result;
}

void ContextQueryItems3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_use_current_query_item", "use"), &ContextQueryItems3D::set_use_current_query_item);
	ClassDB::bind_method(D_METHOD("get_use_current_query_item"), &ContextQueryItems3D::get_use_current_query_item);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_current_query_item"), "set_use_current_query_item", "get_use_current_query_item");
}