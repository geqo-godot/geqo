extends SpringArm3D
@export var target: Node3D

func _physics_process(_delta: float) -> void:
	if target:
		global_position = target.global_position + Vector3.UP * 2
