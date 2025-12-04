#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <vector>
using namespace godot;

struct QueryItem {
	double score = 0.0;
	bool is_filtered = false;
	bool has_score = false;
	Vector3 projection_position = Vector3(0.0, 0.0, 0.0);
	Node *collided_with = nullptr;

	QueryItem(Vector3 pos, Node *collider = nullptr) {
		projection_position = pos;
		collided_with = collider;
	}

	void add_score(double amount) {
		score += amount;
		has_score = true;
	}

	bool operator>(const QueryItem &item) const {
		// Non filtered items come before filtered items
		if (is_filtered != item.is_filtered)
			return !is_filtered;

		// Items with score also come first
		if (has_score != item.has_score)
			return has_score;

		// Both have no score so do nothing
		if (!has_score)
			return false;

		return score > item.score;
	}
};
class QueryResult : public RefCounted {
	GDCLASS(QueryResult, RefCounted)

private:
	std::vector<QueryItem> query_items;

public:
	QueryResult() {}
	~QueryResult() {}

	void set_items(const std::vector<QueryItem> &items) { query_items = items; }

	Vector3 get_highest_score_position() const;
	Node *get_highest_score_node() const;
	std::vector<QueryItem> &get_query_items() { return query_items; }

protected:
	static void _bind_methods();
};