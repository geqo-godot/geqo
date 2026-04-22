using Godot;
public partial class QueryItemWrapper2D(RefCounted refCounted) : RefCounted
{
    private readonly RefCounted refCounted = refCounted;
    public Node2D CollidedWith
    {
        get => (Node2D)(GodotObject)refCounted.Call("get_collided_with");
        set => refCounted.Call("set_collided_with", value);
    }
    public bool IsFiltered
    {
        get => (bool)refCounted.Call("get_is_filtered");
        set => refCounted.Call("set_is_filtered", value);
    }
    public Vector2 ProjectionPosition
    {
        get => (Vector2)refCounted.Call("get_projection_position");
        set => refCounted.Call("set_projection_position", value);
    }
    public float Score
    {
        get => (float)refCounted.Call("get_score");
        set => refCounted.Call("set_score", value);
    }
}
