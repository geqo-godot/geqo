#include "query_result.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

Vector3 CQueryResult::get_highest_score_position() const {
	if (query_items.empty())
		return Vector3();

	std::vector<CQueryItem> final_items = query_items;
	std::sort(final_items.begin(), final_items.end(), std::greater{});

	return final_items.begin()->projection_position;
}

Node *CQueryResult::get_highest_score_node() const {
	if (query_items.empty())
		return nullptr;

	std::vector<CQueryItem> final_items = query_items;
	std::sort(final_items.begin(), final_items.end(), std::greater{});

	return final_items.begin()->collided_with;
}

void CQueryResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &CQueryResult::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &CQueryResult::get_highest_score_node);
}