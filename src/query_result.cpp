#include "query_result.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <numeric>

using namespace godot;

void QueryResult2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_all_results"), &QueryResult2D::get_all_results);
	ClassDB::bind_method(D_METHOD("get_all_position"), &QueryResult2D::get_all_position);
	ClassDB::bind_method(D_METHOD("get_all_node"), &QueryResult2D::get_all_node);
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult2D::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_top_random_position", "percent"), &QueryResult2D::get_top_random_position, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult2D::get_highest_score_node);
	ClassDB::bind_method(D_METHOD("get_top_random_node", "percent"), &QueryResult2D::get_top_random_node, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("has_result"), &QueryResult2D::has_result);
}

void QueryResult3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_all_results"), &QueryResult3D::get_all_results);
	ClassDB::bind_method(D_METHOD("get_all_position"), &QueryResult3D::get_all_position);
	ClassDB::bind_method(D_METHOD("get_all_node"), &QueryResult3D::get_all_node);
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult3D::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_top_random_position", "percent"), &QueryResult3D::get_top_random_position, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult3D::get_highest_score_node);
	ClassDB::bind_method(D_METHOD("get_top_random_node", "percent"), &QueryResult3D::get_top_random_node, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("has_result"), &QueryResult3D::has_result);
}
