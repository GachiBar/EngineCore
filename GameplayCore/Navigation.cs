using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace GameplayCore
{
	public static class Navigation
	{
		public static void SetCellSize(float val)
		{
			Internal_SetCellSize(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetCellSize(float val);

		public static float GetCellSize()
		{
			return Internal_GetCellSize();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetCellSize();

		public static void SetCellHeight(float val)
		{
			Internal_SetCellHeight(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetCellHeight(float val);

		public static float GetCellHeight()
		{
			return Internal_GetCellHeight();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetCellHeight();

		public static void SetAgentHeight(float val)
		{
			Internal_SetAgentHeight(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetAgentHeight(float val);
		public static float GetAgentHeight()
		{
			return Internal_GetAgentHeight();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetAgentHeight();


		public static void SetAgentRadius(float val)
		{
			Internal_SetAgentRadius(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetAgentRadius(float val);

		public static float GetAgentRadius()
		{
			return Internal_GetAgentRadius();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetAgentRadius();

		public static void SetAgentMaxClimb(float val)
		{
			Internal_SetAgentMaxClimb(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetAgentMaxClimb(float val);

		public static float GetAgentMaxClimb()
		{
			return Internal_GetAgentMaxClimb();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetAgentMaxClimb();

		public static void SetAgentMaxSlope(float val)
		{
			Internal_SetAgentMaxSlope(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetAgentMaxSlope(float val);

		public static float GetAgentMaxSlope()
		{
			return Internal_GetAgentMaxSlope();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetAgentMaxSlope();

		public static void SetRegionMinSize(float val)
		{
			Internal_SetRegionMinSize(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetRegionMinSize(float val);

		public static float GetRegionMinSize()
		{
			return Internal_GetRegionMinSize();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetRegionMinSize();

		public static void SetEdgeMaxLen(float val)
		{
			Internal_SetEdgeMaxLen(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetEdgeMaxLen(float val);

		public static float GetEdgeMaxLen()
		{
			return Internal_GetEdgeMaxLen();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetEdgeMaxLen();

		public static void SetEdgeMaxError(float val)
		{
			Internal_SetEdgeMaxError(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetEdgeMaxError(float val);

		public static float GetEdgeMaxError()
		{
			return Internal_GetEdgeMaxError();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetEdgeMaxError();

		public static void SetVertsPerPoly(float val)
		{
			Internal_SetVertsPerPoly(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetVertsPerPoly(float val);

		public static float GetVertsPerPoly()
		{
			return Internal_GetVertsPerPoly();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetVertsPerPoly();

		public static void SetDetailSampleDist(float val)
		{
			Internal_SetDetailSampleDist(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetDetailSampleDist(float val);

		public static float GetDetailSampleDist()
		{
			return Internal_GetDetailSampleDist();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetDetailSampleDist();

		public static void SetDetailSampleMaxError(float val)
		{
			Internal_SetDetailSampleMaxError(val);
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SetDetailSampleMaxError(float val);

		public static float GetDetailSampleMaxError()
		{
			return Internal_GetDetailSampleMaxError();
		}
		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static float Internal_GetDetailSampleMaxError();

		public static bool Build()
		{
			return Internal_Build();
		}

		public static void SaveNavMesh(string NavMeshFilePath = "navmesh.bin")
		{
			Internal_SaveNavMesh(NavMeshFilePath);
		}

		public static void LoadNavMesh(string NavMeshFilePath = "navmesh.bin")
		{
			Internal_LoadNavMesh(NavMeshFilePath);
		}

		public static GameplayCore.Mathematics.Vector3[] FindStraightPath(GameplayCore.Mathematics.Vector3 InStartPos, GameplayCore.Mathematics.Vector3 InEndPos)
		{
			return Internal_FindStraightPath(InStartPos, InEndPos); ;
		}

		public static GameplayCore.Mathematics.Vector3[] FindRandomPointAroundCircle(GameplayCore.Mathematics.Vector3 InCenterPos, int InMaxPoints, float InMaxRadius)
		{
			return Internal_FindRandomPointAroundCircle(InCenterPos, InMaxPoints, InMaxRadius);
		}

		public static GameplayCore.Mathematics.Vector3[] Raycast(GameplayCore.Mathematics.Vector3 InStart, GameplayCore.Mathematics.Vector3 InEnd)
		{
			return Internal_Raycast(InStart, InEnd);
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static bool Internal_Build();

		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_SaveNavMesh(string NavMeshFilePath);

		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static void Internal_LoadNavMesh(string NavMeshFilePath);

		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static GameplayCore.Mathematics.Vector3[] Internal_FindStraightPath(GameplayCore.Mathematics.Vector3 InStartPos, GameplayCore.Mathematics.Vector3 InEndPos);

		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static GameplayCore.Mathematics.Vector3[] Internal_FindRandomPointAroundCircle(GameplayCore.Mathematics.Vector3 InCenterPos, int InMaxPoints, float InMaxRadius);

		[MethodImpl(MethodImplOptions.InternalCall)]
		extern private static GameplayCore.Mathematics.Vector3[] Internal_Raycast(GameplayCore.Mathematics.Vector3 InStartPos, GameplayCore.Mathematics.Vector3 InEndPos);
	}
}
