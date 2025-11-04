## Tests the dot product between two contexts. Currently only supports 1 node3D contexts.
class_name TestDotProduct
extends QueryTest

@export_range(-1.0, 1.0, 0.05) var min_value: float = 0.5
@export_range(-1.0, 1.0, 0.05) var max_value: float = 1

@export var from_place: QueryContext
@export var to_target: QueryContext

func perform_test(projection: QueryItem):
	var context1: Node3D = from_place.get_context()[0]
	var context2: Node3D = to_target.get_context()[0]
	var line_1: Vector3 = - context1.global_transform.basis.z
	var context2_pos: Vector3 = context2.global_position

	var line_2: Vector3 = context1.global_position.direction_to(context2.global_position)
	var dot_product: float = line_1.dot(line_2)

	var score: float = 0.0
	if dot_product >= min_value and dot_product <= max_value:
		score = 1.0

	# print_debug("Dot Product: ", dot_product)
	match test_purpose:
		TestPurpose.FILTER_SCORE:
			if score > 0:
				projection.add_score(score)
			else:
				projection.is_filtered = true
		TestPurpose.FILTER_ONLY:
			if score <= 0:
				projection.is_filtered = true
		TestPurpose.SCORE_ONLY:
			projection.add_score(score)
