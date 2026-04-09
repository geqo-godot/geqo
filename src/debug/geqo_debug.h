#pragma once
#include "query_result.h"
#include <deque>
#include <godot_cpp/classes/node.hpp>

using namespace godot;

class GEQODebug : public Object {
	GDCLASS(GEQODebug, Object)

	static GEQODebug *singleton;

private:
	int64_t min_query_time = UINT64_MAX;
	int64_t max_query_time = 0;
	std::deque<int64_t> query_times;
	int max_times = 1000;

public:
	GEQODebug();
	~GEQODebug();

	static GEQODebug *get_singleton();

	void add_time(uint64_t time);
	void _on_query_finished2d(Ref<QueryResult2D> res);
	void _on_query_finished3d(Ref<QueryResult3D> res);
	float get_average_query_time_usec();
	float get_last_query_time_usec();
	float get_min_query_time_usec();
	float get_max_query_time_usec();
	void init();

protected:
	static void _bind_methods();
};