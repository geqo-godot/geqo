#include "tests/test_path_find_to3d.h"
#include "contexts/context_target_node3d.h"
#include "query_enums.h"
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

void TestPathFindTo3D::set_path_to(QueryContext3D *context_node) {
	path_to = context_node;
}

void TestPathFindTo3D::set_found_path_threshold(double threshold) {
	found_path_threshold = threshold;
}

bool TestPathFindTo3D::evaluate_context_paths(Ref<QueryItem3D> item, const Array &context_positions) {
	RID default_map_rid = get_world_3d()->get_navigation_map();
	Vector3 start = item->get_projection_position();

	for (int i = 0; i < context_positions.size(); i++) {
		Vector3 target = context_positions[i];

		// Project target to navmesh
		Vector3 nav_target = NavigationServer3D::get_singleton()->map_get_closest_point(default_map_rid, target);

		bool is_valid = true;
		PackedVector3Array path_to_pos;

		// Check if it's even reachable
		if (nav_target.distance_to(target) > found_path_threshold) {
			// Can't even be reached
			is_valid = false;

			if (use_debug) {
				// get the path for debugging
				path_to_pos = NavigationServer3D::get_singleton()->map_get_path(default_map_rid, start, nav_target, true);
			}
		} else {
			// get the path
			path_to_pos = NavigationServer3D::get_singleton()->map_get_path(default_map_rid, start, nav_target, true);

			if (path_to_pos.is_empty()) {
				is_valid = false;
			} else {
				Vector3 end = path_to_pos[path_to_pos.size() - 1];

				UtilityFunctions::print("target:", target);
				UtilityFunctions::print("nav_target:", nav_target);
				UtilityFunctions::print("end:", end);

				bool reached_target = end.distance_to(nav_target) <= found_path_threshold;

				if (!reached_target)
					is_valid = false;
			}
		}

		if (use_debug && !path_to_pos.is_empty()) {
			draw_path(path_to_pos, is_valid ? Color(0, 1, 0) : Color(1, 0, 0));
		}

		// ALL_PASS early exit (did not find all paths)
		if (!is_valid &&
			get_multiple_context_filter_operator() == GEQOEnums::OP_ALL_PASS) {
			return false;
		}
	}

	// All of them passed
	return true;
}

void TestPathFindTo3D::perform_test(Ref<QueryInstance3D> query_instance) {
	if (path_to == nullptr) {
		path_to = Object::cast_to<QueryContext3D>(query_instance->get_querier_context());
		return;
	}

	Array context_positions = path_to->get_context_positions(query_instance);

	while (query_instance->has_items()) {
		Ref<QueryItem3D> item = query_instance->get_next_item();
		if (item->get_is_filtered())
			continue;

		bool passes = evaluate_context_paths(item, context_positions);
		item->add_score_boolean(get_test_purpose(), passes, get_bool_match());

		if (!query_instance->has_time_left()) {
			stored_instance = query_instance;
			get_tree()->connect("process_frame", callable_mp(this, &TestPathFindTo3D::_on_next_process_frame), CONNECT_ONE_SHOT);
			return;
		}
	}
	end_test();
}

void TestPathFindTo3D::_on_next_process_frame() {
	stored_instance->refresh_timer();
	perform_test(stored_instance);
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
}

void TestPathFindTo3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_use_debug", "debug"), &TestPathFindTo3D::set_use_debug);
	ClassDB::bind_method(D_METHOD("get_use_debug"), &TestPathFindTo3D::get_use_debug);

	ClassDB::bind_method(D_METHOD("set_found_path_threshold", "threshold"), &TestPathFindTo3D::set_found_path_threshold);
	ClassDB::bind_method(D_METHOD("get_found_path_threshold"), &TestPathFindTo3D::get_found_path_threshold);

	ClassDB::bind_method(D_METHOD("set_path_to", "context_node"), &TestPathFindTo3D::set_path_to);
	ClassDB::bind_method(D_METHOD("get_path_to"), &TestPathFindTo3D::get_path_to);

	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_debug"), "set_use_debug", "get_use_debug");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "path_to", PROPERTY_HINT_NODE_TYPE, "QueryContext3D"), "set_path_to", "get_path_to");
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "found_path_threshold"), "set_found_path_threshold", "get_found_path_threshold");
}