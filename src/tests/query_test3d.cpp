#include "tests/query_test3d.h"
#include "query_test3d.h"
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>

void QueryTest3D::end_test() {
	emit_signal("test_finished");
}

void QueryTest3D::perform_test(Ref<QueryInstance3D> query_instance) {
	GDVIRTUAL_CALL(_perform_test, query_instance);
}
void QueryTest3D::_bind_methods() {
	GDVIRTUAL_BIND(_perform_test, "query_instance");

	ClassDB::bind_method(D_METHOD("end_test"), &QueryTest3D::end_test);

	// Core
	ClassDB::bind_method(D_METHOD("set_test_purpose", "purpose"), &QueryTest3D::set_test_purpose);
	ClassDB::bind_method(D_METHOD("get_test_purpose"), &QueryTest3D::get_test_purpose);

	ClassDB::bind_method(D_METHOD("set_test_type", "type"), &QueryTest3D::set_test_type);
	ClassDB::bind_method(D_METHOD("get_test_type"), &QueryTest3D::get_test_type);

	// Scoring
	ClassDB::bind_method(D_METHOD("set_multiple_context_score_operator", "score_op"), &QueryTest3D::set_multiple_context_score_operator);
	ClassDB::bind_method(D_METHOD("get_multiple_context_score_operator"), &QueryTest3D::get_multiple_context_score_operator);

	ClassDB::bind_method(D_METHOD("set_scoring_curve", "curve"), &QueryTest3D::set_scoring_curve);
	ClassDB::bind_method(D_METHOD("get_scoring_curve"), &QueryTest3D::get_scoring_curve);

	ClassDB::bind_method(D_METHOD("set_clamp_min_type", "clamp_type"), &QueryTest3D::set_clamp_min_type);
	ClassDB::bind_method(D_METHOD("get_clamp_min_type"), &QueryTest3D::get_clamp_min_type);

	ClassDB::bind_method(D_METHOD("set_score_clamp_min", "score"), &QueryTest3D::set_score_clamp_min);
	ClassDB::bind_method(D_METHOD("get_score_clamp_min"), &QueryTest3D::get_score_clamp_min);

	ClassDB::bind_method(D_METHOD("set_clamp_max_type", "clamp_type"), &QueryTest3D::set_clamp_max_type);
	ClassDB::bind_method(D_METHOD("get_clamp_max_type"), &QueryTest3D::get_clamp_max_type);

	ClassDB::bind_method(D_METHOD("set_score_clamp_max", "score"), &QueryTest3D::set_score_clamp_max);
	ClassDB::bind_method(D_METHOD("get_score_clamp_max"), &QueryTest3D::get_score_clamp_max);

	// Filtering
	ClassDB::bind_method(D_METHOD("set_filter_type", "type"), &QueryTest3D::set_filter_type);
	ClassDB::bind_method(D_METHOD("get_filter_type"), &QueryTest3D::get_filter_type);

	ClassDB::bind_method(D_METHOD("set_multiple_context_filter_operator", "filter_op"), &QueryTest3D::set_multiple_context_filter_operator);
	ClassDB::bind_method(D_METHOD("get_multiple_context_filter_operator"), &QueryTest3D::get_multiple_context_filter_operator);

	ClassDB::bind_method(D_METHOD("set_bool_match", "match"), &QueryTest3D::set_bool_match);
	ClassDB::bind_method(D_METHOD("get_bool_match"), &QueryTest3D::get_bool_match);

	ClassDB::bind_method(D_METHOD("set_filter_min", "val"), &QueryTest3D::set_filter_min);
	ClassDB::bind_method(D_METHOD("get_filter_min"), &QueryTest3D::get_filter_min);

	ClassDB::bind_method(D_METHOD("set_filter_max", "val"), &QueryTest3D::set_filter_max);
	ClassDB::bind_method(D_METHOD("get_filter_max"), &QueryTest3D::get_filter_max);

	ClassDB::bind_method(D_METHOD("set_scoring_factor", "val"), &QueryTest3D::set_scoring_factor);
	ClassDB::bind_method(D_METHOD("get_scoring_factor"), &QueryTest3D::get_scoring_factor);

	ClassDB::bind_method(D_METHOD("set_cost", "new_cost"), &QueryTest3D::set_cost);
	ClassDB::bind_method(D_METHOD("get_cost"), &QueryTest3D::get_cost);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_purpose", PROPERTY_HINT_ENUM, "Filter Score,Filter Only,Score Only"), "set_test_purpose", "get_test_purpose");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "test_type", PROPERTY_HINT_ENUM, "Numeric,Boolean", PROPERTY_USAGE_STORAGE), "set_test_type", "get_test_type");

	ADD_GROUP("Filter", "filter_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "filter_bool_match"), "set_bool_match", "get_bool_match");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "filter_type", PROPERTY_HINT_ENUM, "Min,Max,Range"), "set_filter_type", "get_filter_type");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "filter_multiple_context_filter_operator", PROPERTY_HINT_ENUM, "Any Pass,All Pass"), "set_multiple_context_filter_operator", "get_multiple_context_filter_operator");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "filter_min"), "set_filter_min", "get_filter_min");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "filter_max"), "set_filter_max", "get_filter_max");

	ADD_GROUP("Score", "score_");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "score_bool_match"), "set_bool_match", "get_bool_match");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "score_multiple_context_score_operator", PROPERTY_HINT_ENUM, "Average Score,Max Score,Min Score"), "set_multiple_context_score_operator", "get_multiple_context_score_operator");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "score_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_scoring_curve", "get_scoring_curve");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "score_clamp_min_type", PROPERTY_HINT_ENUM, "None,Specified,Same as Filter"), "set_clamp_min_type", "get_clamp_min_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score_clamp_min"), "set_score_clamp_min", "get_score_clamp_min");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "score_clamp_max_type", PROPERTY_HINT_ENUM, "None,Specified, Same as Filter"), "set_clamp_max_type", "get_clamp_max_type");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score_clamp_max"), "set_score_clamp_max", "get_score_clamp_max");

	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "score_factor"), "set_scoring_factor", "get_scoring_factor");

	ADD_GROUP("Misc", "");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "cost"), "set_cost", "get_cost");

	// Keep bool_match so user can use it with this variable name instead of score/filter
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "bool_match", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_STORAGE), "set_bool_match", "get_bool_match");
	ADD_SIGNAL(MethodInfo("test_finished"));
}

void QueryTest3D::_validate_property(PropertyInfo &property) const {
	const StringName name = property.name;
	// 1. Test Purpose-based visibility
	if (get_test_purpose() == GEQOEnums::PURPOSE_SCORE_ONLY) {
		static const TypedArray<StringName> filter_vars = {
			"filter_type", "filter_min", "filter_max", "filter_multiple_context_filter_operator"
		};
		if (filter_vars.has(name)) {
			property.usage &= ~PROPERTY_USAGE_EDITOR;
			return;
		}
	}
	if (get_test_purpose() == GEQOEnums::PURPOSE_FILTER_ONLY) {
		static const TypedArray<StringName> score_vars = {
			"score_multiple_context_score_operator",
			"score_curve",
			"score_factor",
			"score_clamp_min_type", "score_clamp_max_type",
			"score_clamp_min", "score_clamp_max"
		};
		if (score_vars.has(name)) {
			property.usage &= ~PROPERTY_USAGE_EDITOR;
			return;
		}
	}
	// 2. Test type (boolean or numeric)
	if (get_test_type() == GEQOEnums::TEST_TYPE_BOOLEAN) {
		static const TypedArray<StringName> numeric_vars = {
			"filter_type", "filter_min", "filter_max",
			"score_factor", "score_curve",
			"score_clamp_min_type", "score_clamp_max_type",
			"score_clamp_min", "score_clamp_max"
		};
		if (numeric_vars.has(name)) {
			property.usage &= ~PROPERTY_USAGE_EDITOR;
			return;
		}
	}

	if (get_test_type() == GEQOEnums::TEST_TYPE_NUMERIC) {
		if (name == StringName("filter_bool_match") || name == StringName("score_bool_match")) {
			property.usage &= ~PROPERTY_USAGE_EDITOR;
			return;
		}
	}
	// 3. Filter-specific rules
	if (name == StringName("filter_min") && get_filter_type() == GEQOEnums::FILTER_TYPE_MAX) {
		property.usage &= ~PROPERTY_USAGE_EDITOR;
		return;
	}
	if (name == StringName("filter_max") && get_filter_type() == GEQOEnums::FILTER_TYPE_MIN) {
		property.usage &= ~PROPERTY_USAGE_EDITOR;
		return;
	}
	// 4. Clamp-specific rules
	if (name == StringName("score_clamp_min") && get_clamp_min_type() != GEQOEnums::CLAMP_TYPE_VAL) {
		property.usage &= ~PROPERTY_USAGE_EDITOR;
		return;
	}
	if (name == StringName("score_clamp_max") && get_clamp_max_type() != GEQOEnums::CLAMP_TYPE_VAL) {
		property.usage &= ~PROPERTY_USAGE_EDITOR;
		return;
	}

	// 5. Bool match placement
	if (name == StringName("score_bool_match")) {
		if (get_test_purpose() != GEQOEnums::PURPOSE_SCORE_ONLY) {
			property.usage &= ~PROPERTY_USAGE_EDITOR;
			return;
		}
	}

	if (name == StringName("filter_bool_match")) {
		if (get_test_purpose() == GEQOEnums::PURPOSE_SCORE_ONLY) {
			property.usage &= ~PROPERTY_USAGE_EDITOR;
			return;
		}
	}
}

void QueryTest3D::_notification(int p_what) {
	if (p_what == NOTIFICATION_ENTER_TREE) {
		if (!get_scoring_curve().is_valid()) {
			Ref<Curve> new_curve;
			new_curve.instantiate();
			new_curve->add_point(Vector2(0, 0));
			new_curve->add_point(Vector2(1, 1));
			// Makes it linear
			new_curve->set_point_right_mode(0, Curve::TANGENT_LINEAR);
			new_curve->set_point_left_mode(1, Curve::TANGENT_LINEAR);
			set_scoring_curve(new_curve);
		}
		get_scoring_curve()->bake();
	}
}
