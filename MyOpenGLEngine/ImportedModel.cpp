#include "MyOpenGL.h"



ImportedModel::ImportedModel(const char* filePath)
{
	ModelImporter modelImporter = ModelImporter();
	modelImporter.parseOBJ(filePath);
	numVertices = modelImporter.getNumVertices();

	vector<float> verts = modelImporter.getVertices();
	vector<float> tcs = modelImporter.getTextureCoordinates();
	vector<float> normals = modelImporter.getNormals();


	for (int i = 0; i < numVertices; i++)
	{
		vertices.emplace_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
		texCoords.emplace_back(glm::vec2(tcs[i + 2], tcs[i + 2 + 1]));
		normalVecs.emplace_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
	}



}

int ImportedModel::getNumVertices()
{
	return numVertices;
}

vector<glm::vec3> ImportedModel::getVertices()
{
	return vertices;
}

vector<glm::vec2> ImportedModel::getTextureCoords()
{
	return texCoords;
}

vector<glm::vec3> ImportedModel::getNormals()
{
	return normalVecs;
}

ModelImporter::ModelImporter()
{
}

void ModelImporter::parseOBJ(const char* filePath)
{
	float x, y, z;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";

	while (!fileStream.eof())
	{
		getline(fileStream, line);

		if (line.compare(0, 2, "v ") == 0)
		{
			stringstream ss(line.erase(0, 1));
			ss >> x;
			ss >> y;
			ss >> z;
			vertVals.emplace_back(x);
			vertVals.emplace_back(y);
			vertVals.emplace_back(z);
		}

		if (line.compare(0, 2, "vt") == 0)
		{
			stringstream ss(line.erase(0, 2));
			ss >> x;
			ss >> y;
			stVals.emplace_back(x);
			stVals.emplace_back(y);

		}

		if (line.compare(0, 2, "vn") == 0)
		{
			stringstream ss(line.erase(0, 2));
			ss >> x;
			ss >> y;
			ss >> z;
			normVals.emplace_back(x);
			normVals.emplace_back(y);
			normVals.emplace_back(z);

		}

		if (line.compare(0, 2, "f ") == 0)
		{

			string oneCorner, v, t, n;
			stringstream ss(line.erase(0, 2));

			for (int i = 0; i < 3; i++)
			{
				getline(ss, oneCorner, ' ');
				stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');

				int vertRef = (stoi(v) - 1) * 3;
				int tcRef = (stoi(t) - 1) * 2;
				int normRef = (stoi(n) - 1) * 3;

				triangleVerts.emplace_back(vertVals[vertRef]);
				triangleVerts.emplace_back(vertVals[vertRef + 1]);
				triangleVerts.emplace_back(vertVals[vertRef + 2]);


				textureCoords.emplace_back(stVals[tcRef]);
				textureCoords.emplace_back(stVals[tcRef + 1]);


				normals.emplace_back(normVals[normRef]);
				normals.emplace_back(normVals[normRef + 1]);
				normals.emplace_back(normVals[normRef + 2]);
			}
		}
	}
}

int ModelImporter::getNumVertices()
{
	return (triangleVerts.size() / 3);
}

vector<float> ModelImporter::getVertices()
{
	return triangleVerts;
}

vector<float> ModelImporter::getTextureCoordinates()
{
	return textureCoords;
}

vector<float> ModelImporter::getNormals()
{
	return normals;
}
