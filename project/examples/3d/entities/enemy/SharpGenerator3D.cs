using Godot;
using System.Collections.Generic;
[Tool]
public partial class SharpGenerator3D : QueryGenerator3D
{
    [ExportGroup("QueryGenerator")]
    [Export]
    public float GridHalfSize = 20.0f;
    [Export]
    public float SpaceBetween = 5.0f;
    [Export]
    public Node3D generateAround;
    /// <summary>
    /// This wrapper is necessary because C# doesn't recognize QueryContext3D from GDExtension as the same as C# QueryContext3D.
    /// </summary>
    public QueryContextWrapper3D GenerateAround;

    [ExportGroup("Projection Data")]
    [Export]
    public bool UseVerticalProjection = true;
    [Export]
    public float ProjectDown = 100.0f;
    [Export]
    public float ProjectUp = 100.0f;
    [Export]
    public float PostProjectionVerticalOffset = 0.5f;
    [Export(PropertyHint.Layers3DPhysics)]
    public int ProjectionCollisionMask = 1;
    [Export]
    public bool UseShapeCast = false;
    [Export]
    public Shape3D Shape;

    public override void _EnterTree()
    {
        GenerateAround = new QueryContextWrapper3D(generateAround);
        base._EnterTree();
    }

    public override async void _PerformGeneration(QueryInstanceWrapper3D queryInstance)
    {
        GenerateAround ??= queryInstance.QuerierContext;

        int gridSize = Mathf.RoundToInt(GridHalfSize * 2 / SpaceBetween) + 1;

        var contexts = GenerateAround.GetContext(queryInstance);

        foreach (var context in contexts)
        {
            Vector3 startingPos;

            // Check if context is Vector3
            if (context.VariantType == Variant.Type.Vector3)
            {
                startingPos = context.AsVector3();
            }
            // Check if context is Node3D
            else if (context.AsGodotObject() is Node3D node3D)
            {
                startingPos = node3D.GlobalPosition;
            }
            else
            {
                continue;
            }

            startingPos.X -= GridHalfSize;
            startingPos.Z -= GridHalfSize;

            for (int z = 0; z < gridSize; z++)
            {
                for (int x = 0; x < gridSize; x++)
                {
                    float posX = startingPos.X + (x * SpaceBetween);
                    float posZ = startingPos.Z + (z * SpaceBetween);

                    if (UseVerticalProjection)
                    {
                        var rayPos = new Vector3(posX, startingPos.Y, posZ);
                        Godot.Collections.Dictionary rayResult = new();

                        if (UseShapeCast)
                        {
                            var dicts = CastShapeProjection(
                                rayPos + new Vector3(0, ProjectUp, 0),
                                rayPos + new Vector3(0, -ProjectDown, 0),
                                contexts,
                                Shape,
                                ProjectionCollisionMask);

                            if (dicts.Count > 0)
                            {
                                rayResult = dicts[0];
                            }
                        }
                        else
                        {
                            rayResult = CastRayProjection(
                                rayPos + new Vector3(0, ProjectUp, 0),
                                rayPos + new Vector3(0, -ProjectDown, 0),
                                contexts,
                                ProjectionCollisionMask);
                        }

                        if (rayResult.Count > 0)
                        {
                            Node3D collider = (Node3D)(GodotObject)rayResult.GetValueOrDefault("collider");
                            Vector3 castedPosition = (Vector3)rayResult.GetValueOrDefault("position", Vector3.Zero);

                            queryInstance.AddItem(QueryItemWrapper3D.Create(
                                castedPosition + new Vector3(0, PostProjectionVerticalOffset, 0),
                                collider));
                        }
                    }
                    else
                    {
                        queryInstance.AddItem(QueryItemWrapper3D.Create(
                            new Vector3(posX, startingPos.Y, posZ),
                            null));
                    }

                    // Check if time has ran out
                    if (!queryInstance.HasTimeLeft())
                    {
                        await ToSignal(GetTree(), SceneTree.SignalName.ProcessFrame);
                        queryInstance.RefreshTimer();
                    }
                }
            }
        }

        EmitSignal("generator_finished");
    }
}