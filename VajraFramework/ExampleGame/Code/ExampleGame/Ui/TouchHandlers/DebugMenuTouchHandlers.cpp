#ifdef DEBUG
#include "ExampleGame/Ui/TouchHandlers/DebugMenuTouchHandlers.h"
#include "ExampleGame/Ui/TouchHandlers/GameUiTouchHandlers.h"
#include "Vajra/Engine/Components/DerivedComponents/Camera/Camera.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/SceneGraph/SceneGraphUi.h"
#include "Vajra/Engine/SceneLoaders/UiSceneLoader/UiSceneLoader.h"
#include "Vajra/Engine/Ui/UiObject/UiObject.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"

// Todo [HACK] when level loading is better we probably won't need all these
#include "Vajra/Framework/DeviceUtils/FileSystemUtils/FileSystemUtils.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "ExampleGame/GameSingletons/GameSingletons.h"
#include "Vajra/Engine/SceneGraph/SceneGraph3D.h"

DebugMenuTouchHandlers::DebugMenuTouchHandlers() : UiTouchHandlers() {
	//Subscribe to the frame event
	
}

void DebugMenuTouchHandlers::OnTouchDownHandlers(UiObject* uiObject, Touch touch ) {
	if (uiObject->GetUiObjectName() == "touchPad") {
		// Do something
		this->downPos = touch.pos;

	} else {
		// Do something

	}

}

void DebugMenuTouchHandlers::OnTouchMoveHandlers(UiObject* uiObject, Touch touch ) {
	if (uiObject->GetUiObjectName() == "touchPad") {
		// Do something
		glm::vec2 moveDir = touch.pos - touch.prevPos;
		GameObject* debugCam = (GameObject*)ENGINE->GetSceneGraph3D()->GetMainCamera()->GetObject();
		Transform* trans = debugCam->GetTransform();

		trans->Rotate(-moveDir.x inRadians, YAXIS);
		trans->Rotate(-moveDir.y inRadians, trans->GetLeft());

	} else {
		// Do something

	}

}

void DebugMenuTouchHandlers::OnTouchUpHandlers(UiObject* uiObject, Touch /* touch */) {
	GameObject* debugCam = (GameObject*)ENGINE->GetSceneGraph3D()->GetMainCamera()->GetObject();
	Transform* trans = debugCam->GetTransform();
	if (uiObject->GetUiObjectName() == "up") {
		if(this->positionMode) {
			trans->Translate(this->cameraSpeed, trans->GetForward());
		} else {
			trans->Rotate(-this->cameraSpeed inRadians, trans->GetLeft());
		}

	} else if (uiObject->GetUiObjectName() == "down") {
		if(this->positionMode) {
			trans->Translate(-this->cameraSpeed, trans->GetForward());
		} else {
			trans->Rotate(this->cameraSpeed inRadians, trans->GetLeft());
		}

	} else if (uiObject->GetUiObjectName() == "left") {
		if(this->positionMode) {
			trans->Translate(this->cameraSpeed, trans->GetLeft());
		} else {
			trans->Rotate(this->cameraSpeed inRadians, YAXIS);
		}

	} else if (uiObject->GetUiObjectName() == "right") {
		if(this->positionMode) {
			trans->Translate(-this->cameraSpeed, trans->GetLeft());
		} else {
			trans->Rotate(-this->cameraSpeed inRadians, YAXIS);
		}
	} 
	else if (uiObject->GetUiObjectName() == "move") {
		this->positionMode = true;
	} 
	else if (uiObject->GetUiObjectName() == "rotate") {
		this->positionMode = false;
	} 


}
#endif