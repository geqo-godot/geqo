extends SpringArm3D
@export var target: Node3D

func _physics_process(_delta: float) -> void:
	if target:
		global_position = target.global_position + Vector3.UP * 2

func _input(event: InputEvent) -> void:
	if event is InputEventMouseButton:
		if event.is_pressed():
			# zoom in
			if event.button_index == MOUSE_BUTTON_WHEEL_UP:
				spring_length = clamp(spring_length - 1, 5, 15)
			# zoom out
			if event.button_index == MOUSE_BUTTON_WHEEL_DOWN:
				spring_length = clamp(spring_length + 1, 5, 15)
