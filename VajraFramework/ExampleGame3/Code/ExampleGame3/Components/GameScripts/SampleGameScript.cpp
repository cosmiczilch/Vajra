#include "ExampleGame3/Components/ComponentTypes/ComponentTypeIds.h"
#include "ExampleGame3/Components/GameScripts/SampleGameScript.h"
#include "Vajra/Engine/Components/DerivedComponents/Transform/Transform.h"
#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/Input/Input.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Utilities/MathUtilities.h"

unsigned int SampleGameScript::componentTypeId = COMPONENT_TYPE_ID_SAMPLE_GAME_SCRIPT;

SampleGameScript::SampleGameScript() : Component() {
	this->init();

	// TODO [Hack] call start from an event handle once that event is implemented
	this->start();
}

SampleGameScript::SampleGameScript(Object* object_) : Component(object_) {
	this->init();

	// TODO [Hack] call start from an event handle once that event is implemented
	this->start();
}

SampleGameScript::~SampleGameScript() {
	this->destroy();

	// TODO [Hack] call end from an event handle once that event is implemented
	this->end();
}

void SampleGameScript::start() {
	// TODO [Implement] Ensure type-safety here
	this->gameObjectRef = (GameObject*)this->GetObject();

	this->getTransform()->Scale(0.5f, 0.5f, 0.5f);
}

void SampleGameScript::end() {
}

void SampleGameScript::update() {
	if(ENGINE->GetInput()->GetTouchCount() > 0) {
		this->getTransform()->Rotate(5.0f, YAXIS);
	}
}

void SampleGameScript::init() {
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_START, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_FRAME_EVENT, this->GetTypeId(), false);
	this->addSubscriptionToMessageType(MESSAGE_TYPE_SCENE_END  , this->GetTypeId(), false);
}

void SampleGameScript::destroy() {
	this->removeSubscriptionToAllMessageTypes(this->GetTypeId());
}
