#pragma once
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
using namespace godot;

template <typename NodeT, typename PackedTArray, int VectorVariantT>
class QueryContextBase {
protected:
	// The father, aka the node that is inheriting this class
	Object *owner = nullptr;

public:
	~QueryContextBase() = default;

	void set_base_owner(Object *p_owner) {
		owner = p_owner;
	}

	PackedTArray _get_context_positions() {
		Array contexts;
		// Pass in owner, since has_method and call are Godot Object functions only
		if (!owner || !owner->has_method("get_context"))
			return PackedTArray();

		contexts = owner->call("get_context");
		PackedTArray results = PackedTArray();

		for (Variant context : contexts) {
			if (context.get_type() == VectorVariantT) {
				results.append(context);
				continue;
			}

			NodeT *current_context = Object::cast_to<NodeT>(context);
			if (current_context == nullptr) {
				print_error("Context must be a Node or Vector");
				continue;
			}
			results.append(current_context->get_global_position());
		}
		return results;
	}
};