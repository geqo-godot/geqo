#include "tests/query_test2d.h"
#include "query_test2d.h"
#include <godot_cpp/core/class_db.hpp>

void QueryTest2D::perform_test(Ref<QueryInstance2D> query_instance) {
	GDVIRTUAL_CALL(_perform_test, query_instance);
}

void QueryTest2D::_bind_methods() {
	GDVIRTUAL_BIND(_perform_test, "query_instance");
	ClassDB::bind_method(D_METHOD("set_test_type", "type"), &QueryTest2D::set_test_type);
	ClassDB::bind_method(D_METHOD("get_test_type"), &QueryTest2D::get_test_type);

	ClassDB::bind_method(D_METHOD("set_test_purpose", "purpose"), &QueryTest2D::set_test_purpose);
	ClassDB::bind_method(D_METHOD("get_test_purpose"), &QueryTest2D::get_test_purpose);

	ClassDB::bind_method(D_METHOD("set_cost", "new_cost"), &QueryTest2D::set_cost);
	ClassDB::bind_method(D_METHOD("get_cost"), &QueryTest2D::get_cost);

	ClassDB::bind_method(D_METHOD("set_filter_type", "type"), &QueryTest2D::set_filter_type);
	ClassDB::bind_method(D_METHOD("get_filter_type"), &QueryTest2D::get_filter_type);

	ClassDB::bind_method(D_METHOD("set_filter_min", "val"), &QueryTest2D::set_filter_min);
	ClassDB::bind_method(D_METHOD("get_filter_min"), &QueryTest2D::get_filter_min);

	ClassDB::bind_method(D_METHOD("set_filter_max", "val"), &QueryTest2D::set_filter_max);
	ClassDB::bind_method(D_METHOD("get_filter_max"), &QueryTest2D::get_filter_max);

	ClassDB::bind_method(D_METHOD("set_scoring_factor", "val"), &QueryTest2D::set_scoring_factor);
	ClassDB::bind_method(D_METHOD("get_scoring_factor"), &QueryTest2D::get_scoring_factor);

	ClassDB::bind_method(D_METHOD("set_bool_match", "match"), &QueryTest2D::set_bool_match);
	ClassDB::bind_method(D_METHOD("get_bool_match"), &QueryTest2D::get_bool_match);

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cost"), "set_cost", "get_cost");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_purpose", PROPERTY_HINT_ENUM, "Filter Score,Filter Only,Score Only"), "set_test_purpose", "get_test_purpose");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_type", PROPERTY_HINT_ENUM, "Numeric,Boolean", PROPERTY_USAGE_STORAGE), "set_test_type", "get_test_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "multiple_context_filter_operator", PROPERTY_HINT_ENUM, "Any Pass,All Pass"), "set_context_filter_operator", "get_multiple_context_filter_operator");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "multiple_context_score_operator", PROPERTY_HINT_ENUM, "Average Score,Max Score,Min Score"), "set_context_score_operator", "get_multiple_context_score_operator");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "filter_type", PROPERTY_HINT_ENUM, "Min,Max,Range"), "set_filter_type", "get_filter_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "filter_min"), "set_filter_min", "get_filter_min");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "filter_max"), "set_filter_max", "get_filter_max");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "scoring_factor"), "set_scoring_factor", "get_scoring_factor");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bool_match"), "set_bool_match", "get_bool_match");
}

void QueryTest2D::_validate_property(PropertyInfo &property) const {
	// Hide numeric fields when this is a boolean test
	TypedArray<StringName> numeric_vars = { "filter_type", "filter_min", "filter_max" };
	if (numeric_vars.has(property.name))
		if (get_test_type() == GEQOEnums::TEST_TYPE_BOOLEAN || get_test_purpose() == GEQOEnums::PURPOSE_SCORE_ONLY)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide bool_match when this is a numeric test
	if (property.name == StringName("bool_match"))
		if (get_test_type() == GEQOEnums::TEST_TYPE_NUMERIC)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide min/max based on filter type
	if (property.name == StringName("filter_min"))
		if (get_filter_type() == GEQOEnums::FILTER_TYPE_MAX)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	if (property.name == StringName("filter_max"))
		if (get_filter_type() == GEQOEnums::FILTER_TYPE_MIN)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide filter operator when not filtering
	if (property.name == StringName("multiple_context_filter_operator"))
		if (get_test_purpose() == GEQOEnums::PURPOSE_SCORE_ONLY)
			property.usage &= ~PROPERTY_USAGE_EDITOR;

	// Hide score operator when not scoring
	if (property.name == StringName("multiple_context_score_operator"))
		if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_ONLY)
			property.usage &= ~PROPERTY_USAGE_EDITOR;
}
