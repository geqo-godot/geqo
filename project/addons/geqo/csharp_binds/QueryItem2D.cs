using Godot;

public partial class QueryItem2D : RefCounted
{
    public Node2D collidedWith
    {
        get => (Node2D)item.Call("get_collided_with");
        set => item.Call("set_collided_with", value);
    }
    public bool isFiltered
    {
        get => (bool)item.Call("get_is_filtered");
        set => item.Call("set_is_filtered", value);
    }
    public Vector2 projectionPosition
    {
        get => (Vector2)item.Call("get_projection_position");
        set => item.Call("set_projection_position", value);
    }
    public float score
    {
        get => (float)item.Call("get_score");
        set => item.Call("set_score", value);
    }
    private readonly RefCounted item;
    public QueryItem2D(RefCounted refCounted)
    {
        item = refCounted;
    }
}
