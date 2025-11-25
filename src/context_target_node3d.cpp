#include "context_target_node3d.h"

void CContextTargetNode3D::set_target_node(Node3D *target) {
	target_node = target;
}

Array CContextTargetNode3D::get_context() {
	Array result = Array();
	if (target_node == nullptr) {
		print_error("No target node what the hecc");
		return Array();
	}
	result.append(target_node);
	return result;
}

void CContextTargetNode3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_node"), &CContextTargetNode3D::get_target_node);
	ClassDB::bind_method(D_METHOD("set_target_node", "target"), &CContextTargetNode3D::set_target_node);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "target_node", PROPERTY_HINT_NODE_TYPE, "Node3D"), "set_target_node", "get_target_node");
}