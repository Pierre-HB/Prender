#pragma once

#include <vector>
#include "scene.h"

#include "../utils/texture.h"
#include "../../Shader.h"
#include "../buffers/VAO.h"
#include "../utils/PerspectiveCamera.h"
#include "../object/Object3D_P_N_UV.h"

#ifdef IMGUI
//! data for the BasicScene UI
struct imGuiBasicSceneAttr {
	bool showNormal;
	bool showWireframe;
	bool showMaterial;
	bool rotation;
	bool backCulling;

	//! Constructor
	imGuiBasicSceneAttr(bool showNormal, bool showWireframe, bool showMaterial, bool rotation, bool backCulling) : showNormal(showNormal), showWireframe(showWireframe), showMaterial(showMaterial), rotation(rotation), backCulling(backCulling) {
#ifdef DEBUG
		debug::NB_ATTR++;
#endif
	}

	~imGuiBasicSceneAttr() {
#ifdef DEBUG
		debug::NB_ATTR--;
#endif
	}
};

//! a small scene to showcase features of the engine
class BasicScene : public Scene, public ImGuiPrintable
#else
//! a small scene to showcase features of the engine
class BasicScene : public Scene
#endif
{
private:
	Texture* texture;
	Texture* texture_smiley;
	Texture* debug;
	Texture* debugMaterial;
	Texture* normal_gravier;
	Shader* shader_test;
	Shader* shader_tbn;
	PerspectiveCamera* camera;
	Object3D_P_N_UV* object;

	LightManager* lightManager;

	bool showNormal;
	bool showWireframe;
	bool showMaterial;
	bool rotation;
	bool backCulling;

public:
	BasicScene();

	~BasicScene();

	//! render of the scene
	void render();

	//! update of the scene
	void update(Engine* engine);

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