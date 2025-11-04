class_name TestRaycastTo
extends QueryTest

@export var context: QueryContext
## Whether hitting results in a positive value
@export var hitting_is_true: bool = true
## Start raycast from context's position instead
@export var cast_from_context: bool = false
@export var exclusions: Array[Node3D]
@export_flags_3d_physics var collision_mask: int = 1

func perform_test(projection: QueryItem):
	var context_nodes: Array[Node3D] = context.get_context()
	if context_nodes.size() > 1:
		return

	var space_state: PhysicsDirectSpaceState3D = get_world_3d().direct_space_state
	var start_pos: Vector3
	var end_pos: Vector3
	
	if cast_from_context:
		start_pos = context_nodes[0].global_position
		end_pos = projection.projection_position
	else:
		start_pos = projection.projection_position
		end_pos = context_nodes[0].global_position
		
	var query: PhysicsRayQueryParameters3D = PhysicsRayQueryParameters3D.create(start_pos, end_pos)
	query.collision_mask = collision_mask
	var exclusion_rids: Array[RID]
	for exclusion in exclusions:
		exclusion_rids.append(exclusion.get_rid())
	query.exclude = exclusion_rids

	var result = space_state.intersect_ray(query)
	
	var got_hit: bool = false
	if result.get("collider") == context_nodes[0]:
		got_hit = true
		#print_debug(context_nodes[0], " hit!")
		
	var filter: bool
	var score = 0.0

	if got_hit:
		filter = false if hitting_is_true else true
		score = 1.0 if hitting_is_true else 0.0
	else:
		filter = true if hitting_is_true else false
		score = 0.0 if hitting_is_true else 1.0
		

	match test_purpose:
		TestPurpose.FILTER_SCORE:
			projection.is_filtered = filter
			if !projection.is_filtered:
				projection.add_score(score)
		TestPurpose.FILTER_ONLY:
			projection.is_filtered = filter
		TestPurpose.SCORE_ONLY:
			projection.add_score(score)
