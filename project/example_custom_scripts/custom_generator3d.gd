class_name CustomGenerator3D extends QueryGenerator3D

@export_group("QueryGenerator")
@export var grid_half_size: float = 20.0
@export var space_between: float = 5.0
@export var generate_around: QueryContext3D
@export_group("Projection Data")
@export var use_vertical_projection: bool = true
@export var project_down: float = 100.0
@export var project_up: float = 100.0
@export var post_projection_vertical_offset = 0.5
@export_flags_3d_physics var projection_collision_mask: int = 1
@export var use_shape_cast: bool = false
@export var shape: Shape3D

func _perform_generation(query_instance: QueryInstance3D) -> void:
	if not generate_around:
		generate_around = query_instance.querier_context
	
	var grid_size: int = roundi(grid_half_size * 2 / space_between) + 1
	
	var contexts: Array = generate_around.get_context(query_instance)
	
	for context in contexts:
		var starting_pos: Vector3
		if context is Vector3:
			starting_pos = context
		elif context is Node3D:
			starting_pos = context.global_position
		else:
			continue
		
		starting_pos.x -= grid_half_size
		starting_pos.z -= grid_half_size
		
		for z: int in range(grid_size):
			for x: int in range(grid_size):
				var pos_x: float = starting_pos.x + (x * space_between)
				var pos_z: float = starting_pos.z + (z * space_between)
				
				if use_vertical_projection:
					var ray_pos: Vector3 = Vector3(pos_x, starting_pos.y, pos_z)
					var ray_result: Dictionary
					if use_shape_cast:
						var dicts: Array[Dictionary] = cast_shape_projection(
							ray_pos + (Vector3(0, project_up, 0)),
							ray_pos + (Vector3(0, -project_down, 0)),contexts, shape, projection_collision_mask)
						if not dicts.is_empty():
							ray_result = dicts[0]
					else:
						ray_result = cast_ray_projection(
							ray_pos + (Vector3(0, project_up, 0)),
							ray_pos + (Vector3(0, -project_down, 0)), contexts, projection_collision_mask)
					
					if not ray_result.is_empty():
						var collider: Node3D = ray_result.get("collider")
						var casted_position: Vector3
						casted_position = ray_result.get("position", Vector3())
						query_instance.add_item(QueryItem3D.create(casted_position + Vector3(0, post_projection_vertical_offset, 0), collider))
				else:
					query_instance.add_item(QueryItem3D.create(Vector3(pos_x, starting_pos.y, pos_z), null))
				
				# Check if time has ran out
				if not query_instance.has_time_left():
					await get_tree().process_frame
					query_instance.refresh_timer()
	generator_finished.emit()
