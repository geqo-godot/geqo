#include "generators/query_generator.h"

void QueryGeneratorBase::set_query_items_ref(std::vector<QueryItem> &query_items) {
	query_items_ref = &query_items;
}
void QueryGeneratorBase::set_raycast_mode(RaycastMode mode) {
	raycast_mode = mode;
}

bool QueryGeneratorBase::has_time_left(uint64_t initial_time_usec, uint64_t current_time_usec, int time_budget_ms) {
	double time_spent_ms = (current_time_usec - initial_time_usec) / 1000;
	if (time_spent_ms < time_budget_ms)
		return true;
	else
		return false;
}
