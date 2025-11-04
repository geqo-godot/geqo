class_name TestDistanceTo
extends QueryTest


@export var distance_to: QueryContext
@export var min_distance: float = 0.0
@export var max_distance: float = 10

@export_group("Score")
@export var scoring_curve: Curve

func _ready() -> void:
	if scoring_curve == null:
		scoring_curve = Curve.new()
		scoring_curve.add_point(Vector2.ZERO)
		scoring_curve.add_point(Vector2(1, 1))
	scoring_curve.bake()

func perform_test(projection: QueryItem):
	var context_nodes: Array = distance_to.get_context_positions()
	# print_debug("Context positions: ", context_nodes)
	var scores: Array[float] = []

	for context_pos: Vector3 in context_nodes:
		var distance: float = projection.projection_position.distance_to(context_pos)
		if test_purpose in [TestPurpose.FILTER_SCORE, TestPurpose.FILTER_ONLY]:
			if distance < min_distance or distance > max_distance:
				scores.append(0.0)
				continue
		
		var linear_score: float = (distance - min_distance) / (max_distance - min_distance)
		var clamped_score: float = clamp(linear_score, 0.0, 1.0)
		var curve_score: float = scoring_curve.sample_baked(clamped_score)

		scores.append(curve_score)
	
		
	var result: float = 0.0
	match multiple_context_operator:
		ScoreOperator.AVERAGE_SCORE:
			var total_score: float = scores.reduce(sum_f)
			result = total_score / scores.size()
		ScoreOperator.MAX_SCORE:
			result = scores.max()
		ScoreOperator.MIN_SCORE:
			result = scores.min()
		
	match test_purpose:
		TestPurpose.FILTER_SCORE:
			if result == 0.0:
				projection.is_filtered = true
			else:
				projection.add_score(result)
		TestPurpose.FILTER_ONLY:
			if result == 0.0:
				projection.is_filtered = true
		TestPurpose.SCORE_ONLY:
			projection.add_score(result)
