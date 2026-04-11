#include "tests/query_test3d.h"
#include "query_test3d.h"
#include <godot_cpp/core/class_db.hpp>

void QueryTest3D::perform_test(Ref<QueryItem3D> projection) {
	GDVIRTUAL_CALL(_perform_test, projection);
}

void QueryTest3D::_bind_methods() {
	GDVIRTUAL_BIND(_perform_test, "projection");
	ClassDB::bind_method(D_METHOD("set_context_score_operator", "score_op"), &QueryTest3D::set_context_score_operator);
	ClassDB::bind_method(D_METHOD("get_context_score_operator"), &QueryTest3D::get_context_score_operator);

	ClassDB::bind_method(D_METHOD("set_context_filter_operator", "score_op"), &QueryTest3D::set_context_filter_operator);
	ClassDB::bind_method(D_METHOD("get_context_filter_operator"), &QueryTest3D::get_context_filter_operator);

	ClassDB::bind_method(D_METHOD("set_test_type", "type"), &QueryTest3D::set_test_type);
	ClassDB::bind_method(D_METHOD("get_test_type"), &QueryTest3D::get_test_type);

	ClassDB::bind_method(D_METHOD("set_test_purpose", "purpose"), &QueryTest3D::set_test_purpose);
	ClassDB::bind_method(D_METHOD("get_test_purpose"), &QueryTest3D::get_test_purpose);

	ClassDB::bind_method(D_METHOD("set_cost", "new_cost"), &QueryTest3D::set_cost);
	ClassDB::bind_method(D_METHOD("get_cost"), &QueryTest3D::get_cost);

	ClassDB::bind_method(D_METHOD("set_filter_type", "type"), &QueryTest3D::set_filter_type);
	ClassDB::bind_method(D_METHOD("get_filter_type"), &QueryTest3D::get_filter_type);

	ClassDB::bind_method(D_METHOD("set_filter_min", "val"), &QueryTest3D::set_filter_min);
	ClassDB::bind_method(D_METHOD("get_filter_min"), &QueryTest3D::get_filter_min);

	ClassDB::bind_method(D_METHOD("set_filter_max", "val"), &QueryTest3D::set_filter_max);
	ClassDB::bind_method(D_METHOD("get_filter_max"), &QueryTest3D::get_filter_max);

	ClassDB::bind_method(D_METHOD("set_scoring_factor", "val"), &QueryTest3D::set_scoring_factor);
	ClassDB::bind_method(D_METHOD("get_scoring_factor"), &QueryTest3D::get_scoring_factor);

	ClassDB::bind_method(D_METHOD("set_bool_match", "match"), &QueryTest3D::set_bool_match);
	ClassDB::bind_method(D_METHOD("get_bool_match"), &QueryTest3D::get_bool_match);

	BIND_ENUM_CONSTANT(FILTER_SCORE);
	BIND_ENUM_CONSTANT(FILTER_ONLY);
	BIND_ENUM_CONSTANT(SCORE_ONLY);

	BIND_ENUM_CONSTANT(TEST_TYPE_NUMERIC);
	BIND_ENUM_CONSTANT(TEST_TYPE_BOOLEAN);

	BIND_ENUM_CONSTANT(AVERAGE_SCORE);
	BIND_ENUM_CONSTANT(MAX_SCORE);
	BIND_ENUM_CONSTANT(MIN_SCORE);

	BIND_ENUM_CONSTANT(ANY_PASS);
	BIND_ENUM_CONSTANT(ALL_PASS);

	BIND_ENUM_CONSTANT(FILTER_TYPE_MIN);
	BIND_ENUM_CONSTANT(FILTER_TYPE_MAX);
	BIND_ENUM_CONSTANT(FILTER_TYPE_RANGE);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cost"), "set_cost", "get_cost");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_purpose", PROPERTY_HINT_ENUM, "Filter Score,Filter Only,Score Only"), "set_test_purpose", "get_test_purpose");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_type", PROPERTY_HINT_ENUM, "Numeric,Boolean", PROPERTY_USAGE_STORAGE), "set_test_type", "get_test_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "multiple_context_filter_operator", PROPERTY_HINT_ENUM, "Any Pass,All Pass"), "set_context_filter_operator", "get_context_filter_operator");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "multiple_context_score_operator", PROPERTY_HINT_ENUM, "Average Score,Max Score,Min Score"), "set_context_score_operator", "get_context_score_operator");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "filter_type", PROPERTY_HINT_ENUM, "Min,Max,Range"), "set_filter_type", "get_filter_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "filter_min"), "set_filter_min", "get_filter_min");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "filter_max"), "set_filter_max", "get_filter_max");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "scoring_factor"), "set_scoring_factor", "get_scoring_factor");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bool_match"), "set_bool_match", "get_bool_match");
}

void QueryTest3D::_validate_property(PropertyInfo &property) const {
	// Hide filter operator when not filtering
	if (property.name == StringName("multiple_context_filter_operator"))
		if (get_test_purpose() == SCORE_ONLY)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide score operator when not scoring
	if (property.name == StringName("multiple_context_score_operator"))
		if (get_test_purpose() == FILTER_ONLY)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide numeric fields when this is a boolean test
	TypedArray<StringName> numeric_vars = { "filter_type", "filter_min", "filter_max", "scoring_factor" };
	if (numeric_vars.has(property.name))
		if (get_test_type() == TEST_TYPE_BOOLEAN)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide bool_match when this is a numeric test
	if (property.name == StringName("bool_match"))
		if (get_test_type() == TEST_TYPE_NUMERIC)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide min/max based on filter type
	if (property.name == StringName("filter_min"))
		if (get_filter_type() == FILTER_TYPE_MAX)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	if (property.name == StringName("filter_max"))
		if (get_filter_type() == FILTER_TYPE_MIN)
			property.usage &= ~PROPERTY_USAGE_EDITOR;
}
