extends CharacterBody3D

const JUMP_VELOCITY = 4
const SPEED = 500

enum State {IDLE, WALKING}

var current_state: State = State.IDLE
var final_target: Vector3
var current_target

@onready var nav_agent: NavigationAgent3D = $NavigationAgent3D

func _input(event: InputEvent) -> void:
	if event.is_action_pressed("request_query"):
		$EnvironmentQuery.request_query()
		var query_result: EnvironmentQuery.QueryResult = await $EnvironmentQuery.query_finished
		final_target = query_result.get_highest_score_position()
		if !final_target:
			return
		nav_agent.target_position = final_target
		current_target = nav_agent.get_next_path_position()


func _physics_process(delta: float) -> void:
	if not is_on_floor():
		velocity += get_gravity() * delta

	match current_state:
		State.IDLE:
			idle()
		State.WALKING:
			walking(delta)
	move_and_slide()


func move_to_target(delta: float, target: Vector3):
	var direction: Vector3 = (self.global_position.direction_to(target))
	velocity.x = direction.x * SPEED * delta
	velocity.z = direction.z * SPEED * delta


func idle():
	velocity.x = lerp(velocity.x, 0.0, 0.25)
	velocity.z = lerp(velocity.z, 0.0, 0.25)
	if current_target:
		current_state = State.WALKING


func walking(delta: float):
	if !current_target:
		current_state = State.IDLE
		return
	current_target = nav_agent.get_next_path_position()
	move_to_target(delta, current_target)


func _on_navigation_agent_3d_target_reached() -> void:
	current_target = null
