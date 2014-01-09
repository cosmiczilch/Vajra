#ifndef SCENEGRAPH3D_H
#define SCENEGRAPH3D_H

#include "Vajra/Common/Objects/ObjectRegistry.h"
#include "Vajra/Common/Objects/Object.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Utilities/Utilities.h"
#include "Vajra/Utilities/CommonDeclarations.h"

#include <map>

// Forward Declarations:
class Camera;
class DirectionalLight;

class SceneGraph3D : public SceneGraph {
public:
	virtual ~SceneGraph3D();

	DirectionalLight* GetMainDirectionalLight();
	void SetMainDirectionalLightId(ObjectIdType id);

	// @Override
	virtual void update();
	virtual void draw();

	virtual bool TestTouch(int /*touchIndex*/) { return true; }
	virtual void OnTouch(int /*touchIndex*/) {}
protected:
	SceneGraph3D();
	virtual void init();
	virtual void destroy();

	ObjectIdType mainDirectionalLightId;

	friend class Engine;
};

#endif // SCENEGRAPH3D_H
