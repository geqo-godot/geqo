#include "tests/test_path_find_to3d.h"
#include "test_path_find_to3d.h"
#include <algorithm>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/navigation_server3d.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/classes/world3d.hpp>
#include <numeric>
using namespace godot;

void TestPathFindTo3D::set_use_debug(bool debug) {
	use_debug = debug;
}

void TestPathFindTo3D::set_scoring_curve(Ref<Curve> curve) {
	scoring_curve = curve;
}

void TestPathFindTo3D::set_path_to(QueryContext3D *context_node) {
	path_to = context_node;
}

void TestPathFindTo3D::set_found_path_threshold(double threshold) {
	found_path_threshold = threshold;
}

void TestPathFindTo3D::perform_test(QueryItem<Vector3> &projection) {
	if (path_to == nullptr) {
		print_error("TestPathFindTo3D has no context");
		return;
	}

	Array context_positions = path_to->get_context_positions();
	std::vector<double> scores = {};

	for (Variant context_pos : context_positions) {
		if (context_pos.get_type() != Variant::VECTOR3)
			continue;

		PackedVector3Array path = get_navigation_path(projection.projection_position, context_pos);
		if (use_debug) {
			draw_path(path, Color(1, 0, 0));
		}
		//UtilityFunctions::print("Projection Pos: ", projection.projection_position, " Context Pos: ", context_pos);
		//UtilityFunctions::print("Generated Path: ", path);

		RID default_map_rid = get_world_3d()->get_navigation_map();
		Vector3 nav_context =
				NavigationServer3D::get_singleton()->map_get_closest_point(
						default_map_rid,
						context_pos);
		if (path.is_empty()) {
			//UtilityFunctions::print("Path was empty.");
			scores.push_back(0.0);
		} else {
			//UtilityFunctions::print("size - 1 path point: ", path[path.size() - 1], " Context Pos: ", nav_context);
			//UtilityFunctions::print("Threshold: ", found_path_threshold);
			//UtilityFunctions::print("Distance to: ", path[path.size() - 1].distance_to(nav_context));
			if (path[path.size() - 1].distance_to(nav_context) <= found_path_threshold) {
				//UtilityFunctions::print("Final point on path did reach context.");
				scores.push_back(1.0);
			} else {
				//UtilityFunctions::print("Final point on path did not reach context.");
				scores.push_back(0.0);
			}
		}
	}

	double result = 0.0;

	// Choose score for the result
	switch (get_context_score_operator()) {
		case AVERAGE_SCORE: {
			double total_score = std::accumulate(scores.begin(), scores.end(), 0.0);
			result = total_score / scores.size();
			break;
		}
		case MAX_SCORE:
			result = *std::max(scores.begin(), scores.end());
			break;
		case MIN_SCORE:
			result = *std::min_element(scores.begin(), scores.end());
			break;
	}

	switch (get_test_purpose()) {
		case FILTER_SCORE: {
			if (result == 0.0)
				projection.is_filtered = true;
			else
				projection.add_score(result);
			break;
		}
		case FILTER_ONLY:
			if (result == 0.0)
				projection.is_filtered = true;
			break;
		case SCORE_ONLY:
			projection.add_score(result);
			break;
	}
}
PackedVector3Array TestPathFindTo3D::get_navigation_path(Vector3 p_start_position, Vector3 p_target_position) {
	if (!is_inside_tree())
		return PackedVector3Array();

	NavigationServer3D *nav = NavigationServer3D::get_singleton();

	RID default_map_rid = get_world_3d()->get_navigation_map();

	PackedVector3Array path = NavigationServer3D::get_singleton()->map_get_path(
			default_map_rid,
			p_start_position,
			p_target_position,
			true);

	return path;
}

void TestPathFindTo3D::draw_path(const PackedVector3Array &path, Color color) {
	if (path.size() < 2)
		return;

	Ref<ImmediateMesh> mesh;
	mesh.instantiate();

	mesh->surface_begin(Mesh::PRIMITIVE_LINES);
	for (int i = 0; i < path.size() - 1; i++) {
		mesh->surface_add_vertex(path[i]);
		mesh->surface_add_vertex(path[i + 1]);
	}
	mesh->surface_end();

	MeshInstance3D *mi = memnew(MeshInstance3D);
	mi->set_mesh(mesh);
	mi->set_cast_shadows_setting(GeometryInstance3D::SHADOW_CASTING_SETTING_OFF);

	Ref<StandardMaterial3D> mat;
	mat.instantiate();
	mat->set_shading_mode(StandardMaterial3D::SHADING_MODE_UNSHADED);
	mat->set_albedo(color);

	mi->set_material_override(mat);

	get_tree()->get_root()->add_child(mi);
	get_tree()->create_timer(5)->connect("timeout", Callable(mi, "queue_free"));
}

// TODO: Change to NOTIFICATION_READY
void TestPathFindTo3D::_ready() {
	if (Engine::get_singleton()->is_editor_hint()) {
		return;
	}
	if (scoring_curve.is_null()) {
		scoring_curve = Ref<Curve>();
		scoring_curve.instantiate();
		scoring_curve->add_point(Vector2(0, 0));
		scoring_curve->add_point(Vector2(1, 1));
	}
	scoring_curve->bake();
}

void TestPathFindTo3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_use_debug", "debug"), &TestPathFindTo3D::set_use_debug);
	ClassDB::bind_method(D_METHOD("get_use_debug"), &TestPathFindTo3D::get_use_debug);

	ClassDB::bind_method(D_METHOD("set_scoring_curve", "curve"), &TestPathFindTo3D::set_scoring_curve);
	ClassDB::bind_method(D_METHOD("get_scoring_curve"), &TestPathFindTo3D::get_scoring_curve);

	ClassDB::bind_method(D_METHOD("set_found_path_threshold", "threshold"), &TestPathFindTo3D::set_found_path_threshold);
	ClassDB::bind_method(D_METHOD("get_found_path_threshold"), &TestPathFindTo3D::get_found_path_threshold);

	ClassDB::bind_method(D_METHOD("set_path_to", "context_node"), &TestPathFindTo3D::set_path_to);
	ClassDB::bind_method(D_METHOD("get_path_to"), &TestPathFindTo3D::get_path_to);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug"), "set_use_debug", "get_use_debug");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "path_to", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_path_to", "get_path_to");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "found_path_threshold"), "set_found_path_threshold", "get_found_path_threshold");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "scoring_curve", PROPERTY_HINT_RESOURCE_TYPE, "Curve"), "set_scoring_curve", "get_scoring_curve");
}