#include "tests/test_intersects_area3d.h"
#include "test_intersects_area3d.h"

#include <godot_cpp/classes/collision_object3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/classes/physics_shape_query_parameters3d.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/core/class_db.hpp>
void TestIntersectsArea3D::set_context(QueryContext3D *new_context) {
	context = new_context;
}

void TestIntersectsArea3D::set_shape(Ref<Shape3D> new_shape) {
	shape = new_shape;
}

void TestIntersectsArea3D::set_collision_mask(int mask) {
	collision_mask = mask;
}

void TestIntersectsArea3D::set_shape_cast_mode(ShapeCastMode mode) {
	shape_cast_mode = mode;
}

void TestIntersectsArea3D::perform_test(Ref<QueryInstance3D> query_instance) {
	//	if (!context) {
	//		print_error("TestIntersectsArea3D: missing a context");
	//		return;
	//	}
	//
	//	Array context_nodes = context->get_context();
	//
	//	double current_score = 0.0;
	//
	//	PhysicsDirectSpaceState3D *space_state = get_world_3d()->get_direct_space_state();
	//	Ref<PhysicsShapeQueryParameters3D> query;
	//	query.instantiate();
	//
	//	query->set_shape(shape);
	//	query->set_collision_mask(collision_mask);
	//
	//	Transform3D transform = Transform3D();
	//	transform.set_origin(projection->get_projection_position());
	//	query->set_transform(transform);
	//
	//	if (shape_cast_mode == AREA)
	//		query->set_collide_with_bodies(false);
	//	if (shape_cast_mode == AREA || shape_cast_mode == BODY_AREA)
	//		query->set_collide_with_areas(true);
	//
	//	TypedArray<Dictionary> results = space_state->intersect_shape(query);
	//
	//	for (Dictionary result : results) {
	//		if (context_nodes.has(result.get("collider", nullptr))) {
	//			current_score += 1;
	//			if (get_context_filter_operator() == OP_ANY_PASS)
	//				break;
	//		}
	//	}
	//
	//	bool filter = false;
	//	int final_score = 0;
	//	MultipleContextFilterOp filter_op = get_context_filter_operator();
	//
	//	switch (filter_op) {
	//		case OP_ANY_PASS: {
	//			if (current_score > 0) {
	//				final_score = 1;
	//			} else {
	//				filter = true;
	//			}
	//		} break;
	//		case OP_ALL_PASS: {
	//			if (current_score == context_nodes.size()) {
	//				final_score = 1;
	//			} else {
	//				filter = true;
	//			}
	//		} break;
	//	}
	//
	//switch (get_test_purpose()) {
	//	case PURPOSE_FILTER_SCORE: {
	//		projection->set_is_filtered(filter);
	//		projection->add_score(final_score);
	//	} break;
	//	case PURPOSE_FILTER_ONLY: {
	//		projection->set_is_filtered(filter);
	//	} break;
	//	case PURPOSE_SCORE_ONLY: {
	//		projection->add_score(final_score);
	//	} break;
	//}
}

void TestIntersectsArea3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_context", "new_context"), &TestIntersectsArea3D::set_context);
	ClassDB::bind_method(D_METHOD("get_context"), &TestIntersectsArea3D::get_context);

	ClassDB::bind_method(D_METHOD("set_collision_mask", "mask"), &TestIntersectsArea3D::set_collision_mask);
	ClassDB::bind_method(D_METHOD("get_collision_mask"), &TestIntersectsArea3D::get_collision_mask);

	ClassDB::bind_method(D_METHOD("set_shape", "new_shape"), &TestIntersectsArea3D::set_shape);
	ClassDB::bind_method(D_METHOD("get_shape"), &TestIntersectsArea3D::get_shape);

	ClassDB::bind_method(D_METHOD("set_shape_cast_mode", "mode"), &TestIntersectsArea3D::set_shape_cast_mode);
	ClassDB::bind_method(D_METHOD("get_shape_cast_mode"), &TestIntersectsArea3D::get_shape_cast_mode);

	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "context", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_context", "get_context");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "shape_cast_mode", PROPERTY_HINT_ENUM, "Body, Area, Body Area"), "set_shape_cast_mode", "get_shape_cast_mode");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "shape", PROPERTY_HINT_RESOURCE_TYPE, "Shape3D"), "set_shape", "get_shape");
	ADD_PROPERTY(
			PropertyInfo(
					Variant::INT,
					"collision_mask",
					PROPERTY_HINT_LAYERS_3D_PHYSICS),
			"set_collision_mask",
			"get_collision_mask");
}