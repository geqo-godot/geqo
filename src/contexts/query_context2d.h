#pragma once
#include "contexts/query_context.h"
#include "query_instance.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector2_array.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class EnvironmentQuery2D;
struct QueryContextTraits2D {
	using NodeT = Node2D;
	using PackedTArray = PackedVector2Array;
	static constexpr Variant::Type VectorVariantT = Variant::VECTOR2;
	using EnvironmentQueryT = EnvironmentQuery2D;
	using QueryInstanceT = QueryInstance2D;
};
class QueryContext2D : public Node2D, public QueryContextBase<QueryContextTraits2D> {
	GDCLASS(QueryContext2D, Node2D)

public:
	QueryContext2D() { set_base_owner(this); }
	~QueryContext2D() {}

	virtual Array get_context(Ref<QueryInstance2D>);
	GDVIRTUAL1RC_REQUIRED(Array, _get_context, Ref<QueryInstance2D>)

	PackedVector2Array get_context_positions(Ref<QueryInstance2D> query_instance) { return _get_context_positions(query_instance); }
	PackedStringArray _get_configuration_warnings() const override;

protected:
	static void _bind_methods();
};