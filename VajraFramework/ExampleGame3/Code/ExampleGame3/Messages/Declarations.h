//
//  GameMessageTypes.h
//  Created by Matt Kaufmann on 01/05/13.
//

#ifndef GAMEMESSAGETYPES_H
#define GAMEMESSAGETYPES_H

#include "Vajra/Common/Messages/Declarations.h"

enum GameMessageType {
	MESSAGE_TYPE_GRID_CELL_CHANGED = NUM_ENGINE_MESSAGE_TYPES,
	MESSAGE_TYPE_CAMERA_MODE_CHANGED,
	MESSAGE_TYPE_GRID_UNIT_COLLISION,
	MESSAGE_TYPE_GRID_ZONE_ENTERED,
	MESSAGE_TYPE_GRID_ZONE_EXITED,
	MESSAGE_TYPE_GRID_ROOM_ENTERED,
	MESSAGE_TYPE_GRID_ROOM_EXITED,

	// Navigation Messages
	MESSAGE_TYPE_NAVIGATION_REACHED_DESTINATION,

	// Unit Messages
	MESSAGE_TYPE_UNIT_KILLED,
	MESSAGE_TYPE_UNIT_ACTION_STATE_CHANGED,

	// AI Messages
	MESSAGE_TYPE_AI_SIGHTED_PLAYER,
	MESSAGE_TYPE_AI_LOST_SIGHT_OF_PLAYER, // derp
};

#endif // GAMEMESSAGETYPES_H