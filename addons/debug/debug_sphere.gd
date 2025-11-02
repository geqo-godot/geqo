extends Node3D

var immediate_mesh: ImmediateMesh
var mesh_instance: MeshInstance3D
var text_labels: Array
var debug_color: Gradient

func _ready():
    mesh_instance = MeshInstance3D.new()
    immediate_mesh = ImmediateMesh.new()
    mesh_instance.cast_shadow = GeometryInstance3D.SHADOW_CASTING_SETTING_OFF
    mesh_instance.mesh = immediate_mesh

    # Create unlit material for debug lines
    var material = StandardMaterial3D.new()
    material.shading_mode = BaseMaterial3D.SHADING_MODE_UNSHADED
    # this makes 'surface_set_color' work
    material.vertex_color_use_as_albedo = true
    mesh_instance.material_override = material

    add_child(mesh_instance)
    
    debug_color = Gradient.new()
    debug_color.colors = PackedColorArray([Color.RED, Color.YELLOW, Color.GREEN])
    debug_color.offsets = PackedFloat32Array([0.0, 0.5, 1.0])
    debug_color.interpolation_mode = Gradient.GRADIENT_INTERPOLATE_CUBIC

func draw_items(query_items_list: Array[QueryItem], time_to_destroy: float = 2.0) -> void:
    # TODO: MAX_MESH_SURFACES prevents drawing more circles
    immediate_mesh.clear_surfaces()
    remove_labels()
    for item: QueryItem in query_items_list:
        var text_label: Label3D = Label3D.new()
        text_labels.append(text_label)
        get_tree().root.add_child(text_label)
        text_label.set_deferred("billboard", BaseMaterial3D.BILLBOARD_ENABLED)
        text_label.set_deferred("position", Vector3.UP * 0.75)

        if item.is_filtered:
            text_label.set_deferred("text", "Filtered")
            draw_debug_sphere(item.projection_position, 0.5, Color.BLUE)
        else:
            if item.score is float:
                text_label.set_deferred("text", "%0.2f" % item.score)
                draw_debug_sphere(item.projection_position, 0.5, debug_color.sample(item.score))
            else:
                draw_debug_sphere(item.projection_position, 0.5, Color.CYAN)

        text_label.set_deferred("global_position", item.projection_position + Vector3.UP * 0.6)
func remove_labels():
    for label: Label3D in text_labels:
        label.queue_free()
    text_labels.clear()

func draw_debug_sphere(pos: Vector3, radius: float, color: Color, rings: int = 4, segments: int = 8):
    immediate_mesh.surface_begin(Mesh.PRIMITIVE_LINES)

	# Horizontal rings (latitude lines)
    for ring in range(rings):
        var phi = PI * float(ring) / float(rings)

        for seg in range(segments):
            var theta = TAU * float(seg) / float(segments)
            var theta_next = TAU * float(seg + 1) / float(segments)

            # Current ring
            var p1 = _sphere_point(radius, phi, theta) + pos
            var p2 = _sphere_point(radius, phi, theta_next) + pos

            immediate_mesh.surface_set_color(color)
            immediate_mesh.surface_add_vertex(p1)
            immediate_mesh.surface_add_vertex(p2)

	# Vertical segments (longitude lines)
    for seg in range(segments):
        var theta = TAU * float(seg) / float(segments)

        for ring in range(rings):
            var phi = PI * float(ring) / float(rings)
            var phi_next = PI * float(ring + 1) / float(rings)

            var p1 = _sphere_point(radius, phi, theta) + pos
            var p2 = _sphere_point(radius, phi_next, theta) + pos

            immediate_mesh.surface_set_color(color)
            immediate_mesh.surface_add_vertex(p1)
            immediate_mesh.surface_add_vertex(p2)

    immediate_mesh.surface_end()

func _sphere_point(radius: float, phi: float, theta: float) -> Vector3:
    return Vector3(
		radius * sin(phi) * cos(theta),
		radius * cos(phi),
		radius * sin(phi) * sin(theta)
	)
