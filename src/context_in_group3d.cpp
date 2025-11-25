#include "context_in_group3d.h"
#include <godot_cpp/classes/scene_tree.hpp>

void CContextInGroup3D::set_group(String new_group) {
	group = new_group;
}

Array CContextInGroup3D::get_context() {
	if (!is_inside_tree())
		return Array();
	Array nodes = get_tree()->get_nodes_in_group(group);
	return nodes;
}

void CContextInGroup3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_group"), &CContextInGroup3D::get_group);
	ClassDB::bind_method(D_METHOD("set_group", "new_group"), &CContextInGroup3D::set_group);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "group"), "set_group", "get_group");
}