#pragma once
#include "Definitions.h"
#include "libs/Recast/Include/Recast.h"

struct dtMeshTile;

enum SamplePartitionType
{
	SAMPLE_PARTITION_WATERSHED,
	SAMPLE_PARTITION_MONOTONE,
	SAMPLE_PARTITION_LAYERS
};

enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP
};

enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK = 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM = 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR = 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP = 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_DISABLED = 0x10,		// Disabled polygon
	SAMPLE_POLYFLAGS_ALL = 0xffff	// All abilities.
};

class NavigationModule final
{
protected:
	class InputGeom* m_geom;
	unsigned char* m_triareas;
	bool m_keepInterResults = true;

	float m_cellSize = 0.3f;
	float m_cellHeight= 0.2f;
	float m_agentHeight = 2.f;
	float m_agentRadius = 0.6f;
	float m_agentMaxClimb = 0.9f;
	float m_agentMaxSlope = 45;
	float m_regionMinSize = 8;
	float m_regionMergeSize = 20;
	float m_edgeMaxLen = 12;
	float m_edgeMaxError = 1.3f;
	float m_vertsPerPoly = 6;
	float m_detailSampleDist = 6;
	float m_detailSampleMaxError = 1;

	bool m_filterLowHangingObstacles = true;
	bool m_filterLedgeSpans = true;
	bool m_filterWalkableLowHeightSpans = true;

	rcCompactHeightfield* m_chf;
	int m_partitionType = SAMPLE_PARTITION_WATERSHED;

	rcContourSet* m_cset;
	rcPolyMesh* m_pmesh;
	rcPolyMeshDetail* m_dmesh;
	class dtNavMesh* m_navMesh;
	class dtNavMeshQuery* m_navQuery;

	static const int INVALID_NAVMESH_POLYREF;
	static const int MAX_POLYS;

	std::vector<FVector> GetNavPolyMeshPoints(const dtNavMesh& mesh, const dtNavMeshQuery* query,
		const dtMeshTile* tile, unsigned char flags) const;

public:
	static NavigationModule& getInstance();

	dtNavMesh* GetNavMesh();
	std::vector<std::vector<FVector>> GetNavMeshPoints() const;

	bool Build();

	bool FindStraightPath(FVector const & InStartPos, FVector const & InEndPos, std::vector<FVector>& OutPath);
	bool FindRandomPointAroundCircle(FVector const& InCenterPos, std::vector<FVector>& OutPoints, int InMaxPoints, float InMaxRadius);
	bool Raycast(FVector const& InStart, FVector const& InEnd, std::vector<FVector>& OuthitPointVec);

	rcConfig m_cfg;
	rcHeightfield* m_solid;

	~NavigationModule();
};

