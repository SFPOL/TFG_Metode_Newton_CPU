#pragma once

#include <vector>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

#include<iostream>
#include<sstream>
#include <tuple>

#include"Point.hh"
#include "Loader.h"

class PointCloud
{
private:
	std::vector<Point> vertices;
	unsigned nrOfVertices;

	int totalSize;

	float radius;
	float alphaError;
	float alphaGround;
	ivec3 size_grid;

	glm::vec3 uVoxelDimensions;
	glm::vec3 uMinVertex;
	glm::vec3 uMinVoxel;
	glm::vec3 uMaxVertex;
	glm::vec3 uMaxVoxel;

	GLuint id;


	GLuint ssbo;
	Point* point;

	//Variables de los 3 planos que rodean al pointCloud
	glm::vec3 planeOrigin;
	glm::vec3 range;
	float planeSize;
	float scalePlaneFactor;;


	
	void initVariables(glm::vec3 minVertex, glm::vec3 maxVertex, int totalSize, float scale)
	{
		//Inicialitzem el radi
		this->radius = 0.05;
		this->alphaError = 0.1;
		this->alphaGround = 0.3;

		//Variables comunes
		this->totalSize = totalSize;
		this->uMinVertex = minVertex;
		this->uMaxVertex = maxVertex;

		glm::vec3 objectSize = this->uMaxVertex - this->uMinVertex;
		float maxObjectSize = glm::max(glm::max(objectSize.x, objectSize.y), objectSize.z);

		float scalingFactor = totalSize * scale / maxObjectSize;
		

		for (Point& vertex : vertices)
		{
			vertex.position *= scalingFactor;

		}
		this->uMinVertex *= scalingFactor;
		this->uMaxVertex *= scalingFactor;

		objectSize = this->uMaxVertex - this->uMinVertex;

		//Variables pel Voxel
		// Establece las dimensiones del voxel
		this->uVoxelDimensions = vec3(scale);

		// Calcula el tamaño de la grid
		this->size_grid = glm::ivec3(glm::ceil(objectSize / scale));
   }


	
	
	void populateImage()
	{
		

		// Inicializar la imagen en negro
		std::vector<std::vector<std::vector<std::vector<GLubyte>>>> image(this->size_grid[2], std::vector<std::vector<std::vector<GLubyte>>>(this->size_grid[1], std::vector<std::vector<GLubyte>>(this->size_grid[0], std::vector<GLubyte>(4, 0))));
		std::vector<std::vector<std::vector<GLubyte>>> maskImage(size_grid[2], std::vector<std::vector<GLubyte>>(size_grid[1], std::vector<GLubyte>(size_grid[0], 0)));
		// Asignar el color rojo a los voxels donde hay un vértice
		for (const Point& vertex : this->vertices)
		{
			glm::ivec3 voxelCoords = glm::ivec3((vertex.position - this->uMinVertex) / this->uVoxelDimensions);
			voxelCoords = glm::clamp(voxelCoords, glm::ivec3(0), glm::ivec3(this->totalSize - 1));
			ivec3 colorInt = ivec3(vertex.color * 255.0f + 0.5f);
			image[voxelCoords.z][voxelCoords.y][voxelCoords.x][0] = colorInt.x;
			image[voxelCoords.z][voxelCoords.y][voxelCoords.x][1] = colorInt.y;
			image[voxelCoords.z][voxelCoords.y][voxelCoords.x][2] = colorInt.z;
			image[voxelCoords.z][voxelCoords.y][voxelCoords.x][3] = 50;

		}
		this->image = image;
	}

	void initSSBO()
	{
		glGenBuffers(1, &this->ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, this->ssbo);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Point), this->point, GL_DYNAMIC_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, this->ssbo);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}

	
	void initPointCloudAs3DTexture()
	{
		glGenTextures(1, &this->id);
		glBindTexture(GL_TEXTURE_3D, this->id);




		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		std::vector<GLubyte> flatImage(this->size_grid[0] * this->size_grid[1] * this->size_grid[2] * 4);
		for (int z = 0; z < this->size_grid[2]; z++)
		{
			for (int y = 0; y < this->size_grid[1]; y++)
			{
				for (int x = 0; x < this->size_grid[0]; x++)
				{
					for (int c = 0; c < 4; c++)
					{
						flatImage[((z * this->size_grid[1] + y) * this->size_grid[0] + x) * 4 + c] = this->image[z][y][x][c];
						
					}
				}
			}
		}

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, this->size_grid[0], this->size_grid[1], this->size_grid[2], 0, GL_RGBA, GL_UNSIGNED_BYTE, flatImage.data());

		
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_3D, 0);
	}


	void initPointCloudAsOctree()
	{
		std::vector<FlattenedNodeFloats> flattenedNodesFloats;
		std::vector<FlattenedNodeInts> flattenedNodesInts;
		std::tie(flattenedNodesFloats, flattenedNodesInts) = this->octree->getFlattenedNodes();
		std::vector<Point> flattenedPoints = this->octree->getFlattenedPoints();

		


		std::vector<FlattenedNodeIntsSend> flattenedNodesIntsToSend;
		for (int i = 0; i < flattenedNodesInts.size(); i++) {
			FlattenedNodeInts flattenedNodeInts = flattenedNodesInts[i];
			FlattenedNodeIntsSend flattenedNodeIntsSend;

			// Copiar la información de pointIndex y pointCount a la variable pointInfo
			flattenedNodeIntsSend.pointInfo = glm::ivec4(flattenedNodeInts.pointIndex, flattenedNodeInts.pointCount, flattenedNodeInts.depth, 0);

			// Copiar la información de los índices de los hijos a las variables childrenInfo1 y childrenInfo2
			flattenedNodeIntsSend.childrenInfo1 = glm::ivec4(flattenedNodeInts.childrenIndices[0], flattenedNodeInts.childrenIndices[1], flattenedNodeInts.childrenIndices[2], flattenedNodeInts.childrenIndices[3]);
			flattenedNodeIntsSend.childrenInfo2 = glm::ivec4(flattenedNodeInts.childrenIndices[4], flattenedNodeInts.childrenIndices[5], flattenedNodeInts.childrenIndices[6], flattenedNodeInts.childrenIndices[7]);

			// Añadir la estructura convertida al vector flattenedNodesIntsToSend
			flattenedNodesIntsToSend.push_back(flattenedNodeIntsSend);
		}

		// Crear y configurar el buffer de nodos aplanados de punto flotante
		glGenBuffers(1, &this->flattenedNodesBufferFloats);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedNodesBufferFloats);
		glBufferData(GL_TEXTURE_BUFFER, flattenedNodesFloats.size() * sizeof(FlattenedNodeFloats), flattenedNodesFloats.data(), GL_STATIC_DRAW);

		// Crear y configurar el buffer de nodos aplanados de punto entero
		glGenBuffers(1, &this->flattenedNodesBufferInts);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedNodesBufferInts);
		glBufferData(GL_TEXTURE_BUFFER, flattenedNodesIntsToSend.size() * sizeof(FlattenedNodeIntsSend), flattenedNodesIntsToSend.data(), GL_STATIC_DRAW);
		
		// Crear y configurar el buffer de puntos aplanados //sus posiciones
		glGenBuffers(1, &this->flattenedPointsBuffer);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedPointsBuffer);
		glBufferData(GL_TEXTURE_BUFFER, flattenedPoints.size() * sizeof(Point), flattenedPoints.data(), GL_STATIC_DRAW);

		
		// Crear y configurar la textura buffer para nodos aplanados
		
		// Crear y configurar la textura buffer para nodos aplanados de punto flotante
		glGenTextures(1, &this->flattenedNodesTextureFloats);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesTextureFloats);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, this->flattenedNodesBufferFloats);


		// Crear y configurar la textura buffer para nodos aplanados de punto entero
		glGenTextures(1, &this->flattenedNodesTextureInts);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesTextureInts);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32I, this->flattenedNodesBufferInts);

		// Crear y configurar la textura buffer para puntos aplanados
		
		glGenTextures(1, &this->flattenedPointsTexture);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedPointsTexture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, this->flattenedPointsBuffer);

		glBindTexture(GL_TEXTURE_BUFFER, 0);

	}

	void initPointCloudErrorAsOctree()
	{
		std::vector<FlattenedNodeFloats> flattenedNodesFloats;
		std::vector<FlattenedNodeInts> flattenedNodesInts;
		std::tie(flattenedNodesFloats, flattenedNodesInts) = this->octreeError->getFlattenedNodes();
		std::vector<Point> flattenedPoints = this->octreeError->getFlattenedPoints();




		std::vector<FlattenedNodeIntsSend> flattenedNodesIntsToSend;
		for (int i = 0; i < flattenedNodesInts.size(); i++) {
			FlattenedNodeInts flattenedNodeInts = flattenedNodesInts[i];
			FlattenedNodeIntsSend flattenedNodeIntsSend;

			// Copiar la información de pointIndex y pointCount a la variable pointInfo
			flattenedNodeIntsSend.pointInfo = glm::ivec4(flattenedNodeInts.pointIndex, flattenedNodeInts.pointCount, flattenedNodeInts.depth, 0);

			// Copiar la información de los índices de los hijos a las variables childrenInfo1 y childrenInfo2
			flattenedNodeIntsSend.childrenInfo1 = glm::ivec4(flattenedNodeInts.childrenIndices[0], flattenedNodeInts.childrenIndices[1], flattenedNodeInts.childrenIndices[2], flattenedNodeInts.childrenIndices[3]);
			flattenedNodeIntsSend.childrenInfo2 = glm::ivec4(flattenedNodeInts.childrenIndices[4], flattenedNodeInts.childrenIndices[5], flattenedNodeInts.childrenIndices[6], flattenedNodeInts.childrenIndices[7]);

			// Añadir la estructura convertida al vector flattenedNodesIntsToSend
			flattenedNodesIntsToSend.push_back(flattenedNodeIntsSend);
		}

		// Crear y configurar el buffer de nodos aplanados de punto flotante
		glGenBuffers(1, &this->flattenedNodesErrorBufferFloats);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedNodesErrorBufferFloats);
		glBufferData(GL_TEXTURE_BUFFER, flattenedNodesFloats.size() * sizeof(FlattenedNodeFloats), flattenedNodesFloats.data(), GL_STATIC_DRAW);

		// Crear y configurar el buffer de nodos aplanados de punto entero
		glGenBuffers(1, &this->flattenedNodesErrorBufferInts);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedNodesErrorBufferInts);
		glBufferData(GL_TEXTURE_BUFFER, flattenedNodesIntsToSend.size() * sizeof(FlattenedNodeIntsSend), flattenedNodesIntsToSend.data(), GL_STATIC_DRAW);

		// Crear y configurar el buffer de puntos aplanados //sus posiciones
		glGenBuffers(1, &this->flattenedPointsErrorBuffer);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedPointsErrorBuffer);
		glBufferData(GL_TEXTURE_BUFFER, flattenedPoints.size() * sizeof(Point), flattenedPoints.data(), GL_STATIC_DRAW);


		// Crear y configurar la textura buffer para nodos aplanados

		// Crear y configurar la textura buffer para nodos aplanados de punto flotante
		glGenTextures(1, &this->flattenedNodesErrorTextureFloats);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesErrorTextureFloats);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, this->flattenedNodesErrorBufferFloats);


		// Crear y configurar la textura buffer para nodos aplanados de punto entero
		glGenTextures(1, &this->flattenedNodesErrorTextureInts);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesErrorTextureInts);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32I, this->flattenedNodesErrorBufferInts);

		// Crear y configurar la textura buffer para puntos aplanados

		glGenTextures(1, &this->flattenedPointsErrorTexture);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedPointsErrorTexture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, this->flattenedPointsErrorBuffer);

		glBindTexture(GL_TEXTURE_BUFFER, 0);

	}

	void initPointCloudAsMultiOctree()
	{
		std::vector<FlattenedMultiNodeFloats> flattenedNodesFloats;
		std::vector<FlattenedMultiNodeInts> flattenedNodesInts;
		std::tie(flattenedNodesFloats, flattenedNodesInts) = this->multiOctree->getFlattenedNodesMultiOctree(2);
		std::vector<Point> flattenedPoints1 = this->multiOctree->getFlattenedPoints(0);
		std::vector<Point> flattenedPoints2 = this->multiOctree->getFlattenedPoints(1);
		//std::vector<Point> flattenedPoints3 = this->multiOctree->getFlattenedPoints(2);




		std::vector<FlattenedMultiNodeIntsSend2> flattenedNodesIntsToSend;
		for (int i = 0; i < flattenedNodesInts.size(); i++) {
			FlattenedMultiNodeInts flattenedMultiNodeInts = flattenedNodesInts[i];
			FlattenedMultiNodeIntsSend2 flattenedMultiNodeIntsSend2;

			// Copiar la información de pointIndex, pointCount, depends, leaf a las variables pointInfo
			for (int j = 0; j < 2; ++j) {
				glm::ivec4& pointInfo = j == 0 ? flattenedMultiNodeIntsSend2.pointInfo :
					flattenedMultiNodeIntsSend2.pointInfo1;
					//flattenedMultiNodeIntsSend2.pointInfo3;
				pointInfo.x = flattenedMultiNodeInts.octreeInfo[j].pointIndex;
				pointInfo.y = flattenedMultiNodeInts.octreeInfo[j].pointCount;
				pointInfo.z = flattenedMultiNodeInts.octreeInfo[j].depends ? 1 : 0;
				pointInfo.w = flattenedMultiNodeInts.octreeInfo[j].leaf ? 1 : 0;
			}

			// Copiar la información de los índices de los hijos a las variables childrenInfo1 y childrenInfo2
			flattenedMultiNodeIntsSend2.childrenInfo1 = glm::ivec4(flattenedMultiNodeInts.childrenIndices[0], flattenedMultiNodeInts.childrenIndices[1], flattenedMultiNodeInts.childrenIndices[2], flattenedMultiNodeInts.childrenIndices[3]);
			flattenedMultiNodeIntsSend2.childrenInfo2 = glm::ivec4(flattenedMultiNodeInts.childrenIndices[4], flattenedMultiNodeInts.childrenIndices[5], flattenedMultiNodeInts.childrenIndices[6], flattenedMultiNodeInts.childrenIndices[7]);

			// Añadir la estructura convertida al vector flattenedNodesIntsToSend2
			flattenedNodesIntsToSend.push_back(flattenedMultiNodeIntsSend2);
		}

		// Crear y configurar el buffer de nodos aplanados de punto flotante
		glGenBuffers(1, &this->flattenedMultiNodesBufferFloats);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedMultiNodesBufferFloats);
		glBufferData(GL_TEXTURE_BUFFER, flattenedNodesFloats.size() * sizeof(FlattenedMultiNodeFloats), flattenedNodesFloats.data(), GL_STATIC_DRAW);

		// Crear y configurar el buffer de nodos aplanados de punto entero
		glGenBuffers(1, &this->flattenedMultiNodesBufferInts);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedMultiNodesBufferInts);
		glBufferData(GL_TEXTURE_BUFFER, flattenedNodesIntsToSend.size() * sizeof(FlattenedMultiNodeIntsSend2), flattenedNodesIntsToSend.data(), GL_STATIC_DRAW);

		// Crear y configurar el buffer de puntos aplanados //sus posiciones
		glGenBuffers(1, &this->flattenedMultiPoints1Buffer);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedMultiPoints1Buffer);
		glBufferData(GL_TEXTURE_BUFFER, flattenedPoints1.size() * sizeof(Point), flattenedPoints1.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &this->flattenedMultiPoints2Buffer);
		glBindBuffer(GL_TEXTURE_BUFFER, this->flattenedMultiPoints2Buffer);
		glBufferData(GL_TEXTURE_BUFFER, flattenedPoints2.size() * sizeof(Point), flattenedPoints2.data(), GL_STATIC_DRAW);


		// Crear y configurar la textura buffer para nodos aplanados

		// Crear y configurar la textura buffer para nodos aplanados de punto flotante
		glGenTextures(1, &this->flattenedMultiNodesTextureFloats);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiNodesTextureFloats);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, this->flattenedMultiNodesBufferFloats);


		// Crear y configurar la textura buffer para nodos aplanados de punto entero
		glGenTextures(1, &this->flattenedMultiNodesTextureInts);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiNodesTextureInts);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32I, this->flattenedMultiNodesBufferInts);

		// Crear y configurar la textura buffer para puntos aplanados

		glGenTextures(1, &this->flattenedMultiPoints1Texture);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiPoints1Texture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, this->flattenedMultiPoints1Buffer);

		glGenTextures(1, &this->flattenedMultiPoints2Texture);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiPoints2Texture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGB32F, this->flattenedMultiPoints2Buffer);

		glBindTexture(GL_TEXTURE_BUFFER, 0);

	}


	void updateUniforms(Shader* shader)
	{



		//shader->setMat4fv(this->ModelMatrix, "ModelMatrix");
		shader->set1f(this->radius, "radius");
		shader->set1f(this->alphaGround, "alphaGround");
		shader->set1f(this->alphaError, "alphaError");
		shader->setVec3f(this->uVoxelDimensions, "uVoxelDimensions");
		shader->setVec3i(this->size_grid, "grid_dim");
		shader->setVec3f(this->uMinVertex, "uMinVertex");
		shader->setVec3f(this->uMaxVertex, "uMaxVertex");

		shader->setVec3f(this->planeOrigin, "planeOrigin");
		shader->set1f(this->planeSize, "planeSize");
		shader->setVec3f(this->range, "range");
	}




	void generateCheckerboardCubeVertices(int size) {
		for (int z = 0; z < size; z++) {
			for (int y = 0; y < size; y++) {
				for (int x = 0; x < size; x++) {
					int parity = (z + y + x) % 4;
					if (parity == 0) {
						Point point;
						point.position = glm::vec3(x, y, z);
						point.color = glm::vec3(x, y, z);
						point.normal = glm::vec3(x, y, z);
						this->vertices.push_back(point);
					}
				}
			}
		}
	}

public:
	
	PointCloud(int id, const std::string& filePath, int totalSize = 150, float scale = 0.5f, bool voxel = true)

	{
		this->id = id;
		//std::vector<glm::vec3> temp_vertices;
		glm::vec3 minVertex, maxVertex;

		// Determine file extension
		std::string extension = filePath.substr(filePath.find_last_of(".") + 1);

		if (extension == "obj")
		{
			if (!Loader::loadFromOBJFile(filePath, this->vertices, minVertex, maxVertex))
			{
				throw std::runtime_error("Error: Could not load OBJ file.");
			}
		}
		else if (extension == "ply")
		{
			if (!Loader::loadFromPLYFile(filePath, this->vertices, minVertex, maxVertex))
			{
				throw std::runtime_error("Error: Could not load PLY file.");
			}
		}
		else if (extension == "txt")
		{
			if (!Loader::loadFromtxtFile(filePath, this->vertices, minVertex, maxVertex))
			{
				throw std::runtime_error("Error: Could not load PLY file.");
			}
		}
		else
		{
			throw std::runtime_error("Error: Unsupported file type.");
		}
		
		//Iniciamos variables
		this->initVariables(minVertex, maxVertex, totalSize, scale);
		this->populateImage();
		this->initPointCloudAs3DTexture();

		for (int i = 0; i < vertices.size(); i++) {
			// Verificar si el vértice tiene un cero en la tercera componente de 'labels'
			if (vertices[i].labels.z == 0.0f) {
				// Añadir el vértice a la variable 'verticesError'
				verticesError.push_back(vertices[i]);

				// Eliminar el vértice de la lista 'vertices'
				vertices.erase(vertices.begin() + i);
				i--; // Descontar la posición porque el vector se acorta
			}
		}
		

		for (int i = 0; i < vertices.size(); i++) {
			// Verificar si el vértice tiene un cero en la tercera componente de 'labels'
			if (vertices[i].labels.x == 1.0f) {
				// Añadir el vértice a la variable 'verticesError'
				verticesExtra.push_back(vertices[i]);

				// Eliminar el vértice de la lista 'vertices'
				vertices.erase(vertices.begin() + i);
				i--; // Descontar la posición porque el vector se acorta
			}
		}

		/*this->octree = new Octree(this->uMinVertex, this->uMaxVertex, 8, this->vertices, 2);
		this->octreeError = new Octree(this->uMinVertex, this->uMaxVertex, 8, this->verticesError, 2);
		this->multiOctree = new MultiOctree(this->octreeError);
		//TODO arreglar
		
		this->multiOctree->mergeOctree(octree);*/

		this->octree = new Octree(this->uMinVertex, this->uMaxVertex, 8, this->vertices, 2);
		this->octreeError = new Octree(this->uMinVertex, this->uMaxVertex, 8, this->verticesError, 2);
		this->octreeExtra = new Octree(this->uMinVertex, this->uMaxVertex, 8, this->verticesExtra, 2);
		this->multiOctree = new MultiOctree(this->octree);


		this->multiOctree->mergeOctree(octreeError);
		this->multiOctree->mergeOctree(octreeExtra);

		this->initSSBO();
		//this->initPointCloudAsOctree();
		//this->initPointCloudErrorAsOctree();
		this->initPointCloudAsMultiOctree();
		std::vector<OctreeNode*> a = this->octree->getLeafNodes();

		//this->populateImage(this->vertices, totalSize);

		//this->initPointCloudAs3DTexture();
	}


	GLuint getID() const
	{
		return this->id;
	};


	void initPlanes(Shader* shader, float scaleFactor = 1.f)
	{
		this->scalePlaneFactor = scaleFactor;

		glm::vec3 center = (uMaxVertex + uMinVertex) / 2.0f;

		// Calcular el tamaño máximo de los ejes del objeto
		glm::vec3 objectSize = uMaxVertex - uMinVertex;
		float maxSize = glm::max(glm::max(objectSize.x, objectSize.y), objectSize.z);

		// Calcular el tamaño del plano cuadrado
		this->planeSize = maxSize * scaleFactor;

		// Calcular la posición de origen de cada plano
		glm::vec3 halfPlaneSize = glm::vec3(planeSize * 0.5f);
		this->planeOrigin = center - halfPlaneSize;

		// Calcular el rango (este valor puede no ser necesario si los planos ya son cuadrados)
		this->range = glm::vec3(planeSize);

		// Establecer los valores en el shader
		shader->setVec3f(this->planeOrigin, "planeOrigin");
		shader->set1f(this->planeSize, "planeSize");
		shader->setVec3f(this->range, "range");
	}

	void updatePlane(float scaleFactor)
	{
		this->scalePlaneFactor += scaleFactor;

		glm::vec3 center = (uMaxVertex + uMinVertex) / 2.0f;

		// Calcular el tamaño máximo de los ejes del objeto
		glm::vec3 objectSize = uMaxVertex - uMinVertex;
		float maxSize = glm::max(glm::max(objectSize.x, objectSize.y), objectSize.z);

		// Calcular el tamaño del plano cuadrado
		this->planeSize = maxSize * this->scalePlaneFactor;

		// Calcular la posición de origen de cada plano
		glm::vec3 halfPlaneSize = glm::vec3(planeSize * 0.5f);
		this->planeOrigin = center - halfPlaneSize;

		// Calcular el rango (este valor puede no ser necesario si los planos ya son cuadrados)
		this->range = glm::vec3(planeSize);
	}

	void bind3DTexture()
	{
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_3D, this->id);
	};

	void bindBuffer()
	{
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

		Point* p = (Point*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Point), GL_MAP_READ_BIT);

		if (p)
		{
			// Now you can access the data in the buffer
			std::cout << "Point Position: " << p->position.x << ", " << p->position.y << ", " << p->position.z << std::endl;
			// ...and so on for other attributes...

			// Don't forget to unmap the buffer when you're done
			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
		}

		// Unbind the buffer
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	};


	void bindBufferPoints()
	{
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedPointsTexture);
	};

	void bindBufferNodesFloat()
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesTextureFloats);
	};

	void bindBufferNodesInt()
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesTextureInts);
	};

	void bindBufferPointsError()
	{
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedPointsErrorTexture);
	};

	void bindBufferNodesErrorFloat()
	{
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesErrorTextureFloats);
	};

	void bindBufferNodesErrorInt()
	{
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedNodesErrorTextureInts);
	};

	void bindBufferMultiPoints1()
	{
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiPoints1Texture);
	};

	void bindBufferMultiPoints2()
	{
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiPoints2Texture);
	};

	void bindBufferMultiNodesFloat()
	{
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiNodesTextureFloats);
	};

	void bindBufferMultiNodesInt()
	{
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_BUFFER, this->flattenedMultiNodesTextureInts);
	};

	void unbind()
	{
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_3D, this->id);
	}


	~PointCloud()
	{

		delete[] this->vertexArray;
		
		//delete image


		glDeleteTextures(1, &this->id);
		glDeleteTextures(1, &this->flattenedPointsTexture);
		glDeleteTextures(1, &this->flattenedNodesTextureFloats);
		glDeleteTextures(1, &this->flattenedNodesTextureInts);
		glDeleteTextures(1, &this->flattenedPointsErrorTexture);
		glDeleteTextures(1, &this->flattenedNodesErrorTextureFloats);
		glDeleteTextures(1, &this->flattenedNodesErrorTextureInts);

		glDeleteTextures(1, &this->flattenedMultiNodesTextureFloats);
		glDeleteTextures(1, &this->flattenedMultiNodesTextureInts);
		glDeleteTextures(1, &this->flattenedMultiPoints1Texture);
		glDeleteTextures(1, &this->flattenedMultiPoints2Texture);
		delete this->octree;
		delete this->octreeError;
		delete this->octreeground;
		delete this->octreepredicted;

	}

	void update()
	{

	}

	void render(Shader* shader)
	{

		//Update uniforms
		this->updateUniforms(shader);

		shader->use();

		//Cleanup
		glBindVertexArray(0);
		glUseProgram(0);
		glActiveTexture(0);
		glBindTexture(GL_TEXTURE_BUFFER, 0);
	}

	float* getRadius()
	{
		return &this->radius;
	}

	float* getAlphaError()
	{
		return &this->alphaError;
	}

	float* getAlphaGround()
	{
		return &this->alphaGround;
	}

	void addRadius(const float& dt)
	{	
		float newRad = this->radius + dt;
		newRad = max(0.01f, min(newRad, 10.0f));
		this->radius = newRad;
		
	}

	vec3 getCenter() const
	{
		return (this->uMaxVertex + this->uMinVertex) / 2.0f;
	}
	

	void SetMinVertex(const glm::vec3& minVertex) {
		uMinVertex = minVertex;
	}

	void SetMaxVertex(const glm::vec3& maxVertex) {
		uMaxVertex = maxVertex;
	}

	const glm::vec3& GetMinVertex() const {
		return uMinVertex;
	}

	const glm::vec3& GetMaxVertex() const {
		return uMaxVertex;
	}

	
};
