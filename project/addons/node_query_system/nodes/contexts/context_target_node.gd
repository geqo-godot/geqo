extends QueryContext
class_name ContextTargetNode

@export var target_node: Node3D

func get_context() -> Array[Node3D]:
    return [target_node]