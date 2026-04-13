#pragma once
#include "godot_cpp/classes/time.hpp"
#include "query_item.h"
#include <vector>

using namespace godot;

struct TestExecutionData {
	double min = 0.0;
	double max = 0.0;
};

template <typename QueryItemT>
class QueryInstanceBase {
private:
	std::vector<Ref<QueryItemT>> items;
	uint64_t initial_time_usec = 0;
	double time_budget_ms = 0.0;
	int current = 0;

	std::unordered_map<uint64_t, TestExecutionData> test_data;

public:
	bool has_test_data(Object *test) {
		return test_data.count(test->get_instance_id()) > 0;
	}

	void clear_test_data(Object *test) {
		test_data.erase(test->get_instance_id());
	}

	void _set_test_data_max(Object *test, double max) {
		test_data[test->get_instance_id()].max = max;
	}

	double _get_test_data_max(Object *test) {
		auto it = test_data.find(test->get_instance_id());
		if (it == test_data.end()) {
			ERR_PRINT("QueryInstance: get_test_data_max called before data was initialized for this test.");
			return 0.0;
		}
		return it->second.max;
	}

	void _set_test_data_min(Object *test, double min) {
		test_data[test->get_instance_id()].min = min;
	}

	double _get_test_data_min(Object *test) {
		auto it = test_data.find(test->get_instance_id());
		if (it == test_data.end()) {
			ERR_PRINT("QueryInstance: get_test_data_min called before data was initialized for this test.");
			return 0.0;
		}
		return it->second.min;
	}
	bool _has_test_data(Object *test) {
		return test_data.count(test->get_instance_id()) > 0;
	}
	void set_budget(double budget) {
		time_budget_ms = budget;
		initial_time_usec = Time::get_singleton()->get_ticks_usec();
	}

	bool _has_items() {
		return current < items.size();
	}

	Ref<QueryItemT> _get_next_item() {
		if (current < items.size()) {
			int i = current;
			current++;
			return _get_item(i);
		}
		return Ref<QueryItemT>();
	}

	void _reset_iterator() {
		current = 0;
	}

	void set_current(int c) { current = c; }
	int get_current() { return current; }

	void set_items(std::vector<Ref<QueryItemT>> &&p_items) {
		items = std::move(p_items);
	}

	Ref<QueryItemT> _get_item(int index) const {
		if (index < 0 || index >= items.size()) {
			return Ref<QueryItemT>();
		}
		return items[index];
	}

	std::vector<Ref<QueryItemT>> &get_items() {
		return items;
	}
	std::vector<Ref<QueryItemT>> take_items() {
		return std::move(items);
	}

	void _add_item(Ref<QueryItemT> item) {
		items.push_back(item);
	}

	int _get_item_count() const {
		return items.size();
	}

	// Time stuff
	// Resets time itself, because it's shared across everything
	bool _has_time_left() {
		// negative time budget means infinite time
		if (time_budget_ms <= 0.0)
			return true;
		uint64_t current_time_usec = Time::get_singleton()->get_ticks_usec();
		double time_spent_ms = (current_time_usec - initial_time_usec) / 1000;

		return time_spent_ms < time_budget_ms;
	}

	void _refresh_timer() {
		initial_time_usec = Time::get_singleton()->get_ticks_usec();
	}
};

class QueryInstance2D : public RefCounted, public QueryInstanceBase<QueryItem2D> {
	GDCLASS(QueryInstance2D, RefCounted)
public:
	void set_test_data_max(Object *test, double max) { _set_test_data_max(test, max); }
	double get_test_data_max(Object *test) { return _get_test_data_max(test); }
	void set_test_data_min(Object *test, double min) { _set_test_data_min(test, min); }
	double get_test_data_min(Object *test) { return _get_test_data_min(test); }
	bool has_test_data(Object *test) { return _has_test_data(test); }
	bool has_items() { return _has_items(); }
	Ref<QueryItem3D> get_next_item() { return _get_next_item(); }
	int get_item_count() const { return _get_item_count(); }
	Ref<QueryItem3D> get_item(int index) const { return _get_item(index); }
	void add_item(Ref<QueryItem3D> item) { _add_item(item); }
	bool has_time_left() { return _has_time_left(); }
	void refresh_timer() { _refresh_timer(); }
	void reset_iterator() { _reset_iterator(); }

protected:
	static void _bind_methods();
};

class QueryInstance3D : public RefCounted, public QueryInstanceBase<QueryItem3D> {
	GDCLASS(QueryInstance3D, RefCounted)
public:
	void set_test_data_max(Object *test, double max) { _set_test_data_max(test, max); }
	double get_test_data_max(Object *test) { return _get_test_data_max(test); }
	void set_test_data_min(Object *test, double min) { _set_test_data_min(test, min); }
	double get_test_data_min(Object *test) { return _get_test_data_min(test); }
	bool has_test_data(Object *test) { return _has_test_data(test); }
	bool has_items() { return _has_items(); }
	Ref<QueryItem3D> get_next_item() { return _get_next_item(); }
	int get_item_count() const { return _get_item_count(); }
	Ref<QueryItem3D> get_item(int index) const { return _get_item(index); }
	void add_item(Ref<QueryItem3D> item) { _add_item(item); }
	bool has_time_left() { return _has_time_left(); }
	void refresh_timer() { _refresh_timer(); }
	void reset_iterator() { _reset_iterator(); }

protected:
	static void _bind_methods();
};