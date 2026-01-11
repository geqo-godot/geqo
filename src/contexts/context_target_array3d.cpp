#include "contexts/context_target_array3d.h"

void ContextTargetArray3D::set_target_nodes(TypedArray<Node3D> targets) {
	target_nodes = targets;
}

Array ContextTargetArray3D::get_context() {
	return target_nodes;
}

void ContextTargetArray3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_target_nodes"), &ContextTargetArray3D::get_target_node);
	ClassDB::bind_method(D_METHOD("set_target_nodes", "targets"), &ContextTargetArray3D::set_target_nodes);

	ADD_PROPERTY(PropertyInfo(Variant::ARRAY, "target_nodes", PROPERTY_HINT_ARRAY_TYPE, "Node3D"), "set_target_nodes", "get_target_nodes");
}