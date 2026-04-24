class_name QueryResult2DTest
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

var world: Node2D
var character: CharacterBody2D
var env_query: EnvironmentQuery2D
var array_generator: GeneratorInArray2D
var distance_test: TestDistanceTo2D
var test_nodes: Array[Node2D]

func before_test() -> void:
	world = auto_free(Node2D.new())
	get_tree().root.add_child(world)
	
	# Make 100 positions to be queried
	var nodes_amount: int = 100
	# Increment each of their distance
	var distance_increment: float = 32
	for i in range(nodes_amount):
		var new_node: Node2D = auto_free(Node2D.new())
		world.add_child(new_node)
		new_node.global_position.x = i * distance_increment
		test_nodes.append(new_node)
	
	# Dummy character for testing
	character = auto_free(CharacterBody2D.new())
	world.add_child(character)
	# Make a default environment query
	env_query = auto_free(EnvironmentQuery2D.new())
	character.add_child(env_query)
	# Put the nodes into a context
	var context: ContextTargetArray2D = auto_free(ContextTargetArray2D.new())
	env_query.add_child(context)
	context.target_nodes = test_nodes
	# Make generator that takes in all test nodes
	array_generator = auto_free(GeneratorInArray2D.new())
	env_query.add_child(array_generator)
	array_generator.context_array = context
	# Make distance test
	distance_test = auto_free(TestDistanceTo2D.new())
	array_generator.add_child(distance_test)
	distance_test.test_purpose = GEQOEnums.PURPOSE_SCORE_ONLY

func after_test() -> void:
	test_nodes.clear()

func test_has_result_true() -> void:
	# Test if has_result works as expected
	env_query.request_query()
	await env_query.query_finished
	assert_bool(env_query.get_result().has_result()).is_true()

func test_has_result_false() -> void:
	# If no score is set, it should not bring in any results
	distance_test.test_purpose = GEQOEnums.PURPOSE_FILTER_ONLY
	env_query.request_query()
	await env_query.query_finished
	assert_bool(env_query.get_result().has_result()).is_false()

func test_best_result_node() -> void:
	# The best node for this query should be the furthest one
	var best_node: Node2D = test_nodes.back()
	env_query.request_query()
	await env_query.query_finished
	var result: QueryResult2D = env_query.get_result()
	var result_node: Node2D = result.get_highest_score_node()
	assert_object(result_node).is_same(best_node)

func test_worst_result_node() -> void:
	# The worst node for this query should be the closest one
	var worst_node: Node2D = test_nodes.front()
	env_query.request_query()
	await env_query.query_finished
	var result: QueryResult2D = env_query.get_result()
	var result_node: Node2D = result.get_all_node().back()
	assert_object(result_node).is_same(worst_node)

func test_best_result_position() -> void:
	# The best position for this query should be the furthest one
	var best_position: Vector2 = test_nodes.back().global_position
	env_query.request_query()
	await env_query.query_finished
	var result: QueryResult2D = env_query.get_result()
	var result_position: Vector2 = result.get_highest_score_position()
	assert_vector(result_position).is_equal(best_position)

func test_worst_result_position() -> void:
	# The worst position for this query should be the closest one
	var worst_position: Vector2 = test_nodes.front().global_position
	env_query.request_query()
	await env_query.query_finished
	var result: QueryResult2D = env_query.get_result()
	var result_position: Vector2 = result.get_all_position().back()
	assert_vector(result_position).is_equal(worst_position)

func test_get_all_results() -> void:
	# Verify that the results are same size as nodes amount
	env_query.request_query()
	await env_query.query_finished
	var result: QueryResult2D = env_query.get_result()
	assert_int(test_nodes.size()).is_equal(result.get_all_results().size())
