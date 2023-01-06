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
