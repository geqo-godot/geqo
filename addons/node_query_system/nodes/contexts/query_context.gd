## Provides a frame of reference for different query operations, like getting nodes inside a group, or targeting a specific node.
@abstract
class_name QueryContext
extends Node3D

@abstract
func get_context() -> Array[Variant]

## Some tests only require the positions of the contexts, and some contexts might only be positions
func get_context_positions() -> Array[Variant]:
    var contexts = get_context()
    var result: Array
    result.resize(contexts.size())
    
    for i in range(contexts.size()):
        if contexts[i] is Node2D or contexts[i] is Node3D:
            result[i] = contexts[i].global_position
            continue
        result[i] = contexts[i]
    
    return result
