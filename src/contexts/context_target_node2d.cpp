#include "contexts/context_target_node2d.h"
#include "context_target_node2d.h"

void ContextTargetNode2D::set_target_node(Node2D *target) {
	target_node = target;
}

Array ContextTargetNode2D::get_context(Ref<QueryInstance2D> query_instance) {
	Array result = Array();
	if (target_node == nullptr) {
		print_error("ContextTargetNode2D: No target node.");
		return Array();
	}
	result.append(target_node);
}

void ContextTargetNode2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_node"), &ContextTargetNode2D::get_target_node);
	ClassDB::bind_method(D_METHOD("set_target_node", "target"), &ContextTargetNode2D::set_target_node);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_node", PROPERTY_HINT_NODE_TYPE, "Node2D"), "set_target_node", "get_target_node");
}