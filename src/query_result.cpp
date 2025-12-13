#include "query_result.h"
#include <algorithm>
#include <godot_cpp/core/class_db.hpp>
#include <numeric>

using namespace godot;

template <typename VectorT>
VectorT QueryResultBase<VectorT>::_get_highest_score_position() const {
	if (query_items.empty())
		return VectorT();

	_build_cache();

	//UtilityFunctions::print("Array size: ", query_items.size());
	return query_items[sorted_indices[0]].projection_position;
}

template <typename VectorT>
Node *QueryResultBase<VectorT>::_get_highest_score_node() const {
	if (query_items.empty())
		return nullptr;

	_build_cache();
	return query_items[sorted_indices[0]].collided_with;
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

	is_cache_built = true;
}

void QueryResult3D::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_highest_score_position"), &QueryResult3D::get_highest_score_position);
	ClassDB::bind_method(D_METHOD("get_highest_score_node"), &QueryResult3D::get_highest_score_node);
}