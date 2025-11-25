@tool
class_name GeneratorCircleShape
extends QueryGenerator

@export_group("QueryGenerator")
@export var circle_center: QueryContext
@export var circle_radius: float = 100
@export var space_between: float = 5

@export_range(0.0, 360.0, 0.5) var arc_angle: float = 360.0

@export_group("Cast Data")
@export var use_casting: bool = true
@export_flags_3d_physics var cast_collision_mask: int = 1


@export_group("Projection Data")
@export var use_vertical_projection: bool = false
@export var project_down: float = 100.0
@export var project_up: float = 100.0
@export var post_projection_vertical_offset: float = 0.0
@export_flags_3d_physics var projection_collision_mask: int = 1

func perform_generation(query_items_list: Array[QueryItem]) -> void:
	var contexts: Array[Variant] = circle_center.get_context()
	var points_amount: int = roundi(circle_radius / space_between)
	var processed_count: int = 0

	for context: Variant in contexts:
		var starting_pos: Vector3 = context if context is Vector3 else context.global_position
		var previous_angle: float = 0.0
		var angle_step: float = TAU / points_amount
		for point in points_amount:
			var pos_x = cos(previous_angle) * circle_radius + starting_pos.x
			var pos_z = sin(previous_angle) * circle_radius + starting_pos.z

			previous_angle += angle_step
			
			var final_pos = Vector3(pos_x, starting_pos.y, pos_z)
			if use_casting:
				var casted_ray: Dictionary = cast_ray_projection(starting_pos, final_pos, contexts, cast_collision_mask)
				if casted_ray:
					final_pos = casted_ray.position
				

			if not use_vertical_projection:
				query_items_list.append(QueryItem.new(final_pos))
				continue
		
			if frame_slice > 0 and processed_count >= frame_slice:
				await get_tree().process_frame
				processed_count = 0
			processed_count += 1
			var ray_pos: Vector3 = final_pos
			
			var ray_result: Dictionary = cast_ray_projection(ray_pos + Vector3.UP * project_up, ray_pos + Vector3.DOWN * project_down,
			contexts, projection_collision_mask)
			
			if ray_result:
				query_items_list.append(QueryItem.new(ray_result.position + Vector3.UP * post_projection_vertical_offset, ray_result.collider))


func cast_ray_projection(start_pos: Vector3, end_pos: Vector3, exclusions: Array, col_mask: int) -> Dictionary:
	var space_state: PhysicsDirectSpaceState3D = get_world_3d().direct_space_state
	var query: PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(start_pos, end_pos, col_mask)
	
	# Choose what to collide with
	if raycast_mode == RaycastMode.AREA:
		query.collide_with_bodies = false
	if raycast_mode in [RaycastMode.AREA, RaycastMode.BODY_AREA]:
		query.collide_with_areas = true
	
	var exclusion_rids: Array[RID] = []

	for exclusion in exclusions:
		if exclusion is not Node:
			continue
		exclusion_rids.append(exclusion.get_rid())
	query.exclude = exclusion_rids

	return space_state.intersect_ray(query)
