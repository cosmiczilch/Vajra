#ifndef MESSAGEHUB_DECLARATIONS_H
#define MESSAGEHUB_DECLARATIONS_H

enum EngineMessageType {
	MESSAGE_TYPE_UNSPECIFIED,
	//
	MESSAGE_TYPE_FRAME_EVENT,
	MESSAGE_TYPE_FIFTH_FRAME_EVENT,
	MESSAGE_TYPE_TENTH_FRAME_EVENT,
	MESSAGE_TYPE_500_MS_TIME_EVENT,
	MESSAGE_TYPE_5_S_TIME_EVENT,
	//
	MESSAGE_TYPE_SCENE_START,
	MESSAGE_TYPE_SCENE_END,
	//
	MESSAGE_TYPE_TRANSFORM_CHANGED_EVENT,
	//
	MESSAGE_TYPE_ANIMATION_BEGAN_EVENT,
	MESSAGE_TYPE_ANIMATION_PAUSED_EVENT,
	MESSAGE_TYPE_ANIMATION_RESUMED_EVENT,
	MESSAGE_TYPE_ANIMATION_ENDED_EVENT,
	//
	MESSAGE_TYPE_PINCH_GESTURE,
	
	// MUST BE LAST
	NUM_ENGINE_MESSAGE_TYPES,
};

enum MessageDataType {
	MESSAGEDATA_TYPE_GENERIC_EMPTY_MESSAGE,
	MESSAGEDATA_TYPE_1S_1I_3FV,				// Generic carrier for 1 string, 1 int, and 3 vec3<float>s
};



#endif // MESSAGEHUB_DECLARATIONS_H
