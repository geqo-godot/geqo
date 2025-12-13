#pragma once
#include "query_result.h"
#include <vector>

class QueryGeneratorBase {
public:
	enum RaycastMode {
		BODY,
		AREA,
		BODY_AREA
	};

protected:
	RaycastMode raycast_mode = BODY;
	std::vector<QueryItem> *query_items_ref = nullptr;

public:
	virtual ~QueryGeneratorBase() = default;

	void set_query_items_ref(std::vector<QueryItem> &query_items);
	std::vector<QueryItem> &get_query_items_ref() { return *query_items_ref; }

	void set_raycast_mode(RaycastMode mode);
	RaycastMode get_raycast_mode() const { return raycast_mode; }

	virtual void perform_tests(int current_item) = 0;
	virtual void perform_generation(uint64_t initial_time_usec, int time_budget_ms) = 0;
	bool has_time_left(uint64_t initial_time_usec, uint64_t current_time_usec, int time_budget_ms);
};

VARIANT_ENUM_CAST(QueryGeneratorBase::RaycastMode);
