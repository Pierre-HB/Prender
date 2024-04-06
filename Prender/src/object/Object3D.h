#pragma once

#include "../algebra/Algebra.h"
#include "../buffers/VAO.h"

#ifdef IMGUI
struct imGuiObject3DAttr {
	ImGuiTransformationAttr object;
	ImGuiTransformationAttr world;
	

	imGuiObject3DAttr(const mat4& object, const mat4& world) : object(ImGuiTransformationAttr(object, "object")), world(ImGuiTransformationAttr(world, "world")) {}
};

class Object3D : public ImGuiPrintable
#else
class Object3D
#endif
{
protected:

	mat4 object;
	mat4 world;
	VAO* vao;

public:

	Object3D() : object(mat4::identity()), world(mat4::identity()) {
		vao = new VAO();
	}

	virtual ~Object3D() {

	}

	void moove(const mat4& transformation);

	virtual void draw();

#ifdef IMGUI
	virtual void* getAttribute() const;

	virtual void updateAttribute(void* attr) const;

	virtual void setAttribute(void* attr);

	virtual void imGuiPrintAttribute(void* attr) const;
#endif
};