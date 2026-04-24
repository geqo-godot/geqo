class_name EnvironmentQuery3DTest
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

var world: Node3D
var character: CharacterBody3D
var environment_query: EnvironmentQuery3D
var grid_generator: GeneratorGridShape3D

func before_test() -> void:
	# Make a default environment query
	world = auto_free(Node3D.new())
	get_tree().root.add_child(world)
	# Dummy character for testing
	character = auto_free(CharacterBody3D.new())
	world.add_child(character)
	environment_query = auto_free(EnvironmentQuery3D.new())
	grid_generator = auto_free(GeneratorGridShape3D.new())
	character.add_child(environment_query)

func test_querier() -> void:
	# The query should have defaulted to character node when being added to scene
	assert_object(environment_query.querier).is_equal(character)

func test_request_query_no_generator() -> void:
	# request should fail if it has no generator child
	var result: bool = environment_query.request_query()
	assert_bool(result).is_false()

func test_request_query_with_generator() -> void:
	# Add a default GridGenerator3D
	grid_generator = auto_free(GeneratorGridShape3D.new())
	environment_query.add_child(grid_generator)
	var result: bool = environment_query.request_query()
	# request should now execute with a generator present
	assert_bool(result).is_true()

func test_request_query_repeated_request() -> void:
	# request should fail if still processing a query
	# Add a default GridGenerator3D
	grid_generator = auto_free(GeneratorGridShape3D.new())
	environment_query.add_child(grid_generator)
	# Make generator take a bit
	grid_generator.space_between = 0.5
	environment_query.request_query()
	var result: bool = environment_query.request_query()
	assert_bool(result).is_false()

func test_query_finished_exists() -> void:
	environment_query.add_child(grid_generator)
	# Make generator take a bit
	grid_generator.space_between = 0.5
	environment_query.request_query()
	assert_signal(environment_query).is_signal_exists(environment_query.query_finished)

func test_get_result() -> void:
	environment_query.add_child(grid_generator)
	# Make generator take a bit
	grid_generator.space_between = 0.5
	environment_query.request_query()
	await environment_query.query_finished
	# Try getting the cached result from query
	var result: QueryResult3D = environment_query.get_result()
	assert_object(result).is_not_null()
