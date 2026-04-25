class_name QueryItem3DTest
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

var world: Node3D
var character: CharacterBody3D
var env_query: EnvironmentQuery3D
var array_generator: GeneratorInArray3D
var distance_test: TestDistanceTo3D
var test_nodes: Array[Node3D]

func test_create_initializes_all_fields() -> void:
	# Check that the 4 fields initialize correctly
	var node: Node3D = auto_free(Node3D.new())
	var pos: Vector3 = Vector3(1, 2, 3)
	var item: QueryItem3D = QueryItem3D.create(pos, node)
	assert_object(item.collided_with).is_same(node)
	assert_vector(item.projection_position).is_equal(pos)
	assert_float(item.score).is_equal(0.0)
	assert_bool(item.is_filtered).is_false()

func test_create_with_null_collider() -> void:
	# Make sure collided_with is null when it's supposed to
	var item: QueryItem3D = QueryItem3D.create(Vector3.ONE, null)
	assert_object(item.collided_with).is_null()

func test_property_roundtrip() -> void:
	# Make sure all the getters and setters are working
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	var node: Node3D = auto_free(Node3D.new())
	item.collided_with = node
	item.projection_position = Vector3.ONE
	item.score = 42.0
	item.is_filtered = true
	assert_object(item.collided_with).is_same(node)
	assert_vector(item.projection_position).is_equal(Vector3.ONE)
	assert_float(item.score).is_equal(42.0)
	assert_bool(item.is_filtered).is_true()

func test_apply_filter_boolean_truth_table() -> void:
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	# (value, expected, result, is_filtered)
	var cases = [
		[true,  true,  true,  false],
		[true,  false, false, true],
		[false, true,  false, true],
		[false, false, true,  false],
	]
	# Make sure each combination results in the correct output
	# apply_filter returns whether the item PASSED the filter (true = not filtered)
	for c in cases:
		item.set_is_filtered(false)
		var result: bool = item.apply_filter_boolean(c[0], c[1])
		assert_bool(result).is_equal(c[2])
		assert_bool(item.is_filtered).is_equal(c[3])

func test_filter_numeric_range() -> void:
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	# Check that it's not filtered if within range
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_RANGE, 5.0, 0.0, 10.0)).is_true()
	assert_bool(item.is_filtered).is_false()
	item.is_filtered = false
	# Check that it's filtered if outside range
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_RANGE, -1.0, 0.0, 10.0)).is_false()
	assert_bool(item.is_filtered).is_true()

func test_filter_numeric_range_boundaries() -> void:
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	# Check if the min/max boundaries are working
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_RANGE, 0.0, 0.0, 10.0)).is_true()
	item.is_filtered = false
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_RANGE, 10.0, 0.0, 10.0)).is_true()

func test_filter_numeric_min() -> void:
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	# Check if it's true above min
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_MIN, 5.0, 3.0, 0.0)).is_true()
	item.is_filtered = false
	# Check if it's false below min
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_MIN, 2.0, 3.0, 0.0)).is_false()

func test_filter_numeric_max() -> void:
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	# Check if it's true below max
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_MAX, 5.0, 0.0, 10.0)).is_true()
	item.is_filtered = false
	# Check if it's false above max
	assert_bool(item.apply_filter_numeric(GEQOEnums.FILTER_TYPE_MAX, 15.0, 0.0, 10.0)).is_false()

func test_add_score_direct_basic() -> void:
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	# Check if 0.5 * 2.0 is 1.0
	item.add_score_direct(GEQOEnums.PURPOSE_SCORE_ONLY, 0.5, 2.0)
	assert_float(item.score).is_equal(1.0)

func test_add_score_direct_accumulates() -> void:
	# Make sure the clamp makes the accumulate 2.0 and not 3.0
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_direct(GEQOEnums.PURPOSE_SCORE_ONLY, 1.0, 1.0)
	# Clamp will make 2.0 into 1.0
	item.add_score_direct(GEQOEnums.PURPOSE_SCORE_ONLY, 2.0, 1.0)
	assert_float(item.score).is_equal(2.0)

func test_score_only_does_not_filter() -> void:
	# Make sure add_score_boolean doesn't filter when PURPOSE_SCORE_ONLY is used
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_boolean(GEQOEnums.PURPOSE_SCORE_ONLY, true, false)
	assert_bool(item.is_filtered).is_false()

func test_filter_only_does_not_add_score() -> void:
	# Make sure add_score_direct doesn't add any score if PURPOSE_FILTER_ONLY
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_direct(GEQOEnums.PURPOSE_FILTER_ONLY, 1.0, 10.0)
	assert_float(item.score).is_equal(0.0)

func test_filter_score_does_both() -> void:
	# Make sure add_score_boolean does both if PURPOSE_FILTER_SCORE
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_boolean(GEQOEnums.PURPOSE_FILTER_SCORE, true, false)
	assert_bool(item.is_filtered).is_true()
	assert_float(item.score).is_greater_equal(0.0)

func test_add_score_boolean_match_increases_score() -> void:
	# Make sure PURPOSE_SCORE_ONLY increases the score in add_score_boolean if true
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_boolean(GEQOEnums.PURPOSE_SCORE_ONLY, true, true)
	assert_float(item.score).is_greater(0.0)

func test_add_score_boolean_mismatch_does_not_increase_score() -> void:
	# Make sure PURPOSE_SCORE_ONLY does not change score in add_score_boolean if false
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_boolean(GEQOEnums.PURPOSE_SCORE_ONLY, true, false)
	assert_float(item.score).is_equal(0.0)

func test_add_score_numeric_increases_score_in_range() -> void:
	# Verify that add_score_numeric increases the score if within range
	var item := QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_numeric(
		GEQOEnums.PURPOSE_SCORE_ONLY,
		GEQOEnums.FILTER_TYPE_RANGE,
		5.0,
		0.0,
		10.0
	)
	assert_float(item.score).is_greater(0.0)

func test_filter_is_sticky() -> void:
	# Make sure apply_filter_boolean doesn't unfilter a filtered item
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.apply_filter_boolean(true, false)
	assert_bool(item.is_filtered).is_true()
	item.apply_filter_boolean(true, true)
	assert_bool(item.is_filtered).is_true()

func test_filtered_item_score_behavior() -> void:
	# Make sure adding exactly 1 equals a score of 1
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.is_filtered = true
	item.add_score_direct(GEQOEnums.PURPOSE_SCORE_ONLY, 1.0, 1.0)
	assert_float(item.score).is_equal(1.0)

func test_add_score_direct_clamps_above_one() -> void:
	# Make sure the normalize value clamp works above 1
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_direct(GEQOEnums.PURPOSE_SCORE_ONLY, 5.0, 1.0)
	assert_float(item.score).is_equal(1.0)

func test_add_score_direct_clamps_below_zero() -> void:
	# Make sure the normalize value clamp works below 0
	var item: QueryItem3D = QueryItem3D.create(Vector3.ZERO, null)
	item.add_score_direct(GEQOEnums.PURPOSE_SCORE_ONLY, -1.0, 1.0)
	assert_float(item.score).is_equal(0.0)
