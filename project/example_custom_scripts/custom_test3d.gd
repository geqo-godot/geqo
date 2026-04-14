## This custom test mirrors the functionality of TestDistanceTo3D as example of how it would be implemented in GDScript.
@tool
class_name CustomDistanceTo3D extends QueryTest3D

@export var distance_to: QueryContext3D

func _enter_tree() -> void:
	cost = 1
	test_type = GEQOEnums.TEST_TYPE_NUMERIC

func _perform_test(query_instance: QueryInstance3D) -> void:
	if not distance_to:
		distance_to = query_instance.querier_context
		end_test()
		return
	
	var context_positions: PackedVector3Array = distance_to.get_context_positions(query_instance)
	if context_positions.is_empty():
		end_test()
		return
	
	# Pre pass test to get the highest and lowest distance and store it for second pass
	var needs_pre_pass: bool = score_clamp_min_type == GEQOEnums.CLAMP_TYPE_NONE or score_clamp_max_type == GEQOEnums.CLAMP_TYPE_NONE
	if needs_pre_pass and not query_instance.has_test_data(self ):
		var smallest_item: float = INF
		var biggest_item: float = - INF
		while query_instance.has_items():
			var item: QueryItem3D = query_instance.get_next_item()
			if item.is_filtered:
				continue
			var score: float = calculate_context_score(item, context_positions)
			if (score < smallest_item):
				smallest_item = score
			if (score > biggest_item):
				biggest_item = score
			
		query_instance.set_test_data_min(self , smallest_item)
		query_instance.set_test_data_max(self , biggest_item)
		query_instance.reset_iterator()
	query_instance.get_querier_context()
	# Second pass
	var clamp_min: float = get_effective_clamp_min(query_instance)
	var clamp_max: float = get_effective_clamp_max(query_instance)
	while query_instance.has_items():
		var item: QueryItem3D = query_instance.get_next_item()
		if item.is_filtered:
			continue
		
		var raw_score: float = calculate_context_score(item, context_positions)
		
		# Filter first, because filtering requires raw values as opposed to scoring
		if test_purpose in [GEQOEnums.PURPOSE_FILTER_SCORE, GEQOEnums.PURPOSE_FILTER_ONLY]:
				match filter_multiple_context_filter_operator:
					GEQOEnums.OP_ANY_PASS:
						if not evaluate_context_filter_any(item, context_positions):
							item.is_filtered = true
							continue
					GEQOEnums.OP_ALL_PASS:
						if not evaluate_context_filter_all(item, context_positions):
							item.is_filtered = true
							continue
					_:
						if not item.apply_filter_numeric(filter_type, raw_score, filter_min, filter_max):
							continue
		# Now use the curve score for the score calculations
		# value * score_factor
		if test_purpose in [GEQOEnums.PURPOSE_FILTER_SCORE, GEQOEnums.PURPOSE_SCORE_ONLY]:
			var normalized: float = clamp(remap(raw_score, clamp_min, clamp_max, 0.0, 1.0), 0.0, 1.0)
			var curve_score: float = score_curve.sample(normalized)
			item.add_score_direct(test_purpose, curve_score, score_factor)
		# Time ran out so wait until next frame
		if not query_instance.has_time_left():
			await get_tree().process_frame
			print("Awaited next frame")
			query_instance.refresh_timer()
	end_test()

## Passes if at least one context position is within filter range
func evaluate_context_filter_any(item: QueryItem3D, context_positions: PackedVector3Array) -> bool:
	for pos: Vector3 in context_positions:
		var dist: float = item.projection_position.distance_to(pos)
		if dist >= filter_min and dist <= filter_max:
			return true
	return false

## Passes only if every context position is within filter range
func evaluate_context_filter_all(item: QueryItem3D, context_positions: PackedVector3Array) -> bool:
	for pos: Vector3 in context_positions:
		var dist: float = item.projection_position.distance_to(pos)
		if dist < filter_min or dist > filter_max:
			return false
	return true

func get_effective_clamp_min(query_instance: QueryInstance3D) -> float:
	match score_clamp_min_type:
		GEQOEnums.CLAMP_TYPE_VAL:
			return score_clamp_min
		GEQOEnums.CLAMP_TYPE_FILTER:
			return filter_min
		_:
			return query_instance.get_test_data_min(self )

func get_effective_clamp_max(query_instance: QueryInstance3D) -> float:
	match score_clamp_max_type:
		GEQOEnums.CLAMP_TYPE_VAL:
			return score_clamp_max
		GEQOEnums.CLAMP_TYPE_FILTER:
			return filter_max
		_:
			return query_instance.get_test_data_max(self )

func calculate_context_score(item: QueryItem3D, context_positions: PackedVector3Array) -> float:
	var sum: float = 0.0
	var smallest: float = INF
	var biggest: float = - INF
	
	for pos: Vector3 in context_positions:
		var dist: float = item.projection_position.distance_to(pos)
		sum += dist
		if (dist < smallest):
			smallest = dist
		if (dist > biggest):
			biggest = dist
	var average: float = (sum / context_positions.size())
	
	match score_multiple_context_score_operator:
		GEQOEnums.OP_AVERAGE_SCORE:
			return average
		GEQOEnums.OP_MIN_SCORE:
			return smallest
		GEQOEnums.OP_MAX_SCORE:
			return biggest
	return 0.0
