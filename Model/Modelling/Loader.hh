#pragma once

#include <vector>
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

//STD Libs
#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<sstream>
#include<algorithm>

//Own libs
#include"Point.hh"


class Loader
{
public:
	static bool loadFromtxtFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex);
	static bool loadFromOBJFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex);
	static bool loadFromBigEndianPLYFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex);
	static bool isBigEndian();
	static void swapBytes(char* data, int size);
	static bool loadFromPLYFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex);
	static bool loadFromASCIIPLYFile(const std::string& filePath, std::vector<Point>& vertices, glm::vec3& minVertex, glm::vec3& maxVertex);
	static glm::vec3 getColor(int index);
private:
	// Hacer el constructor privado para que la clase no pueda ser instanciada
	Loader() {}
};



