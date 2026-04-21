using Godot;

namespace GEQO;
public partial class QueryItem3D(RefCounted refCounted) : RefCounted
{
    public Node3D CollidedWith
    {
        get => (Node3D)(GodotObject)item.Call("get_collided_with");
        set => item.Call("set_collided_with", value);
    }
    public bool IsFiltered
    {
        get => (bool)item.Call("get_is_filtered");
        set => item.Call("set_is_filtered", value);
    }
    public Vector3 ProjectionPosition
    {
        get => (Vector3)item.Call("get_projection_position");
        set => item.Call("set_projection_position", value);
    }
    public float Score
    {
        get => (float)item.Call("get_score");
        set => item.Call("set_score", value);
    }
    private readonly RefCounted item = refCounted;
}
