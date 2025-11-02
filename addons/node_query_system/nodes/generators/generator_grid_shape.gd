@tool
class_name GeneratorGridShape extends Generator

@export_group("Generator")
@export var grid_half_size: float = 20
@export var space_between: float = 5
@export var generate_around: QueryContext

@export_group("Projection Data")
@export var use_vertical_projection: bool = true
@export var project_down: float = 100.0
@export var project_up: float = 100.0
@export var post_projection_vertical_offset: float = 0.0
@export_flags_3d_physics var projection_collision_mask: int = 1

func perform_generation(query_item_list: Array[QueryItem]) -> void:
	var grid_size: int = roundi(grid_half_size * 2 / space_between) + 1
	var contexts: Array[Node3D] = generate_around.get_context()

	for context: Node3D in contexts:
		var starting_pos = context.global_position
		starting_pos.x -= grid_half_size
		starting_pos.z -= grid_half_size
		for z in grid_size:
			for x in grid_size:
				var pos_x: float = starting_pos.x + (x * space_between)
				var pos_z: float = starting_pos.z + (z * space_between)

				if not use_vertical_projection:
					query_item_list.append(QueryItem.new(Vector3(pos_x, context.global_position.y, pos_z)))
					continue

				var ray_pos: Vector3 = Vector3(pos_x, starting_pos.y, pos_z)
				
				var ray_result: Dictionary = cast_ray_projection(ray_pos + (Vector3.UP * project_up), ray_pos + (Vector3.DOWN * project_down), contexts)
				if ray_result:
					query_item_list.append(QueryItem.new(ray_result.position + Vector3.UP * post_projection_vertical_offset, ray_result.collider))
	

func cast_ray_projection(start_pos: Vector3, end_pos: Vector3, exclusions: Array) -> Dictionary:
	var space_state: PhysicsDirectSpaceState3D = get_world_3d().direct_space_state
	var query: PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(start_pos, end_pos, projection_collision_mask)
	var exclusion_rids: Array[RID] = []

	for exclusion in exclusions:
		exclusion_rids.append(exclusion.get_rid())
	query.exclude = exclusion_rids

	return space_state.intersect_ray(query)
