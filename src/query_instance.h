#pragma once
#include "query_item.h"
#include <vector>

using namespace godot;

struct TestExecutionData {
	std::vector<double> raw_values;
	double min = 0.0;
	double max = 0.0;
};

template <typename QueryItemT>
class QueryInstanceBase {
private:
	std::vector<Ref<QueryItemT>> items;
	//std::unordered_map<void *, TestExecutionData> test_data;

public:
	void set_items(std::vector<Ref<QueryItemT>> &&p_items) {
		items = std::move(p_items);
	}

	int _get_item_count() const {
		return items.size();
	}

	Ref<QueryItemT> _get_item(int index) const {
		// TODO: Test for bad index
		if (index < 0 || index > items.size()) {
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
};

class QueryInstance2D : public RefCounted, public QueryInstanceBase<QueryItem2D> {
	GDCLASS(QueryInstance2D, RefCounted)

public:
	int get_item_count() const {
		return _get_item_count();
	}

	Ref<QueryItem2D> get_item(int index) const {
		return _get_item(index);
	}
	void add_item(Ref<QueryItem2D> item) {
		_add_item(item);
	}

protected:
	static void _bind_methods();
};

class QueryInstance3D : public RefCounted, public QueryInstanceBase<QueryItem3D> {
	GDCLASS(QueryInstance3D, RefCounted)

public:
	int get_item_count() const {
		return _get_item_count();
	}

	Ref<QueryItem3D> get_item(int index) const {
		return _get_item(index);
	}
	void add_item(Ref<QueryItem3D> item) {
		_add_item(item);
	}

protected:
	static void _bind_methods();
};