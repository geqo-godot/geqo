## Stores information about where the item is, and if it collided with anything, or got filtered.
class_name QueryItem
extends RefCounted

var projection_position: Vector3
var score = null
var collided_with: Node3D
var is_filtered = false

func _init(pos: Vector3, collider: Node3D = null, new_score = null) -> void:
    projection_position = pos
    collided_with = collider
    score = new_score
    
func add_score(incoming_score: float):
    if score is not float:
        score = 0.0
    score += incoming_score