/*
	Model.cpp

		Abstract base class for a model. The specific extended class will render the given model. 

	Author:	Brett Porter
	Email: brettporter@yahoo.com
	Website: http://www.geocities.com/brettporter/
	Copyright (C)2000, Brett Porter. All Rights Reserved.

	This file may be used only as long as this copyright notice remains intact.
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library

#include "Model.h"
#include "Lesson31.h"

void Model_Create(struct Model *pModel)
{
	pModel->m_numMeshes = 0;
	pModel->m_pMeshes = NULL;
	pModel->m_numMaterials = 0;
	pModel->m_pMaterials = NULL;
	pModel->m_numTriangles = 0;
	pModel->m_pTriangles = NULL;
	pModel->m_numVertices = 0;
	pModel->m_pVertices = NULL;
}

void Model_Destroy(struct Model *pModel)
{
	int i;
	for ( i = 0; i < pModel->m_numMeshes; i++ )
		free(pModel->m_pMeshes[i].m_pTriangleIndices);
	for ( i = 0; i < pModel->m_numMaterials; i++ )
		free(pModel->m_pMaterials[i].m_pTextureFilename);

	pModel->m_numMeshes = 0;
	if ( pModel->m_pMeshes != NULL )
	{
		free(pModel->m_pMeshes);
		pModel->m_pMeshes = NULL;
	}

	pModel->m_numMaterials = 0;
	if ( pModel->m_pMaterials != NULL )
	{
		free(pModel->m_pMaterials);
		pModel->m_pMaterials = NULL;
	}

	pModel->m_numTriangles = 0;
	if ( pModel->m_pTriangles != NULL )
	{
		free(pModel->m_pTriangles);
		pModel->m_pTriangles = NULL;
	}

	pModel->m_numVertices = 0;
	if ( pModel->m_pVertices != NULL )
	{
		free(pModel->m_pVertices);
		pModel->m_pVertices = NULL;
	}
}

void Model_Draw(struct Model *pModel) 
{
	GLboolean texEnabled = glIsEnabled( GL_TEXTURE_2D );

	// Draw by group
	for ( int i = 0; i < pModel->m_numMeshes; i++ )
	{
		int materialIndex = pModel->m_pMeshes[i].m_materialIndex;
		if ( materialIndex >= 0 )
		{
			glMaterialfv( GL_FRONT, GL_AMBIENT, pModel->m_pMaterials[materialIndex].m_ambient );
			glMaterialfv( GL_FRONT, GL_DIFFUSE, pModel->m_pMaterials[materialIndex].m_diffuse );
			glMaterialfv( GL_FRONT, GL_SPECULAR, pModel->m_pMaterials[materialIndex].m_specular );
			glMaterialfv( GL_FRONT, GL_EMISSION, pModel->m_pMaterials[materialIndex].m_emissive );
			glMaterialf( GL_FRONT, GL_SHININESS, pModel->m_pMaterials[materialIndex].m_shininess );

			if ( pModel->m_pMaterials[materialIndex].m_texture > 0 )
			{
				glBindTexture( GL_TEXTURE_2D, pModel->m_pMaterials[materialIndex].m_texture );
				glEnable( GL_TEXTURE_2D );
			}
			else
				glDisable( GL_TEXTURE_2D );
		}
		else
		{
			// Material properties?
			glDisable( GL_TEXTURE_2D );
		}

		glBegin( GL_TRIANGLES );
		{
			for ( int j = 0; j < pModel->m_pMeshes[i].m_numTriangles; j++ )
			{
				int triangleIndex = pModel->m_pMeshes[i].m_pTriangleIndices[j];
				const struct Triangle* pTri = &pModel->m_pTriangles[triangleIndex];

				for ( int k = 0; k < 3; k++ )
				{
					int index = pTri->m_vertexIndices[k];

					glNormal3fv( pTri->m_vertexNormals[k] );
					glTexCoord2f( pTri->m_s[k], pTri->m_t[k] );
					glVertex3fv( pModel->m_pVertices[index].m_location );
				}
			}
		}
		glEnd();
	}

	if ( texEnabled )
		glEnable( GL_TEXTURE_2D );
	else
		glDisable( GL_TEXTURE_2D );
}

void Model_ReloadTextures(struct Model *pModel)
{
	for ( int i = 0; i < pModel->m_numMaterials; i++ )
		if ( strlen( pModel->m_pMaterials[i].m_pTextureFilename ) > 0 )
			pModel->m_pMaterials[i].m_texture = LoadGLTexture( pModel->m_pMaterials[i].m_pTextureFilename );
		else
			pModel->m_pMaterials[i].m_texture = 0;
}


