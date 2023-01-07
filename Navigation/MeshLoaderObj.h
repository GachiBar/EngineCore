#pragma once

#include <string>
#include <vector>

#include "Definitions.h"


class MeshLoaderObj
{
public:
	MeshLoaderObj();
	~MeshLoaderObj();

	bool load(const std::string& fileName);
	bool load(std::vector<FVector> const & InVerts, std::vector<uint32_t> const& InIndexes);

	bool loadTest();

	const float* getVerts() const { return m_verts; }
	const float* getNormals() const { return m_normals; }
	const int* getTris() const { return m_tris; }
	int getVertCount() const { return m_vertCount; }
	int getTriCount() const { return m_triCount; }
	const std::string& getFileName() const { return m_filename; }

	// Explicitly disabled copy constructor and copy assignment operator.
	MeshLoaderObj(const MeshLoaderObj&) = delete;
	MeshLoaderObj& operator=(const MeshLoaderObj&) = delete;

private:
	void addVertex(FVector vert, int& cap);

	void addVertex(float x, float y, float z, int& cap);
	void addTriangle(int a, int b, int c, int& cap);

	std::string m_filename;
	float m_scale;
	float* m_verts;
	int* m_tris;
	float* m_normals;
	int m_vertCount;
	int m_triCount;
};

