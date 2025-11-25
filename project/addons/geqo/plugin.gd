@tool
extends EditorPlugin


func _enable_plugin() -> void:
	# Add autoloads here.
	add_autoload_singleton("GEQODebugSpheres", "./debug/debug_sphere.gd")


func _disable_plugin() -> void:
	# Remove autoloads here.
	remove_autoload_singleton("GEQODebugSpheres")


func _enter_tree() -> void:
	# Initialization of the plugin goes here.
	pass


func _exit_tree() -> void:
	# Clean-up of the plugin goes here.
	pass
