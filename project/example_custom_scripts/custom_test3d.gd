## This custom test mirrors the functionality of TestDistanceTo3D as example of how it would be implemented in GDScript.
@tool
class_name CustomDistanceTo3D extends QueryTest3D

enum ClampType {
	NONE,
	VAL,
	FILTER
}

@export var distance_to: QueryContext3D
@export var scoring_curve: Curve
@export var clamp_min_type: ClampType = ClampType.NONE :
	set(val):
		clamp_min_type = val
		notify_property_list_changed()
@export var score_clamp_min: float = 0.0
@export var clamp_max_type: ClampType = ClampType.NONE :
	set(val):
		clamp_max_type = val
		notify_property_list_changed()
@export var score_clamp_max: float = 1.0

func _validate_property(property: Dictionary) -> void:
	if property.name == "score_clamp_min":
		if clamp_min_type != ClampType.VAL:
			property.usage &= ~PROPERTY_USAGE_EDITOR
	if property.name == "score_clamp_max":
		if clamp_max_type != ClampType.VAL:
			property.usage &= ~PROPERTY_USAGE_EDITOR

func _enter_tree() -> void:
	cost = 1
	test_type = GEQOEnums.TEST_TYPE_NUMERIC
	if not scoring_curve:
		scoring_curve = Curve.new()
		scoring_curve.add_point(Vector2(0, 0))
		scoring_curve.add_point(Vector2(1, 1))
	scoring_curve.bake()

func _perform_test(query_instance: QueryInstance3D) -> void:
	if not distance_to:
		push_error("Test has no context")
		end_test()
		return
	
	var context_positions: PackedVector3Array = distance_to.get_context_positions(query_instance)
	if context_positions.is_empty():
		end_test()
		return
	
	# Pre pass test to get the highest and lowest distance and store it for second pass
	var needs_pre_pass: bool = clamp_min_type == ClampType.NONE or clamp_max_type == ClampType.NONE
	if needs_pre_pass and not query_instance.has_test_data(self):
		var smallest_item: float = INF
		var biggest_item: float = -INF
		while query_instance.has_items():
			var item: QueryItem3D = query_instance.get_next_item()
			if item.is_filtered:
				continue
			var score: float = calculate_context_score(item, context_positions)
			if (score < smallest_item):
				smallest_item = score
			if (score > biggest_item):
				biggest_item = score
			
		query_instance.set_test_data_min(self, smallest_item)
		query_instance.set_test_data_max(self, biggest_item)
		query_instance.reset_iterator()
	
	# Second pass
	var clamp_min: float = get_effective_clamp_min(query_instance)
	var clamp_max: float = get_effective_clamp_max(query_instance)
	while query_instance.has_items():
		var item: QueryItem3D = query_instance.get_next_item()
		if item.is_filtered:
			continue
		
		var score: float = calculate_context_score(item, context_positions)
		var curve_score = scoring_curve.sample(clamp(remap(score, clamp_min, clamp_max, 0, 1),0, 1))
		# Filter first, because filtering requires raw values as opposed to scoring
		if test_purpose in [GEQOEnums.FILTER_SCORE, GEQOEnums.FILTER_ONLY]:
				match multiple_context_filter_operator:
					GEQOEnums.OP_ANY_PASS:
						if not evaluate_context_filter_any(item, context_positions):
							item.is_filtered = true
							continue
					GEQOEnums.OP_ALL_PASS:
						if not evaluate_context_filter_all(item, context_positions):
							item.is_filtered = true
							continue
					_:
						item.add_score_numeric(GEQOEnums.FILTER_ONLY, filter_type, score, filter_min, filter_max)
		# Now use the curve score for the score calculations
		if test_purpose in [GEQOEnums.FILTER_SCORE, GEQOEnums.SCORE_ONLY]:
			item.add_score_numeric(test_purpose, filter_type, curve_score, 0.0, 1.0)
		
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
	match clamp_min_type:
		ClampType.VAL:
			return score_clamp_min
		ClampType.FILTER:
			return filter_min
		_:
			return query_instance.get_test_data_min(self)

func get_effective_clamp_max(query_instance: QueryInstance3D) -> float:
	match clamp_max_type:
		ClampType.VAL:
			return score_clamp_max
		ClampType.FILTER:
			return filter_max
		_:
			return query_instance.get_test_data_max(self)

func calculate_context_score(item: QueryItem3D, context_positions: PackedVector3Array) -> float:
	var sum: float = 0.0
	var smallest: float = INF
	var biggest: float = -INF
	
	for pos: Vector3 in context_positions:
		var dist: float = item.projection_position.distance_to(pos)
		sum += dist
		if (dist < smallest):
			smallest = dist
		if (dist > biggest):
			biggest = dist
	var average: float = (sum / context_positions.size()) 
	
	match multiple_context_score_operator:
		GEQOEnums.OP_AVERAGE_SCORE:
			return average
		GEQOEnums.OP_MIN_SCORE:
			return smallest
		GEQOEnums.OP_MAX_SCORE:
			return biggest
	return 0.0
