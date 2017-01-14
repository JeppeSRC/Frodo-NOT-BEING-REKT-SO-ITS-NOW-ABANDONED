#pragma once

#include <fd.h>
#include <math/math.h>
#include <graphics/render/mesh/mesh.h>
#include <graphics/render/renderable/renderable.h>

class FDAPI Model : public Renderable {
private:
	Mesh* mesh;

	

public:
	Model(Mesh* mesh){ }
	~Model() { delete mesh; }

	inline Mesh* GetMesh() { return mesh; }
};
