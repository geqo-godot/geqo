# meta-description: Base template for QueryTest3D with basic structure and a random number test
@tool
extends QueryTest3D

# You can add as many contexts as you need for the test
## Context the test will be performed on
@export var context: QueryContext3D


func _enter_tree() -> void:
	# Define the cost and test_type of the test. (Numeric/Boolean)
	cost = 1
	test_type = GEQOEnums.TEST_TYPE_NUMERIC

func _perform_test(query_instance: QueryInstance3D) -> void:
	if not context:
		# query_instance stores querier, so it can default into it
		context = query_instance.querier_context
	
	# You can get the positions of the contexts as a PackedVector3Array
	var context_positions: PackedVector3Array = context.get_context_positions(query_instance)
	# You can also get the nodes within that context, as some tests could use that information
	# var context_nodes: Array[Node3D] = context.get_context_nodes(query_instance)
	
	# If the array is empty there's nothing to do so end the test
	if context_positions.is_empty():
		end_test()
		return
	
	while query_instance.has_items():
		# Get the item and check that it's not filtered
		var item: QueryItem3D = query_instance.get_next_item()
		if item.is_filtered:
			continue
		
		# This is where the tests for each item will be performed
		var average: float = 0.0
		var sum: float = 0.0
		
		for pos: Vector3 in context_positions:
			# Make a random float for each position, then average them
			var num: float = randf()
			sum += num
		average = sum / context_positions.size()
		item.add_score_numeric(test_purpose, filter_type, average, filter_min, filter_max)
		
		# Time ran out so wait until next frame
		if not query_instance.has_time_left():
			await get_tree().process_frame
			query_instance.refresh_timer()
	
	# Ends the test
	# WARNING: This is required.
	end_test()
