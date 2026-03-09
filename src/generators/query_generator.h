#pragma once
#include "query_result.h"
#include <vector>

template <typename VectorT, typename QueryItemT>
class QueryGeneratorBase {
public:
	enum RaycastMode {
		BODY,
		AREA,
		BODY_AREA
	};

protected:
	RaycastMode raycast_mode = BODY;
	std::vector<Ref<QueryItemT>> *query_items_ref = nullptr;

public:
	virtual ~QueryGeneratorBase() = default;

	void _set_query_items_ref(std::vector<Ref<QueryItemT>> &query_items) {
		query_items_ref = &query_items;
	}
	void _add_query_item(Ref<QueryItemT> query_item) {
		get_query_items_ref().push_back(query_item);
		perform_tests(get_query_items_ref()->size() - 1);
	}
	std::vector<QueryItemT> &get_query_items_ref() { return *query_items_ref; }

	RaycastMode _get_raycast_mode() const { return raycast_mode; }
	void _set_raycast_mode(RaycastMode mode) {
		raycast_mode = mode;
	}

	virtual void perform_tests(int current_item) = 0;
	virtual void perform_generation(uint64_t initial_time_usec, double time_budget_ms) = 0;
	bool has_time_left(uint64_t initial_time_usec, uint64_t current_time_usec, double time_budget_ms) {
		double time_spent_ms = (current_time_usec - initial_time_usec) / 1000;
		if (time_spent_ms < time_budget_ms)
			return true;
		else
			return false;
	}
};
