using Godot;

[Tool]
public partial class SharpDistanceTo3D : QueryTest3D
{
	[Export]
	public Node3D distanceTo;
	/// <summary>
	/// This wrapper is necessary because C# doesn't recognize QueryContext3D from GDExtension as the same as C# QueryContext3D.
	/// </summary>
	public QueryContextWrapper3D DistanceTo;
	public override void _EnterTree()
	{
		DistanceTo = new QueryContextWrapper3D(distanceTo);
		Cost = 1.0f;
		TestType = GEQOEnums.TestType.Numeric;
		base._EnterTree();
	}

	public override async void _PerformTest(QueryInstanceWrapper3D queryInstance)
	{
		if (DistanceTo == null)
		{
			DistanceTo = queryInstance.QuerierContext;
			EndTest();
			return;
		}

		Vector3[] contextPositions = DistanceTo.GetContextPositions(queryInstance);
		if (contextPositions.IsEmpty())
		{
			EndTest();
			return;
		}

		// Pre-pass to get min/max if needed
		bool needsPrePass = ScoreClampMinType == GEQOEnums.ScoreClampType.None ||
						   ScoreClampMaxType == GEQOEnums.ScoreClampType.None;

		if (needsPrePass && !queryInstance.HasTestData(this))
		{
			float smallestItem = float.PositiveInfinity;
			float biggestItem = float.NegativeInfinity;

			while (queryInstance.HasItems())
			{
				var item = queryInstance.GetNextItem();
				if (item.IsFiltered)
					continue;

				float score = CalculateContextScore(item, contextPositions);
				if (score < smallestItem)
					smallestItem = score;
				if (score > biggestItem)
					biggestItem = score;
			}

			queryInstance.SetTestDataMin(this, smallestItem);
			queryInstance.SetTestDataMax(this, biggestItem);
			queryInstance.ResetIterator();
		}

		// Second pass
		float clampMin = GetEffectiveClampMin(queryInstance);
		float clampMax = GetEffectiveClampMax(queryInstance);

		while (queryInstance.HasItems())
		{
			QueryItemWrapper3D item = queryInstance.GetNextItem();
			if (item.IsFiltered)
				continue;

			float rawScore = CalculateContextScore(item, contextPositions);

			// Filter first
			if (TestPurpose == GEQOEnums.TestPurpose.FilterScore ||
				TestPurpose == GEQOEnums.TestPurpose.FilterOnly)
			{
				if (MultipleContextFilterOperator == GEQOEnums.MultipleContextFilterOp.AnyPass)
				{
					if (!EvaluateContextFilterAny(item, contextPositions))
					{
						item.IsFiltered = true;
						continue;
					}
				}
				else if (MultipleContextFilterOperator == GEQOEnums.MultipleContextFilterOp.AllPass)
				{
					if (!EvaluateContextFilterAll(item, contextPositions))
					{
						item.IsFiltered = true;
						continue;
					}
				}
				else
				{
					if (!item.ApplyFilterNumeric(FilterType, rawScore, FilterMin, FilterMax))
						continue;
				}
			}

			// Apply scoring
			if (TestPurpose == GEQOEnums.TestPurpose.FilterScore ||
				TestPurpose == GEQOEnums.TestPurpose.ScoreOnly)
			{
				float normalized = Mathf.Clamp(
					Mathf.Remap(rawScore, clampMin, clampMax, 0.0f, 1.0f),
					0.0f, 1.0f
				);
				float curveScore = ScoreCurve.Sample(normalized);
				item.AddScoreDirect(TestPurpose, curveScore, ScoreFactor);
			}

			// Check if time ran out
			if (!queryInstance.HasTimeLeft())
			{
				await ToSignal(GetTree(), SceneTree.SignalName.ProcessFrame);
				GD.Print("Awaited next frame");
				queryInstance.RefreshTimer();
			}
		}

		EndTest();
	}

	private bool EvaluateContextFilterAny(QueryItemWrapper3D item, Vector3[] contextPositions)
	{
		foreach (Vector3 pos in contextPositions)
		{
			float dist = item.ProjectionPosition.DistanceTo(pos);
			if (dist >= FilterMin && dist <= FilterMax)
				return true;
		}
		return false;
	}

	private bool EvaluateContextFilterAll(QueryItemWrapper3D item, Vector3[] contextPositions)
	{
		foreach (Vector3 pos in contextPositions)
		{
			float dist = item.ProjectionPosition.DistanceTo(pos);
			if (dist < FilterMin || dist > FilterMax)
				return false;
		}
		return true;
	}

	private float GetEffectiveClampMin(QueryInstanceWrapper3D queryInstance)
	{
		return ScoreClampMinType switch
		{
			GEQOEnums.ScoreClampType.Val => ScoreClampMin,
			GEQOEnums.ScoreClampType.Filter => FilterMin,
			_ => queryInstance.GetTestDataMin(this)
		};
	}

	private float GetEffectiveClampMax(QueryInstanceWrapper3D queryInstance)
	{
		return ScoreClampMaxType switch
		{
			GEQOEnums.ScoreClampType.Val => ScoreClampMax,
			GEQOEnums.ScoreClampType.Filter => FilterMax,
			_ => queryInstance.GetTestDataMax(this)
		};
	}

	private float CalculateContextScore(QueryItemWrapper3D item, Vector3[] contextPositions)
	{
		float sum = 0.0f;
		float smallest = float.PositiveInfinity;
		float biggest = float.NegativeInfinity;

		foreach (Vector3 pos in contextPositions)
		{
			float dist = item.ProjectionPosition.DistanceTo(pos);
			sum += dist;
			if (dist < smallest)
				smallest = dist;
			if (dist > biggest)
				biggest = dist;
		}

		float average = sum / contextPositions.Length;

		return MultipleContextScoreOperator switch
		{
			GEQOEnums.MultipleContextScoreOp.AverageScore => average,
			GEQOEnums.MultipleContextScoreOp.MinScore => smallest,
			GEQOEnums.MultipleContextScoreOp.MaxScore => biggest,
			_ => 0.0f
		};
	}
}
