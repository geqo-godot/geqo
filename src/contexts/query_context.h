#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>
using namespace godot;

template <typename Traits>
class QueryContextBase {
protected:
	using NodeT = typename Traits::NodeT;
	using PackedTArray = typename Traits::PackedTArray;
	using EnvironmentQueryT = typename Traits::EnvironmentQueryT;
	using QueryInstanceT = typename Traits::QueryInstanceT;
	// The parent, aka the node that is inheriting this class
	Object *owner = nullptr;

public:
	~QueryContextBase() = default;

	void set_base_owner(Object *p_owner) {
		owner = p_owner;
	}

	PackedTArray _get_context_positions(Ref<QueryInstanceT> query_instance) {
		Array contexts;
		// Pass in owner, since has_method and call are Godot Object functions only
		if (!owner)
			return PackedTArray();
		if (!owner->has_method("get_context"))
			return PackedTArray();

		contexts = owner->call("get_context", query_instance);
		PackedTArray results = PackedTArray();

		for (Variant context : contexts) {
			if (context.get_type() == Traits::VectorVariantT) {
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

	TypedArray<NodeT> _get_context_nodes(Ref<QueryInstanceT> query_instance) {
		Array contexts;
		// Pass in owner, since has_method and call are Godot Object functions only
		if (!owner)
			return TypedArray<NodeT>();
		if (!owner->has_method("get_context"))
			return TypedArray<NodeT>();

		contexts = owner->call("get_context", query_instance);
		TypedArray<NodeT> results = TypedArray<NodeT>();

		for (Variant context : contexts) {
			if (context.get_type() == Traits::VectorVariantT) {
				continue;
			}

			NodeT *current_context = Object::cast_to<NodeT>(context);
			if (current_context == nullptr) {
				print_error("Context must be a Node");
				continue;
			}
			results.append(current_context);
		}
		return results;
	}
};