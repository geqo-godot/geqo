extends QueryContext3D
class_name ContextTargetArray

@export var targets: Array[Node3D]

func _get_context() -> Array:
	return targets
