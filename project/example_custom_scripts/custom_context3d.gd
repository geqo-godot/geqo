class_name CustomContext3D extends QueryContext3D

func _get_context(query_instance: QueryInstance3D) -> Array:
	return [owner]
