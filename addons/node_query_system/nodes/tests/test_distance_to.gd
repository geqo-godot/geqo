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
	var context_nodes: Array[Node3D] = distance_to.get_context()
	var scores: Array[float] = []

	for node: Node3D in context_nodes:
		var distance: float = projection.projection_position.distance_to(node.global_position)
		print_debug("Current distance to context: ", distance)
		
		var linear_score: float = (distance - min_distance) / (max_distance - min_distance)
		var clamped_score: float = clamp(linear_score, 0, 1)
		print_debug("The clamped score: ", linear_score)
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
		
	print_debug("Added score: ", result)
	projection.add_score(result)
