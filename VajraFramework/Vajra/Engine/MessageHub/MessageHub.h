#ifndef MESSAGE_HUB_H
#define MESSAGE_HUB_H

#include "Vajra/Engine/MessageHub/MessageCache.h"
#include "Vajra/Utilities/CommonDeclarations.h"

// Forward Declarations:
class Message;
class MessageCache;

class MessageHub {
public:
	~MessageHub();

	void SendPointcastMessage(Message* message, ObjectIdType receiverId, ObjectIdType senderId = OBJECT_ID_INVALID);
	void SendMulticastMessage(Message* message, ObjectIdType senderId = OBJECT_ID_INVALID);

	void SubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId);
	void UnsubscribeToMessageType(MessageType messageType, ObjectIdType subscriberId);

	Message* RetrieveNextMessage(ObjectIdType id);


private:
	MessageHub();
	void init();
	void destroy();

	void drainMessages();
	void drainMessageCache_internal();

	MessageCache frontMessageCache;
	MessageCache backMessageCache;
	//
	MessageCache* currentlyAcceptingMessageCache;
	MessageCache* currentlyDrainingMessageCache;

	// TODO [Implement] Change this to be a sorted list so that we can binary search it maybe (low priority)
	std::vector<ObjectIdType> subscribersForMessageType[NUM_MESSAGE_TYPES];

	friend class Engine;
};

#endif // MESSAGE_HUB_H
