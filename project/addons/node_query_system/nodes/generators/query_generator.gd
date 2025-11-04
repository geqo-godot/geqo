## Generates an array of QueryItems that can then be used to perform tests.
@tool
@icon("../../icons/generator3d.svg")
@abstract
class_name Generator
extends Node3D

enum RaycastMode {BODY, AREA, BODY_AREA}

@export_group("Projection Data")
@export var raycast_mode: RaycastMode
@export var frame_slice: int = 300

@abstract
func perform_generation(query_item_list: Array[QueryItem]) -> void

func perform_tests(query_item_list: Array[QueryItem]):
	var processed_count: int = 0
	for test: QueryTest in get_children():
		if frame_slice > 0 and processed_count >= frame_slice:
			await get_tree().process_frame
			processed_count = 0
		for query_item: QueryItem in query_item_list:
			if !query_item.is_filtered:
				test.perform_test(query_item)

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

	var has_invalid_node: bool = false
	var has_generator: bool = false

	for child in get_children():
		if child is Generator:
			has_generator = true
		elif child is not QueryTest:
			has_invalid_node = true

	if has_invalid_node:
		result.append("Generator has invalid children. Only QueryTest children are allowed")

	if has_generator:
		result.append("Cannot nest Generators on each other.")


	return result
