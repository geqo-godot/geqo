using Godot;

public partial class QueryGenerator3D : Node3D
{
    public enum RaycastModeEnum
    {
        Body,
        Area,
        BodyArea,
    }

    public RaycastModeEnum RaycastMode {  
        get => (RaycastModeEnum)(int)Call("get_raycast_mode");
        set => Call("set_raycast_mode", (int)value);
    }
    /// <summary>
    /// The GDExtension calls this function.
    /// </summary>
    void _perform_generation(RefCounted queryInstance)
    {
        QueryInstanceWrapper3D instance = new QueryInstanceWrapper3D(queryInstance);
        _PerformGeneration(instance);
    }

    public virtual void _PerformGeneration(QueryInstanceWrapper3D queryInstance)
    {
        GD.PrintErr("Need to override PerformTest in derived class");
    }

    public Godot.Collections.Dictionary CastRayProjection(Vector3 startPos, Vector3 endPos, Godot.Collections.Array exclusions, int colMask)
    {
        return (Godot.Collections.Dictionary)Call(Methods.CastRayProjection, startPos, endPos, exclusions, colMask);
    }

    public Godot.Collections.Array<Godot.Collections.Dictionary> CastShapeProjection(Vector3 startPos, Vector3 endPos, Godot.Collections.Array exclusions,Shape3D shape,  int colMask)
    {
        return (Godot.Collections.Array<Godot.Collections.Dictionary>)Call(Methods.CastRayProjection, startPos, endPos, exclusions, shape, colMask);
    }

    private static class Methods
    {
        public static readonly StringName CastRayProjection = "cast_ray_projection";
        public static readonly StringName CastShapeProjection = "cast_shape_projection";
    }
}
