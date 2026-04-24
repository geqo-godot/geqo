class_name Visibility3DTest
extends GdUnitTestSuite
@warning_ignore('unused_parameter')
@warning_ignore('return_value_discarded')

# TestSuite generated from
const __source: String = 'res://test/querier3d.gd'

var raycast_test: TestRaycastTo3D
var querier: Querier
var area_not_visible: Area3D

func before_test() -> void:
	var runner: GdUnitSceneRunner = scene_runner("res://test/visibility_test_scene3d.tscn")
	var q_resource: PackedScene = load("res://test/querier3d.tscn")
	querier = auto_free(q_resource.instantiate())
	var visible_spawn: Marker3D = runner.invoke("find_child", "VisibleSpawn")
	area_not_visible = runner.invoke("find_child", "AreaNotVisible")
	visible_spawn.add_child(querier)
	# Make a query that hides querier out of the player's sight
	var env_query: EnvironmentQuery3D = EnvironmentQuery3D.new()
	# Make debug shapes visible
	env_query.use_debug_shapes = true
	querier.add_child(env_query)
	querier.env_query = env_query
	# Add context for the test
	var test_context: ContextInGroup3D = ContextInGroup3D.new()
	test_context.group = "player"
	env_query.add_child(test_context)
	# Add the generator
	var grid_generator: GeneratorGridShape3D = GeneratorGridShape3D.new()
	env_query.add_child(grid_generator)
	# Add the test
	raycast_test = TestRaycastTo3D.new()
	grid_generator.add_child(raycast_test)
	raycast_test.context = test_context
	# Target ground and "player"
	raycast_test.collision_mask = 0b11

func test_not_visible() -> void:
	raycast_test.bool_match = false
	await await_idle_frame()
	querier.execute()
	await assert_signal(area_not_visible).wait_until(5000).is_emitted(area_not_visible.body_entered, [querier])

func test_visible() -> void:
	raycast_test.bool_match = true
	await await_idle_frame()
	querier.execute()
	await assert_signal(area_not_visible).wait_until(4000).is_not_emitted(area_not_visible.body_entered, [querier])
