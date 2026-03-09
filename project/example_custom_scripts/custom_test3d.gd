## This custom test mirrors the functionality of TestDistanceTo3D as example of how it would be implemented in GDScript.
class_name CustomDistanceTo3D extends QueryTest3D

@export var distance_to: QueryContext3D
@export var min_distance: float = 0.0
@export var max_distance: float = 10.0
@export var scoring_curve: Curve

func _ready() -> void:
	if not scoring_curve:
		scoring_curve = Curve.new()
		scoring_curve.add_point(Vector2(0, 0))
		scoring_curve.add_point(Vector2(1, 1))
	scoring_curve.bake()

func _perform_test(projection: QueryItem3D) -> void:
	if not distance_to:
		push_error("Test has no context")
		return
	var scores: Array[float] = []
	for context_pos: Vector3 in distance_to.get_context_positions():
		var distance: float = projection.projection_position.distance_to(context_pos)
		if test_purpose == TestPurpose.FILTER_SCORE or test_purpose == TestPurpose.FILTER_ONLY:
			if distance < min_distance or distance > max_distance:
				scores.append(0.0)
				continue
		var linear_score: float = (distance - min_distance) / (max_distance - min_distance)
		var clamped_score: float = clamp(linear_score, 0.0, 1.0)
		scores.append(scoring_curve.sample_baked(clamped_score))
	var result: float = 0.0
	match multiple_context_score_operator:
		ScoreOperator.AVERAGE_SCORE:
			result = scores.reduce(func(a: float, b: float): return a + b, 0.0) / scores.size()
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
