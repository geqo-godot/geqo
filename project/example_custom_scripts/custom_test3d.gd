## This custom test mirrors the functionality of TestDistanceTo3D as example of how it would be implemented in GDScript.
@tool
class_name CustomDistanceTo3D extends QueryTest3D

@export var distance_to: QueryContext3D
@export var min_distance: float = 0.0
@export var max_distance: float = 10.0
@export var scoring_curve: Curve
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
		return
	
	while query_instance.has_items():
		var item: QueryItem3D = query_instance.get_next_item()
		if item.is_filtered:
			continue
		print("Item position :sad_cat:", item.projection_position)
		item.add_score_numeric(test_purpose, filter_type, randf_range(0.0, 1.0), filter_min, filter_max)
		print("Adding random amount of score: ", item.score)
		var choice: bool = randi() % 2 == 0
		if choice:
			print("Simulating mega heavy work and calculations")
			mega_heavy_work(5000)
		if not query_instance.has_time_left():
			print("AWAITING NEXT FRAME")
			await get_tree().process_frame
			query_instance.refresh_timer()
	end_test()

func mega_heavy_work(iterations: int) -> float:
	var acc := 0.0
	for i in iterations:
		acc += sin(i) * cos(i)
	return acc
