#include "debug/geqo_debug.h"
#include "geqo_debug.h"
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/performance.hpp>
#include <numeric>

GEQODebug *GEQODebug::singleton = nullptr;

GEQODebug::GEQODebug() {
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;
}

GEQODebug::~GEQODebug() {
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

GEQODebug *GEQODebug::get_singleton() {
	return singleton;
}

void GEQODebug::add_time(uint64_t time) {
	query_times.push_back(time);
	if (query_times.size() > max_times)
		query_times.pop_front();

	if (time < min_query_time)
		min_query_time = time;
	if (time > max_query_time)
		max_query_time = time;
}

void GEQODebug::_on_query_finished2d(Ref<QueryResult2D> res) {
	add_time(res->get_time_it_took());
}

void GEQODebug::_on_query_finished3d(Ref<QueryResult3D> res) {
	add_time(res->get_time_it_took());
}

float GEQODebug::get_average_query_time_usec() {
	if (query_times.empty())
		return -1.0;
	return float(std::accumulate(query_times.begin(), query_times.end(), int64_t(0))) / float(query_times.size());
}

float GEQODebug::get_last_query_time_usec() {
	if (query_times.empty())
		return -1.0;
	return query_times.back();
}
float GEQODebug::get_min_query_time_usec() {
	return min_query_time;
}
float GEQODebug::get_max_query_time_usec() {
	return max_query_time;
}
void GEQODebug::init() {
	if (!Engine::get_singleton()->is_editor_hint()) {
		Performance *p_monitor = Performance::get_singleton();
		p_monitor->add_custom_monitor("geqo/min_query_time_usec", callable_mp(this, &GEQODebug::get_min_query_time_usec));
		p_monitor->add_custom_monitor("geqo/max_query_time_usec", callable_mp(this, &GEQODebug::get_max_query_time_usec));
		p_monitor->add_custom_monitor("geqo/latest_query_time_usec", callable_mp(this, &GEQODebug::get_last_query_time_usec));
		p_monitor->add_custom_monitor("geqo/average_query_time_usec", callable_mp(this, &GEQODebug::get_average_query_time_usec));
	}
}

void GEQODebug::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_average_query_time_usec"), &GEQODebug::get_average_query_time_usec);
	ClassDB::bind_method(D_METHOD("get_latest_query_time_usec"), &GEQODebug::get_last_query_time_usec);
}
