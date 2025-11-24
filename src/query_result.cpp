#include "query_result.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

Vector3 CQueryResult::get_highest_score_position() const {
	std::vector<CQueryItem> final_items = query_items;
	std::vector<CQueryItem> items_with_score;

	if (final_items.empty())
		return Vector3();

	items_with_score.reserve(query_items.size());
	std::sort(final_items.begin(), final_items.end());

	for (CQueryItem &item : final_items) {
		if (item.is_filtered || !item.has_score)
			break;
		items_with_score.push_back(item);
	}

	if (items_with_score.empty())
		return Vector3();

	return items_with_score.back().projection_position;
}

Node *CQueryResult::get_highest_score_node() const {
	std::vector<CQueryItem> final_items = query_items;
	std::vector<CQueryItem> items_with_score;

	if (final_items.empty())
		return nullptr;

	items_with_score.reserve(query_items.size());
	std::sort(final_items.begin(), final_items.end());

	for (CQueryItem &item : final_items) {
		if (item.is_filtered || !item.has_score)
			break;
		items_with_score.push_back(item);
	}

	if (items_with_score.empty())
		return nullptr;

	return items_with_score.back().collided_with;
}

void CQueryResult::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &CQueryResult::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &CQueryResult::get_highest_score_node);
}