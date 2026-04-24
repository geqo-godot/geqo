extends Node3D
@onready var ray_cast_3d: RayCast3D = $RayCast3D
@onready var shape_cast_3d: ShapeCast3D = $ShapeCast3D
var target_querier: Querier

func _physics_process(delta: float) -> void:
	if not target_querier:
		return
	ray_cast_3d.target_position = target_querier.global_position
	shape_cast_3d.target_position = target_querier.global_position
