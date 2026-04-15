#pragma once
#include "query_result.h"
#include <vector>

template <typename VectorT, typename QueryItemT, typename QueryInstanceT>
class QueryGeneratorBase {
public:
	enum RaycastMode {
		BODY,
		AREA,
		BODY_AREA
	};

protected:
	RaycastMode raycast_mode = BODY;
	Ref<QueryInstanceT> query_instance;

public:
	virtual ~QueryGeneratorBase() = default;

	void _set_query_instance(Ref<QueryInstanceT> instance) {
		query_instance = instance;
	}

	Ref<QueryInstanceT> _get_query_instance() {
		return query_instance;
	}

	void _add_query_item(Ref<QueryItemT> query_item) {
		query_instance->add_item(query_item);
	}

	RaycastMode _get_raycast_mode() const { return raycast_mode; }
	void _set_raycast_mode(RaycastMode mode) {
		raycast_mode = mode;
	}

	virtual void perform_generation(uint64_t initial_time_usec, double time_budget_ms) = 0;
	bool has_time_left(uint64_t initial_time_usec, uint64_t current_time_usec, double time_budget_ms) {
		double time_spent_ms = (current_time_usec - initial_time_usec) / 1000;
		if (time_spent_ms < time_budget_ms)
			return true;
		else
			return false;
	}
};
