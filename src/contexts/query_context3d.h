#pragma once
#include "contexts/query_context.h"
#include "query_instance.h"
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/packed_vector3_array.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

using namespace godot;

class EnvironmentQuery3D;
struct QueryContextTraits3D {
	using NodeT = Node3D;
	using PackedTArray = PackedVector3Array;
	static constexpr Variant::Type VectorVariantT = Variant::VECTOR3;
	using EnvironmentQueryT = EnvironmentQuery3D;
	using QueryInstanceT = QueryInstance3D;
};
class QueryContext3D : public Node3D, public QueryContextBase<QueryContextTraits3D> {
	GDCLASS(QueryContext3D, Node3D)

public:
	QueryContext3D() { set_base_owner(this); }
	~QueryContext3D() {}

	virtual Array get_context(Ref<QueryInstance3D>);
	GDVIRTUAL1RC_REQUIRED(Array, _get_context, Ref<QueryInstance3D>)

	PackedVector3Array get_context_positions(Ref<QueryInstance3D> query_instance) { return _get_context_positions(query_instance); }
	PackedStringArray _get_configuration_warnings() const override;

protected:
	static void _bind_methods();
};