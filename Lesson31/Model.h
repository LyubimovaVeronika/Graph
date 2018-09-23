/*
	Model.h

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>

//	Mesh
struct Mesh
{
	int m_materialIndex;
	int m_numTriangles;
	int *m_pTriangleIndices;
};

//	Material properties
struct Material
{
	float m_ambient[4], m_diffuse[4], m_specular[4], m_emissive[4];
	float m_shininess;
	GLuint m_texture;
	char *m_pTextureFilename;
};

//	Triangle structure
struct Triangle
{
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	int m_vertexIndices[3];
};

//	Vertex structure
struct Vertex
{
	char m_boneID;	// for skeletal animation
	float m_location[3];
};

struct Model
{
	// ? protected:
	//	Meshes used
	int m_numMeshes;
	struct Mesh *m_pMeshes;

	//	Materials used
	int m_numMaterials;
	struct Material *m_pMaterials;

	//	Triangles used
	int m_numTriangles;
	struct Triangle *m_pTriangles;

	//	Vertices Used
	int m_numVertices;
	struct Vertex *m_pVertices;
};

void Model_Create(struct Model *pModel);
void Model_Destroy(struct Model *pModel);
void Model_Draw(struct Model *pModel);
void Model_ReloadTextures(struct Model *pModel);

#endif // ndef MODEL_H
