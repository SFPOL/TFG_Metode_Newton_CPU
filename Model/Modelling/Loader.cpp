#ifndef LOADER_H
#define LOADER_H

#include "Loader.h"
#include <unordered_map>

bool Loader::loadFromtxtFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex)
{
	std::ifstream file(filePath, std::ios::in);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open txt file: " << filePath << std::endl;
		return false;
	}
	bool firstVertex = true;
	glm::vec3 vertex;
	std::string line;
	int cont = 0;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		Point point;

		float intensity; // Variable temporal para almacenar el valor de intensidad
		//No quiero que lea la intensity
		iss >> point.position.x >> point.position.y >> point.position.z
			>> intensity >> point.color.r >> point.color.g >> point.color.b;

		point.color.r /= 255.0f;
		point.color.g /= 255.0f;
		point.color.b /= 255.0f;

		vertex = point.position;
		if (firstVertex)
		{
			minVertex = maxVertex = vertex;
			firstVertex = false;
		}
		else
		{
			minVertex = glm::min(minVertex, vertex);
			maxVertex = glm::max(maxVertex, vertex);
		}

		// Aquí se pueden añadir otros datos adicionales necesarios para cada punto
		vertices.push_back(point);
		cont += 1;

		if (cont % 1000000 == 0)
		{
			std::cerr << cont << "Points" << std::endl;
		}
		if (cont % 10500000 == 0)
		{
			break;
		}
	}

	// El vector 'vertices' ahora contiene todos los puntos leídos del archivo
	return true;
}


glm::vec3 Loader::getColor(int index) {
	glm::vec3 colors[8] = {
		//glm::vec3(1.0, 0.0, 0.0), // Rojo
		glm::vec3(0.0, 1.0, 0.0), // Verde
		glm::vec3(0.0, 0.0, 1.0), // Azul
		glm::vec3(1.0, 1.0, 0.0), // Amarillo
		glm::vec3(1.0, 0.0, 1.0), // Magenta
		glm::vec3(0.0, 1.0, 1.0), // Cyan
		glm::vec3(0.5, 0.5, 0.5), // Gris
		glm::vec3(0.0, 0.0, 0.0) // Negro
	};
	return colors[index];
}

bool Loader::loadFromOBJFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open OBJ file: " << filePath << std::endl;
		return false;
	}

	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec3> temp_normals;
	std::vector<glm::vec3> temp_color;
	bool firstVertex = true;
	int cont = 0;
	glm::vec3 vertex;
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "v")
		{

			iss >> vertex.x >> vertex.y >> vertex.z;
			temp_vertices.push_back(vertex);

			if (firstVertex)
			{
				minVertex = maxVertex = vertex;
				firstVertex = false;
			}
			else
			{
				minVertex = glm::min(minVertex, vertex);
				maxVertex = glm::max(maxVertex, vertex);
			}
			cont += 1;

			if (cont % 1000000 == 0)
			{
				std::cerr << cont << "Points" << std::endl;
			}
		}
		else if (type == "vn")
		{
			iss >> vertex.x >> vertex.y >> vertex.z;
			temp_normals.push_back(vertex);
		}
	}

	file.close();
	int con2t = 0;
	for (int i = 0; i < temp_vertices.size(); i++)
	{	
		Point newP;
		newP.position = temp_vertices[i];
		if (temp_normals.size() == temp_vertices.size())
		{
			newP.normal = temp_normals[i];
		}
		//newP.color = getColor(con2t);
		//con2t++;
		//newP.color = temp_color[i];
		vertices.push_back(newP);
	}
	//vertices = temp_vertices;
	// Procesar los datos de temp_vertices según las necesidades de tu aplicación
	// y almacenarlos en las estructuras de datos de la clase PointCloud.

	return true;
}

bool Loader::loadFromBigEndianPLYFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex)
{
	std::ifstream file(filePath, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open PLY file: " << filePath << std::endl;
		return false;
	}

	int numVertices = -1;

	std::unordered_map<double, int> lCounts; // Diccionario para contar los valores de "l"
	std::unordered_map<double, int> pCounts; // Diccionario para contar los valores de "p"

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "element")
		{
			std::string elementType;
			int count;
			iss >> elementType >> count;
			if (elementType == "data_visual")
			{
				numVertices = count;
			}
		}
		else if (type == "end_header")
		{
			if (numVertices == -1)
			{
				std::cerr << "Error: Invalid PLY file format." << std::endl;
				return false;
			}

			vertices.resize(numVertices);
			file.seekg(0, std::ios_base::cur);
			for (int i = 0; i < numVertices; i++)
			{
				file.seekg(0, std::ios_base::cur);

				double l, x, y, z, p;
				bool error;
				file.read(reinterpret_cast<char*>(&l), sizeof(double));
				file.read(reinterpret_cast<char*>(&x), sizeof(double));
				file.read(reinterpret_cast<char*>(&y), sizeof(double));
				file.read(reinterpret_cast<char*>(&z), sizeof(double));
				file.read(reinterpret_cast<char*>(&p), sizeof(double));

				if (!isBigEndian())
				{
					swapBytes(reinterpret_cast<char*>(&l), sizeof(double));
					swapBytes(reinterpret_cast<char*>(&x), sizeof(double));
					swapBytes(reinterpret_cast<char*>(&y), sizeof(double));
					swapBytes(reinterpret_cast<char*>(&z), sizeof(double));
					swapBytes(reinterpret_cast<char*>(&p), sizeof(double));
				}

				glm::vec3 vertex(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));

				if (i == 0)
				{
					minVertex = maxVertex = vertex;
				}
				else
				{
					minVertex = glm::min(minVertex, vertex);
					maxVertex = glm::max(maxVertex, vertex);
				}

				Point point;
				point.position = vertex;
				point.labels = glm::vec3(l, p, l==p);
				if (p != l)
				{
					//point.color = getColor(0);
				}
				else
				{
					//point.color = getColor(1);
				}
				point.color = getColor(l);//glm::vec3(1.f, 0.f, 0.f);
				point.normal = getColor(p);//glm::vec3(1.f, 0.f, 0.f);
				vertices[i] = point;

				//file.seekg(sizeof(double), std::ios_base::cur);

				// Contar los valores distintos de "l"
				if (lCounts.find(l) != lCounts.end())
				{
					lCounts[l]++;
				}
				else
				{
					lCounts[l] = 1;
				}

				// Contar los valores distintos de "p"
				if (pCounts.find(p) != pCounts.end())
				{
					pCounts[p]++;
				}
				else
				{
					pCounts[p] = 1;
				}
			}
		}
	}

	file.close();
	return true;
}




bool Loader::isBigEndian()
{
	const uint32_t one = 1;
	return *reinterpret_cast<const char*>(&one) == 0;
}

void Loader::swapBytes(char* data, int size)
{
	for (int i = 0; i < size / 2; ++i)
	{
		std::swap(data[i], data[size - i - 1]);
	}
}

bool Loader::loadFromPLYFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open PLY file: " << filePath << std::endl;
		return false;
	}

	std::string line;
	std::getline(file, line);
	if (line != "ply")
	{
		std::cerr << "Error: Not a valid PLY file: " << filePath << std::endl;
		return false;
	}

	std::getline(file, line);
	file.close();

	if (line == "format ascii 1.0")
	{
		return loadFromASCIIPLYFile(filePath, vertices, minVertex, maxVertex);
	}
	else if (line == "format binary_big_endian 1.0")
	{
		return loadFromBigEndianPLYFile(filePath, vertices, minVertex, maxVertex);
	}
	else
	{
		std::cerr << "Error: Unsupported PLY format: " << line << std::endl;
		return false;
	}
}


bool Loader::loadFromASCIIPLYFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Error: Could not open PLY file: " << filePath << std::endl;
		return false;
	}

	// Variables para almacenar el número de vertices y la posición actual en el archivo
	int numVertices = -1;
	int currentVertexIndex = 0;

	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream iss(line);
		std::string type;
		iss >> type;

		if (type == "element")
		{
			// Leer el número de vertices del archivo
			std::string elementType;
			int count;
			iss >> elementType >> count;
			if (elementType == "vertex")
			{
				numVertices = count;
			}
		}
		else if (type == "end_header")
		{
			// Después de leer el header, se deben haber encontrado el número de vertices
			if (numVertices == -1)
			{
				std::cerr << "Error: Invalid PLY file format." << std::endl;
				return false;
			}

			// Leer los vertices del archivo
			vertices.resize(numVertices);
			for (int i = 0; i < numVertices; i++)
			{
				glm::vec3 vertex;
				file >> vertex.x >> vertex.y >> vertex.z;

				// Actualizar las coordenadas mínimas y máximas
				if (i == 0)
				{
					minVertex = maxVertex = vertex;
				}
				else
				{
					minVertex = glm::min(minVertex, vertex);
					maxVertex = glm::max(maxVertex, vertex);
				}

				// Agregar el vértice al vector
				Point p;
				p.position = vertex;
				p.color = glm::vec3(1.f, 0.f, 0.f);
				vertices[i] = p;

				// Incrementar el índice actual del vértice
				currentVertexIndex++;
			}
		}
	}

	file.close();
	return true;
}

#endif // LOADER_H
