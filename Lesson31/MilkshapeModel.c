/*
	MilkshapeModel.cpp

		Loads and renders a Milkshape3D model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <stdio.h>

#include "MilkshapeModel.h"

/* 
	MS3D STRUCTURES 
*/

// byte-align structures
#ifdef _MSC_VER
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error you must byte-align these structures with the appropriate compiler directives
#endif

typedef unsigned char byte;
typedef unsigned short word;

// File header
struct MS3DHeader
{
	char m_ID[10];
	int m_version;
} PACK_STRUCT;

// Vertex information
struct MS3DVertex
{
	byte m_flags;
	float m_vertex[3];
	char m_boneID;
	byte m_refCount;
} PACK_STRUCT;

// Triangle information
struct MS3DTriangle
{
	word m_flags;
	word m_vertexIndices[3];
	float m_vertexNormals[3][3];
	float m_s[3], m_t[3];
	byte m_smoothingGroup;
	byte m_groupIndex;
} PACK_STRUCT;

// Material information
struct MS3DMaterial
{
    char m_name[32];
    float m_ambient[4];
    float m_diffuse[4];
    float m_specular[4];
    float m_emissive[4];
    float m_shininess;	// 0.0f - 128.0f
    float m_transparency;	// 0.0f - 1.0f
    byte m_mode;	// 0, 1, 2 is unused now
    char m_texture[128];
    char m_alphamap[128];
} PACK_STRUCT;

//	Joint information
struct MS3DJoint
{
	byte m_flags;
	char m_name[32];
	char m_parentName[32];
	float m_rotation[3];
	float m_translation[3];
	word m_numRotationKeyframes;
	word m_numTranslationKeyframes;
} PACK_STRUCT;

// Keyframe data
struct MS3DKeyframe
{
	float m_time;
	float m_parameter[3];
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

bool MilkshapeModel_LoadModelData( struct Model *pModel, const char *filename )
{
	FILE *fd = fopen(filename, "rb");
	if (!fd) return false;	// "Couldn't open the model file."

	fseek(fd, 0, SEEK_END);
	long fileSize = ftell(fd);
	fseek(fd, 0, SEEK_SET);

	byte *pBuffer = malloc(fileSize);
	fread(pBuffer, fileSize, 1, fd);
	fclose(fd);
	fd = NULL;

	const byte *pPtr = pBuffer;
	struct MS3DHeader *pHeader = ( struct MS3DHeader* )pPtr;
	pPtr += sizeof( struct MS3DHeader );

	if ( strncmp( pHeader->m_ID, "MS3D000000", 10 ) != 0 )
		return false; // "Not a valid Milkshape3D model file."

	if ( pHeader->m_version < 3 || pHeader->m_version > 4 )
		return false; // "Unhandled file version. Only Milkshape3D Version 1.3 and 1.4 is supported." );

	int nVertices = *( word* )pPtr; 
	pModel->m_numVertices = nVertices;
	pModel->m_pVertices = malloc(sizeof(struct Vertex) * nVertices);
	pPtr += sizeof( word );

	int i;
	for ( i = 0; i < nVertices; i++ )
	{
		struct MS3DVertex *pVertex = ( struct MS3DVertex* )pPtr;
		pModel->m_pVertices[i].m_boneID = pVertex->m_boneID;
		memcpy( pModel->m_pVertices[i].m_location, pVertex->m_vertex, sizeof( float )*3 );
		pPtr += sizeof( struct MS3DVertex );
	}

	int nTriangles = *( word* )pPtr;
	pModel->m_numTriangles = nTriangles;
	pModel->m_pTriangles = malloc(sizeof(struct Triangle) * nTriangles);
	pPtr += sizeof( word );

	for ( i = 0; i < nTriangles; i++ )
	{
		struct MS3DTriangle *pTriangle = ( struct MS3DTriangle* )pPtr;
		int vertexIndices[3];
		vertexIndices[0] = pTriangle->m_vertexIndices[0];
		vertexIndices[1] = pTriangle->m_vertexIndices[1];
		vertexIndices[2] = pTriangle->m_vertexIndices[2];
		float t[3];
		t[0] = 1.0f-pTriangle->m_t[0];
		t[1] = 1.0f-pTriangle->m_t[1];
		t[2] = 1.0f-pTriangle->m_t[2];
		memcpy( pModel->m_pTriangles[i].m_vertexNormals, pTriangle->m_vertexNormals, sizeof( float )*3*3 );
		memcpy( pModel->m_pTriangles[i].m_s, pTriangle->m_s, sizeof( float )*3 );
		memcpy( pModel->m_pTriangles[i].m_t, t, sizeof( float )*3 );
		memcpy( pModel->m_pTriangles[i].m_vertexIndices, vertexIndices, sizeof( int )*3 );
		pPtr += sizeof( struct MS3DTriangle );
	}

	int nGroups = *( word* )pPtr;
	pModel->m_numMeshes = nGroups;
	pModel->m_pMeshes = malloc(sizeof(struct Mesh) * nGroups);
	pPtr += sizeof( word );
	for ( i = 0; i < nGroups; i++ )
	{
		pPtr += sizeof( byte );	// flags
		pPtr += 32;				// name

		word nTriangles = *( word* )pPtr;
		pPtr += sizeof( word );
		int *pTriangleIndices = malloc(sizeof(int) * nTriangles);
		for ( int j = 0; j < nTriangles; j++ )
		{
			pTriangleIndices[j] = *( word* )pPtr;
			pPtr += sizeof( word );
		}

		char materialIndex = *( char* )pPtr;
		pPtr += sizeof( char );
	
		pModel->m_pMeshes[i].m_materialIndex = materialIndex;
		pModel->m_pMeshes[i].m_numTriangles = nTriangles;
		pModel->m_pMeshes[i].m_pTriangleIndices = pTriangleIndices;
	}

	int nMaterials = *( word* )pPtr;
	pModel->m_numMaterials = nMaterials;
	pModel->m_pMaterials = malloc(sizeof(struct Material) * nMaterials);
	pPtr += sizeof( word );
	for ( i = 0; i < nMaterials; i++ )
	{
		struct MS3DMaterial *pMaterial = ( struct MS3DMaterial* )pPtr;
		memcpy( pModel->m_pMaterials[i].m_ambient, pMaterial->m_ambient, sizeof( float )*4 );
		memcpy( pModel->m_pMaterials[i].m_diffuse, pMaterial->m_diffuse, sizeof( float )*4 );
		memcpy( pModel->m_pMaterials[i].m_specular, pMaterial->m_specular, sizeof( float )*4 );
		memcpy( pModel->m_pMaterials[i].m_emissive, pMaterial->m_emissive, sizeof( float )*4 );
		pModel->m_pMaterials[i].m_shininess = pMaterial->m_shininess;
		pModel->m_pMaterials[i].m_pTextureFilename = malloc(strlen( pMaterial->m_texture )+1);
		strcpy( pModel->m_pMaterials[i].m_pTextureFilename, pMaterial->m_texture );
		pPtr += sizeof( struct MS3DMaterial );
	}

	Model_ReloadTextures(pModel);

	free(pBuffer);

	return true;
}

