#include "format.h"

using namespace FD;

struct Vertex {
	vec3 position;
	vec2 texCoord;
	vec3 normal;
};

template<uint32 N>
struct Face {
	struct Vertex {
		uint32 vertex;
		uint32 texCoord;
		uint32 normal;
	} v[N];

	inline Vertex& operator[](uint_t index) { return v[index]; }
};

void MakeFaces(List<Vertex>& data, List<vec3>& vertices, List<vec2>& texCoords, List<vec3>& normals, List<uint32>& indices, List<Face<3>> faces) {

	uint_t numFaces = faces.GetSize();

	indices.Reserve(numFaces * 3);
	data.Reserve(numFaces * 3);
#pragma omp for
	for (int32 i = 0; i < numFaces; i++) {
		Face<3> face = faces[i];
		for (int32 j = 0; j < 3; j++) {
			uint32 index = i * 3 + j;//indices.GetSize();

			indices.Push_back(index);
			Vertex& v = data[index];
			v.position = vertices[face[j].vertex - 1];
			v.texCoord = texCoords[face[j].texCoord - 1];
			v.normal = normals[face[j].normal - 1];
		}
	}
}


bool ConvertOBJToFDM(const String& filename, const String& newFilename, uint32 attributes) {
	
	String data = FDReadTextFile(filename);

	List<Face<3>> faces;
	List<String*> lines = data.Split('\n');
	List<vec3> vertices, normals;
	List<vec2> texCoords;
	List<uint32> indices;
	List<Vertex> result;


	uint_t numLines = lines.GetSize();
	FD_DEBUG("[OBJConverter] Parsing text");

	for (int32 i = 0; i < numLines; i++) {
		String line = lines[i];

		if (line.StartsWith("v ")) {
			vec3 vert;
			sscanf(*line, "v %f %f %f", &vert.x, &vert.y, &vert.z);
			vertices.Push_back(vert);
		}
		else if (line.StartsWith("vt ")) {
			vec2 tex;
			sscanf(*line, "vt %f %f", &tex.x, &tex.y);
			texCoords.Push_back(tex);
		}
		else if (line.StartsWith("vn ")) {
			vec3 norm;
			sscanf(*line, "vn %f %f %f", &norm.x, &norm.y, &norm.z);
			normals.Push_back(norm);
		}
		else if (line.StartsWith("f ")) {
			Face<3> face;
			sscanf(*line, "f %u/%u/%u %u/%u/%u %u/%u/%u", &face[0].vertex, &face[0].texCoord, &face[0].normal, &face[1].vertex, &face[1].texCoord, &face[1].normal, &face[2].vertex, &face[2].texCoord, &face[2].normal);
			faces.Push_back(face);
		}
	}

	lines.Free();
	FD_DEBUG("[OBJConverter] Found %u vertices %u normals %u uvs %u faces", vertices.GetSize(), normals.GetSize(), texCoords.GetSize(), faces.GetSize());
	FD_DEBUG("[OBJConverter] Allocating space");

	FD_DEBUG("[OBJConverter] Making faces");

	MakeFaces(result, vertices, texCoords, normals, indices, faces);

	FDWriteFile(newFilename, result.GetData(), result.GetSizeInBytes());

	FD_DEBUG("[OBJConverter] Writing to file \"%s\"", *newFilename);

	return true;
}

