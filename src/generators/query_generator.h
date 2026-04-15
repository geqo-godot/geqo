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

public:
	virtual ~QueryGeneratorBase() = default;

	RaycastMode _get_raycast_mode() const { return raycast_mode; }
	void _set_raycast_mode(RaycastMode mode) {
		raycast_mode = mode;
	}
};
