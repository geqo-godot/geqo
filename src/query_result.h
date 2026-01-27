#pragma once
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <vector>
using namespace godot;

template <typename VectorT>
struct QueryItem {
	double score = 0.0;
	bool is_filtered = false;
	bool has_score = false;
	VectorT projection_position = VectorT();
	Node *collided_with = nullptr;

	QueryItem(VectorT pos, Node *collider = nullptr) {
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
template <typename VectorT>
class QueryResultBase {
private:
	// Query Items of result, EnvironmentQuery should tranfer ownership to it
	std::vector<QueryItem<VectorT>> query_items;
	mutable bool is_cache_built = false;
	mutable std::vector<size_t> sorted_indices;
	mutable int highest_unfiltered_index = -1;

public:
	~QueryResultBase() = default;

	void set_items(const std::vector<QueryItem<VectorT>> &items) { query_items = items; }
	void _build_cache() const;

	TypedArray<VectorT> _get_all_position() const;
	TypedArray<Node> _get_all_node() const;

	VectorT _get_highest_score_position() const;
	VectorT _get_top_random_position(double percent = 0.1) const;
	Node *_get_highest_score_node() const;
	Node *_get_top_random_node(double percent = 0.1) const;
};

class QueryResult2D : public RefCounted, public QueryResultBase<Vector2> {
	GDCLASS(QueryResult2D, RefCounted)

protected:
	static void _bind_methods();

public:
	TypedArray<Vector2> get_all_position() const { return _get_all_position(); }
	TypedArray<Node> get_all_node() const { return _get_all_node(); }
	Vector2 get_highest_score_position() const { return _get_highest_score_position(); };
	Vector2 get_top_random_position(double percent = 0.05) const { return _get_top_random_position(percent); }
	Node *get_highest_score_node() const { return _get_highest_score_node(); };
	Node *get_top_random_node(double percent = 0.05) const { return _get_top_random_node(percent); }
};

class QueryResult3D : public RefCounted, public QueryResultBase<Vector3> {
	GDCLASS(QueryResult3D, RefCounted)

protected:
	static void _bind_methods();

public:
	TypedArray<Vector3> get_all_position() const { return _get_all_position(); }
	TypedArray<Node> get_all_node() const { return _get_all_node(); }
	Vector3 get_highest_score_position() const { return _get_highest_score_position(); };
	Vector3 get_top_random_position(double percent = 0.1) const { return _get_top_random_position(percent); }
	Node *get_highest_score_node() const { return _get_highest_score_node(); };
	Node *get_top_random_node(double percent = 0.1) const { return _get_top_random_node(percent); }
};
