class_name ContextInGroup
extends QueryContext

@export var group: StringName

func get_context() -> Array[Node3D]:
	# TODO: Figure out why the casting didn't work
	# return get_tree().get_nodes_in_group(the_group) as Array[Node3D]
	var result: Array[Node3D]
	
	for node in get_tree().get_nodes_in_group(group):
		result.append(node as Node3D)
	
	return result
