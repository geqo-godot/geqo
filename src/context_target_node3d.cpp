#include "context_target_node3d.h"

void ContextTargetNode3D::set_target_node(Node3D *target) {
	target_node = target;
}

Array ContextTargetNode3D::get_context() {
	Array result = Array();
	if (target_node == nullptr) {
		print_error("No target node what the hecc");
		return Array();
	}
	result.append(target_node);
	return result;
}

void ContextTargetNode3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_node"), &ContextTargetNode3D::get_target_node);
	ClassDB::bind_method(D_METHOD("set_target_node", "target"), &ContextTargetNode3D::set_target_node);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_node", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_target_node", "get_target_node");
}