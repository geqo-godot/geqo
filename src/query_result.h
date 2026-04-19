#pragma once
#include "query_item.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/vector3.hpp>

#include <algorithm>
#include <numeric>
#include <vector>

using namespace godot;

template <typename VectorT, typename QueryItemT, typename NodeT>
class QueryResultBase {
private:
	// Query Items of result, EnvironmentQuery should tranfer ownership to it
	std::vector<Ref<QueryItemT>> query_items;
	mutable bool is_cache_built = false;
	mutable std::vector<size_t> sorted_indices;
	mutable int highest_unfiltered_index = -1;

	// For debugging
	uint64_t time_it_took_usec = 0;

public:
	~QueryResultBase() = default;

	void set_items(const std::vector<Ref<QueryItemT>> &items) { query_items = items; }

	// Sort indices and store them for future calls
	void _build_cache() const {
		if (is_cache_built) {
			return;
		}

		sorted_indices.resize(query_items.size());
		// Init sorted_vertices
		std::iota(sorted_indices.begin(), sorted_indices.end(), 0);

		// Sort in descending order
		std::sort(
				sorted_indices.begin(),
				sorted_indices.end(),
				[&](size_t lhs, size_t rhs) {
					return query_items[lhs]->is_higher_than(query_items[rhs]);
				});
		for (size_t i = 0; i < sorted_indices.size(); i++) {
			if (query_items[sorted_indices[i]]->get_is_filtered())
				break;
			else
				highest_unfiltered_index = i;
		}

		is_cache_built = true;
	}

	TypedArray<Ref<QueryItemT>> _get_all_results() const {
		TypedArray<QueryItemT> result;

		_build_cache();

		if (highest_unfiltered_index == -1)
			return result; // Return empty

		// Has to be turned into type array for godot
		for (int i = 0; i <= highest_unfiltered_index; i++) {
			result.append(query_items[sorted_indices[i]]);
		}
		return result;
	}
	TypedArray<VectorT> _get_all_position() const {
		TypedArray<VectorT> result;

		_build_cache();

		if (highest_unfiltered_index == -1)
			return result; // Return empty

		for (int i = 0; i <= highest_unfiltered_index; i++) {
			result.append(query_items[sorted_indices[i]]->get_projection_position());
		}
		return result;
	}
	TypedArray<NodeT> _get_all_node() const {
		TypedArray<NodeT> result;

		_build_cache();

		if (highest_unfiltered_index == -1)
			return result; // Return empty

		for (int i = 0; i <= highest_unfiltered_index; i++) {
			result.append(query_items[sorted_indices[i]]->get_collided_with());
		}
		return result;
	}

	VectorT _get_highest_score_position() const {
		if (query_items.empty())
			return VectorT();

		_build_cache();

		if (highest_unfiltered_index == -1)
			return VectorT(); // Return empty

		return query_items[sorted_indices[0]]->get_projection_position();
	}
	VectorT _get_top_random_position(double percent = 0.1) const {
		if (query_items.empty())
			return VectorT();

		_build_cache();

		// If there are no results
		if (highest_unfiltered_index == -1)
			return VectorT(); // Return empty

		int unfiltered_count = highest_unfiltered_index + 1;
		percent = std::clamp(percent, 0.0, 1.0);

		// Get the top percentage
		int top_count = static_cast<int>(std::ceil(unfiltered_count * percent));
		top_count = std::max(top_count, 1);

		int random_i = UtilityFunctions::randi_range(0, top_count - 1);
		return query_items[sorted_indices[random_i]]->get_projection_position();
	}
	NodeT *_get_highest_score_node() const {
		if (query_items.empty())
			return nullptr;

		_build_cache();

		if (highest_unfiltered_index == -1)
			return nullptr; // Return empty

		return query_items[sorted_indices[0]]->get_collided_with();
	}
	NodeT *_get_top_random_node(double percent = 0.1) const {
		if (query_items.empty())
			return nullptr;

		_build_cache();

		if (highest_unfiltered_index == -1)
			return nullptr; // Return empty

		percent = std::clamp(percent, 0.0, 1.0);

		int unfiltered_count = highest_unfiltered_index + 1;

		// Get the top percentage
		int top_count = static_cast<int>(std::ceil(unfiltered_count * percent));
		top_count = std::max(top_count, 1);

		int random_i = UtilityFunctions::randi_range(0, top_count - 1);
		return query_items[sorted_indices[random_i]]->get_collided_with();
	}

	bool _has_result() {
		_build_cache();
		return highest_unfiltered_index != -1;
	}
	uint64_t get_time_it_took() { return time_it_took_usec; }
	void set_time_it_took(uint64_t usecs) { time_it_took_usec = usecs; }
};

class QueryResult2D : public RefCounted, public QueryResultBase<Vector2, QueryItem2D, Node2D> {
	GDCLASS(QueryResult2D, RefCounted)

protected:
	static void _bind_methods();

public:
	TypedArray<Ref<QueryItem2D>> get_all_results() const { return _get_all_results(); }
	TypedArray<Vector2> get_all_position() const { return _get_all_position(); }
	TypedArray<Node> get_all_node() const { return _get_all_node(); }
	Vector2 get_highest_score_position() const { return _get_highest_score_position(); };
	Vector2 get_top_random_position(double percent = 0.05) const { return _get_top_random_position(percent); }
	Node2D *get_highest_score_node() const { return _get_highest_score_node(); };
	Node2D *get_top_random_node(double percent = 0.05) const { return _get_top_random_node(percent); }
	bool has_result() { return _has_result(); }
};

class QueryResult3D : public RefCounted, public QueryResultBase<Vector3, QueryItem3D, Node3D> {
	GDCLASS(QueryResult3D, RefCounted)

protected:
	static void _bind_methods();

public:
	TypedArray<Ref<QueryItem3D>> get_all_results() const { return _get_all_results(); }
	TypedArray<Vector3> get_all_position() const { return _get_all_position(); }
	TypedArray<Node> get_all_node() const { return _get_all_node(); }
	Vector3 get_highest_score_position() const { return _get_highest_score_position(); };
	Vector3 get_top_random_position(double percent = 0.1) const { return _get_top_random_position(percent); }
	Node3D *get_highest_score_node() const { return _get_highest_score_node(); };
	Node3D *get_top_random_node(double percent = 0.1) const { return _get_top_random_node(percent); }
	bool has_result() { return _has_result(); }
};
