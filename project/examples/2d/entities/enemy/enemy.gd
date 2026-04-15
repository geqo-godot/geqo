extends CharacterBody2D

const SPEED = 400

enum State {IDLE, WALKING}

var current_state: State = State.IDLE
var final_target: Vector2
var current_target

@onready var nav_agent: NavigationAgent2D = $NavigationAgent2D
@onready var env_query: EnvironmentQuery2D = $EnvironmentQuery2D


func _input(event: InputEvent) -> void:
	if event.is_action_pressed("request_query"):
		env_query.request_query()
		await env_query.query_finished
		var query_result: QueryResult2D = env_query.get_result()
		final_target = query_result.get_highest_score_position()
		if !final_target:
			return
		nav_agent.target_position = final_target
		current_target = nav_agent.get_next_path_position()


func _physics_process(_delta: float) -> void:
	match current_state:
		State.IDLE:
			idle()
		State.WALKING:
			walking()
	move_and_slide()


func move_to_target(target: Vector2):
	var direction: Vector2 = (self.global_position.direction_to(target))
	velocity.x = direction.x * SPEED
	velocity.y = direction.y * SPEED


func idle():
	velocity.x = lerp(velocity.x, 0.0, 0.25)
	velocity.y = lerp(velocity.y, 0.0, 0.25)
	if current_target:
		current_state = State.WALKING


func walking():
	if nav_agent.is_navigation_finished():
		current_state = State.IDLE
		return
	
	if !current_target:
		current_state = State.IDLE
		return
	current_target = nav_agent.get_next_path_position()
	move_to_target(current_target)


func _on_navigation_agent_2d_target_reached() -> void:
	current_target = null
