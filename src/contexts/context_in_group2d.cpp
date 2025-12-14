#include "contexts/context_in_group2d.h"
#include <godot_cpp/classes/scene_tree.hpp>

void ContextInGroup2D::set_group(String new_group) {
	group = new_group;
}

Array ContextInGroup2D::get_context() {
	if (!is_inside_tree())
		return Array();
	Array nodes = get_tree()->get_nodes_in_group(group);
	return nodes;
}

void ContextInGroup2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_group"), &ContextInGroup2D::get_group);
	ClassDB::bind_method(D_METHOD("set_group", "new_group"), &ContextInGroup2D::set_group);

	ADD_PROPERTY(PropertyInfo(Variant::STRING, "group"), "set_group", "get_group");
}