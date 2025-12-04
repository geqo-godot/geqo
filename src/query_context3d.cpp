#include "query_context3d.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

PackedVector3Array QueryContext3D::get_context_positions() {
	Array contexts;
	PackedVector3Array results = PackedVector3Array();
	if (has_method("get_context"))
		contexts = call("get_context");
	else
		return PackedVector3Array();

	for (Variant context : contexts) {
		if (context.get_type() == Variant::VECTOR3) {
			results.append(context);
			continue;
		}

		Node3D *current_context = Object::cast_to<Node3D>(context);
		if (current_context == nullptr) {
			print_error("Context must be a Node3D or Vector3");
			continue;
		}
		results.append(current_context->get_global_position());
	}
	return results;
}

void QueryContext3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_context"), &QueryContext3D::get_context);
	ClassDB::bind_method(D_METHOD("get_context_positions"), &QueryContext3D::get_context_positions);
}