## Performs tests on a QueryItem, it can be a variety of operations, such as getting the distance between the Item and the context, or casting a ray to the context.
@abstract
@icon("../../icons/query_test3d.svg")
class_name QueryTest
extends Node3D

enum TestPurpose {FILTER_SCORE, FILTER_ONLY, SCORE_ONLY}
enum ScoreOperator {AVERAGE_SCORE, MAX_SCORE, MIN_SCORE}

@export var test_purpose: TestPurpose

@export_group("Score")
@export var multiple_context_operator: ScoreOperator

@abstract
func perform_test(projection: QueryItem)

## Helper function to sum a list of floats
func sum_f(accum: float, number: float) -> float:
	return accum + number
