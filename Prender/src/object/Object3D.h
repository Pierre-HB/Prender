#pragma once

#include "../algebra/Algebra.h"
#include "../buffers/VAO.h"

#ifdef IMGUI
//! data for the Object3D UI
struct imGuiObject3DAttr {
	ImGuiTransformationAttr object;
	ImGuiTransformationAttr world;
	
	//! Constructor
	imGuiObject3DAttr(const mat4& object, const mat4& world) : object(ImGuiTransformationAttr(object, "object")), world(ImGuiTransformationAttr(world, "world")) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	~imGuiObject3DAttr() {
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}
	
};

//! Virtual class for 3D obejct
class Object3D : public ImGuiPrintable
#else
//! Virtual class for 3D obejct
class Object3D
#endif
{
protected:

	mat4 object;
	mat4 world;
	VAO* vao;

public:

	//! constructor for an object3D
	Object3D() : object(mat4::identity()), world(mat4::identity()) {
		vao = new VAO();
#ifdef DEBUG
		debug::NB_INSTANCES++;
#endif
	}

	//! constructor for an object3D
	Object3D(const mat4& object, const mat4& world) : object(object), world(world) {
		vao = new VAO();
#ifdef DEBUG
		debug::NB_INSTANCES++;
#endif
	}

	virtual ~Object3D() {
		delete vao;
#ifdef DEBUG
		debug::NB_INSTANCES--;
#endif
	}

	//! moove a 3D object on the world
	void moove(const mat4& transformation);

	//! virtual function to draw the object on the framebuffer
	virtual void draw();

#ifdef IMGUI
	//! \copydoc ImGuiPrintable::getAttribute()
	virtual void* getAttribute() const;

	//! \copydoc ImGuiPrintable::updateAttribute()
	virtual void updateAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::setAttribute()
	virtual void setAttribute(void* attr);

	//! \copydoc ImGuiPrintable::imGuiPrintAttribute()
	virtual void imGuiPrintAttribute(void* attr) const;

	//! \copydoc ImGuiPrintable::deleteAttribute()
	virtual void deleteAttribute(void* attr) const;
#endif
};