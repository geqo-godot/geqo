#include "query_result.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <numeric>

using namespace godot;

template <typename VectorT>
TypedArray<VectorT> QueryResultBase<VectorT>::_get_all_position() const {
	TypedArray<VectorT> result;

	_build_cache();

	for (int index : sorted_indices) {
		result.append(query_items[index].projection_position);
	}
	return result;
}

template <typename VectorT>
TypedArray<Node> QueryResultBase<VectorT>::_get_all_node() const {
	TypedArray<Node> result;

	_build_cache();

	for (int index : sorted_indices) {
		result.append(query_items[index].collided_with);
	}
	return result;
}

template <typename VectorT>
VectorT QueryResultBase<VectorT>::_get_highest_score_position() const {
	if (query_items.empty())
		return VectorT();

	_build_cache();

	return query_items[sorted_indices[0]].projection_position;
}

template <typename VectorT>
VectorT QueryResultBase<VectorT>::_get_top_random_position(double percent) const {
	if (query_items.empty())
		return VectorT();

	_build_cache();

	percent = std::clamp(percent, 0.0, 1.0);

	int unfiltered_count = highest_unfiltered_index + 1;
	// If there are no results
	if (unfiltered_count <= 0)
		return VectorT();

	// Get the top percentage
	int top_count = static_cast<int>(std::ceil(unfiltered_count * percent));
	top_count = std::max(top_count, 1);

	int random_i = UtilityFunctions::randi_range(0, top_count - 1);
	return query_items[sorted_indices[random_i]].projection_position;
}

template <typename VectorT>
Node *QueryResultBase<VectorT>::_get_highest_score_node() const {
	if (query_items.empty())
		return nullptr;

	_build_cache();
	return query_items[sorted_indices[0]].collided_with;
}
template <typename VectorT>
Node *QueryResultBase<VectorT>::_get_top_random_node(double percent) const {
	if (query_items.empty())
		return nullptr;

	_build_cache();

	percent = std::clamp(percent, 0.0, 1.0);

	int unfiltered_count = highest_unfiltered_index + 1;
	// If there are no results
	if (unfiltered_count <= 0)
		return nullptr;

	// Get the top percentage
	int top_count = static_cast<int>(std::ceil(unfiltered_count * percent));
	top_count = std::max(top_count, 1);

	int random_i = UtilityFunctions::randi_range(0, top_count - 1);
	return query_items[sorted_indices[random_i]].collided_with;
}

// Sort indices and store them for future calls
template <typename VectorT>
void QueryResultBase<VectorT>::_build_cache() const {
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
				return query_items[lhs] > query_items[rhs];
			});
	for (size_t i = 0; i < sorted_indices.size(); i++) {
		if (query_items[sorted_indices[i]].is_filtered)
			break;
		else
			highest_unfiltered_index = i;
	}

	is_cache_built = true;
}

void QueryResult2D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_all_position"), &QueryResult2D::get_all_position);
	ClassDB::bind_method(D_METHOD("get_all_node"), &QueryResult2D::get_all_node);
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult2D::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_top_random_position", "percent"), &QueryResult2D::get_top_random_position, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult2D::get_highest_score_node);
	ClassDB::bind_method(D_METHOD("get_top_random_node", "percent"), &QueryResult2D::get_top_random_node, DEFVAL(0.1));
}

void QueryResult3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_all_position"), &QueryResult3D::get_all_position);
	ClassDB::bind_method(D_METHOD("get_all_node"), &QueryResult3D::get_all_node);
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult3D::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_top_random_position", "percent"), &QueryResult3D::get_top_random_position, DEFVAL(0.1));
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult3D::get_highest_score_node);
	ClassDB::bind_method(D_METHOD("get_top_random_node", "percent"), &QueryResult3D::get_top_random_node, DEFVAL(0.1));
}
