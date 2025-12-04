#include "query_result.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

Vector3 QueryResult::get_highest_score_position() const {
	if (query_items.empty())
		return Vector3();

	std::vector<QueryItem> final_items = query_items;
	std::sort(final_items.begin(), final_items.end(), std::greater{});

	return final_items.begin()->projection_position;
}

Node *QueryResult::get_highest_score_node() const {
	if (query_items.empty())
		return nullptr;

	std::vector<QueryItem> final_items = query_items;
	std::sort(final_items.begin(), final_items.end(), std::greater{});

	return final_items.begin()->collided_with;
}

void QueryResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult::get_highest_score_node);
}