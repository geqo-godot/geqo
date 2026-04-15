## This context implements the functionality of ContextInGroup3D, but with an exclusion filter in place.
class_name CustomContextInGroup3D extends QueryContext3D
@export var group: StringName
@export var exclusion: Node3D

func _get_context(_query_instance: QueryInstance3D) -> Array:
	return get_tree().get_nodes_in_group(group).filter(func(node): return node != exclusion)
