## Uses generators and tests to gather data about the environment.
@tool
@icon("./icons/environment_query.svg")
class_name EnvironmentQuery
extends Node

signal query_finished(result: QueryResult)

## Visualize each item with spheres
@export var use_debug_shapes: bool = false

var query_items: Array[QueryItem]

func _ready() -> void:
	if Engine.is_editor_hint():
		return
	init_generator()

func init_generator():
	query_items = []

## Get query as a return or a signal
func request_query() -> QueryResult:
	query_items.clear()
	var result: QueryResult

	for generator: QueryGenerator in get_children():
		await generator.perform_generation(query_items)
		await generator.perform_tests(query_items)

	if use_debug_shapes:
		draw_debug(query_items)

	result = QueryResult.new(query_items)

	query_finished.emit(result)
	return result


## Call Debug to draw the items
func draw_debug(query_items_list: Array[QueryItem]):
	GEQODebugSpheres.draw_items(query_items_list)


func _notification(what: int) -> void:
	if !Engine.is_editor_hint():
		return
	match what:
		NOTIFICATION_CHILD_ORDER_CHANGED:
			update_configuration_warnings()
		NOTIFICATION_ENTER_TREE:
			update_configuration_warnings()

func _get_configuration_warnings() -> PackedStringArray:
	var result: Array = []

	if get_children().is_empty():
		result.append("Query needs at least one QueryGenerator node.")
	else:
		var has_invalid_node: bool = false
		for child in get_children():
			if child is not QueryGenerator:
				has_invalid_node = true
				break
		if has_invalid_node:
			result.append("EnvironmentQuery can only have QueryGenerator children.")
		if !check_has_tests():
			result.append("Query needs at least one test on any QueryGenerator")


	return result

# Recursively check if there are any tests at all
func check_has_tests() -> bool:
	var has_test: bool = false

	var nodes: Array = get_all_children(self)

	for node in nodes:
		if node is QueryTest:
			has_test = true
			break

	return has_test


func get_all_children(node) -> Array:
	var nodes: Array = []

	for n in node.get_children():
		if n.get_child_count() > 0:
			nodes.append(n)
			nodes.append_array(get_all_children(n))
		else:
			nodes.append(n)

	return nodes

## EnvironmentQuery sends result and has some functions to quickly get a return value
class QueryResult extends RefCounted:
	var query_items: Array[QueryItem]
	# TODO: Maybe a function to get copy of the results
	func _init(val: Array[QueryItem]):
		query_items = val

	func get_highest_score_position() -> Vector3:
		var highest: float
		var highest_position: Vector3
		for item: QueryItem in query_items:
			if item.is_filtered:
				continue
			if highest == null:
				highest = item.score
				highest_position = item.projection_position
			else:
				if item.score > highest:
					highest = item.score
					highest_position = item.projection_position
		return highest_position
