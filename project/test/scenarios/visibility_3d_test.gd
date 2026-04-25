class_name Visibility3DTest
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

# TestSuite generated from
const __source: String = 'res://test/querier3d.gd'

var raycast_test: TestRaycastTo3D
var querier: Querier
var ray_cast: RayCast3D
var grid_generator: GeneratorGridShape3D
var shape_cast: ShapeCast3D

func before_test() -> void:
	var runner: GdUnitSceneRunner = scene_runner("res://test/visibility_test_scene3d.tscn")
	var q_resource: PackedScene = load("res://test/querier3d.tscn")
	querier = auto_free(q_resource.instantiate())
	runner.scene().target_querier = querier
	var visible_spawn: Marker3D = runner.invoke("find_child", "VisibleSpawn")
	ray_cast = runner.invoke("find_child", "RayCast3D")
	shape_cast = runner.invoke("find_child", "ShapeCast3D")
	visible_spawn.add_child(querier)
	# Make a query that hides querier out of the player's sight
	var env_query: EnvironmentQuery3D = auto_free(EnvironmentQuery3D.new())
	# Make debug shapes visible
	env_query.use_debug_shapes = true
	querier.add_child(env_query)
	querier.env_query = env_query
	# Add context for the test
	var test_context: ContextInGroup3D = ContextInGroup3D.new()
	test_context.group = "player"
	env_query.add_child(test_context)
	# Add the generator
	grid_generator = auto_free(GeneratorGridShape3D.new())
	env_query.add_child(grid_generator)
	grid_generator.grid_half_size = 10
	# Add the test
	raycast_test = auto_free(TestRaycastTo3D.new())
	grid_generator.add_child(raycast_test)
	raycast_test.context = test_context
	# Target ground and "player"
	raycast_test.collision_mask = 0b11

func test_not_visible() -> void:
	raycast_test.bool_match = false
	await await_idle_frame()
	querier.execute()
	@warning_ignore("redundant_await")
	await assert_signal(querier).wait_until(5000).is_emitted(querier.reached_destination)
	var collider = ray_cast.get_collider()
	var hit_querier: bool = (collider == querier)
	assert_bool(hit_querier).is_false()

func test_visible() -> void:
	raycast_test.bool_match = true
	await await_idle_frame()
	querier.execute()
	@warning_ignore("redundant_await")
	await assert_signal(querier).wait_until(5000).is_emitted(querier.reached_destination)
	var collider = ray_cast.get_collider()
	var hit_querier: bool = (collider == querier)
	assert_bool(hit_querier).is_true()

func test_not_visible_with_shape() -> void:
	grid_generator.grid_half_size = 20
	raycast_test.bool_match = false
	raycast_test.use_shape_cast = true
	raycast_test.shape = SphereShape3D.new()
	shape_cast.shape = raycast_test.shape
	await await_idle_frame()
	querier.execute()
	@warning_ignore("redundant_await")
	await assert_signal(querier).wait_until(5000).is_emitted(querier.reached_destination)
	var collider = shape_cast.get_collider(0)
	var hit_querier: bool = (collider == querier)
	assert_bool(hit_querier).is_false()

func test_visible_with_shape() -> void:
	raycast_test.bool_match = true
	raycast_test.use_shape_cast = true
	raycast_test.shape = SphereShape3D.new()
	shape_cast.shape = raycast_test.shape
	await await_idle_frame()
	querier.execute()
	@warning_ignore("redundant_await")
	await assert_signal(querier).wait_until(5000).is_emitted(querier.reached_destination)
	var collider = shape_cast.get_collider(0)
	var hit_querier: bool = (collider == querier)
	assert_bool(hit_querier).is_true()
